/* setup_building: candidate for 1008:80f4 (117 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Fills one entry of the 12-byte building table at SEG20:0xd422: bytes +3, +2, +4 and +6
 * (+2 is a second prerequisite building: init_data passes -1 for all 42, so none has one)
 * from the four arguments, and the table in data segment 21 indexed by the fourth argument
 * receives the entry's byte +5.
 *
 * THE TABLE IS `g_site_kind` AT SEG21:0x0, MEASURED 2026-09-28. It was declared
 * `g_kind_by_family` ("data segment 21", no offset; earlier `g_building_by_kind`), and a
 * gap-elimination argument here put it at SEG21:0x1fa. The shipped bytes say otherwise: the
 * OFFSET16 at 1008:8163 is 0x0000 in SEG21 (tools/check_data_refs.py), which is
 * `g_site_kind`, the fifteen-byte site -> kind table `make_building_table` fills and
 * `ColonyWin_DrawBuildings` reads. So this seeds that same table indexed by family
 * (`d`, written into `g_buildings[i].family` on the line above) with the entry's `kind`;
 * `make_building_table` rewrites all fifteen entries in kind order before any site is drawn.
 *
 * RENAMED 2026-09-27 from building_table_set_fields: BEYOND THE PROPOSALS, the neighbour of init_data. Mac setup_building(short x5), CODE 26 0x44c4, 128 bytes, called only by init_data (42 times per its seq). 80f4 (int i, int prev_level, int b, int next_level, int family): five words, 117 bytes, called only by building_table_init, 42 times. Leaf on both sides. Layout: first of the setup_building / setup_purchase / init_data triple on both. The old name was descriptive. tools/check_findings.py:1968 labels its check 'building_table_set_fields writes fields 2, 3, 4 and 6'; the check is by address (0x80f4), only the prose goes stale. No collision.
 */

#include "colonization.h"

#include "records.h"

void setup_building(int i, int prev_level, int prereq2, int next_level, int family)
{
    g_buildings[i].prev_level = prev_level;
    g_buildings[i].prereq2 = prereq2;
    g_buildings[i].next_level = next_level;
    g_buildings[i].family = family;
    g_site_kind[family] = g_buildings[i].kind;
}
