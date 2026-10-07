/* MSEnableItem_1008_cec2: candidate for 1008:cec2 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `menu_enable_item` on the menu handle the object holds at +0: the item id
 * as a `long` and the enable flag as a byte. One of TWO copies.
 *
 * The callee is a call into segment 1070 and is `extern`.
 *
 * RENAMED 2026-09-27 from menu_enable_item_1008_cec2: the Mac build's MSEnableItem(MSMenu**, long, char), bound by call-sequence alignment (5.5 votes to 1.0): it takes the menu object, a long item id and the enable flag and hands the menu's hmenu to menu_enable_item.
 */

#include "colonization.h"

#include "classes.h"
extern void menu_enable_item(unsigned hmenu, unsigned long id, char enable);

void MSEnableItem_1008_cec2(MenuClass *menu_owner, unsigned long id, char enable)
{
    menu_enable_item(menu_owner->hmenu, id, enable);
}
