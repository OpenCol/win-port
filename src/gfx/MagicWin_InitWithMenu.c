/* PORT: this file's own declarations of get_win_1068_d72a, subclass_control_1070_bb63 removed: the header declares them from the definition (W1 step 3). */
/* MagicWin_InitWithMenu: candidate for 1068:a7fa (150 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_InitWithMenu
 * name-source: descriptive
 *
 * Builds a control with a port: the rectangle is `SetRect(&r, 0, 0, w, h)`, the eleven words
 * the caller hands over go to `init_and_set_1060_a11e`, the control is subclassed against the
 * window at +0x26, and the port at +0xc4 is told the rectangle through the first slot of its
 * vtable.
 *
 * ITS TWIN AT 1068:a890 IS THE SAME FOUR STEPS IN A DIFFERENT ORDER, with a different second
 * step. This one calls the virtual LAST, after the control is built and subclassed; that one
 * calls it FIRST, before anything else happens to the object, and it builds the control with
 * `GameWin_Init` (1060:a16b) where this one calls `init_and_set_1060_a11e`. Both are 150
 * bytes and every instruction is shared apart from that callee; the order is what Borland's
 * source-order emission makes visible and nothing else would.
 *
 * The virtual call is why this file is C++: the vtable pointer is a NEAR word at the start of
 * the sub-object and the entry it reads is a function, and `-P` with one `virtual`
 * declaration produces both.
 *
 * `SetRect` is ``, so its arguments go left to right and the width and height are the
 * LAST pair pushed.
 *
 * RENAMED 2026-10-02 from build_control_1068_a7fa, when its twin became MagicWin_Init: the
 * same MagicWin initialisation, through init_and_set_1060_a11e (GameWin_Init plus the menu).
 * The Mac build has one MagicWin::Init, so this name is descriptive.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void MagicWin_InitWithMenu(MagicWin *o, char *name, int style, int x, int y,
                             register int w, register int h, void *pal, void *parent)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    init_and_set_1060_a11e((char *)o, name, style, x, y, w, h, pal, parent);
    subclass_control_1070_bb63(o, (MSWin *)(get_win_1068_d72a((Screen *)(&(DialogWin &)*o))));
    o->port.ResizePort(&r);
}
