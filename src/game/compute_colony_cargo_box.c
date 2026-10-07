/* compute_colony_cargo_box: candidate for 1010:beae (53 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * The geometry of column n of a 24-pixel grid starting at x=250: the x through
 * the first pointer, then the fixed 0x161, 10 and 12 through the other three.
 *
 * RENAMED 2026-09-27 from column_metrics: CONST PROPOSAL. Mac compute_colony_cargo_box(short, short*, short*, short*, short*), CODE 31 0x0048, 88 bytes, called only by DrawColonyShips. beae (int n, int *px, *py, *pw, *ph): five arguments, the same shape. It is called only by ColonyWin_DrawColonyShips, twice. Leaf on both. Layout: Mac CODE 31 runs draw_colony_map, draw_colonists, compute_colony_cargo_box, draw_colony_ships, draw_miscellaneous; Windows runs empty stubs, beae, draw_colony_ships (bee3), draw_miscellaneous (bef1). Size 88 against 53, the same ratio as its Europe twin (below). Constants: overlap 1.0, rare 0x161.
 */

#include "colonization.h"

void compute_colony_cargo_box(int n, int *px, int *py, int *pw, int *ph)
{
    *px = n * 0x18 + 0xfa;
    *py = 0x161;
    *pw = 10;
    *ph = 12;
}
