/* parse_hotkey_marker: candidate for 1070:06f9 (202 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: parse_hotkey_marker
 *
 * Finds the '~' in a menu label, removes it by shifting the rest of the string down, and
 * records the position (+0x18) and the lower-cased character that followed it (+0x17);
 * no marker leaves the position at -1.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void parse_hotkey_marker(DlgControl *it, char *s)
{
    char *p = s;
    char buf[4];
    int i;

    it->hotkey_pos = 0;
    while (*p != 0) {
        if ((int)*p == '~') {
            i = it->hotkey_pos;
            while (STRLEN(s) > i) {
                s[i] = s[i + 1];
                i++;
            }
            break;
        } else {
            it->hotkey_pos++;
            p++;
        }
    }
    if (STRLEN(s) <= it->hotkey_pos)
        it->hotkey_pos = -1;
    else {
        buf[0] = s[it->hotkey_pos];
        buf[1] = 0;
        STRLWR(buf);
        it->hotkey = buf[0];
    }
}
