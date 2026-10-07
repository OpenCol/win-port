/* build_window_1060_9e96: candidate for 1060:9e96 (107 bytes).  Verdict: RELOCATABLE.
 * symbol: build_window_1060_9e96
 *
 * build_window_owned with eleven words of arguments, then subclass_control_1070_67dd on the object
 * and its window (get_win_1060_ab6e) and ClearEventTable_1060_acb2 on its EventTable part
 * (+0xd).  The two helpers are -called and defined in-file as stubs.
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
extern int g_stub_a;
extern void build_window_owned(char *o, char *name, int style, int x, int y, int w, int h, void *pal, void *parent);
extern void subclass_control_1070_67dd(void *obj, void *control);

extern void *get_win_1060_ab6e(void *o);
extern void ClearEventTable_1060_acb2(void *table);

void build_window_1060_9e96(char *o, char *name, int style, int x, int y, int w, int h, void *pal, void *parent)
{
    build_window_owned((Screen *)o, name, style, x, y, w, h, pal, parent);
    subclass_control_1070_67dd(o, (MSWin *)(get_win_1060_ab6e((Screen *)o)));
    ClearEventTable_1060_acb2((EventTable *)(o + 0xd));
}

