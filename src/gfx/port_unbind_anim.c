/* port_unbind_anim: candidate for 1068:af4e (36 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_unbind_anim
 * name-source: descriptive
 *
 * Clears what `MagicWin_LoadFlic` set, 36 bytes above it: the flag at +0xfa and the
 * pointer at +0xfc. The null is one 32-bit store under `-3`, which is what says the slot
 * is a pointer and not two words.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void port_unbind_anim(MagicWin *p)
{
    p->bound = 0;
    p->anim = 0;
}
