/* MSModifyMenu_1048_e300: candidate for 1048:e300 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * MSModifyMenu with the menu handle at +0 of the record, a long and two
 * words.
 *
 * RENAMED 2026-09-26 from menu_call_da9f: MSModifyMenu on the menu record's handle, the rest passed
 * through; MainWin_MainMenuProc and the orders-menu builders use it to relabel entries.
 *
 * RENAMED 2026-09-27 from menu_modify: the Mac build's MSModifyMenu_1048_e300, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: MSModifyMenu on the MenuClass's hmenu (+0) with a long id and text; the object-level form matches Mac MSModifyMenu(MSMenu**, long, char*) exactly. MSModifyMenu is already the hmenu-level 1070:da9f, so this takes the address suffix, the precedent MSEnableItem_1048_e2dd set in 0117ccaa6.
 */

#include "colonization.h"

#include "classes.h"

void MSModifyMenu_1048_e300(MenuClass *menu, long id, char *text)
{
    MSModifyMenu(menu->hmenu, id, text);
}
