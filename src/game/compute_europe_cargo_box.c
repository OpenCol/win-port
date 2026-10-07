/* compute_europe_cargo_box: candidate for 1048:4dbc (53 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Fills the caller's four out-parameters with the on-screen rectangle of slot i: a fixed row of 24-pixel cells 10 by 12 in size.
 *
 * RENAMED 2026-09-27 from slot_rect: CONST PROPOSAL, the twin of compute_colony_cargo_box. Mac compute_europe_cargo_box(short, short* x4), CODE 25 0x135a, 88 bytes, called only by DrawEuropeShips. 4dbc (int i, int *x, *y, *w, *h), 53 bytes, is called only by EuropeWin_DrawEuropeShips, twice. Leaf. Layout: between get_transit_loc and get_unit_loc on both. Constants: overlap 1.0.
 */

#include "colonization.h"

void compute_europe_cargo_box(int i, int *x, int *y, int *w, int *h)
{
    *x = i * 0x18 + 0x122;
    *y = 0x171;
    *w = 10;
    *h = 12;
}
