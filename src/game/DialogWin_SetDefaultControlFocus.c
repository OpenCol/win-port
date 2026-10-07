/* DialogWin_SetDefaultControlFocus: candidate for 1060:9b3f (167 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: DialogWin_SetDefaultControlFocus
 *
 * Gives the keyboard focus to the first control of the window whose byte at +0x16 is set, falling back to the first control when none is.
 *
 * RENAMED 2026-09-27 from focus_marked_ctl: NOT PROPOSED directly (mac_bind proposed win_show_and_focus_1010_e4ff, a wrapper that calls this). Mac DialogWin::SetDefaultControlFocus(void). Layout: Mac CODE 3 DialogWin::Init, AddControl, RemoveControl, SetDefaultControlFocus, EventWin::Init; Windows 1060 DialogWin_AddControl, DialogWin_RemoveControl, [controls_unlink_all, controls_invalidate], 9b3f, [find_control, dialog_mnemonic], EventWin_Init. SEQUENCE: wherever the Mac has MSSetFocus + SetDefaultControlFocus or MSShowWindow + SetDefaultControlFocus, Windows calls a win_show_and_focus_* copy, which is MSSetFocus_of / MSShowWindow_of + 9b3f. Examples: colony_hammer_change's tail, Popup::draw's MSShowWindow/SetDefaultControlFocus, ReportWin::DoReport's pair. Arity: this. Size 167/128. Name exactly 32 characters. The 18 win_show_and_focus_* copies keep their names (Windows-only combined helpers).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern char get_initial_focus(DlgControl *ctl);
extern DlgControl * ctl_next_1060_abe3(DlgControl *ctl);

void DialogWin_SetDefaultControlFocus(DialogWin *o)
{
    DlgControl *c;

    c = o->first_ctl;
    while (c != 0) {
        if (get_initial_focus(c)) {
            if (o->keyboard_nav != 0)
                focus_at_or_after(o, c);
            return;
        }
        c = ctl_next_1060_abe3(c);
    }
    c = o->first_ctl;
    if (c != 0) {
        if (o->keyboard_nav != 0)
            focus_at_or_after(o, c);
    }
}
