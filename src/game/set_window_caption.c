/* set_window_caption -- shipped at 1058:bd6c.
 *
 * Ported by tools/port.py from matched/game/set_window_caption.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetWinTitle_of_1058_c555(void *w, char *s);

typedef struct { char pad[6]; void *win; } GameWinBase;
typedef struct { char pad[0x26]; GameWinBase win; char pad2[0xc4 - 0x26 - sizeof(GameWinBase)]; char caption[1]; } GameWin_local;
extern void MSSetWinTitle(void *w, char *s);

extern void draw_window_title(GameWin_local *w);



void set_window_caption(GameWin_local *w, char *s)
{
    MSSetWinTitle_of_1058_c555(&w->win, s);
    strcpy(w->caption, s);
    draw_window_title(w);
}
