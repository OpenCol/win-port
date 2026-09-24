/* join_path -- shipped at 1040:6862.
 *
 * Ported by tools/port.py from matched/game/join_path.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_backslash[];                           DGROUP:0x1d93
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * STRUPR(char *s);


char * join_path(char *dst, char *dir, char *name)
{
    char *p;
    char buf[0x50];

    strcpy(buf, dir);
    p = buf;
    while (*p != 0)
        p++;
    p--;
    if ((col_i16)*p != '\\')
        strcat(buf, g_backslash);
    strcpy(dst, buf);
    strcat(dst, name);
    STRUPR(dst);
    return dst;
}
