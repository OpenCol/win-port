/* PORT: this file's own declarations of MSDisposeWindow, create_MSWindowClass_window, set_hwnd_object removed: the header declares them from the definition (W1 step 3). */
/* Screen_Init: candidate for 1060:a4d4 (151 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The bottom of the window-building chain: `EventWin_Init` calls it with nine words
 * of arguments and it is what actually creates the window. The style word goes into the
 * object at +0 first; a window already at +6 is torn down by `MSDisposeWindow` and its 0 result
 * written back; `create_MSWindowClass_window` then OVERWRITES +6 with the window it makes;
 * `set_hwnd_object` registers the object against it, and `screen_set_ctable` hands
 * over the palette the caller passed last.
 *
 * +6 holding the old window (disposed, its slot cleared) and then the new window is what the shipped code does, not a reading of
 * it: both stores are to the same pair of words, twenty bytes apart, with the dispose call in
 * between.
 *
 * The final `0L` is one 32-bit push under `-3`, which is what says it is a pointer or a
 * long and not two zero words.
 *
 * THE NINE WORDS ARE NOT NINE INTS. The pair after the object is the window's NAME, the
 * trailing four bytes its PALETTE, and (in the eleven-word variants) the four after that its
 * PARENT -- splits that total correctly either way, so the comparator cannot see them
 * (§119). `PostLowMemReport` settles the first two by passing "" and `get_ctable_1068_d77f(win)`,
 * `ClosingWin_ctor` passes `&g_game_palette` last, and at the bottom of the chain
 * `Screen_Init` hands the same pair to `create_MSWindowClass_window` where
 * CreateWindow's own argument order makes them unmistakable.
 *
 * RENAMED 2026-09-26 from build_window_1060_a4d4: the Mac build's Screen::Init(char*, short, short, short, short, short, CTable&): this plus a title, style, x, y, w, h and the palette -- nine words -- and the bottom of the window-building chain, the one that creates the window. The width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern void screen_set_ctable(Screen *o, void *t);

void Screen_Init(Screen *o, char *name, register int style, int x, int y,
                                int w, int h, CTable *pal)
{
    o->style = style;
    if (o->win != 0)
        o->win = MSDisposeWindow((MSWin *)o->win);
    o->win = create_MSWindowClass_window(name, style, x, y, w, h, 0L);
    set_hwnd_object(o, (MSWin *)o->win);
    screen_set_ctable(o, pal);
}
