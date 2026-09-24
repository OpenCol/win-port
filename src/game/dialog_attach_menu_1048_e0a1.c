/* dialog_attach_menu_1048_e0a1 -- shipped at 1048:e0a1.
 *
 * Ported by tools/port.py from matched/game/dialog_attach_menu_1048_e0a1.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GRPort_Init_wh_1048_e613(void *w);
void menu_attach_1048_e895(void *d);

typedef struct { char pad[6]; void *menu; } DialogWin;

extern void * menu_attach_to_window(void *menu);





void dialog_attach_menu_1048_e0a1(char *w)
{
    menu_attach_1048_e895((DialogWin *)(w + 0x26));
    GRPort_Init_wh_1048_e613(w);
}
