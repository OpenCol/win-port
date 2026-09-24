/* sprite_extract_to_screen -- shipped at 1008:d6b2.
 *
 * Ported by tools/port.py from matched/game/sprite_extract_to_screen.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_screen_surface;                    SEG20:0xdea0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void call_message_pump(void);

extern void MSDoTask(void);
extern void Sprite_Extract_mask(void *s, void *src, col_i16 mode, col_i16 x, col_i16 y, col_i16 w, col_i16 h);



void sprite_extract_to_screen(void *s, col_i16 mode, col_i16 x, col_i16 y, col_i16 w, col_i16 h)
{
    call_message_pump();
    Sprite_Extract_mask(s, &g_screen_surface, mode, x, y, w, h);
}
