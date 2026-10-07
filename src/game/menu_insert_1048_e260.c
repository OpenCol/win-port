/* menu_insert_1048_e260: candidate for 1048:e260 (43 bytes).  Verdict: RELOCATABLE.
 *
 * menu_insert with the menu handle at +0 of the record.
 *
 * The label is ONE `const char *`, not two ints: `menu_insert{,_auto_id}_1070` take it that
 * way and hand it to `InsertMenu`, and `StartCheat` pushes eleven named
 * strings through this forwarder. A forwarder cannot show the split itself (§119).
 */

#include "colonization.h"

#include "classes.h"

void menu_insert_1048_e260(MenuClass *menu_owner, int pos, int item, const char *text, unsigned id)
{
    menu_insert_1070_d855(menu_owner->hmenu, pos, item, text, id);
}
