/* radio_double_click: candidate for 1050:2c62 (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the current control's item list counting to index idx, records that item's id + 1
 * as the selection (-1 when none), and ends the popup's modal session (popup_done). The first parameter is unused.
 *
 * RENAMED 2026-09-27 from ctl_select_item_index: BEYOND THE PROPOSALS. Mac radio_double_click(short, short), CODE 19 0x34ee, 122 bytes. Its body, decoded, reads the SECOND argument (move.w $a(a6)), walks the current popup's item list at +0x380 to that index, stores -1 in +0xba or the found item's +2 field plus one, and calls popup_done. 2c62 (int unused, int idx), 141 bytes, is that code line for line (g_cur_dialog->items, result = -1 or found->id + 1, popup_end_modal). No direct callers on either side; it is installed as a handler. Layout: second in the block on both.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void popup_done(void);
void radio_double_click(int unused, int idx)
{
    PopupItem *item;
    PopupItem *found = 0;
    int i;

    if (g_cur_dialog == 0)
        return;
    i = 0;
    item = g_cur_dialog->items;
    while (item != 0) {
        if (i == idx)
            found = item;
        i++;
        item = (PopupItem *)item->next;
    }
    g_cur_dialog->result = -1;
    if (found != 0)
        g_cur_dialog->result = found->id + 1;
    popup_done();
}
