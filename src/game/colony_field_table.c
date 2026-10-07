/* PORT: this file's own declarations of colony_field_status removed: the header declares it from the definition (W1 step 3). */
/* colony_field_table: candidate for 1018:70cd (76 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_field_table
 *
 * Fills the 5x5 table at SEG21:0x14a from colony_field_status(i, j) once; the
 * byte at DGROUP:0xc22 remembers that it has been done.  The helper is
 * -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from init_grid_marks: the Mac build's colony_field_table(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_indian_table; caller colony_plan; callee colony_field_status), the runner-up gets 1, and the arity agrees (0).
 */

#include "colonization.h"

extern int g_stub_a;


void colony_field_table(void)
{
    register int j;
    register int i;

    if (g_grid_marks_done != 0)
        goto done;
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++)
            g_grid_marks[i][j] = colony_field_status(i, j);
    }
done:
    g_grid_marks_done = 1;
}
