/* win_load_cursor: candidate for 1038:e331 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the cursor of the window the object holds at +6: MSSetCursor_id(o->win, id), which loads
 * cursor `id` into the window's hcursor word (+0xc).
 *
 * TRANSFERRED from `win_set_drag_height_1048_d714` (1048:d714), byte-identical except for the one relocated call:
 * this copy reaches `MSSetCursor_id` where that one reaches `win_set_drag_height_1078_2aef`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 *
 * RENAMED 2026-09-27 from win_load_cursor_1038_e331: the address suffix went with the family: its window-level sibling is now MSSetCursor_id, so this object-level copy is the only win_load_cursor.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
extern void MSSetCursor_id(MSWin *win, int id);

void win_load_cursor(DialogWin *o, int id)
{
    MSSetCursor_id((MSWin *)o->win, id);
}
