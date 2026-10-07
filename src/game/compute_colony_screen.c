/* compute_colony_screen: candidate for 1018:8c92 (18 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `make_building_table`, named for the routine it
 * reaches. The body reads no argument.
 *
 * The callee is -called and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from rec12_d422: this module's one-line forwarder to make_building_table, called
 * from ColonyWin_Show.
 *
 * RENAMED 2026-09-27 from colony_site_layout_fwd: Mac compute_colony_screen(void). The Mac body (84 bytes with its symbol) is LINK, JSR make_building_table, UNLK, RTS, which is exactly this 18-byte function. Caller: ColonyWin::Show on both. Layout: after compute_colony_parameters on both. Arity: void. The _fwd name assumed a per-module forwarder copy; it is a real function of the colony module, in the Mac's position.
 */

#include "colonization.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` */
extern void make_building_table(void);

void compute_colony_screen(void)
{
    make_building_table();
}
