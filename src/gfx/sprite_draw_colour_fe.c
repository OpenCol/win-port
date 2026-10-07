/* sprite_draw_colour_fe: candidate for 1068:798a (58 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `Sprite_Draw` (1068:7894) with -2 in the colour slot instead of -1: the same arguments
 * through to `Sprite_DrawSprite`, the same rectangle pointer back.
 *
 * Argument split corrected with the rest of the family -- see `Sprite_Draw.c`.
 *
 * RENAMED 2026-09-26 from sprite_draw_colour_fe: the Mac build's Sprite::DrawMono(GRPort&, short, short, char), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between Sprite_Draw and Sprite_Lock, where the Mac gap holds 3 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 1.52x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 *
 * RENAMED 2026-09-27 from Sprite_DrawMono: WRONG EXISTING NAME, CHAIN 1 step 1 (vacates Sprite_DrawMono). 4edb965a1 bound this to Mac Sprite::DrawMono(GRPort&, short, short, char) by LAYOUT inside an uneven gap (3 Mac functions against 8 Windows), paired by size. The BSR-complete graph refutes it. Mac DrawMono has 8 callers (ColonyLibertyReport, ColonyWin::DrawColonists, DrawIconRow, DrawMultiIconRow, MapWin::PieceIconDraw, both PieceIconDraw overloads, ReportsLabor); 798a has 0 callers. Its width is Draw's (out, s, port, px, py), not DrawMono's (one more byte). Mac DrawMono calls DrawMonoSprite(this, &r, port, -1, x, y, mode); 798a calls sprite_draw_scaled with -2 and no mode. The Mac Sprite file has nothing between DrawMono and CopyTo, so 798a (like 78ce and 790c, also 0 callers) is a Windows-only colour variant with no developers' name. The proposal restores its pre-4edb965a1 descriptive name.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *Sprite_DrawSprite(RECT *out, Sprite *sprite, GRPort *port,
                                 int colour, int px, int py);

RECT *sprite_draw_colour_fe(RECT *out, Sprite *s, GRPort *port,
                                    int px, int py)
{
    Sprite_DrawSprite(out, s, port, -2, px, py);
    return out;
}
