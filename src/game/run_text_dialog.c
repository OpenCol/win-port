/* run_text_dialog: candidate for 1050:5f98 (323 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: run_text_dialog
 *
 * The fifth argument of `Popup_start` is a `void *`, not a `long`: this file
 * passes a literal 0 there either way, but `popup_ask_nocancel` passes a real pointer
 * through and its two-word push settles it (§119).
 *
 * Runs a text dialog through a stack Popup of 0x800 entries: the directive parser fills it
 * (0 = ok); a check-list popup (bit 2 of +0x86) has its +0x7c boxes preset from the mask
 * at DGROUP:0x2514, the dialog runs (popup_run_default), the chosen item at +0xaa is saved to
 * 0x2516, the boxes are read back into the mask, and 0x4dfe records whether Popup_help_me
 * approves. Returns the dialog result -- through the third parameter, which the shipped code
 * reuses as its result variable.
 *
 * `g_dialog_help_used` AND `g_chosen_item` ARE WRITTEN HERE AND READ NOWHERE. Neither address is
 * loaded anywhere in the image -- 0x4dfe has three absolute stores and 0x2516 has one, and no
 * instruction reads either (`tools/check_dead_dgroup.py`). They are not how the answer leaves
 * this function: it `return`s `a`, the popup's own result, and that is what every caller uses.
 * So the two globals are a parallel copy nobody collects, and their names promise a reader that
 * does not exist. They are left as they are because the names say what the STORE means, which
 * is all this function establishes.
 * Popup::Popup, Popup::~Popup and the Popup helpers it calls (Popup_start through Popup_help_me) are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern "C" int Popup_start(Popup *p, void *ctx, char *name, int sel, char *dflt, GameWin *win, GRPort *port, RECT *rect, int kind);
extern "C" void Popup_set_check(Popup *p, int key, int on);
extern "C" int popup_run_default(Popup *popup);
extern "C" int Popup_get_check(Popup *p, int key);
extern "C" int Popup_help_me(Popup *popup);

extern "C" int run_text_dialog(void *file, void *name, int sel, void *d1, void *d2, void *d3, int kind)
{
    Popup popup(0x800);
    int i;

    g_dialog_help_used = 0;
    if (Popup_start(&popup, file, (char *)name, sel, 0L, (GameWin *)d1, (GRPort *)d2, (RECT *)d3, kind) != 0)
        goto done;
    if (popup.flags & 4) {
        for (i = 0; i < popup.n_items; i++)
            Popup_set_check(&popup, i, (1 << i) & g_check_mask);
    }
    sel = popup_run_default(&popup);
    g_chosen_item = popup.extra_button;
    if (popup.flags & 4) {
        g_check_mask = 0;
        for (i = 0; i < popup.n_items; i++)
            if (Popup_get_check(&popup, i))
                g_check_mask |= 1 << i;
    }
    if (Popup_help_me(&popup))
        g_dialog_help_used = 1;
done:
    return sel;
}
