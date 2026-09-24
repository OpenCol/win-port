/* parse_hotkey_marker -- shipped at 1070:06f9.
 *
 * Ported by tools/port.py from matched/platform/parse_hotkey_marker.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Item dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * STRLWR(char *s);

void parse_hotkey_marker(Item *it, char *s)
{
    char *p = s;
    char buf[4];
    col_i16 i;

    it->pos = 0;
    while (*p != 0) {
        if ((col_i16)*p == '~') {
            i = it->pos;
            while (strlen(s) > i) {
                s[i] = s[i + 1];
                i++;
            }
            break;
        } else {
            it->pos++;
            p++;
        }
    }
    if (strlen(s) <= it->pos)
        it->pos = -1;
    else {
        buf[0] = s[it->pos];
        buf[1] = 0;
        STRLWR(buf);
        it->hotkey = buf[0];
    }
}
