/* LinkedList_PediaEntry_dtor: candidate for 1008:fbd2 (74 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: LinkedList_PediaEntry_dtor
 * name-source: the binary's own type descriptor
 *
 * THE DESTRUCTOR OF `LinkedList<PediaEntry>`. The name is not a guess: the `__tpdsc__` type
 * descriptor at SEG2:0xfac2 -- one of the run-time type records Borland emits for `delete` --
 * carries a pointer to this function followed by the string `LinkedList<PediaEntry>`. The
 * class derives from `VList` (the destructor calls `VList::~VList(this, 0)`), and the 0x22-byte
 * record just before it at SEG2:0xfbc2 is this class's own descriptor.
 *
 * WHY IT IS WRITTEN OUT BY HAND AND NOT AS A DESTRUCTOR. Every other destructor in the binary --
 * and every destructor Borland C++ 4.52 generates from a class declaration, for a plain base, a
 * virtual base, a member, a virtual destructor, an inline body or a template instance, under every
 * `-x`, `-V`, `-R` and `-J` flag -- puts the constructed-object counter FIRST and the null check
 * second:
 *
 *     MOV SI,[BP+0xA] / DEC DWORD [SS:0x10] / CMP DWORD [BP+6],0 / JZ end
 *
 * This one is the other way round, and it is the ONLY function in the image that is: a scan of all
 * 131 sites that touch the counter finds no other place where the null check comes first. It also
 * ends with an `XOR AX,AX` that no generated destructor has, on a path that shares the single
 * epilogue rather than getting one of its own. So this is reconstructed as the equivalent C, and
 * two Borland-specific spellings are what make the bytes come out:
 *
 *   - `(*(long _ss *)0x10)--` is the counter. `extern long g_count;` -- with or without ``,
 *     and whether or not it is `static` -- gives `DEC DWORD [0x0000]` with a relocation and no
 *     segment prefix. Only an `_ss`-qualified pointer to the literal address produces the shipped
 *     `66 36 FF 0E 10 00`, which is exactly what the compiler emits for the counter of its own
 *     accord in `GamePrefs::~GamePrefs` and the other thirty destructors in this segment.
 *   - `_AX = 0;` is the else arm. Borland's `_AX` pseudo-register assigns the return value without
 *     a `return` statement: two bytes, `XOR AX,AX`, falling straight into the one epilogue. Every
 *     `return 0;` spelling tried instead -- in the else, as an early exit, after a `goto`, with the
 *     return type `int`, `unsigned` or `short` -- emits its own epilogue and leaves a dead second
 *     one for the function to fall off into (rule 44), which is three bytes too many.
 *
 * The empty `else` on the delete test is what gives the `EB 00` before it (rule 54).
 */

#include "colonization.h"

/* PORT: this file's own VList removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"
extern void VList_dtor(VList *p, int flag);

void LinkedList_PediaEntry_dtor(VList *p, unsigned flag)
{
    register unsigned f;

    f = flag;
    if (p != 0) {
        /* PORT: `(*(long _ss *)0x10)--` removed: Borland's exception-handling count of live
           objects at SS:0x10, which nothing on Win32 keeps (W1). */
        VList_dtor(p, 0);
        if (f & 1)
            __bdele_qnv_rtl(p);
        else
            ;
    }
    /* PORT: `else _AX = 0;` removed: it set the return register of a void function (W1). */
}

