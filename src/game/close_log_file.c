/* close_log_file -- shipped at 1038:d175.
 *
 * Ported by tools/port.py from matched/game/close_log_file.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_log_flag;                               DGROUP:0x1c68
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 FCLOSE(void *f);

col_i16 close_log_file(void)
{
    register col_i16 r = 1;
    void *f = 0;

    r = 0;
    g_log_flag = 0;
    if (f != 0)
        FCLOSE(f);
    return r;
}
