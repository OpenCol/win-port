/* europe_load_picture -- shipped at 1048:89dc.
 *
 * Ported by tools/port.py from matched/game/europe_load_picture.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_palette[];                    SEG20:0x01ac
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void load_picture_resource(void *port, col_i16 id, col_i16 w, col_i16 h, void *pal);

void europe_load_picture(char *w)
{
    load_picture_resource(w + 0x884, 0xfb, 0x28, 0x60, g_game_palette);
}
