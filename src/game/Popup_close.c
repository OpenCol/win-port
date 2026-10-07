/* Popup_close: candidate for 1050:054e (606 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup_close
 * name-source: sibling-build
 *
 * TEARING A POPUP DOWN. Seven `delete`s in a row, one per kind of control the popup can own, each
 * guarded by its own null test and each followed by clearing the pointer. Two are single objects
 * (the radio set and the list box), four are ARRAYS -- check boxes, edit boxes, buttons and regions
 * -- and the window itself is the odd one out.
 *
 * THE WINDOW IS DELETED ONCE AND ONLY ONCE. Bit 4 of the flags at +0x86 is the guard: it is tested
 * before the delete and SET after it, so a second call walks past the window and frees only the
 * controls. None of the other six pointers has such a flag; they rely on being nulled.
 *
 * WHAT THE BYTES GIVE AWAY IS THE CLASS TREE. Borland's `delete` adds the number of DESTRUCTIBLE
 * SUB-OBJECTS to the allocator's headcount at SS:0x10 (rule 49), and the constants here name the
 * inheritance: 6 for the window, which is `GameWin : GRPort, DialogWin : EventWin : Screen,
 * EventTable`, and 2 for every control, which is `S<something> : DlgControl`. The array deletes say
 * the same thing a different way -- `__vdelxf` is handed a 2 where a flat class would make it 1 --
 * and they also hand over the element size, so the four array element types are 0x22, 0x28, 0x22
 * and 0x2e bytes. A flat class of the right SIZE is not enough here: it compiles a different
 * headcount and a different flag, and the function does not match until the bases are spelled out.
 *
 * The popup's address is cached in the dword at DGROUP:0x2500 while it lives and cleared here,
 * which makes this the one place that both ends of that handshake are visible.
 *
 * RENAMED 2026-09-26 from popup_free_controls: the Mac build's Popup::close(void): this only, tearing the popup down -- each kind of control it owns deleted and its pointer cleared. It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern Popup *g_cur_dialog;               /* DGROUP:0x2500 */

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void Popup_close(Popup *popup)
{
    if ((popup->flags & 0x10) == 0 && popup->win != 0) {
        delete popup->win;
        popup->win = 0;
        popup->flags |= 0x10;
    }
    if (popup->radios != 0) {
        delete popup->radios;
        popup->radios = 0;
    }
    if (popup->checks != 0) {
        delete [] popup->checks;
        popup->checks = 0;
    }
    if (popup->edit_boxes != 0) {
        delete [] popup->edit_boxes;
        popup->edit_boxes = 0;
    }
    if (popup->buttons != 0) {
        delete [] popup->buttons;
        popup->buttons = 0;
    }
    if (popup->listbox != 0) {
        delete popup->listbox;
        popup->listbox = 0;
    }
    if (popup->regions != 0) {
        delete [] popup->regions;
        popup->regions = 0;
    }
    g_cur_dialog = 0;
}
