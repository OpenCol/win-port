/* unit_refresh_move_points -- shipped at 1038:3bf5.
 *
 * Ported by tools/port.py from matched/game/unit_refresh_move_points.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char unit_move_points(col_i16 i);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void panel_clear_4(void *panel, col_i16 unit);

void unit_refresh_move_points(col_i16 unit)
{
    col_i16 n = unit;

    g_units[n].moves = unit_move_points(n);
    if (g_selected_unit == n)
        panel_clear_4(&(*g_game_record2), n);
}
