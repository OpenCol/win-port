/* sprite_draw9_in_colour -- shipped at 1068:790c.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw9_in_colour.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

RECT * sprite_blit_mode(RECT *out, Sprite *s, GRPort *port, col_i16 colour,
                            col_i16 px, col_i16 py, col_i16 e);

RECT * sprite_draw9_in_colour(RECT *out, Sprite *s, GRPort *port,
                                     unsigned char colour, col_i16 px, col_i16 py, col_i16 e)
{
    sprite_blit_mode(out, s, port, colour, px, py, e);
    return out;
}
