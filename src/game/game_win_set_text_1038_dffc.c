/* game_win_set_text_1038_dffc -- shipped at 1038:dffc.
 *
 * Ported by tools/port.py from matched/game/game_win_set_text_1038_dffc.c.
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
void MSSetWinTitle_of_1038_e517(void *d, char *s);

extern void draw_window_title(void *w);



void game_win_set_text_1038_dffc(char *o, char *s)
{
    MSSetWinTitle_of_1038_e517(o + 0x26, s);
    strcpy(o + 0xc4, s);
    draw_window_title(o);
}
