/* sprite_draw_in_colour -- shipped at 1068:78ce.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw_in_colour.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

RECT * sprite_draw_scaled(RECT *out, Sprite *s, GRPort *port,
                                 col_i16 colour, col_i16 px, col_i16 py);

RECT * sprite_draw_in_colour(RECT *out, Sprite *s, GRPort *port,
                                    unsigned char colour, col_i16 px, col_i16 py)
{
    sprite_draw_scaled(out, s, port, colour, px, py);
    return out;
}
