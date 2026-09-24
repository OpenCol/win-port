/* MainWin_CanClose -- shipped at 1048:adf3.
 *
 * Ported by tools/port.py from matched/game/MainWin_CanClose.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_quit_flag;                          DGROUP:0x4b04
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ask_text_resource(char *name);
extern void low_memory_watch_stop(void);


col_i16 MainWin_CanClose(void)
{
    if (ask_text_resource("DOS") == 2) {
        low_memory_watch_stop();
        g_quit_flag = 1;
        return 1;
    } else {
        return 0;
    }
}
