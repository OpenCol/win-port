/* unit_rules: candidate for 1038:3c3a (344 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A converted native (type 0, specialty 27) standing outside any settlement (is_city < 0) and alone
 * on its square (stack_check kind 2, every unit, below 2) grows restless (+0x16); past 8 the unit is removed -- a human
 * owner's square is refreshed by force_map, the spot marked by DrawMapRegion and
 * DEADCONVERTS shown. Returns 0 when the unit was removed, else 1.
 *
 * RENAMED 2026-09-26 from converts_lose_faith: the Mac build's unit_rules(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; callee delete_unit; callee is_city; callee on_map; callee popm), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "units.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern int is_city(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int stack_check(int u, int kind);
extern void force_map(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void delete_unit(int u);
extern void DrawMapRegion(int x, int y, int a, int b, int flag);
extern int popm(char *msg, int n);

int unit_rules(int u)
{
    int ux;
    int uy;
    int owner;
    int keep = 1;

    ux = g_units[u].x;
    uy = g_units[u].y;
    if (!on_map(ux, uy))
        goto done;
    if (g_units[u].type == UNIT_COLONISTS && (int)g_units[u].spec == JOB_CONVERT && is_city(ux, uy) < 0 && stack_check(u, 2) < 2) {
        g_units[u].counter++;
        if (g_units[u].counter > 8) {
            owner = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
            if (owner < NATION_EUROPEAN_COUNT && g_nation_text[owner].control == CONTROL_HUMAN)
                force_map(ux, uy);
            delete_unit(u);
            keep = 0;
            if (owner < NATION_EUROPEAN_COUNT && g_nation_text[owner].control == CONTROL_HUMAN) {
                DrawMapRegion(ux, uy, 1, 1, 1);
                popm("DEADCONVERTS", 4);
            }
        }
    }
done:
    return keep;
}
