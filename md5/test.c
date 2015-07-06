#include <stdio.h>
#include <libintl.h>

#define ULONG_MAX (unsigned long)(0-1)

int main()
{
unsigned long t=0;
t= t-1;
printf(gettext("t values is %u.\n"), ULONG_MAX);
}
