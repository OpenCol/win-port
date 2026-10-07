/* say_version: candidate for 1008:cc49 (40 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Fills name slots 0 and 1 with the strings at DGROUP:0x93f and 0x949 -- two calls to
 * `set_name_slot_1008_cdca`, which is reached by the four-byte form and so is defined here as a
 * stub (rule 47).
 *
 * RENAMED 2026-09-27 from set_default_name_slots: String witness: this function's only literals are '3.1415926' and '24-May-95', and Mac say_version(void) has exactly that pair (df 1/1). They fill %STRING0 and %STRING1 of BEGINMENU, '{COLONIZATION} Version %STRING0 -- %STRING1'. Call graph: Mac say_version's only caller is begin_game, and cc49's only caller is ca99, which becomes begin_game. It is at the same position in both sequences, after sound_play and CopyRawColors. Arity: (void) on both sides. Size is 58 Mac against 40 Windows. The descriptive name describes the mechanism (two set_name_slot calls) and not the purpose. The name is free.
 */

#include "colonization.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */

void say_version(void)
{
    set_name_slot_1008_cdca(0, "3.1415926");
    set_name_slot_1008_cdca(1, "24-May-95");
}
