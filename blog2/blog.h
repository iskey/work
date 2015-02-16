/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    blog.h
 * @brief   simpe blog macros for C.
 * @author  iskey@outlook.com
 * @version Initial Draft
 * @note    none
 * @date    2013/10/31
 */
/******************************************************************************
 *  Function List :
 *  History       :
 *  1.Date        : 2013/10/31
 *    Author      : iskey
 *    Modification: Created file
 *
******************************************************************************/

/** external variables */

/** external routine prototypes */

/** internal routine prototypes */

/** project-wide global variables */

/** module-wide global variables */

/** constants */

/** macros */

/** routines' implementations */

#ifndef __B__LOG__
#define __B__LOG__

#include "stdio.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLOG_STR_(a,b,c) a#b#c
#define BLOG_STR(a,b) a b"\n"

#define LOG_NOW	LOG_DBG
#define LOG_DBG	20
#define LOG_INF	40
#define LOG_NOT	60
#define LOG_WAR	80
#define LOG_ERR	100
#define LOG_FAT	120

extern FILE* g_blog_stdout;

#if (LOG_NOW<=LOG_FAT)
    #define _LOG_FAT(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_FAT: FILE:%s, %s():%d,",format), __FILE__,__func__,__LINE__,##__VA_ARGS__);
#else
    #define _LOG_FAT(format,...)
#endif
#if (LOG_NOW<=LOG_ERR)
    #define _LOG_ERR(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_ERR: FILE:%s, %s():%d,",format), __FILE__,__func__,__LINE__,##__VA_ARGS__);
#else
    #define _LOG_ERR(format,...)
#endif
#if (LOG_NOW<=LOG_INF)
    #define _LOG_INF(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_INF: FILE:%s, %s():%d,",format), __FILE__,__func__,__LINE__,##__VA_ARGS__);
#else
    #define _LOG_INF(format,...)
#endif
#if (LOG_NOW<=LOG_WAR)
    #define _LOG_WAR(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_WAR: FILE:%s, %s():%d,",format), __FILE__,__func__,__LINE__,##__VA_ARGS__);
#else
    #define _LOG_WAR(format,...)
#endif
#if (LOG_NOW<=LOG_DBG)
    #define _LOG_DBG(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_DBG: FILE:%s, %s():%d,",format), __FILE__,__func__,__LINE__,##__VA_ARGS__);
#else
    #define _LOG_DBG(format,...)
#endif
#if (LOG_NOW<=LOG_NOT)
    #define _LOG_NOT(format,...) fprintf(g_blog_stdout, BLOG_STR("LOG_NOT: FILE:%s, %s():%d, %s:%s,",format), __FILE__,__func__,__LINE__, __DATE__,__TIME__,##__VA_ARGS__);
#else
    #define _LOG_NOT(format,...)
#endif


#define blog(level, format, ...) _##level(format, ##__VA_ARGS__);
int blog_init(char *stdout_file, char *blog_file);
int blog_destroy(void);

#ifdef __cplusplus
}
#endif

#endif//End of __B__LOG__
