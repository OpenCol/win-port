/* PORT: this file's own declarations of get_cur_item_1050_848f removed: the header declares it from the definition (W1 step 3). */
/* button_click: candidate for 1050:31dd (316 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Handles a command id for the current dialog (pointer at DGROUP:0x2500). Ids from
 * 100 up map to a selection code: past the count at +0x80 they record an extra index at
 * +0xaa with code 0, otherwise the code comes from the 4-byte table at +0x3c4. Code 1 sets
 * flag 0x80 at +0x86; code 2 stores -1 at +0xa8; any other code resets +0xa8 when the
 * mode at +0x7e is 1 or less or +0x7c is set, and, when +0x7c is set and neither 0x1000
 * nor 0x4 is in the flags, finds the n-th item of the list at +0x36e (n from the list
 * head at +0x3a0) and stores its +2 field plus 1. Finally popup_done sets bit 0x400 in the dialog's flags and ends its modal session.
 *
 * RENAMED 2026-09-27 from dialog_menu_command: CONST PROPOSAL; the name is taken by 1070:1dbb, which vacates it first (-> MSPushButton). Mac button_click(short), CODE 19 0x3abe, 288 bytes. Decoded, it matches 31dd line for line: return if the current popup is null; subtract 0x64; if past n_buttons (+0x92), store id-n+1 in +0xbc with code 0, else take the code from the 4-byte table at +0x3d8; code 1 ORs 0x80 into +0x98; code 2 stores -1 in +0xba; otherwise it clears +0xba when mode (+0x90) <= 1 or +0x8e is set, and when +0x8e is set and (+0x98 & 0x1004) == 0 it walks the list to the n-th item and stores its +2 field plus one; finally popup_done. The reconstruction's header describes the same steps at the Windows offsets (100, +0x80, +0x3c4, 0x1000|0x4, +0x36e). Callees: popup_done against popup_end_modal (plus the item-list helper). Arity: (short) against (int id). Layout: after DrawSpriteSquare / list_highlight and before port_update / redraw_current_popup, then Popup::make_window, on both. Size 288 against 316. Constants: 0x1004 is shared.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void popup_done(void);
void button_click(int id)
{
    int code;
    int n;
    PopupItem *item;
    PopupItem *found;
    int i;

    if (g_cur_dialog == 0)
        return;
    id -= 100;
    if (g_cur_dialog->n_buttons <= id) {
        g_cur_dialog->extra_button = id - g_cur_dialog->n_buttons + 1;
        code = 0;
    } else {
        code = g_cur_dialog->btn[id].cmd;
    }
    if (code == 1)
        g_cur_dialog->flags |= 0x80;
    switch (code) {
    case 2:
        g_cur_dialog->result = -1;
        break;
    default:
        if (g_cur_dialog->n_sprites <= 1 || g_cur_dialog->n_items != 0)
            g_cur_dialog->result = 0;
        if (g_cur_dialog->n_items != 0 && !(g_cur_dialog->flags & 0x1004)) {
            i = 0;
            found = 0;
            item = g_cur_dialog->items;
            n = get_cur_item_1050_848f(g_cur_dialog->radios);
            for (; item != 0; item = item->next) {
                if (i == n)
                    found = item;
                i++;
            }
            if (found != 0)
                g_cur_dialog->result = found->id + 1;
        }
        break;
    }
    popup_done();
}
