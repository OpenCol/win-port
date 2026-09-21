/* win_clamp_client_size -- shipped at 1060:a395.
 *
 * Ported by tools/port.py from matched/game/win_clamp_client_size.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void get_pos_words_1060_af79(void *r, col_i16 *pa, col_i16 *pb);
void get_size_words_1060_af51(void *r, col_i16 *pa, col_i16 *pb);
void win_set_client_size_1060_aecf(void *r, col_i16 w, col_i16 h);

/* same object: 4-byte `push cs; call ` (rule 47) */

void win_clamp_client_size(Obj *o, register col_i16 w, register col_i16 h)
{
    col_i16 minw, minh;
    col_i16 maxw, maxh;

    get_size_words_1060_af51(((char *)((unsigned char *)(o) + 0x26)), &minw, &minh);
    get_pos_words_1060_af79(((char *)((unsigned char *)(o) + 0x26)), &maxw, &maxh);
    if (w < minw)
        w = minw;
    if (w > maxw)
        w = maxw;
    if (h < minh)
        h = minh;
    if (h > maxh)
        h = maxh;
    win_set_client_size_1060_aecf(((char *)((unsigned char *)(o) + 0x26)), w, h);
}
