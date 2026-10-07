/* PORT: this file's own declarations of win_set_cursor_1078_2986 removed: the header declares it from the definition (W1 step 3). */
/* win_set_cursor_1038_e30a: candidate for 1038:e30a (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Installs a cursor on a window: the window's pointer at +0x6 and the cursor object's first
 * word, handed to `win_set_cursor_1078_2986`.
 *
 * THE CALLEE TAKES A FAR POINTER, NOT TWO INTS, and the bytes are what settled it. Written as
 * `win_set_cursor_1078_2986(w->a, w->b, c->hcursor)` -- which is how another file declares it --
 * this compiles with a redundant `LES BX,[BP+6]` between the two pushes, because the two window
 * words are then two separate expressions. The shipped loads that pointer ONCE and pushes both
 * halves, which is a single 32-bit argument. RELOCATABLE became EXACT on that one change.
 *
 * `MapWin_TrackMouse` reaches it with `g_cursors[r]` -- the fifteen four-byte CursorClass
 * objects `create_game_globals` defines at SEG20:0x7746 -- and `MapWin_LeftMouseDown` with the
 * first of them, which is the default. That file had read the same call as showing hover text.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

void win_set_cursor_1038_e30a(DialogWin *w, CursorClass *cursor_obj)
{
    win_set_cursor_1078_2986((MSWin *)w->win, cursor_obj->hcursor);
}
