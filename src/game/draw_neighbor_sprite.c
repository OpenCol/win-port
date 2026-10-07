/* draw_neighbor_sprite: candidate for 1040:1464 (83 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Draws the sprite chosen by the neighbour-clear mask at the current tile, when non-zero.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void draw_neighbor_sprite(void)
{
    char buf[8];
    int m = tile_neighbor_clear();

    if (m)
        Sprite_Draw((RECT *)buf, (Sprite *)&g_edge_sprites[m], &g_map_port, g_icon_dx + g_draw_px, g_icon_dy + g_draw_py);
}
