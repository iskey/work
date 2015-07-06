/* Compute checksums of files or strings.
   Copyright (C) 1995-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>.  */

///\#include <config.h>

//#include "stdint.in.h"
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libintl.h>

#define HASH_ALGO_MD5 1
#define _(x) gettext(x)

typedef unsigned long long int gl_uintmax_t;
#define uintmax_t gl_uintmax_t

#define bool int

#if HASH_ALGO_MD5
//#include "md5.h"
#endif
#include "error.h"
#include "errno.h"
#include <stdio.h>

#if HASH_ALGO_MD5
#define PROGRAM_NAME "md5sum"
#define DIGEST_TYPE_STRING "MD5"
#define DIGEST_STREAM md5_stream
#define DIGEST_BITS 128
#define DIGEST_REFERENCE "RFC 1321"
#define DIGEST_ALIGN 4
#endif

#define DIGEST_HEX_BYTES (DIGEST_BITS / 4)
#define DIGEST_BIN_BYTES (DIGEST_BITS / 8)

/* The minimum length of a valid digest line.  This length does
   not include any newline character at the end of a line.  */
#define MIN_DIGEST_LINE_LENGTH \
  (DIGEST_HEX_BYTES /* length of hexadecimal message digest */ \
   + 1 /* blank */ \
   + 1 /* minimum filename length */ )


/* Tandem/NSK and other platforms that define EXIT_FAILURE as -1 interfere
   with proper operation of xargs.  */
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#elif EXIT_FAILURE != 1
#undef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef O_BINARY
# define O_BINARY 0
# define O_TEXT 0
#endif

/* OS/2 EMX lacks these macros.  */
#ifndef STDIN_FILENO
# define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
# define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
# define STDERR_FILENO 2
#endif

#define true 1
#define false 0

#define ULONG_MAX ((unsigned long)(0 - 1))

/* True if any of the files read were the standard input. */
static bool have_read_stdin;

/* With --check, print a message to standard error warning about each
   improperly formatted checksum line.  */
static bool warn = false;

/* With --check, don't generate any output.
   The exit code indicates success or failure.  */
static bool status_only = false;

/* Set to the length of a digest hex string for the selected algorithm.  */
static size_t digest_hex_bytes;

/* With --check, suppress the "OK" printed for each verified file.  */
static bool quiet = false;

/* With --check, exit with a non-zero return code if any line is
   improperly formatted. */
static bool strict = false;

/* The minimum length of a valid checksum line for the selected algorithm.  */
static size_t min_digest_line_length;

/* Whether a BSD reversed format checksum is detected.  */
static int bsd_reversed = -1;

#define STREQ(a, b) (strcmp (a, b) == 0)
#define STREQ_LEN(a, b, n) (strncmp (a, b, n) == 0)
#define STRPREFIX(a, b) (strncmp(a, b, strlen (b)) == 0)

#define ISWHITE(c) ((c) == ' ' || (c) == '\t')

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#define STRNCMP_LIT(s, literal) \
  strncmp (s, "" literal "", sizeof (literal) - 1)


/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\\\/\\/g' i.e., replacing each "\\n" string with a newline
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

static char *
filename_unescape (char *s, size_t s_len)
{
  char *dst = s;
  size_t i = 0;

  for (i = 0; i < s_len; i++)
    {
      switch (s[i])
        {
        case '\\':
          if (i == s_len - 1)
            {
              /* File name ends with an unescaped backslash: invalid.  */
              return NULL;
            }
          ++i;
          switch (s[i])
            {
            case 'n':
              *dst++ = '\n';
              break;
            case '\\':
              *dst++ = '\\';
              break;
            default:
              /* Only '\' or 'n' may follow a backslash.  */
              return NULL;
            }
          break;

        case '\0':
          /* The file name may not contain a NUL.  */
          return NULL;

        default:
          *dst++ = s[i];
          break;
        }
    }
  if (dst < s + s_len)
    *dst = '\0';

  return s;
}


/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

static inline void *
ptr_align (void const *ptr, size_t alignment)
{
  char const *p0 = ptr;
  char const *p1 = p0 + alignment - 1;
  return (void *) (p1 - (size_t) p1 % alignment);
}

/* There are a few hints one can provide, which have the
   following characteristics on Linux 2.6.31 at least.

   POSIX_FADV_SEQUENTIAL
     Doubles the size of read ahead done for file
   POSIX_FADV_WILLNEED
     _synchronously_ prepopulate the buffer cache with the file
   POSIX_FADV_NOREUSE
     Could lower priority of data in buffer caches,
     but currently does nothing.
   POSIX_FADV_DONTNEED
     Drop the file from cache.
     Note this is automatically done when files are unlinked.

   We use this enum "type" both to make it explicit that
   these options are mutually exclusive, and to discourage
   the passing of the possibly undefined POSIX_FADV_... values.
   Note we could #undef the POSIX_FADV_ values, but that would
   preclude using the posix_fadvise() function with its standard
   constants. Using posix_fadvise() might be required if the return
   value is needed, but it must be guarded by appropriate #ifdefs.  */

#if HAVE_POSIX_FADVISE
typedef enum {
  FADVISE_NORMAL =     POSIX_FADV_NORMAL,
  FADVISE_SEQUENTIAL = POSIX_FADV_SEQUENTIAL,
  FADVISE_NOREUSE =    POSIX_FADV_NOREUSE,
  FADVISE_DONTNEED =   POSIX_FADV_DONTNEED,
  FADVISE_WILLNEED =   POSIX_FADV_WILLNEED,
  FADVISE_RANDOM =     POSIX_FADV_RANDOM,
} fadvice_t;
#else
typedef enum {
  FADVISE_NORMAL,
  FADVISE_SEQUENTIAL,
  FADVISE_NOREUSE,
  FADVISE_DONTNEED,
  FADVISE_WILLNEED,
  FADVISE_RANDOM,
} fadvice_t;
#endif

void
fdadvise (int fd, off_t offset, off_t len, fadvice_t advice)
{
#if HAVE_POSIX_FADVISE
    posix_fadvise (fd, offset, len, advice);
#endif
}

void
fadvise (FILE *fp, fadvice_t advice)
{
  if (fp)
    fdadvise (fileno (fp), 0, 0, advice);
}

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
static inline unsigned long int
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  enum { PLURAL_REDUCER = 1000000 };
  return (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

/* Split the string S (of length S_LEN) into three parts:
   a hexadecimal digest, binary flag, and the file name.
   S is modified.  Return true if successful.  */

static bool
split_3 (char *s, size_t s_len,
         unsigned char **hex_digest, int *binary, char **file_name)
{
  bool escaped_filename = false;
  size_t algo_name_len;

  size_t i = 0;
  while (ISWHITE (s[i]))
    ++i;

  if (s[i] == '\\')
    {
      ++i;
      escaped_filename = true;
    }

  /* Check for BSD-style checksum line. */

  algo_name_len = strlen (DIGEST_TYPE_STRING);
  if (STREQ_LEN (s + i, DIGEST_TYPE_STRING, algo_name_len))
    {
      if (s[i + algo_name_len] == ' ')
        ++i;
      if (s[i + algo_name_len] == '(')
        {
          *binary = 0;
          return bsd_split_3 (s +      i + algo_name_len + 1,
                              s_len - (i + algo_name_len + 1),
                              hex_digest, file_name, escaped_filename);
        }
    }

  /* Ignore this line if it is too short.
     Each line must have at least 'min_digest_line_length - 1' (or one more, if
     the first is a backslash) more characters to contain correct message digest
     information.  */
  if (s_len - i < min_digest_line_length + (s[i] == '\\'))
    return false;

  *hex_digest = (unsigned char *) &s[i];

  /* The first field has to be the n-character hexadecimal
     representation of the message digest.  If it is not followed
     immediately by a white space it's an error.  */
  i += digest_hex_bytes;
  if (!ISWHITE (s[i]))
    return false;

  s[i++] = '\0';

  /* If "bsd reversed" format detected.  */
  if ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
    {
      /* Don't allow mixing bsd and standard formats,
         to minimize security issues with attackers
         renaming files with leading spaces.
         This assumes that with bsd format checksums
         that the first file name does not have
         a leading ' ' or '*'.  */
      if (bsd_reversed == 0)
        return false;
      bsd_reversed = 1;
    }
  else if (bsd_reversed != 1)
    {
      bsd_reversed = 0;
      *binary = (s[i++] == '*');
    }

  /* All characters between the type indicator and end of line are
     significant -- that includes leading and trailing white space.  */
  *file_name = &s[i];

  if (escaped_filename)
    return filename_unescape (&s[i], s_len - i) != NULL;

  return true;
}

/* The __attribute__ feature is available in gcc versions 2.5 and later.
   The attribute __pure__ was added in gcc 2.96.  */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#define _GL_ATTRIBUTE_PURE __attribute__ ((__pure__))
#else
#define _GL_ATTRIBUTE_PURE /* empty */
#endif

/* An interface to the function, DIGEST_STREAM.
   Operate on FILENAME (it may be "-").

   *BINARY indicates whether the file is binary.  BINARY < 0 means it
   depends on whether binary mode makes any difference and the file is
   a terminal; in that case, clear *BINARY if the file was treated as
   text because it was a terminal.

   Put the checksum in *BIN_RESULT, which must be properly aligned.
   Return true if successful.  */

static bool
digest_file (const char *filename, int *binary, unsigned char *bin_result)
{
  FILE *fp;
  int err;
  bool is_stdin = STREQ (filename, "-");

  if (is_stdin)
    {
      have_read_stdin = true;
      fp = stdin;
      if (O_BINARY && *binary)
        {
          if (*binary < 0)
            *binary = ! isatty (STDIN_FILENO);
          if (*binary)
            xfreopen (NULL, "rb", stdin);
        }
    }
  else
    {
      fp = fopen (filename, (O_BINARY && *binary ? "rb" : "r"));
      if (fp == NULL)
        {
          error (0, errno, "%s", filename);
          return false;
        }
    }

  fadvise (fp, FADVISE_SEQUENTIAL);

  err = DIGEST_STREAM (fp, bin_result);
  if (err)
    {
      error (0, errno, "%s", filename);
      if (fp != stdin)
        fclose (fp);
      return false;
    }

  if (!is_stdin && fclose (fp) != 0)
    {
      error (0, errno, "%s", filename);
      return false;
    }

  return true;
}

/* Return true if S is a NUL-terminated string of DIGEST_HEX_BYTES hex digits.
   Otherwise, return false.  */
static bool _GL_ATTRIBUTE_PURE
hex_digits (unsigned char const *s)
{
  unsigned int i;
  for (i = 0; i < digest_hex_bytes; i++)
    {
      if (!isxdigit (*s))
        return false;
      ++s;
    }
  return *s == '\0';
}

static bool
digest_check (const char *checkfile_name)
{
  FILE *checkfile_stream;
  uintmax_t n_misformatted_lines = 0;
  uintmax_t n_properly_formatted_lines = 0;
  uintmax_t n_improperly_formatted_lines = 0;
  uintmax_t n_mismatched_checksums = 0;
  uintmax_t n_open_or_read_failures = 0;
  unsigned char bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned char *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  uintmax_t line_number;
  char *line;
  size_t line_chars_allocated;
  bool is_stdin = STREQ (checkfile_name, "-");

  if (is_stdin)
    {
      have_read_stdin = true;
      checkfile_name = _("standard input");
      checkfile_stream = stdin;
    }
  else
    {
      checkfile_stream = fopen (checkfile_name, "r");
      if (checkfile_stream == NULL)
        {
          error (0, errno, "%s", checkfile_name);
          return false;
        }
    }

  line_number = 0;
  line = NULL;
  line_chars_allocated = 0;
  do
    {
      char *filename;
      int binary;
      unsigned char *hex_digest;
      ssize_t line_length;

      ++line_number;
      if (line_number == 0)
        error (EXIT_FAILURE, 0, _("%s: too many checksum lines"),
               checkfile_name);

      line_length = getline (&line, &line_chars_allocated, checkfile_stream);
      if (line_length <= 0)
        break;

      /* Ignore comment lines, which begin with a '#' character.  */
      if (line[0] == '#')
        continue;

      /* Remove any trailing newline.  */
      if (line[line_length - 1] == '\n')
        line[--line_length] = '\0';

      if (! (split_3 (line, line_length, &hex_digest, &binary, &filename)
             && ! (is_stdin && STREQ (filename, "-"))
             && hex_digits (hex_digest)))
        {
          ++n_misformatted_lines;

          if (warn)
            {
              error (0, 0,
                     "%s: %: improperly formatted %s checksum line",
                     checkfile_name, line_number,
                     DIGEST_TYPE_STRING);
            }

          ++n_improperly_formatted_lines;
        }
      else
        {
          static const char bin2hex[] = { '0', '1', '2', '3',
                                          '4', '5', '6', '7',
                                          '8', '9', 'a', 'b',
                                          'c', 'd', 'e', 'f' };
          bool ok;

          ++n_properly_formatted_lines;

          ok = digest_file (filename, &binary, bin_buffer);

          if (!ok)
            {
              ++n_open_or_read_failures;
              if (!status_only)
                {
                  printf (_("%s: FAILED open or read\n"), filename);
                }
            }
          else
            {
              size_t digest_bin_bytes = digest_hex_bytes / 2;
              size_t cnt;
              /* Compare generated binary number with text representation
                 in check file.  Ignore case of hex digits.  */
              for (cnt = 0; cnt < digest_bin_bytes; ++cnt)
                {
                  if (tolower (hex_digest[2 * cnt])
                      != bin2hex[bin_buffer[cnt] >> 4]
                      || (tolower (hex_digest[2 * cnt + 1])
                          != (bin2hex[bin_buffer[cnt] & 0xf])))
                    break;
                }
              if (cnt != digest_bin_bytes)
                ++n_mismatched_checksums;

              if (!status_only)
                {
                  if (cnt != digest_bin_bytes)
                    printf ("%s: %s\n", filename, _("FAILED"));
                  else if (!quiet)
                    printf ("%s: %s\n", filename, _("OK"));
                }
            }
        }
    }
  while (!feof (checkfile_stream) && !ferror (checkfile_stream));

  free (line);

  if (ferror (checkfile_stream))
    {
      error (0, 0, _("%s: read error"), checkfile_name);
      return false;
    }

  if (!is_stdin && fclose (checkfile_stream) != 0)
    {
      error (0, errno, "%s", checkfile_name);
      return false;
    }

  if (n_properly_formatted_lines == 0)
    {
      /* Warn if no tests are found.  */
      error (0, 0, _("%s: no properly formatted %s checksum lines found"),
             checkfile_name, DIGEST_TYPE_STRING);
    }
  else
    {
      if (!status_only)
        {
          if (n_misformatted_lines != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %u line is improperly formatted",
                     "WARNING: %u lines are improperly formatted",
                     select_plural (n_misformatted_lines))),
                   n_misformatted_lines);

          if (n_open_or_read_failures != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %u listed file could not be read",
                     "WARNING: %u listed files could not be read",
                     select_plural (n_open_or_read_failures))),
                   n_open_or_read_failures);

          if (n_mismatched_checksums != 0)
            error (0, 0,
                   (ngettext
                    ("WARNING: %u computed checksum did NOT match",
                     "WARNING: %u computed checksums did NOT match",
                     select_plural (n_mismatched_checksums))),
                   n_mismatched_checksums);
        }
    }

  return (n_properly_formatted_lines != 0
          && n_mismatched_checksums == 0
          && n_open_or_read_failures == 0
          && (!strict || n_improperly_formatted_lines == 0));
}


int
main (int argc, char **argv)
{
  unsigned char bin_buffer_unaligned[DIGEST_BIN_BYTES + DIGEST_ALIGN];
  /* Make sure bin_buffer is properly aligned. */
  unsigned char *bin_buffer = ptr_align (bin_buffer_unaligned, DIGEST_ALIGN);
  bool do_check = false;
  int opt;
  bool ok = true;
  int binary = -1;
  bool prefix_tag = false;

  digest_hex_bytes = DIGEST_HEX_BYTES;
  min_digest_line_length = MIN_DIGEST_LINE_LENGTH;

  char *file = argv[1];

  if (do_check)
    ok &= digest_check (file);

}
