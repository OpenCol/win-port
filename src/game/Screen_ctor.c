/* Screen_ctor: candidate for 1060:a41c (102 bytes, 13 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Screen::Screen()
 *
 * The constructor of Screen, a 14-byte object (13 bytes of members, word-aligned under -a):
 * the colour table, the window pointer, two shorts and a byte, all zeroed.
 * The constructed-object counter is incremented, so the class declares a destructor.
 */

#include "colonization.h"

#pragma option -a
#include "classes.h"
#pragma option -a-

Screen::Screen()
{
    win = 0;
    ctable = 0;
    fa = 0;
    style = 0;
    disabled = 0;
}
