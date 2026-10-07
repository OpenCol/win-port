/* get_transit_loc: candidate for 1048:4ccc (240 bytes, 3 callers).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Places index idx into one of three tiers -- the first 4, the next 8, then tier3_count
 * more -- and returns through pointers the tier, the cell size (32 >> tier), the width,
 * the x position (base_x + pos * advance, nudged per tier) and the sprite id per tier.
 *
 * RENAMED 2026-09-27 from classify_index_into_tier: CONST PROPOSAL. Mac get_transit_loc(short x4, short* x5), CODE 25 0x1290, 202 bytes. 4ccc (int idx, int base_x, int tier3_count, int extra, int *tier, *x, *sprite, *w, *size) has the same shape: four words and five pointers. Callers: Mac DrawTransitUnit, EuropeMouseShipsProper, EuropeMouseTransit; Windows europe_draw_ship, europe_ships_click, europe_voyage_click, the same three (see their rows). Leaf on both. Layout: Mac reset_europe_units, compute_europe_parameters, get_transit_loc, compute_europe_cargo_box, get_unit_loc; Windows reset_europe_units, compute_europe_parameters, 4ccc, slot_rect, get_unit_loc. Size 202 against 240. Constants: overlap 1.0.
 */

#include "colonization.h"

void get_transit_loc(int idx, int base_x, int tier3_count, int extra,
                                  int *tier, int *x, int *sprite,
                                  int *w, int *size)
{
    int pos = 0;
    int n = idx;
    int step;
    int adv;

    *tier = 0;
    if (n < 4)
        pos = n;
    else {
        (*tier)++;
        n -= 4;
        if (n < 8)
            pos = n;
        else {
            (*tier)++;
            n -= 8;
            if (n < tier3_count)
                pos = n;
            else
                (*tier)++;
        }
    }
    step = 0x20 >> *tier;
    adv = step;
    if (*tier == 0)
        adv += extra;
    if (*tier == 2)
        adv++;
    *w = *size = step;
    *x = pos * adv + base_x;
    switch (*tier) {
    case 0:
        *sprite = 0x146;
        break;
    case 1:
        *x += 2;
        *w -= 2;
        *sprite = 0x134;
        break;
    case 2:
        (*x)++;
        (*w)--;
        *sprite = 0x12a;
        break;
    }
}
