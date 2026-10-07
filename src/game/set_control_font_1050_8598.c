/* set_control_font_1050_8598: candidate for 1050:8598 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores a 32-bit value at SEG34:0x2ace, one of FOUR identical setters back to back at
 * 1050:8598, 85b5, 85d2 and 85ef writing 0x2ace, 0x2aca, 0x2ad2 and 0x2ad6. The four are the
 * text layer's font slots: `radio_wndproc` hands 0x2ace to `font_handle_1070_9806` and then
 * `MSHLock` and `SelectObject`, and `text_row_rect` does the same with 0x2ad2.
 *
 * The store is one 32-bit `MOV` under `-3`, which is what says the slot is four bytes and not
 * two words.
 */

#include "colonization.h"

void set_control_font_1050_8598(void *font)
{
    g_control_font = font;
}
