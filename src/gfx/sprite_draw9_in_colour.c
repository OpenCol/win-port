/* sprite_draw9_in_colour: candidate for 1068:790c (65 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The seven-argument member of the same family: `Sprite_DrawMonoSprite` rather than
 * `Sprite_DrawSprite`, with the colour coming from the caller as a byte and widened in the
 * push.
 *
 * Argument split corrected with the rest of the family -- see `Sprite_Draw.c`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *Sprite_DrawMonoSprite(RECT *out, Sprite *sprite, GRPort *port, int colour,
                            int px, int py, unsigned char mode);

RECT *sprite_draw9_in_colour(RECT *out, Sprite *sprite, GRPort *port,
                                     unsigned char colour, int px, int py, unsigned char mode)
{
    Sprite_DrawMonoSprite(out, sprite, port, colour, px, py, mode);
    return out;
}
