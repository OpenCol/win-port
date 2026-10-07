/* WhichColonist: candidate for 1010:5dac (315 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: WhichColonist
 *
 * Which colonist portrait a horizontal position falls on: walks the
 * colony's colonists plus the tile's (DGROUP:0x4bb2), each portrait's
 * width from its 32-byte record at SEG20:0x6246 (halved for the blank
 * picture 0x5a) plus the spacing byte at 0x4bc0, squeezed to a minimum of 1
 * while an earlier shortfall is carried, a gap of 8 after the last colony
 * colonist.  Returns the index, or the last one when px is past them all.
 * The two helpers are -called and defined in-file (max_int real, the
 * width reader a stub).
 *
 * RENAMED 2026-09-26 from colonist_at_x: the Mac build's WhichColonist(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between ColonyWin_ColonyMouseMap and ColonyWin_ColonyClickUnits, where the Mac gap holds 2 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.02x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int g_stub_a;
extern int get_icon(int colonist);

extern int sprite_width_1010_e60f(void *sprite);

int WhichColonist(int px, int unused1)
{
    int i;
    int x;
    int found;
    int done;
    int total;
    int pic;
    register int w;
    register int carry;

    total = g_cur_colony_rec->pop + g_tile_colonists;
    found = g_cur_colony_rec->pop + g_tile_colonists - 1;
    done = 0;
    x = 1;
    carry = 0;
    for (i = 0; done == 0 && i < total; i++) {
        pic = get_icon(i);
        if (pic != 0x5a)
            w = sprite_width_1010_e60f(&g_icon_sprites[pic]) + g_portrait_gap;
        else
            w = sprite_width_1010_e60f(&g_icon_sprites[pic]) / 2 + g_portrait_gap;
        if (w < 1)
            carry += 1 - w;
        w = max_int_1010_e632(1, w);
        while (w > 1 && carry > 0) {
            w--;
            carry--;
        }
        x += w;
        if (g_cur_colony_rec->pop - 1 == i)
            x += 8;
        if (px < x) {
            found = i;
            done = 1;
        }
    }
    return found;
}
