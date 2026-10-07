/* submenu_of_1060_a938: candidate for 1060:a938 (35 bytes).  Verdict: RELOCATABLE.
 * size-source: 35, not the census 31. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * `MSGetSubMenu(o->hmenu, pos)` for the menu handle at +0 of the object: the submenu at a
 * 1-based position, since `MSGetSubMenu` (1070:d9e3) is the layer that subtracts the one.
 * Its only caller is `submenu_append_item` (1060:8f99), which passes its own index argument
 * through; it is `track_popup_submenu` (1048:e1fd), calling the 1048 copy, that passes 0 and
 * so asks `GetSubMenu` for position -1.
 *
 * TRANSFERRED: byte-identical to `submenu_of_1048_e8c1` (1048:e8c1), one of 3 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "classes.h"

unsigned submenu_of_1060_a938(MenuClass *menu, int pos)
{
    return MSGetSubMenu(menu->hmenu, pos);
}
