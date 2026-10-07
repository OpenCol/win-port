/* port_fade_through_black: candidate for 1068:b351 (176 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_fade_through_black
 * name-source: descriptive
 *
 * The fade transition of the `port_*` family at 1068:b144..b65b: fade the screen's palette
 * DOWN to the key colour one step at a time, redraw the port underneath while nothing is
 * visible, then fade back UP to where it started.
 *
 * The third argument is a STEP COUNT, not a mode. It reaches `CTable_SetScale` through
 * `CTable_set_key_and_reinstall` -- whose only caller this is -- and lands in the colour
 * table at +0x408, the slot `CTable_Scale` divides by. This function then counts its own
 * loop from that same number down to zero and back, which is what settles it: a mode would
 * not be a loop bound.
 *
 * The two loops share the counter -- the fade-out runs it to 0 and the fade-in runs it back
 * up -- so the second loop's bound is the word the first one started from. That is why the
 * argument is loaded into DI and copied to SI rather than used from the frame: one is the
 * bound, the other the counter.
 *
 * The step is `--i` and `++i` INSIDE the call, not a statement before it: the shipped code
 * moves SI to AX and pushes AX, which is a pre-decrement's value being used, where a separate
 * statement would push SI directly.
 *
 * The two loops are NOT the same body. The fade-DOWN calls `MSDoTask_1068_d906` (MSDoTask, the
 * task pump) between the fade step and the tick wait; the fade-UP does not -- it goes
 * straight from the step to the wait. Going down the rest of the program still has to run,
 * because what is being taken away is the old screen; coming up the new one is already in
 * place and the fade is the only thing left to do.
 *
 * `port_copy_full` between the two loops is the whole trick: the port is copied
 * while the screen is black.
 *
 * The colour table is the pointer at +2 of the drawing window at +0x26 -- `get_ctable_1068_d77f` --
 * the same +0x26 object the wipe and scroll wrappers pass on, though they read its +6 through
 * `get_win_1068_d72a`.
 *
 * The three palette calls are reached by the five-byte NOP form and are `extern`; the four
 * others by the four-byte form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from port_crossfade: The palette is faded DOWN to the key colour (0,0,0) with CTable_Scale(--i). The port is copied while the screen is black (port_copy_full), and then the palette is faded back UP. That is a dip-to-black transition, not a crossfade: the two images are never blended. The name also clashes with its neighbour palette_cross_fade (1068:0d94), which really does blend two RGB sources a*step + b*(steps-step).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void CTable_set_key_and_reinstall(CTable *table, short first, short count,
                                             short steps, char r, char g, char b);
extern void CTable_Scale(CTable *table, int step);
extern void CTable_ResetScale(CTable *pal);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_ctable_1068_d77f(Screen *p);
extern "C" void port_copy_full(MagicWin *o);

void port_fade_through_black(MagicWin *s, short first, short count,
                                  register short steps)
{
    CTable *tbl;
    register int i;

    i = steps;
    tbl = (CTable *)get_ctable_1068_d77f(&(DialogWin &)*s);
    CTable_set_key_and_reinstall(tbl, first, count, steps, 0, 0, 0);
    while (i > 0) {
        CTable_Scale(tbl, --i);
        MSDoTask_1068_d906();
        MSWaitTicks_1068_d93a(1);
    }
    port_copy_full(s);
    while (i < steps) {
        CTable_Scale(tbl, ++i);
        MSWaitTicks_1068_d93a(1);
    }
    CTable_ResetScale(tbl);
}
