/* PORT: this file's own declarations of GameWin_Init, get_win_1068_d72a, subclass_control_1070_bb63 removed: the header declares them from the definition (W1 step 3). */
/* MagicWin_Init: candidate for 1068:a890 (150 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_Init
 * name-source: sibling-build
 *
 * Builds a control with a port: the rectangle is `SetRect(&r, 0, 0, w, h)`, the eleven words
 * the caller hands over go to `GameWin_Init` (1060:a16b -- the shipped call's relocation
 * names a16b, per tools/check_data_refs.py; an earlier reading had `init_and_set_1060_a11e`,
 * which the twin calls), the control is subclassed against the window at +0x26, and the port
 * at +0xc4 is told the rectangle through the first slot of its vtable.
 *
 * ITS TWIN AT 1068:a7fa IS THE SAME FOUR STEPS IN A DIFFERENT ORDER, with a different second
 * step. This one calls the virtual FIRST, before the control is built, and builds it with
 * `GameWin_Init`; that one calls it LAST, after the subclass, and builds with
 * `init_and_set_1060_a11e` (GameWin_Init plus `eventwin_set_menu`). Both are 150 bytes and
 * every instruction is shared apart from that callee; the order is what Borland's
 * source-order emission makes visible and nothing else would.
 *
 * The virtual call is why this file is C++: the vtable pointer is a NEAR word at the start of
 * the sub-object and the entry it reads is a function, and `-P` with one `virtual`
 * declaration produces both.
 *
 * `SetRect` is ``, so its arguments go left to right and the width and height are the
 * LAST pair pushed.
 *
 * RENAMED 2026-10-02 from build_control_1068_a890: the Mac build's MagicWin::Init(char*, short,
 * short, short, short, short, CTable&, Screen&) -- the same eight arguments after `this`, which
 * this function hands to GameWin_Init -- and its one caller is ClosingWin's constructor, a
 * MagicWin subclass, passing `this`. The `Obj` view became the MagicWin class the same day.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x41e bytes: the palette, its handle and the cycling state */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void MagicWin_Init(MagicWin *o, char *name, int style, int x, int y,
                             register int w, register int h, void *pal, void *parent)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    o->port.ResizePort(&r);
    GameWin_Init((GameWin *)o, name, style, x, y, w, h, (CTable *)pal, (Screen *)parent);
    subclass_control_1070_bb63(o, (MSWin *)(get_win_1068_d72a((Screen *)(&(DialogWin &)*o))));
}
