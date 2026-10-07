/* PORT: this file's own declarations of port_unbind_anim removed: the header declares it from the definition (W1 step 3). */
/* anim_unbind: candidate for 1068:3401 (35 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_unbind
 * name-source: descriptive
 *
 * Hands the object's pointer at +0x570 to `port_unbind_anim`.
 *
 * `timer_tick` already describes it: "runs 1068:3401, which forwards the object's 
 * pointer at +0x570 to ..." -- and `timer_tick_chain` pairs it with `Flic_Reset`.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own MagicWin, GRPort, DiskFile removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#include "records.h"

/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

void anim_unbind(Flic *anim)
{
    port_unbind_anim((MagicWin *)anim->port);
}
