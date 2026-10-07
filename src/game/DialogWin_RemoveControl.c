/* PORT: this file's own declarations of ctl_id_1060_abca removed: the header declares it from the definition (W1 step 3). */
/* DialogWin_RemoveControl: candidate for 1060:99dc (205 bytes, 8 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Removes the item whose id matches from the singly linked list at +0x8c: the head
 * directly, otherwise by relinking the predecessor's next (+0x12). Exits are `goto`s.
 *
 * RENAMED 2026-09-26 from list_unlink_by_id: the Mac build's DialogWin::RemoveControl(short): this plus an id, unlinked from the control list at +0x8c. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern DlgControl *ctl_next_1060_abe3(DlgControl *item);

void DialogWin_RemoveControl(DialogWin *list, int id)
{
    DlgControl *ctl;

    if (list->first_ctl == 0)
        return;
    else {
        ctl = list->first_ctl;
        if (ctl_id_1060_abca(ctl) == id)
            list->first_ctl = (DlgControl *)ctl->next;
        else
            while (ctl != 0) {
                if (ctl_next_1060_abe3(ctl) == 0)
                    goto done;
                if (ctl_id_1060_abca((DlgControl *)(ctl_next_1060_abe3(ctl))) == id) {
                    ctl->next = ctl_next_1060_abe3((DlgControl *)(ctl_next_1060_abe3(ctl)));
                    goto done;
                }
                ctl = ctl_next_1060_abe3(ctl);
            }
    }
done:
    ;
}
