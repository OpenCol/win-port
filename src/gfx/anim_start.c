/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 81 of the imported file. */
/* PORT: this file's own declarations of MagicWin_LoadFlic removed: the header declares it from the definition (W1 step 3). */
/* anim_start: candidate for 1068:234a (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_start
 * name-source: descriptive
 *
 * Arms the animation timer. The destination port goes into the animation object at +0x570
 * AND into DGROUP:0x5690; the object itself goes into DGROUP:0x568c; `MagicWin_LoadFlic` binds
 * the two; and `start_timer` installs `timer_tick` with the object's own
 * interval at +0x36 and frame count at +0x2c (the timer's repeat count), the returned slot parked at +0x56e.
 *
 * The two globals are not incidental. `timer_tick` and its looping sibling
 * `timer_tick_chain` take no arguments and read exactly those two addresses, so
 * this function is where a callback with no parameters gets its subject.
 *
 * `g_anim_port = a->port = port;` is a CHAINED assignment: the shipped loads the pointer into
 * EAX once and stores it twice. Written as two statements Borland reloads `[bp+0xa]` in
 * between, which is four bytes more and the only difference in the function.
 *
 * Both -pointer stores are single 32-bit `MOV`s, which is what `-3` gives.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own MagicWin, GRPort, DiskFile removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#include "records.h"

/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

extern void *g_anim_port;   /* DGROUP:0x5690 */

/* same object: 5-byte NOP form, so extern (rule 47) */

void anim_start(Flic *anim, void *port)
{
    g_anim_port = anim->port = port;
    MagicWin_LoadFlic((MagicWin *)port, anim);
    g_anim = anim;
    anim->timer = start_timer((void*)(timer_tick), anim->speed, anim->frames);
}
