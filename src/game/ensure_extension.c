/* ensure_extension -- shipped at 1040:6773.
 *
 * Ported by tools/port.py from matched/game/ensure_extension.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * strrchr_rtl(const char *s, col_i16 c);

extern char * STRUPR(char *s);

void ensure_extension(char *path, char *ext)
{
    char *dot;

    dot = strrchr_rtl(path, '.');
    if (dot == 0) {
        strcat(path, ".");
        strcat(path, ext);
    }
    STRUPR(path);
}
