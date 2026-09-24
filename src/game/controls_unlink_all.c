/* controls_unlink_all -- shipped at 1060:9aa9.
 *
 * Ported by tools/port.py from matched/game/controls_unlink_all.c.
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
col_i16 ctl_id_1060_abca(Ctl *c);
void list_unlink_by_id(void *w, col_i16 id);

void controls_unlink_all(Win *w)
{
    Ctl *c;

    for (c = (*((Ctl * *)((unsigned char *)(w) + 0x8c))); c != 0; c = (*((void * *)((unsigned char *)(c) + 0x12))))
        list_unlink_by_id(w, ctl_id_1060_abca(c));
}
