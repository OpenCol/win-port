/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of get_win_1060_ab6e, subclass_control_1070_a0e2 removed: the header declares them from the definition (W1 step 3). */
/* GameWin_Init: candidate for 1060:a16b (139 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The wider twin of `build_window_with_port` (1060:a099): the same four steps --
 * `build_window_*` on the window at +0x26, `GRPort_Init_wh` on the port at +0 with the width
 * and height, `GRPort_ColorSync` with the following pair, and `subclass_control` with
 * the HWND `get_win_1060_ab6e` gives for +0x26 -- with nine parameters passed through instead of
 * eight. `init_and_set_1060_a1f6` is one of its callers (AboutWin_ctor, Fame and PostLowMemReport call it too), and already declares the same
 * signature this file reproduces.
 *
 * The width and height are again the only two that get registers, loaded into SI and DI at
 * entry because each is used twice; the other nine are pushed once straight from the frame.
 *
 * `build_window_1060_98d7` and `get_win_1060_ab6e` are reached by the four-byte `PUSH CS / CALL NEAR`
 * form and are stubs here (rule 47); the three calls into other segments are `extern`.
 *
 * THE NINE WORDS ARE NOT NINE INTS. The pair after the object is the window's NAME, the
 * trailing four bytes its PALETTE, and (in the eleven-word variants) the four after that its
 * PARENT -- splits that total correctly either way, so the comparator cannot see them
 * (§119). `PostLowMemReport` settles the first two by passing "" and `get_ctable_1068_d77f(win)`,
 * `ClosingWin_ctor` passes `&g_game_palette` last, and at the bottom of the chain
 * `Screen_Init` hands the same pair to `create_MSWindowClass_window` where
 * CreateWindow's own argument order makes them unmistakable.
 *
 * RENAMED 2026-09-27 from build_window_with_port_1060_a16b: Mac GameWin::Init(char*, short, short, short, short, short, CTable&, Screen&), 152 bytes, against 139, the only GameWin::Init overload. Callers: 15 of 15 match (AboutWin ctor, ColChild InitCWin, Customize Init, Fame, HallOfFame, KingRoom, Opening ctor, PopupBoxDraw, PostLowMemReport, DoExtendedLaborReport, SetDifficulty/SetNations Init, SuperWin Init, art_cheat...). Seq: Mac DialogWin::Init, GRPort::Init(short, short), ColorSync, MSGameWinInit is the Windows build_window_1060_98d7, GRPort_Init_wh, GRPort_ColorSync, subclass_control. Layout: GameWin_Redraw and GameWin_ResizePort follow on both. Arity: this+8 = 9 = (o, name, style, x, y, w, h, pal, parent). The narrower twin build_window_with_port (1060:a099, 8 parameters) has no Mac overload and keeps its name.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` */


void GameWin_Init(GameWin *o, char *name, int style, int x, int y,
                                          register int w, register int h,
                                          CTable *pal, Screen *parent)
{
    build_window_1060_98d7(((char *)o + PORT_DIALOG_BASE), name, style, x, y, w, h, pal, parent);
    GRPort_Init_wh(o, w, h);
    GRPort_ColorSync((GRPort *)o, pal);
    subclass_control_1070_a0e2(o, (MSWin *)(get_win_1060_ab6e((Screen *)(((char *)o + PORT_DIALOG_BASE)))));
}
