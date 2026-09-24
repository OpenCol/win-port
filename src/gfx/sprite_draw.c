/* sprite_draw -- shipped at 1068:7894.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

RECT * sprite_draw_scaled(RECT *out, Sprite *s, GRPort *port,
                                 col_i16 colour, col_i16 px, col_i16 py);

RECT * sprite_draw(RECT *out, Sprite *s, GRPort *port, col_i16 px, col_i16 py)
{
    sprite_draw_scaled(out, s, port, -1, px, py);
    return out;
}
