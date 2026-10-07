/* PORT: this file's own declarations of Flic_ShowFrame, anim_unbind removed: the header declares them from the definition (W1 step 3). */
/* timer_tick: candidate for 1068:3424 (50 bytes).  Verdict: RELOCATABLE.
 *
 * A timer callback. `anim_start` stores the object it is animating in the pointer at
 * DGROUP:0x568c and then hands THIS function's address to `start_timer`, so the
 * callback takes no object of its own and reads the global instead -- which is why it has
 * arguments it does not use and one it only tests.
 *
 * Every tick runs `Flic_ShowFrame` on the object; when the second argument is zero it also
 * runs `anim_unbind`, which forwards the object's pointer at +0x570 to
 * `port_unbind_anim`. What the argument means is not established here -- only that zero is the
 * case that does the extra work.
 *
 * The name is the function's ROLE, which the registration proves, rather than its effect,
 * which it does not: `start_timer` is handed this address and nothing else calls
 * it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */

void timer_tick(int unused1, int count)
{
    Flic_ShowFrame((Flic *)g_anim);
    if (count == 0)
        anim_unbind((Flic *)g_anim);
}
