/* sprite_draw9 -- shipped at 1068:794d.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw9.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

RECT * sprite_blit_mode(RECT *out, Sprite *s, GRPort *port, col_i16 colour,
                            col_i16 px, col_i16 py, col_i16 e);

RECT * sprite_draw9(RECT *out, Sprite *s, GRPort *port,
                           col_i16 px, col_i16 py, col_i16 e)
{
    sprite_blit_mode(out, s, port, -1, px, py, e);
    return out;
}
