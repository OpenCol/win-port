/* random_activate: candidate for 1040:7508 (18 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `randmize`, named for the routine it
 * reaches.
 *
 * THE BODY READS NO ARGUMENT and `route_create` declares one -- `random_activate(int seed)`
 * -- so the parameter is the caller's claim and not this function's (§119). It is carried
 * here for that reason.
 *
 * The callee is -called and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from seed_random: the Mac build's random_activate(short), bound by call-graph agreement (16 of its Mac call edges already bound land here, runner-up 5) and LAYOUT; its one Mac callee is randmize, as this one's is seed_random_from_ticks. Both builds keep the random module in one order -- Mac randmize, random_reseed(long), random_activate(short), random(short, short); Windows 1040:74cd, 74eb, 7508, 751a -- and the arities line up one for one. random itself stays rand_range: Borland's stdlib.h defines a one-argument random macro.
 */

#include "colonization.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` */
extern void randmize(void);

void random_activate(int seed)
{
    randmize();
}
