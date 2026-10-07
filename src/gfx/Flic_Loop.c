/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 84 of the imported file. */
/* PORT: this file's own declarations of MagicWin_LoadFlic removed: the header declares it from the definition (W1 step 3). */
/* Flic_Loop: candidate for 1068:23e0 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Loop
 * name-source: sibling-build
 *
 * `anim_start`'s looping twin, 150 bytes below it: the same two globals bound to the same
 * two objects and the same timer installed with the same interval and id, but with
 * `timer_tick_chain` as the callback instead of `timer_tick` -- and a
 * completion hook at the end.
 *
 * The hook is an indirect FAR call through the pointer at +0x574, made only when it is
 * set. It takes no arguments, which is what `CALL FAR [es:bx+0x574]` with nothing pushed
 * says, and it is why the object carries a pointer there at all.
 *
 * `g_anim_port = a->port = port` is the same chained assignment `anim_start` needs: the shipped
 * code loads the pointer into EAX once and stores it twice. Written as two statements
 * Borland reloads `[bp+0xa]` in between.
 *
 * `start_timer` is reached by the five-byte NOP form and is `extern` (rule 47);
 * `MagicWin_LoadFlic` is a call into this same segment and is `extern` too.
 *
 * RENAMED 2026-09-26 from anim_start_chained: the Mac build's Flic::Loop(MagicWin&): this plus the window, the looping start -- anim_start's twin with the chaining timer callback. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, DiskFile, MagicWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "records.h"

/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */


extern void *g_anim_port;   /* DGROUP:0x5690 */

/* 5-byte NOP form, so extern (rule 47) */

void Flic_Loop(Flic *anim, MagicWin *port)
{
    g_anim_port = anim->port = port;
    MagicWin_LoadFlic(port, anim);
    g_anim = anim;
    anim->timer = start_timer((void*)(timer_tick_chain), anim->speed, anim->frames);
    if (anim->loop_hook != 0)
        (*anim->loop_hook)();
}
