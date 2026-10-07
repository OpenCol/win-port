/* menu_insert_auto: candidate for 1048:e238 (40 bytes).  Verdict: RELOCATABLE.
 *
 * menu_insert_auto_id with the menu handle at +0 of the record.
 *
 * The label is ONE `const char *`, not two ints: `menu_insert{,_auto_id}_1070` take it that
 * way and hand it to `InsertMenu`, and `StartCheat` pushes its "Cheat" menu
 * title through this forwarder (the eleven item labels go through `menu_insert_1048_e260`). A forwarder cannot show the split itself (§119).
 */

#include "colonization.h"

#include "classes.h"

void menu_insert_auto(MenuClass *menu_owner, int pos, int item, const char *text)
{
    menu_insert_auto_id(menu_owner->hmenu, pos, item, text);
}
