/* set_default_font_ref: candidate for 1050:85d2 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores the default font reference at SEG34:0x2ad2 -- which is DGROUP, the NE header's
 * auto-data segment being 34, and both spellings appear in the corpus.
 *
 * ONE OF FOUR IDENTICAL SETTERS back to back, and `Popup_make_controls` calls all four in
 * a row with the same `p->font`, which is what says they are one family:
 *
 *   1050:8598  0x2ace  set_control_font_1050_8598   read by radio_wndproc, static_text_wndproc
 *   1050:85b5  0x2aca  set_button_font_1050_85b5    read by MSInitButton
 *   1050:85d2  0x2ad2  set_default_font_ref         read by text_row_rect, font_row_rect,
 *                                                   checkbox_wndproc
 *   1050:85ef  0x2ad6  set_editbox_font_1050_85ef   read by create_MSEditBoxClass_window
 *
 * Each reader hands its slot to `font_handle_*` and then `MSHLock`, so the four differ only
 * in which control kind reads them -- `radio_wndproc`'s header makes the point the other
 * way round, that its font is 0x2ace "not the check box's 0x2ad2".
 *
 * The store is one 32-bit `MOV` under `-3`, which is what says the slot is four bytes and not
 * two words.
 */

#include "colonization.h"

#include "classes.h"

void set_default_font_ref(Font *font)
{
    g_default_font_ref = font;
}
