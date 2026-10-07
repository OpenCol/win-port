/* PORT: this file's own declarations of get_check_enabled_1070_997b removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of ctl_kind_1070_9669, get_button_enabled_1070_981e, get_win_1070_95f0 removed: the header declares them from the definition (W1 step 3). */
/* focus_walk_siblings: candidate for 1070:9050 (570 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: focus_walk_siblings
 * name-source: descriptive
 *
 * Walks a dialog's child windows from `hwnd` in one direction -- 2 is forward through
 * `next_sibling_window`, anything else backward through `prev_sibling_window`,
 * both wrapping -- and gives the keyboard focus to the first control that will take it.
 * A zero `hwnd` means "from the focus", and a focus outside the dialog (IsChild against the
 * window at +2 of `get_win_1070_95f0`) means "from the end": the first child going forward, the
 * last going back. A dialog with no control list at +0x8c returns before any of that.
 *
 * THE WINDOW HANDED IN IS TRIED LAST, NOT FIRST. A non-zero `hwnd` (or the focus) is stepped
 * past before the loop, and the loop stops on coming back round to the window after it -- so
 * the lap ends on `hwnd` itself. A caller that wants a control tried FIRST hands in its
 * predecessor, which is what `focus_at_or_after` does. Starting from the end of the
 * list there is no step: the first (or last) child is the first candidate.
 *
 * Invisible windows are skipped. A window that is one of this layer's own controls is looked up
 * in the list, and takes the focus when it is a button (kind 6) with its enabled word at +0x1a
 * set, a check box (kind 2) with its word at +0x20 set, an edit box (kind 4), or kind 7. The
 * kind accessor is called afresh for every comparison, up to four times per control.
 *
 * A window `hwnd_is_ms_control` claims -- an MSCONTROLCLASS window of kind 3, a radio group --
 * is walked as a group: the walk goes on while `same_control_owner` says each window
 * shares the previous one's owner, and focuses the member `control_is_current` says is
 * the selected one. When the group ends first, the function CALLS ITSELF from the group's last
 * member and stops there; the recursion is the only way the walk resumes after a group.
 *
 * NEITHER THE GROUP WALK NOR THE RECURSION CARRIES THE LAP'S STARTING WINDOW. Each recursive call
 * begins a fresh lap, so a radio group with no selected member, in a dialog where nothing else
 * will take the focus, comes round to the same group and recurses again without end -- and a
 * dialog holding nothing but such a group never leaves the inner loop at all.
 *
 * Only one of the exits is a written-out `return` inside the loop (the walk coming back to its
 * start); the focus being set, the recursion and the group running out are all `break`s, which
 * is why they share the final epilogue instead of carrying their own (rule 44). The null-list
 * test is an `if`/`else` whose else arm is the `return`, so its epilogue lands between the two
 * arms of the outer `if`.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */


void focus_walk_siblings(DialogWin *o, unsigned hwnd, int dir)
{
    void *ctl;
    unsigned start;
    register unsigned h;
    register unsigned last_member;

    h = hwnd;
    if (h == 0) {
        h = GetFocus();
        if (IsChild(get_win_1070_95f0(o)->hwnd, h) == 0)
            h = 0;
    }
    if (h == 0) {
        if (o->first_ctl != 0) {
            if (dir == 2)
                h = GetWindow(get_win_1070_95f0(o)->hwnd, GW_CHILD);
            else
                h = GetWindow(GetWindow(get_win_1070_95f0(o)->hwnd, GW_CHILD), GW_HWNDLAST);
        } else {
            return;
        }
    } else {
        if (dir == 2)
            h = next_sibling_window(h);
        else
            h = prev_sibling_window(h);
    }
    start = h;
    while (IsWindow(h)) {
        if (IsWindowVisible(h) == 0) {
            if (dir == 2)
                h = next_sibling_window(h);
            else
                h = prev_sibling_window(h);
        } else if (hwnd_is_ms_control(h) == 0) {
            ctl = list_find_by_id(h, o->first_ctl);
            if (ctl != 0) {
                if ((ctl_kind_1070_9669((DlgControl *)ctl) == 6 && get_button_enabled_1070_981e((SButton *)ctl) != 0)
                 || (ctl_kind_1070_9669((DlgControl *)ctl) == 2 && get_check_enabled_1070_997b((SCheckBox *)ctl) != 0)
                 || ctl_kind_1070_9669((DlgControl *)ctl) == 4
                 || ctl_kind_1070_9669((DlgControl *)ctl) == 7) {
                    SetFocus(h);
                    break;
                }
            }
            if (dir == 2)
                h = next_sibling_window(h);
            else
                h = prev_sibling_window(h);
        } else {
            last_member = h;
            while (IsWindow(h)) {
                if (same_control_owner(last_member, h) != 0) {
                    if (control_is_current(h) != 0) {
                        SetFocus(h);
                        break;
                    } else {
                        last_member = h;
                        if (dir == 2)
                            h = next_sibling_window(h);
                        else
                            h = prev_sibling_window(h);
                    }
                } else {
                    focus_walk_siblings(o, last_member, dir);
                    break;
                }
            }
            break;
        }
        if (h == start)
            return;
    }
}
