/* nation52_swap_word_at_32: candidate for 1008:8bba (61 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * The word at +0x32 of four 52-byte records through `swap_word_identity` in place.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void nation52_swap_word_at_32(NationText *nation_text)
{
    int i;

    for (i = 0; i < 4; i++)
        nation_text[i].colony_names_used = swap_word_identity(nation_text[i].colony_names_used);
}
