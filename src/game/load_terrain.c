/* load_terrain: candidate for 1008:9bc8 (156 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reads one line of the terrain table into the 16-byte row at SEG20:0x78f8 + row * 0x10: a CSV
 * value into the word at +0, four numbers into the bytes at +2..+5, and nine more into +7..+0xf.
 * Byte +6 is not read from the file. `load_data` calls it once per row.
 *
 * RENAMED 2026-09-26 from read_terrain_row: the Mac build's load_terrain(short), bound by call-graph agreement (3 vs 1): one row of the terrain table, called once per row by load_data; the arity agrees and the Mac body is 1.23x this one.
 */

#include "colonization.h"

#include "records.h"

extern char *text_get(void);
extern int text_item_string(void);
extern int text_item_number(void);

void load_terrain(int row)
{
    register int i;

    text_get();
    g_terrain_rows[row].name = text_item_string();
    g_terrain_rows[row].movement = text_item_number();
    g_terrain_rows[row].defence = text_item_number();
    g_terrain_rows[row].work_turns = text_item_number();
    g_terrain_rows[row].value = text_item_number();
    for (i = 0; i < 9; i++)
        g_terrain_rows[row].yield[i] = text_item_number();
}
