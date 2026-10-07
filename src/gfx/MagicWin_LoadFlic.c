/* MagicWin_LoadFlic: candidate for 1068:af2a (36 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_LoadFlic
 * name-source: sibling-build
 *
 * Binds an animation to a port: the flag at +0xfa set and the pointer at +0xfc stored.
 * `anim_start` and `Flic_Loop` both call it with the port first and the animation
 * second, which is what makes the pointer the animation and not the other way round.
 *
 * Its twin `port_unbind_anim` is the next function and clears both.
 *
 * RENAMED 2026-09-26 from port_bind_anim: the Mac build's MagicWin::LoadFlic(Flic&): the window plus the animation, bound together (+0xfa flag, +0xfc pointer); Flic_Loop and anim_start call it window first. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








typedef struct { char pad[0x574]; void *loop_hook; } Flic;

void MagicWin_LoadFlic(MagicWin *p, Flic *anim)
{
    p->bound = 1;
    p->anim = anim;
}
