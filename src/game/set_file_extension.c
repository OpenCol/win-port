/* set_file_extension: candidate for 1040:67d8 (138 bytes).  Verdict: RELOCATABLE.
 *
 * Copies src to dst unless they are the same buffer, cuts dst at its last '.', appends
 * "." and the extension, and upper-cases the result.
 */

#include "colonization.h"

void set_file_extension(char *dst, char *src, char *ext)
{
    char *p;

    if (src != dst)
        STRCPY(dst, src);
    p = strrchr_rtl(dst, '.');
    if (p != 0)
        *p = 0;
    STRCAT(dst, ".");
    STRCAT(dst, ext);
    STRUPR(dst);
}
