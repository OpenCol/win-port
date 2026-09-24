/* intro_tick -- shipped at 1010:0500.
 *
 * Ported by tools/port.py from matched/game/intro_tick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Opening far *g_opening;                       DGROUP:0x0964
 *
 * The file's own Opening dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void intro_animation_frame(Opening *o);
extern void opening_scene_step(Opening *o);
extern void opening_card_show(Opening *o);
extern void opening_scroll_step(Opening *o);
extern void colony_copy_backdrop(Opening *o);
extern void draw_active_animations(Opening *o);
extern void opening_card_flip(Opening *o);

void intro_tick(void)
{
    if (g_opening != 0) {
        if (g_opening->state != 4)
            intro_animation_frame(g_opening);
        opening_scene_step(g_opening);
        if (g_opening->state != 4) {
            opening_card_show(g_opening);
            opening_scroll_step(g_opening);
            colony_copy_backdrop(g_opening);
            draw_active_animations(g_opening);
            opening_card_flip(g_opening);
            if ((col_i16)g_opening->skip == 1) {
                g_opening->state = 3;
                opening_scene_step(g_opening);
            }
        }
    }
}
