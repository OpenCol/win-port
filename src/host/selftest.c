/* selftest.c -- the W0 bootstrap check, built by every toolchain and run under Wine in CI.
 *
 * It holds the roadmap's section 1 decisions to the toolchain actually in use: the first
 * architecture is 32-bit x86 (the game casts pointers through `long`), and the game's
 * integers are 16 bits wide. It prints what it found and exits 1 on any mismatch.
 */
#include <stdio.h>
#include <windows.h>
#include "port/types.h"

static int check(const char *what, long got, long want)
{
    printf("%-28s %ld%s\n", what, got, got == want ? "" : "  <-- expected different");
    return got == want ? 0 : 1;
}

int main(void)
{
    int bad = 0;
    bad += check("sizeof(void *)", (long)sizeof(void *), 4);
    bad += check("sizeof(long)", (long)sizeof(long), 4);
    bad += check("sizeof(i16)", (long)sizeof(i16), 2);
    bad += check("sizeof(u16)", (long)sizeof(u16), 2);
    bad += check("(i16)0x8000 < 0", (long)((i16)0x8000 < 0), 1);
    bad += check("(u16)(0xffff + 1)", (long)(u16)(0xffff + 1), 0);
    bad += check("GetVersion() != 0", (long)(GetVersion() != 0), 1);
    printf("%s\n", bad ? "selftest FAILED" : "selftest ok");
    return bad ? 1 : 0;
}
