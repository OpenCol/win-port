/* set_button_font_1010_ea9e: candidate for 1010:ea9e (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores a 32-bit value at SEG34:0x2aca, one of FOUR identical setters back to back at
 * 1050:8598, 85b5, 85d2 and 85ef writing 0x2ace, 0x2aca, 0x2ad2 and 0x2ad6. The four are the
 * text layer's font slots: `radio_wndproc` hands 0x2ace to `font_handle_1070_9806` and then
 * `MSHLock` and `SelectObject`, and `text_row_rect` does the same with 0x2ad2.
 *
 * The store is one 32-bit `MOV` under `-3`, which is what says the slot is four bytes and not
 * two words.
 *
 * TRANSFERRED: byte-identical to `set_button_font_1050_85b5` (1050:85b5), one of 4 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

void set_button_font_1010_ea9e(void *font)
{
    g_button_font = font;
}
