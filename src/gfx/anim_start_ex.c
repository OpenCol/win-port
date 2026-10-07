/* anim_start_ex: candidate for 1068:23a9 (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_start_ex
 * name-source: descriptive
 *
 * Stores two words into the animation object at +0x24e and +0x250 and then arms the timer
 * through `anim_start` (1068:234a). The only caller-visible difference from `anim_start`
 * is those two words.
 *
 * Nothing in the shipped image ever reads them. The four instructions that touch either
 * offset are all stores: the two here, and the two in the object's constructor at
 * 1068:1d14 that zero them. They are vestigial fields, kept in the record and still
 * written by this entry point -- which is the reason to name the function for the two
 * arguments it forwards rather than for a meaning the bytes do not support.
 *
 * `anim_start` is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

/* same object: 4-byte `push cs; call ` */
extern "C" void anim_start(Flic *anim, void *port);

void anim_start_ex(Flic *anim, void *port, int a4, int a5)
{
    anim->f24e = a4;
    anim->f250 = a5;
    anim_start(anim, port);
}
