/* submenu_of_1048_e8c1: candidate for 1048:e8c1 (35 bytes).  Verdict: RELOCATABLE.
 * size-source: 35, not the census 31. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * `MSGetSubMenu(o->hmenu, pos)` for the menu handle at +0 of the object: the submenu at a
 * 1-based position, since `MSGetSubMenu` (1070:d9e3) is the layer that subtracts the one.
 * Its only caller is `track_popup_submenu` two hundred bytes above, which passes 0 and so
 * asks `GetSubMenu` for position -1; that is the shipped code's own arithmetic, not this
 * reconstruction's.
 */

#include "colonization.h"

#include "classes.h"

unsigned submenu_of_1048_e8c1(MenuClass *menu, int pos)
{
    return MSGetSubMenu(menu->hmenu, pos);
}
