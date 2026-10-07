/* alert_box_1070_66cf: candidate for 1070:66cf (238 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 238, the full gap, not the census 153. The 85 bytes past the census are the
 * dead second epilogue and then the TWO CS-relative jump tables this function's `switch`es
 * need -- at +0xd8 and +0xe6 -- which Borland puts in the code segment and Ghidra's
 * reachable-code size excludes. A first grading stopped at 185 and matched, because the code
 * ends there; `re_agent/check_sizes.py` is what caught it, by finding a switch table 31 bytes
 * past the stated end and saying so.
 *
 * A message box behind the layer's own vocabulary: the caller names an icon and a button set
 * by index, this turns them into Win16 style bits, and the answer comes back renumbered.
 * `MB_TASKMODAL` (0x2000) is added unconditionally, and the owner window is a literal 0.
 *
 * Three switches in one function, and the bodies of each are emitted in SOURCE order while
 * the comparisons are ordered by value -- which is how the source order of the second one is
 * recoverable at all: its cases run 4, 8, 0x10, 1, 2, 0x20 in the file and 1, 2, 4, 8, 0x10,
 * 0x20 in the compare chain.
 *
 * The icon switch is dense (0..3) and gets a jump table; the button switch is sparse and gets
 * a binary compare chain -- `CMP AX,8 / JZ / JG` splitting the range first. Both are the same
 * `switch` keyword.
 *
 * The answer mapping is 1..7 to 0,1,4,5,6,2,3 -- the Win16 IDOK..IDNO renumbered so that
 * yes and no come out 2 and 3 rather than 6 and 7.
 */

#include "colonization.h"

int alert_box_1070_66cf(char *caption, char *text, int icon, int buttons)
{
    register int style;
    register int r;
    int id;

    style = 0;
    switch (icon) {
    case 0:
        style = 0x40;
        break;
    case 1:
        style = 0x20;
        break;
    case 2:
        style = 0x30;
        break;
    case 3:
        style = 0x10;
        break;
    }
    switch (buttons) {
    case 4:
        style |= 4;
        break;
    case 8:
        style |= 3;
        break;
    case 0x10:
        style |= 2;
        break;
    case 1:
        break;
    case 2:
        style |= 1;
        break;
    case 0x20:
        style |= 5;
        break;
    }
    id = MessageBox(0, text, caption, style | 0x2000);
    switch (id) {
    case 1:
        r = 0;
        break;
    case 2:
        r = 1;
        break;
    case 6:
        r = 2;
        break;
    case 7:
        r = 3;
        break;
    case 4:
        r = 5;
        break;
    case 3:
        r = 4;
        break;
    case 5:
        r = 6;
        break;
    }
    return r;
}
