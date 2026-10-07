/* PORT: this file's own declarations of win_select_palette_1078_279f removed: the header declares it from the definition (W1 step 3). */
/* win_select_palette_1060_b07f: candidate for 1060:b07f (41 bytes).  Verdict: RELOCATABLE.
 *
 * Selects a palette into the window the object holds at +6, taken from the word at +0x404 of the second record.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_select_palette_1060_b07f(DialogWin *o, CTable *pal)
{
    win_select_palette_1078_279f((MSWin *)o->win, pal->hpal);
}
