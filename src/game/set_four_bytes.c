/* set_four_bytes: candidate for 1050:0090 (38 bytes).  Verdict: RELOCATABLE.
 *
 * Stores four byte parameters at DGROUP:0x24f9, 0x24f8, 0x24fa and 0x24fb.
 */

#include "colonization.h"

void set_four_bytes(char text_color, char hilite_color, char c, char d)
{
    g_popup_text_color = text_color;
    g_popup_hilite_color = hilite_color;
    g_popup_m2 = c;
    g_popup_m3 = d;
}
