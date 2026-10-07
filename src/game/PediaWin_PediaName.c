/* PediaWin_PediaName: candidate for 1060:27a8 (134 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 134, not the census 127. The trailing 7 bytes are the dead second epilogue.
 *
 * Builds an entry's display name in the shared buffer at DGROUP:0x4e14 and returns a 
 * pointer to it: the text string named by the entry's first word, plus -- for category 2 (terrain) with
 * an index between 8 and 15 -- a space and the word at SEG20:0xd8d8, which `InfoTerrain2`
 * already reads as a terrain-feature table.
 *
 * `signatures.py` gives two pointers over six call sites and a return. The FIRST
 * pointer is never touched: the function reads only `[bp+0xa]`. A byte match cannot see an
 * unused parameter (§121), so the arity is the call sites' evidence, not this file's.
 *
 * The three tests are one `&&` chain -- three direct conditional jumps to the same label,
 * which is rule 12's positive form, not three `if`s.
 *
 * RENAMED 2026-09-27 from entry_display_name: BEYOND THE PROPOSALS: the Pedia block (see chains). Mac PediaWin::PediaName(PediaEntry*), CODE 20 0x1292, 148 bytes. Callers: FillListArea, HandleLeftClick, SortEntries. 27a8 (void *unused_this, PediaEntry *entry), 134 bytes; callers PediaWin_DrawListArea (282e, = FillListArea), PediaWin_HandleLeftClick, PediaWin_SortEntries. Callees: strings, space_string, say_string on both. Layout: directly after FillStringList on both.
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern char *strings(int n);

extern void space_string(char *buf);

char *PediaWin_PediaName(PediaWin *unused, PediaEntry *entry)
{
    char *s;

    s = strings(entry->name);
    STRCPY(g_display_name_buf, s);
    if (entry->category == 2 && entry->index >= 8 && entry->index < 0x10) {
        space_string(g_display_name_buf);
        say_string(g_display_name_buf, g_feature_words[FEATURE_FOREST]);
    }
    return g_display_name_buf;
}
