/* join_path: candidate for 1040:6862 (160 bytes).  Verdict: RELOCATABLE.
 *
 * dst = dir + "\" + name, upper-cased; the backslash is added only when dir does not
 * already end in one. Returns dst.
 */

#include "colonization.h"

char * join_path(char *dst, char *dir, char *name)
{
    char *p;
    char buf[0x50];

    STRCPY(buf, dir);
    p = buf;
    while (*p != 0)
        p++;
    p--;
    if ((int)*p != '\\')
        STRCAT(buf, "\\");
    STRCPY(dst, buf);
    STRCAT(dst, name);
    STRUPR(dst);
    return dst;
}
