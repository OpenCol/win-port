/* dialog_mnemonic: candidate for 1060:9c4c (412 bytes).  Verdict: RELOCATABLE.
 *
 * Presses the control whose mnemonic is `key`, lower-cased first with STRLWR on a two-byte
 * buffer. It walks the window's control list from +0x8c. A control with a non-negative word at
 * +0x18 and the key in byte +0x17 is clicked if it is an enabled button (kind 6, word +0x1a set)
 * or a checkbox (kind 2, word +0x20 set). A control with a negative +0x18 of kind 3 is a radio
 * group: its 0x92-byte items at +0x2e, counted at +0x26, are searched the same way through their
 * own key byte +0x8e and word +0x90, and a match clicks that item's window. The answer is whether
 * anything was pressed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void MSPushButton(unsigned hwnd);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int ctl_kind_1060_ac19(DlgControl *ctl);
extern int get_button_enabled_1060_ac31(DlgControl *button);
extern int ctl_hwnd_1060_ac4a(DlgControl *ctl);
extern int get_check_enabled_1060_ac63(DlgControl *check);
extern RadioItem *get_items_1060_ac7c(DlgControl *radios);
extern int radio_count_1060_ac99(DlgControl *radios);

int dialog_mnemonic(DialogWin *w, char key)
{
    DlgControl *c;
    char buf[2];
    char found;
    DlgControl *grp;
    RadioItem *items;
    int n;
    register int i;
    register int r;

    found = 0;
    r = 0;
    buf[0] = key;
    buf[1] = 0;
    STRLWR(buf);
    key = buf[0];
    for (c = w->first_ctl; c != 0 && found == 0; c = c->next) {
        if (c->hotkey_pos >= 0) {
            if (c->hotkey == key) {
                if (ctl_kind_1060_ac19(c) == 6 && get_button_enabled_1060_ac31((SButton *)c) != 0) {
                    MSPushButton(ctl_hwnd_1060_ac4a(c));
                    r = 1;
                    break;
                } else if (ctl_kind_1060_ac19(c) == 2 && get_check_enabled_1060_ac63((SCheckBox *)c) != 0) {
                    checkbox_click(ctl_hwnd_1060_ac4a(c));
                    r = 1;
                    break;
                }
            }
        } else if (ctl_kind_1060_ac19(c) == 3) {
            grp = c;
            items = (RadioItem *)get_items_1060_ac7c((SRadioSet *)grp);
            n = radio_count_1060_ac99((SRadioSet *)grp);
            for (i = 0; i < n; i++) {
                if (items[i].accel >= 0 && items[i].key == key) {
                    radio_click(items[i].hwnd);
                    r = 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    return r;
}
