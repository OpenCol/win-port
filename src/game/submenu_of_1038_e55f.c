/* submenu_of_1038_e55f: candidate for 1038:e55f (35 bytes).  Verdict: RELOCATABLE.
 * size-source: 35, not the census 31. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * `MSGetSubMenu(o->hmenu, pos)` for the menu handle at +0 of the object: the submenu at a
 * 1-based position, since `MSGetSubMenu` (1070:d9e3) is the layer that subtracts the one.
 * Its only caller is `track_popup_submenu_1038_e2b5` (1038:e2b5), which passes its own
 * `submenu_pos` argument through; it is the 1048 twin `track_popup_submenu` that passes 0
 * and so asks `GetSubMenu` for position -1.
 *
 * TRANSFERRED: byte-identical to `submenu_of_1048_e8c1` (1048:e8c1), one of 3 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "classes.h"

unsigned submenu_of_1038_e55f(MenuClass *menu, int pos)
{
    return MSGetSubMenu(menu->hmenu, pos);
}
