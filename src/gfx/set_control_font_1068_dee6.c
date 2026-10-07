/* set_control_font_1068_dee6: candidate for 1068:dee6 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `set_button_font_1068_dec9`'s twin, four bytes along: the text layer's other font slot at
 * SEG34:0x2ace, which `radio_wndproc` is the reader of.
 */

#include "colonization.h"

void set_control_font_1068_dee6(void *font)
{
    g_control_font = font;
}
