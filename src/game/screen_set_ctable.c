/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 17 of the imported file. */
/* PORT: this file's own declarations of win_select_palette_1060_b07f removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 16 of the imported file. */
/* screen_set_ctable: candidate for 1060:afa1 (45 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Stores a pointer at +2 and passes object and pointer on.
 *
 * RENAMED 2026-09-27 from obj_set_target_and_notify: the object is a Screen and +2 is its colour table (Screen_Init passes its CTable argument here; the Mac Screen::Init takes CTable&): it stores the table and selects its palette into the window through win_select_palette. The Mac build has no separate setter, so the name is descriptive.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void screen_set_ctable(Screen *o, void *p)
{
    o->ctable = p;
    win_select_palette_1060_b07f((DialogWin *)((Screen *)((DialogWin *)o)), (CTable *)p);
}
