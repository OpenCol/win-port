/* MSModifyMenu: candidate for 1070:da9f (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 43, not the 44 this file carried while it was parked. The next census entry
 * begins at 1070:daca, which is 0x2b bytes on, and the reconstruction consumes exactly that.
 *
 * `ModifyMenu(hmenu, id, MF_BYCOMMAND, id, text)` on a non-null menu handle: the item keeps
 * its command id and gets new text. Like the rest of this family -- `MSDeleteMenu` beside
 * it -- it addresses items by command rather than by position, which is why nothing here
 * subtracts one.
 *
 * PARKED SINCE 2026-09-12 ON ONE REGISTER, and the Macintosh signature is what unparked it.
 * The shipped code enregisters the handle in SI and pushes the id TWICE straight from
 * `[bp+8]`; every spelling tried enregistered both and pushed SI and DI. The earlier note
 * ruled out `register` on the handle, the inverted `if`, `-r-` and dropping `-Ot`, and
 * concluded Borland was choosing two registers where the shipped build chose one.
 *
 * THE ID IS A `long`. `MSModifyMenu(MSMenu**, long, char*)` is in macui, and it accounts for
 * every byte that did not fit. Both pushes are `(unsigned)id` -- a conversion, not a
 * variable, so there is nothing for Borland to enregister and it reloads the low half each
 * time. The word at `[bp+0xa]` that the parked version had to call an unused parameter is
 * the id's HIGH half, which this function has no use for. And the argument total agrees:
 * handle 2 + long 4 + pointer 4 = 10, which is what the call site clears.
 *
 * That is §119's width blind spot exactly -- a parameter whose low half is all the callee
 * touches reads equally well as an `int` -- and the fix for it came from the same place as
 * the fix for `GameFont::GameFont` earlier today: look at what the caller pushes.
 * `MSModifyMenu_1048_e300` pushes one `DWORD` where the parked reading needed two words.
 */

#include "colonization.h"

void MSModifyMenu(unsigned hmenu, long id, char *text)
{
    if (hmenu != 0)
        ModifyMenu(hmenu, (unsigned)id, 0, (unsigned)id, text);
}
