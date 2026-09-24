/* dialog_attach_menu_1060_659e -- shipped at 1060:659e.
 *
 * Ported by tools/port.py from matched/game/dialog_attach_menu_1060_659e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GRPort_Init_wh_1060_65fc(void *w);
void menu_attach_1060_6c08(void *d);

typedef struct { char pad[6]; void *menu; } DialogWin;

extern void * menu_attach_to_window(void *menu);





void dialog_attach_menu_1060_659e(char *w)
{
    menu_attach_1060_6c08((DialogWin *)(w + 0x26));
    GRPort_Init_wh_1060_65fc(w);
}
