/* PlaySnd: candidate for 1068:a582 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `MSPlaySound`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from MSPlaySound_1068_a582: the Mac build's PlaySnd(char*), bound by call-sequence alignment (8.0 aligned gaps in sound_play leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.21x this one. The address suffix goes with the MS-layer spelling; the Mac name is the game-side one.
 */

#include "colonization.h"

void PlaySnd(void *name)
{
    MSPlaySound((const char *)name);
}
