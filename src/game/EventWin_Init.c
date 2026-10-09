/* PORT: a member reached by the game's byte offset is reached by name or by its class sizes (PORT_EVENT_TABLE, PORT_SIZEOF_*): the offsets move on 64-bit Windows (W9). */
/* PORT: this file's own declarations of ClearEventTable_1060_acb2, Screen_Init, get_win_1060_ab6e, subclass_control_1070_67dd removed: the header declares them from the definition (W1 step 3). */
/* EventWin_Init: candidate for 1060:9de8 (101 bytes).  Verdict: RELOCATABLE.
 * symbol: EventWin_Init
 * name-source: sibling-build
 *
 * Screen_Init with nine words of arguments, then subclass_control_1070_67dd on the object
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
 *
 * RENAMED 2026-09-26 from build_window_1060_9de8: the Mac build's EventWin::Init(char*, short, short, short, short, short, CTable&): the same nine words; it calls Screen_Init and then initialises its EventTable part, as an EventWin (Screen + EventTable) would. The width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern char g_stub_s[];
extern int g_stub_a;


void EventWin_Init(char *o, char *name, int style, int x, int y, int w, int h, CTable *pal)
{
    Screen_Init((Screen *)o, name, style, x, y, w, h, pal);
    subclass_control_1070_67dd(o, (MSWin *)(get_win_1060_ab6e((Screen *)o)));
    ClearEventTable_1060_acb2((EventTable *)(o + PORT_SCREEN_DSIZE));
}

