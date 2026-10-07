/* Sprite_DrawMono: candidate for 1068:794d (61 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `Sprite_DrawMonoSprite` with -1 forced into the colour slot: the six-parameter counterpart of
 * `Sprite_Draw` (1068:7894), reaching the seven-parameter sibling of `Sprite_DrawSprite`
 * rather than `Sprite_DrawSprite` itself. It carries one extra trailing word, which this
 * function only passes on -- what it is has not been read out of `Sprite_DrawMonoSprite`.
 *
 * Argument split corrected with the rest of the family -- see `Sprite_Draw.c`.
 *
 * RENAMED 2026-09-26 from sprite_draw9: the Mac build's Sprite::DrawSprite(GRPort&, short, short, short): the sprite as `this` behind the hidden RECT return slot, then the port, x, y and the blit mode, in that order.
 *
 * RENAMED 2026-09-27 from Sprite_DrawSprite: WRONG EXISTING NAME, CHAIN 1 step 2 (vacates Sprite_DrawSprite). This is Mac Sprite::DrawMono(GRPort&, short, short, char), not Sprite::DrawSprite. Callers: Windows ColonyWin_DrawColonists, DrawIconRow, DrawMultiIconRow, MapWin_PieceIconDraw, PieceIconDraw and ReportWin_ColonyLibertyReport are exactly Mac DrawMono's callers. Mac DrawSprite has ONE caller, Sprite::Draw. Body: Mac DrawMono is DrawMonoSprite(this, &rect, port, -1 (move.w #$ffff), x, y, (char)mode), and 794d is sprite_blit_mode(out, sprite, port, -1, px, py, mode), the same arguments in the same order. Width: this+port+x+y+mode = 5 on both, not counting the hidden RECT slot. Layout: after Sprite_Draw, before Sprite_CopyTo, as Mac Draw, DrawMono, CopyTo. The earlier binding (from sprite_draw9) was by arity only, and Mac DrawSprite has the same count.
 *
 * THE MODE IS A BYTE ALL THE WAY DOWN (2026-10-02, replacing a 2026-09-28 note that it could not
 * be spelled one way). The four callers that pass a computed mode need it declared as a byte:
 * `(flags & 4) ? 0x5f : g_draw_color` is evaluated as `mov al,5Fh`, not `mov ax,5Fh`. The
 * "word read" in Sprite_DrawMonoSprite was BCC passing a char argument to a char parameter:
 * it pushes the whole word, `push word [bp+18h]`, with no extension, just as it pushes the char
 * local `col` (`push word [bp-17h]`). The earlier DIFFERS came from declaring the segment-18
 * blitter's last parameter `int`. With `unsigned char mode` in this function, in
 * Sprite_DrawMonoSprite, in sprite_draw9_in_colour and in the blitter's prototype, every body
 * grades and every caller's declaration names the same symbol (the Mac DrawMono and
 * DrawMonoSprite take a char too).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *Sprite_DrawMonoSprite(RECT *out, Sprite *sprite, GRPort *port, int colour,
                            int px, int py, unsigned char mode);

RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port,
                           int px, int py, unsigned char mode)
{
    Sprite_DrawMonoSprite(out, sprite, port, -1, px, py, mode);
    return out;
}
