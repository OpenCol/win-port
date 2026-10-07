/* set_name_slot_1048_de36: candidate for 1048:de36 (41 bytes, 12 callers).  Verdict: RELOCATABLE.
 *
 * The module-1048 copy of set_name_slot, under the name it already carried. Copies a string into slot i of a table of 0x50-byte strings. The same 41 bytes,
 * with the same table, occur in twelve segments (1048:de36 is named
 * set_name_slot_1048_de36): one small method compiled into every module that uses it.
 *
 * RENAMED 2026-09-26 from dialog_set_option_text: byte-identical to set_name_slot/set_name_slot_1008_cdca/set_name_slot_1010_e658/set_name_slot_1018_b3f6/set_name_slot_1020_a94a/set_name_slot_1028_e412/set_name_slot_1038_8775/set_name_slot_1058_c2a5 (analysis/re-exact-dupes.json)/so it takes that family's name.
 */

#include "colonization.h"

void set_name_slot_1048_de36(int i, char *s)
{
    STRCPY(g_name_slots[i], s);
}
