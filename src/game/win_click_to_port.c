/* win_click_to_port -- shipped at 1060:a281.
 *
 * Ported by tools/port.py from matched/game/win_click_to_port.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1060_aeaf(void *ev);
col_i16 rect_width_1060_ae90(void *ev);
void win_set_client_size_1060_aecf(void *port, col_i16 x, col_i16 y);

extern col_i16 grport_init_1068_4eae(Win *w, void *ev);
/* same object: 4-byte `push cs; call ` (rule 47) */



col_i16 win_click_to_port(Win *w, void *ev)
{
    if (grport_init_1068_4eae(w, ev) == 1) {
        win_set_client_size_1060_aecf(((char *)((unsigned char *)(w) + 0x26)), rect_width_1060_ae90(ev), rect_height_1060_aeaf(ev));
        return 1;
    } else
        return 0;
}
