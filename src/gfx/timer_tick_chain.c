/* timer_tick_chain: candidate for 1068:3456 (73 bytes).  Verdict: RELOCATABLE.
 *
 * `timer_tick`'s looping sibling, and the loop is the point: this callback is
 * registered by `Flic_Loop`, and when its second argument is zero it stops the current
 * animation and calls `Flic_Loop` again -- which registers this same function for the
 * next one. The chain is a timer that reschedules itself from inside its own callback,
 * carrying the two objects at DGROUP:0x568c and DGROUP:0x5690 rather than any argument.
 *
 * Every tick runs `Flic_ShowFrame` on the first object, exactly as the non-looping sibling
 * does; the difference is entirely in the zero branch, where `1068:3424` calls
 * `anim_unbind` and this one calls `Flic_Reset` and then re-enters the registrar.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own MagicWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"

extern void *g_anim_port;   /* DGROUP:0x5690 */

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern void Flic_ShowFrame(void *anim);
extern void Flic_Reset(void *p);
extern void Flic_Loop(void *anim, void *port);

void timer_tick_chain(int unused1, int count)
{
    Flic_ShowFrame((Flic *)g_anim);
    if (count == 0) {
        Flic_Reset((Flic *)g_anim);
        Flic_Loop((Flic *)g_anim, (MagicWin *)g_anim_port);
    }
}
