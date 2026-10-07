/* focus_at_or_after: candidate for 1070:928c (165 bytes).  Verdict: RELOCATABLE.
 * symbol: focus_at_or_after
 * name-source: descriptive
 *
 * Gives the keyboard focus to a control, or to the first control after it that will take it.
 * The byte at +0x98 of the dialog gates the whole thing; then the control's kind decides where
 * its window handle comes from -- a radio group (kind 3) reads it through the pointer at +0x2e,
 * anything else asks +0x10 -- and the walk is handed the window BEFORE that one.
 *
 * THE STEP BACK IS NOT A MOVE BACKWARDS. `focus_walk_siblings` tries the window it is
 * given LAST, after a whole forward lap, so this function steps back one with
 * `prev_sibling_window` and asks for a FORWARD walk (direction 2) from there: the
 * control itself is then the first candidate and the lap ends on its predecessor. Its only caller,
 * `DialogWin_SetDefaultControlFocus`, uses it to focus a dialog's marked control. (This was filed as
 * `focus_prev_sibling_1070_928c`, read from the backward step alone; RE_STATUS 306.)
 *
 * The kind-3 arm COPIES the pointer into a local before passing it on, a 32-bit move the other
 * arm does not make, which is what a declared local with an initialiser looks like when the
 * accessor wants a different pointer type.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern char get_keyboard_nav(void *o);
extern int ctl_kind_1070_9669(void *ctl);
extern int *get_items_1070_99c5(void *radios);

extern int ctl_hwnd_1070_9681(void *ctl);
extern void focus_walk_siblings(void *o, int h, int dir);

void focus_at_or_after(void *o, void *w)
{
    void *p;
    int *q;
    register int h;

    if (get_keyboard_nav((DialogWin *)o) == 0)
        return;
    if (ctl_kind_1070_9669((DlgControl *)w) == 3) {
        p = w;
        q = (int *)get_items_1070_99c5((SRadioSet *)p);
        h = *q;
        h = prev_sibling_window(h);
        focus_walk_siblings((DialogWin *)o, h, 2);
    } else {
        h = prev_sibling_window(ctl_hwnd_1070_9681((DlgControl *)w));
        focus_walk_siblings((DialogWin *)o, h, 2);
    }
}
