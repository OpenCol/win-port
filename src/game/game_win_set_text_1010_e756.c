/* game_win_set_text_1010_e756 -- shipped at 1010:e756.
 *
 * Ported by tools/port.py from matched/game/game_win_set_text_1010_e756.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetWinTitle_of_1010_edd2(void *d, char *s);

extern void draw_window_title(void *w);



void game_win_set_text_1010_e756(char *o, char *s)
{
    MSSetWinTitle_of_1010_edd2(o + 0x26, s);
    strcpy(o + 0xc4, s);
    draw_window_title(o);
}
