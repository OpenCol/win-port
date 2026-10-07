/* king_meeting: candidate for 1048:6bc1 (144 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 144, not the census 85. The extra bytes are this `switch`'s CS-relative jump
 * table, which Borland puts in the CODE segment and Ghidra's reachable-code size excludes.
 *
 * The audience with the King, and it happens ONCE: bit 5 of the first byte of the record at
 * DGROUP:0x4cc4 is a latch. Set already, the player gets one text resource and nothing else;
 * clear, it is set, `king_welcome` runs, and the answer to the following prompt picks one of
 * five replies -- two more texts and the three things the King can actually do.
 *
 * The `switch` is over 1..5 with no `case 0`, which is why Borland biases by one (`DEC BX`)
 * before bounding at 4. The zero answer is tested separately, ahead of it, and that test is
 * a bare conditional `goto` -- `OR SI,SI / JNZ +2 / JMP` -- where the latch test above it is
 * a block and gets a direct jump (rules 12 and 12a in one function).
 *
 * The label carries an explicit `return;` and not a bare `;`. That is rule 44 arithmetic:
 * with the empty statement the function is five bytes short, exactly one epilogue, because
 * both `goto`s and the fall-through all reach the same end and Borland still writes the
 * `return`'s own.
 *
 * The pointer at DGROUP:0x4cc4 is a NEAR variable holding a pointer, so it loads
 * with a bare `LES BX,[0x4cc4]`; declared `` as well it would be `MOV AX,SEG / MOV ES,AX`
 * first, which is the declaration seam `GameBrains_Init`'s header records.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */

extern void europe_recruit_any(void);
extern void king_grant_funds(void);

void king_meeting(void)
{
    register int r;

    if (g_cur_nation_rec->flags & 0x20) {
        ask_text_resource("KINGGOAWAY");
        goto out;
    }
    g_cur_nation_rec->flags |= 0x20;
    king_welcome();
    r = ask_text_resource("AUDIENCE");
    if (r == 0)
        goto out;
    switch (r) {
    case 1:
        ask_text_resource("KINGBLESS");
        break;
    case 2:
        europe_recruit_any();
        break;
    case 3:
        king_grant_funds();
        break;
    case 4:
        king_adjust_tax();
        break;
    case 5:
        ask_text_resource("KINGLAUGH");
        break;
    }
out:
    return;
}
