/* sprite_draw_in_colour: candidate for 1068:78ce (62 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The sibling of `Sprite_Draw` (1068:7894) that takes the colour from its caller instead of
 * forcing -1. The parameter is a BYTE and the push widens it -- `MOV AL,[BP+0x12] /
 * MOV AH,0 / PUSH AX` -- which is what a `unsigned char` parameter passed to an `int`
 * argument compiles to, and what settles its width. Everything else goes straight through
 * and the incoming rectangle pointer comes back as the result.
 *
 * Argument split corrected with the rest of the family -- see `Sprite_Draw.c`.
 *
 * The NOP before PUSH CS is TLINK's -call optimisation.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *Sprite_DrawSprite(RECT *out, Sprite *sprite, GRPort *port,
                                 int colour, int px, int py);

RECT *sprite_draw_in_colour(RECT *out, Sprite *sprite, GRPort *port,
                                    unsigned char colour, int px, int py)
{
    Sprite_DrawSprite(out, sprite, port, colour, px, py);
    return out;
}
