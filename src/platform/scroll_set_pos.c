/* scroll_set_pos: candidate for 1070:5fe3 (36 bytes).  Verdict: RELOCATABLE.
 * symbol: scroll_set_pos
 *
 * Sends WM_COMMAND (0x111) with code 0x7f and v as the long parameter to
 * the window through scrollbar_message (-called, so defined in-file as a stub).
 */

#include "colonization.h"

extern int g_stub_a;

void scroll_set_pos(int h, int pos)
{
    scrollbar_message(h, 0x111, 0x7f, (long)pos);
}
