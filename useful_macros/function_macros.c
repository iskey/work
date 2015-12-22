#include "stdio.h"

#define ARCH_X86    1
#define ARCH_MSXP   2
#define ARCH_MIPS   3

#define ARCH ARCH_MSXP

#if (ARCH== ARCH_MSXP)
#define ARCH_FUNC(func) MSXP##_##func
#elif (ARCH== ARCH_X86)
#define ARCH_FUNC(func) X86##_##func
#elif (ARCH== ARCH_MIPS)
#define ARCH_FUNC(func) MIPS##_##func
#endif


void MSXP_hello(unsigned char *s)
{
    printf("I am msxp hello %s.\n", s);
}

void X86_hello(unsigned char *s)
{
    printf("I am X86 hello %s.\n", s);
}

void MIPS_hello(unsigned char *s)
{
    printf("I am mips hello %s.\n", s);
}

void hello(unsigned char *s)
{
    ARCH_FUNC(hello)(s);
}

int main()
{
    hello("world");
}
