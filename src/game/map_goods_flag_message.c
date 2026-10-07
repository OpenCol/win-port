/* map_goods_flag_message: candidate for 1010:c0e5 (97 bytes).  Verdict: RELOCATABLE.
 * symbol: map_goods_flag_message
 *
 * The same "goods X on/off" message put on the map window (SEG20:0x658)
 * after cancelling the colony window's: colony_message 0xf or 0x10 by
 * `flag`, the goods name, activated as kind 1.  The two helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
/* PORT: this file's own ColonyWin removed: include/gen/classes.h define it once (W1 step 2). */

#include "classes.h"

extern int g_stub_a;
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int idx);

void map_goods_flag_message(int goods, int unused, int flag)
{
    ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
    if (flag != 0)
        colony_message(CMSG_LOADING);
    else
        colony_message(CMSG_UNLOADING);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_goods_name_word[goods]);
    colony_activate_message(1, 0L);
}
