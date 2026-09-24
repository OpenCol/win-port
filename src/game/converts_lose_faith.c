/* converts_lose_faith -- shipped at 1038:3c3a.
 *
 * Ported by tools/port.py from matched/game/converts_lose_faith.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_record_free(col_i16 u);

#include "control.h"
#include "nations.h"
#include "units.h"
#include "jobs.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 tile_unit_tally(col_i16 u, col_i16 n);
extern void redraw_tile(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);
extern void text_message_show(char *msg, col_i16 n);

col_i16 converts_lose_faith(col_i16 u)
{
    col_i16 ux;
    col_i16 uy;
    col_i16 owner;
    col_i16 keep = 1;

    ux = g_units[u].x;
    uy = g_units[u].y;
    if (!on_map(ux, uy))
        goto done;
    if (g_units[u].type == UNIT_COLONISTS && (col_i16)g_units[u].spec == JOB_CONVERT && square_nation(ux, uy) < 0 && tile_unit_tally(u, 2) < 2) {
        g_units[u].counter++;
        if (g_units[u].counter > 8) {
            owner = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
            if (owner < NATION_EUROPEAN_COUNT && g_nation52[owner].control == CONTROL_HUMAN)
                redraw_tile(ux, uy);
            unit_record_free(u);
            keep = 0;
            if (owner < NATION_EUROPEAN_COUNT && g_nation52[owner].control == CONTROL_HUMAN) {
                redraw_map_area(ux, uy, 1, 1, 1);
                text_message_show("DEADCONVERTS", 4);
            }
        }
    }
done:
    return keep;
}
