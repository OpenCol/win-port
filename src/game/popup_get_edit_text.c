/* PORT: this file's own declarations of ctl_get_text removed: the header declares it from the definition (W1 step 3). */
/* popup_get_edit_text: candidate for 1050:0fd8 (49 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Reads the text of edit box i of the popup's 40-byte edit-box array (pointer at +0x3a8) into the buffer p, through ctl_get_text.
 *
 * RENAMED 2026-09-26 from ctl_item_apply: Calls ctl_get_text on edit box i of the popup's edit-box array (+0x3a8, PopupEdit), and ctl_get_text is get_window_text on the control's hwnd, i.e. WM_GETTEXTLENGTH plus WM_GETTEXT into p. So it READS edit box i's text into the caller's buffer p. 'apply p to item i' says the opposite direction (writing p into the item). Its forwarder ctl_item_apply_first (1050:1009, item 0) has the same problem.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
/* same object: 4-byte `push cs; call ` (rule 47) */
void popup_get_edit_text(Popup *control, int i, void *p)
{
    ctl_get_text(&control->edit_boxes[i], p);
}
