/* toggle_a06_window -- shipped at 1010:adfa.
 *
 * Ported by tools/port.py from matched/game/toggle_a06_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_flag_a06;                               DGROUP:0x0a06
 *   extern char far *g_owner_a00;                        DGROUP:0x0a00
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void control_hide_1010_e54d(DlgControl *c);
void control_show_1010_e574(DlgControl *c);

extern void hide_hwnd(col_i16 h);
extern void show_hwnd(col_i16 h);
extern void invalidate_hwnd(col_i16 h);





void toggle_a06_window(void)
{
    g_flag_a06 = !g_flag_a06;
    if (g_flag_a06 == 0)
        control_hide_1010_e54d((DlgControl *)(g_owner_a00 + 0x972));
    else
        control_show_1010_e574((DlgControl *)(g_owner_a00 + 0x972));
}
