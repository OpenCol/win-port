/* find_control -- shipped at 1060:9be6.
 *
 * Ported by tools/port.py from matched/game/find_control.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Ctl, Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hbutton
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Ctl * get_p_12_1060_abe3(Ctl *c);
char byte_at_16(Ctl *c);

Ctl * find_control(Win *w)
{
    Ctl *c;

    for (c = (*((Ctl * *)((unsigned char *)(w) + 0x8c))); c != 0; c = get_p_12_1060_abe3(c)) {
        if (byte_at_16(c))
            return c;
    }
    return 0;
}
