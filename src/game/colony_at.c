/* colony_at -- shipped at 1018:471a.
 *
 * Ported by tools/port.py from matched/game/colony_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void show_text_dialog_1018_b3d1(char *msg);

extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 colony_at(col_i16 x, col_i16 y)
{
    col_i16 i;
    col_i16 found = -1;

    if (!on_map(x, y))
        goto done;
    if (square_european_nation(x, y) < 0)
        goto done;
    for (i = 0; found < 0 && g_colony_count > i; i++)
        if (g_colonies[i].x == (unsigned char)x && g_colonies[i].y == (unsigned char)y)
            found = i;
    if (found < 0)
        show_text_dialog_1018_b3d1("COLONYFLAG");
done:
    return found;
}
