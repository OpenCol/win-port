/* colony_panels_reset: candidate for 1010:bf45 (97 bytes).  Verdict: RELOCATABLE.
 * symbol: colony_panels_reset
 *
 * Recounts the colony (compute_colony_parameters) and resets the nine panel states with
 * argument 0.  The nine setters are -called and defined in-file as
 * stubs.
 */

#include "colonization.h"

extern int g_stub_a;
extern void compute_colony_parameters(void);

extern void draw_colony_map(int a);
extern void draw_colonists(int a);
extern void draw_market(int a);

extern void draw_colony_ships(int a);
extern void draw_miscellaneous(int a);

void colony_panels_reset(void)
{
    compute_colony_parameters();
    empty_1010_bddc();
    empty_1010_bdf8(0);
    draw_colony_map(0);
    draw_colonists(0);
    draw_market(0);
    empty_1010_bf37(0, 0);
    draw_colony_ships(0);
    draw_miscellaneous(0);
    empty_1010_be3e(0);
}
