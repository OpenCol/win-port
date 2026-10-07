/* popm: candidate for 1050:64d1 (47 bytes, 155 call sites in 67 functions).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * size-source: 47, not the census 43. The trailing 4 bytes are a SECOND epilogue, and
 * they are there because the body ends in a return STATEMENT -- Borland emits an
 * epilogue for it AND one for the end of the function (rule 44).
 *
 * IT RETURNS AN INT, and this file said `void` until 2026-09-21. Nothing in the 47
 * bytes sets AX, which is why `void` byte-matches; but the value the inner call leaves
 * there is the function's answer, and the callers read it. `ColonyWin_colony_hammer_buy` does
 * `r = popm("BUYME1", 5)` and then `if (r != 2)`, and 40 of the 65
 * declarations across the corpus say `int`. Under -Od the two spellings compile to the
 * same bytes, so the byte match cannot choose between them -- the CALL SITES can, and
 * a port cannot assign the result of a void function at all.
 *
 *   MOV AX,[BP+0xa] ; MOV [0x251a],AX      store an int into a DGROUP global.
 *                                          Plain DS addressing (a3 = MOV [imm16],AX),
 *                                          so the global is NEAR -- contrast 1068:616a,
 *                                          where a `` global forced an ES load.
 *   PUSH 0 ; PUSH 0                        TWO 16-bit zeros, not one NULL: a 
 *                                          NULL compiles to `66 6A 00` (one 32-bit
 *                                          push) under -3, so these are two int args
 *   PUSH [BP+8] ; PUSH [BP+6]              second argument: the incoming pointer
 *   PUSH 0x1108 ; PUSH 0x9e                first argument: &object at DGROUP:0x9e
 *   PUSH CS ; CALL 1050:8e85 ; ADD SP,0xc  12 bytes = two pointers and two ints
 *
 * RENAMED 2026-09-26 from text_message_show: the Mac build's popm(char*, short), bound by call-graph agreement (tools/mac_callgraph.py): 12 of its Mac call edges already bound land on this function (caller ReportsForeign; caller Show; caller add_liberty; caller colony_select_profession; caller combat_fight), the runner-up gets 6, and the arity agrees (2).
 */

#include "colonization.h"

/* same object: the shipped reaches this through the 4-byte `PUSH CS / CALL NEAR`, which is
 * the form Borland emits for a callee in the SAME translation unit -- so it is defined
 * here and not declared extern (rule 47). */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

int popm(void *msg, int value)
{
    g_popup_dude = value;
    return text_dialog_1050_8e85(g_game_txt, msg, 0, 0);
}
