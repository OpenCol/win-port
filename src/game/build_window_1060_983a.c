/* build_window_1060_983a: candidate for 1060:983a (84 bytes).  Verdict: RELOCATABLE.
 * symbol: build_window_1060_983a
 *
 * EventWin_Init with nine words of arguments, then subclass_control_1070_8f26 on the object
 * and its window (get_win_1060_ab6e, -called and defined in-file as a stub).
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

#include "records.h"
#include "classes.h"
extern char g_stub_s[];
extern void EventWin_Init(char *o, char *name, int style, int x, int y, int w, int h, CTable *pal);
extern void subclass_control_1070_8f26(void *control, void *win);

extern void *get_win_1060_ab6e(void *o);

void build_window_1060_983a(void *o, char *name, int style, int x, int y, int w, int h, void *pal)
{
    EventWin_Init((char *)o, name, style, x, y, w, h, (CTable *)pal);
    subclass_control_1070_8f26(o, (MSWin *)(get_win_1060_ab6e((Screen *)o)));
}

