/* gui_init -- shipped at 1070:af5c.
 *
 * Ported by tools/port.py from matched/platform/gui_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_hinstance;                              DGROUP:0x56e2
 *   extern int g_hprev;                                  DGROUP:0x56e4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void write_gui_error_log(void);
extern void register_window_classes(void);

col_i16 gui_init(col_i16 hinst, col_i16 hprev)
{
    g_hinstance = hinst;
    g_hprev = hprev;
    write_gui_error_log();
    register_window_classes();
    return 1;
}
