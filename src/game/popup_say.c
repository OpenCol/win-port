/* popup_say: candidate for 1050:5f30 (37 bytes, 284 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the `n`-th string of the packed text block into name slot `slot` -- the game's
 * commonest way of filling a `%1`-style substitution before a message is shown, and one of
 * the three most-called functions in the corpus.
 *
 * Both halves were nameless when this was first matched and both are named now, which is
 * what settles the meaning. `strings` (1040:774e) walks the packed NUL-separated
 * string block at SEG29:0x10e0; `set_name_slot_1050_8e3a` STRCPYs into `g_name_slots[i]`,
 * the table of 0x50-byte strings the TEXT formatter substitutes from. Its sibling
 * `set_text_word` calls this one with a word-table entry, and
 * `popup_country` fills the same table from a built-up buffer instead.
 *
 * `strings` is in another module and is reached by a real CALL (9A);
 * `set_name_slot_1050_8e3a` by PUSH CS + CALL, the form Borland emits for a 
 * function it knows is in the SAME segment, so it has to be defined here for the bytes to
 * agree (rule 47). Only this function's own bytes are compared -- `--symbol` picks it out of
 * the object by its PUBDEF offset -- so the stub's body is irrelevant.
 *
 * RENAMED 2026-09-26 from set_name_slot_from_text: the Mac build's popup_say(int, int), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller Show; caller colony_click_ship; caller europe_click_ship; caller europe_purchase; caller europe_recruit), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

char *strings(int n);                     /* another module: real call */

/* same module: PUSH CS + call */

void popup_say(int slot, int n)
{
    set_name_slot_1050_8e3a(slot, strings(n));
}
