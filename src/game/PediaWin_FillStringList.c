/* PediaWin_FillStringList: candidate for 1060:251a (654 bytes).  Verdict: RELOCATABLE.
 * symbol: PediaWin_FillStringList
 * name-source: sibling-build
 * size-source: 654, not the census 640 -- the last fourteen bytes are the switch's seven-entry
 *   jump table.
 *
 * Fills the Colonopedia's list for whichever category is selected. Seven cases, one per
 * category, each walking a different table and adding `(name word, category, index)` triples:
 * goods, unit types, terrain, the eight-byte spec records, buildings, Founding Fathers, and
 * the advances, whose count alone is a variable (DGROUP:0x98).
 *
 * THREE OF THE SEVEN SKIP ENTRIES. Terrain skips 0x10..0x17 -- eight rows in the middle of the
 * table that are not terrain the player can read about. The spec list skips 0x12 alone.
 * Buildings skip anything whose NAME matches the one before it, which is how the upgrade levels
 * of one building collapse into a single entry, and 0x1e and 0x1f as well; the name is rendered
 * through `say_string` into a scratch buffer and compared with `STRCMP`, so the test is on the
 * displayed text and not on the table.
 *
 * Afterwards the wanted article at +0x854, if there is one, is searched for by walking the list
 * and comparing each entry's index. The search calls `PediaWin_Entry` TWICE PER ITERATION and once
 * more after the loop -- and that last call dereferences the answer without testing it, so a
 * wanted index that is not in the list reads through a null pointer. Reading 0000:0004 is
 * harmless under Win16, which is presumably why it was never noticed.
 *
 * `default: break;` is in the shipped bytes: the out-of-range jump lands on an `EB 00` that
 * falls straight through, where a switch with no default jumps to the end directly (rule 54).
 *
 * RENAMED 2026-09-26 from pedia_fill_list: the Mac build's PediaWin::FillStringList(void): this only, rebuilding the index list for the selected category through PediaWin_AddString. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "fathers.h"
#include "goods.h"
#include "records.h"
#include "classes.h"

extern int VList_Count(void *list);

/* 5-byte NOP form, so extern (rule 47) */
extern void PediaWin_EmptyStringList(PediaWin *w);
extern void PediaWin_AddString(PediaWin *w, int word, int cat, int idx);
extern PediaEntry *PediaWin_Entry(PediaWin *w, int idx);
extern void PediaWin_SortEntries(PediaWin *w);

void PediaWin_FillStringList(PediaWin *w)
{
    char prev[0x50];
    char name[0x50];
    register int i;

    PediaWin_EmptyStringList(w);
    switch (w->kind) {
    case 0:
        for (i = 0; i < GOODS_COUNT; i++)
            PediaWin_AddString(w, g_goods_name_word[i], 0, i);
        break;
    case 1:
        for (i = 0; i < 0x17; i++)
            PediaWin_AddString(w, g_unit_types[i].name, 1, i);
        break;
    case 2:
        for (i = 0; i < 0x1d; i++) {
            if (i >= GOODS_COUNT && i < 0x18)
                continue;
            PediaWin_AddString(w, g_terrain_rows[i].name, 2, i);
        }
        break;
    case 3:
        for (i = 0; i < 0x1c; i++) {
            if (i == 0x12)
                continue;
            PediaWin_AddString(w, g_spec_recs[i].name, 3, i);
        }
        break;
    case 4:
        prev[0] = 0;
        for (i = 0; i < 0x2a; i++) {
            name[0] = 0;
            say_string(name, g_buildings[i].name);
            if (STRCMP(name, prev) == 0)
                continue;
            if (i == 0x1e || i == 0x1f)
                continue;
            PediaWin_AddString(w, g_buildings[i].name, 4, i);
            STRCPY(prev, name);
        }
        break;
    case 5:
        for (i = 0; i < FATHER_COUNT; i++)
            PediaWin_AddString(w, g_fathers[i].name, 5, i);
        break;
    case 6:
        for (i = 0; g_pedia_count > i; i++)
            PediaWin_AddString(w, g_pedia_word[i], 6, i);
        break;
    default:
        break;
    }
    w->count = VList_Count(&w->list);
    PediaWin_SortEntries(w);
    if (w->wanted != -1) {
        for (i = 0; PediaWin_Entry(w, i) != 0 && PediaWin_Entry(w, i)->index != w->wanted; i++)
            ;
        if (PediaWin_Entry(w, i)->index == w->wanted)
            w->sel = i;
    }
}
