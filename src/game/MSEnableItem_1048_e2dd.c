/* MSEnableItem_1048_e2dd: candidate for 1048:e2dd (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `menu_enable_item` on the menu handle the object holds at +0: the item id
 * as a `long` and the enable flag as a byte. One of TWO copies.
 *
 * The callee is a call into segment 1070 and is `extern`.
 *
 * RENAMED 2026-09-27 from menu_enable_item_1048_e2dd: the Mac build's MSEnableItem(MSMenu**, long, char), bound by call-sequence alignment (5.5 votes to 1.0): it takes the menu object, a long item id and the enable flag and hands the menu's hmenu to menu_enable_item.
 */

#include "colonization.h"

#include "classes.h"
extern void menu_enable_item(unsigned hmenu, unsigned long id, char enable);

void MSEnableItem_1048_e2dd(MenuClass *menu, unsigned long id, char enable)
{
    menu_enable_item(menu->hmenu, id, enable);
}
