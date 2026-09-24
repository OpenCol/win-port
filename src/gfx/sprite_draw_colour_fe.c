/* sprite_draw_colour_fe -- shipped at 1068:798a.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw_colour_fe.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

RECT * sprite_draw_scaled(RECT *out, Sprite *s, GRPort *port,
                                 col_i16 colour, col_i16 px, col_i16 py);

RECT * sprite_draw_colour_fe(RECT *out, Sprite *s, GRPort *port,
                                    col_i16 px, col_i16 py)
{
    sprite_draw_scaled(out, s, port, -2, px, py);
    return out;
}
