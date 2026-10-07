/* ClearEventTable_1020_b1d2: candidate for 1020:b1d2 (289 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Clears an EventTable (0x5c bytes, twenty-three handler slots, on_mousemove .. on_vthumb) one field at a
 * time; the compiler reloads the pointer for every store.
 *
 * TRANSFERRED: byte-identical to `ClearEventTable_1010_3036` (1010:3036), one of 14 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-26 from event_table_clear_1020_b1d2: the Mac build's EventTable::ClearEventTable(void): this only, clearing the 0x5c-byte table field by field. One of the per-module copies, so it keeps its address suffix. The class prefix is left off: Borland keeps only 32 characters of a public symbol, and _EventTable_ClearEventTable_ssss_oooo would be 37.
 */

#include "colonization.h"

#include "classes.h"

void ClearEventTable_1020_b1d2(EventTable *table)
{
    table->on_mousemove = 0;
    table->on_lbutton_down = 0;
    table->on_lbutton_up = 0;
    table->on_lclick = 0;
    table->on_rbutton_down = 0;
    table->on_rbutton_up = 0;
    table->on_rclick = 0;
    table->on_ldblclk = 0;
    table->on_keydown = 0;
    table->on_keyup = 0;
    table->on_keydown_after = 0;
    table->on_char = 0;
    table->on_close = 0;
    table->on_minimize = 0;
    table->on_restore = 0;
    table->on_setfocus = 0;
    table->on_size = 0;
    table->on_move = 0;
    table->on_commnotify = 0;
    table->on_hscroll = 0;
    table->on_hthumb = 0;
    table->on_vscroll = 0;
    table->on_vthumb = 0;
}

