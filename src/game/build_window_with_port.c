/* build_window_with_port: candidate for 1060:a099 (133 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Builds a window and gives it a drawing port of the same size: `build_window_1060_983a` on
 * the object at +0x26 with all eight parameters passed straight through, `GRPort_Init_wh` on
 * the object itself with the width and height, `GRPort_ColorSync` with the last pair,
 * and finally `subclass_control_1070_a0e2` with the HWND `get_win_1060_ab6e` gives for +0x26.
 *
 * The object at +0x26 is the window and the object at +0 is the port -- the same +0x26 offset
 * `EuropeWin_EuropeButtonClick` walks BACK across to get from a member to its container.
 *
 * The width and height are the only two that get registers, loaded into SI and DI
 * at entry because each is used twice; the others are pushed once straight from the
 * frame.
 *
 * THE NINE WORDS ARE NOT NINE INTS. The pair after the object is the window's NAME, the
 * trailing four bytes its PALETTE, and (in the eleven-word variants) the four after that its
 * PARENT -- splits that total correctly either way, so the comparator cannot see them
 * (§119). `PostLowMemReport` settles the first two by passing "" and `get_ctable_1068_d77f(win)`,
 * `ClosingWin_ctor` passes `&g_game_palette` last, and at the bottom of the chain
 * `Screen_Init` hands the same pair to `create_MSWindowClass_window` where
 * CreateWindow's own argument order makes them unmistakable.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void GRPort_ColorSync(GRPort *port, CTable *table);
extern void subclass_control_1070_a0e2(GameWin *win, void *hwnd);

/* same object: 4-byte `push cs; call ` */

extern void *get_win_1060_ab6e(void *o);

void build_window_with_port(GameWin *o, char *name, int style, int x, int y,
                                register int w, register int h, void *pal)
{
    build_window_1060_983a(((char *)o + 0x26), name, style, x, y, w, h, pal);
    GRPort_Init_wh(o, w, h);
    GRPort_ColorSync((GRPort *)o, (CTable *)pal);
    subclass_control_1070_a0e2(o, (MSWin *)(get_win_1060_ab6e((Screen *)(((char *)o + 0x26)))));
}
