/* set_name_slot_1008_cdca: candidate for 1008:cdca (41 bytes).  Verdict: RELOCATABLE.
 *
 * The copy in segment 1008 of set_name_slot (1030:eb8e). Copies a string into slot i of a table of 0x50-byte strings. The same 41 bytes,
 * with the same table, occur in twelve segments (1048:de36 is named
 * set_name_slot_1048_de36): one small method compiled into every module that uses it.
 */

#include "colonization.h"

void set_name_slot_1008_cdca(int i, char *s)
{
    STRCPY(g_name_slots[i], s);
}
