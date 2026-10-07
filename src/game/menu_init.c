/* menu_init: candidate for 1048:e111 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Builds menu resource `id` with `MSInitMenu` and stores the handle in the word the first
 * argument points at. Its caller, `MainWin_Init`, passes `(&o->menu, 0x80)`: MainWin's
 * MenuClass member, whose first word is the hmenu.
 */

#include "colonization.h"

#include "classes.h"

void menu_init(MenuClass *m, int id)
{
    m->hmenu = MSInitMenu(id);
}
