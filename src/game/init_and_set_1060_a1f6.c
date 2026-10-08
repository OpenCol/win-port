/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of GameWin_Init, eventwin_set_menu removed: the header declares them from the definition (W1 step 3). */
/* init_and_set_1060_a1f6: candidate for 1060:a1f6 (83 bytes).  Verdict: RELOCATABLE.
 * symbol: init_and_set_1060_a1f6
 *
 * GameWin_Init with nine parameters, then eventwin_set_menu on the DialogWin part (+0x26)
 * with the trailing pointer.  Both are -called and defined in-file as
 * stubs.
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

#include "classes.h"
extern int g_stub_a;


void init_and_set_1060_a1f6(char *o, char *name, int style, int x, int y, int w, int h, void *pal, void *parent, void *p)
{
    GameWin_Init((GameWin *)o, name, style, x, y, w, h, (CTable *)pal, (Screen *)parent);
    eventwin_set_menu((EventWin *)(o + PORT_DIALOG_BASE), p);
}

