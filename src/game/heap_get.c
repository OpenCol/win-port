/* heap_get: candidate for 1040:6c52 (148 bytes).  Verdict: RELOCATABLE.
 * symbol: heap_get
 *
 * Carves `n` bytes off a heap block and returns a pointer to them, or NULL. The block
 * is a bump allocator: a base pointer, a used count and a remaining count, and a request
 * moves `used` up and `avail` down by the same amount. Over-allocating does NOT fail --
 * it reports "No more heap" (DGROUP:0x1dbb, read out of the shipped image) through the
 * same reporter and context byte `Sprite_ChangeColor` and `GRPort_ChangeColor` use, and
 * then carries on and hands out the memory anyway. The only thing that returns NULL is
 * `heap_lock` refusing.
 *
 * The pointer is `h->base + h->used`, and it is FAR pointer arithmetic: the shipped code
 * loads the segment and the offset separately, adds `used` to the offset alone, and never
 * normalises. So a block that grows past 64K wraps within its segment rather than
 * advancing it -- which is the documented behaviour of `+` on a `` pointer and the
 * reason `` exists (rule 38).
 *
 * The second local is not decoration: the frame is 8 bytes and holds TWO pointers, the
 * result and the temporary the sum is built in. Written with one, the function is four
 * bytes short and `SUB SP,4` gives it away at +0xc before any other divergence appears.
 *
 * The `heap_lock` guard is a `goto` and not an `if` body -- the shipped bytes are
 * `JZ +2 / JMP`, Borland's invert-and-jump-over-a-jump form (rule 12), where the block
 * form emits a single `JNZ`.
 *
 * NAME: the Macintosh build's, and the binary corroborates it in its own words.
 * `heap_get(Heap*, unsigned short)` has exactly this arity and role, and macui carries
 * `heap_create(Heap*, int, unsigned short)`, `heap_destroy(Heap*)`, `heap_lock(Heap*)` and
 * `heap_release(Heap*)` alongside it -- a family whose members line up on arity, which is
 * the argument that named the explore chain in finding 96.
 *
 * The corroboration that settles it is not the arity. `matched/heap_lock.c` -- the
 * function called here, at `1040:6b68` -- reports **"Heap Lock Failed"** when its lock
 * returns NULL. So the callee says in the shipped strings that the thing it locks is a
 * HEAP, and it takes one pointer, which is `heap_lock(Heap*)`. Its record is this record:
 * it declares `{ unsigned char kind; char flag; short handle; void *ptr; }` with the
 * kind at +0 and the pointer at +4, the same two fields this function reads as `id` and
 * `base`. Two functions, one record, and the other one names it.
 */

#include "colonization.h"

#include "records.h"

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern void report_error_1040_aa02(void *ctx, char *msg, int id, unsigned n);
extern int heap_lock(MemBlock *block);   /* the Macintosh heap_lock(Heap*) */

void *heap_get(MemBlock *h, unsigned n)
{
    char *p;
    char *q;

    p = 0;
    if (h->avail < n)
        report_error_1040_aa02(&g_error_log, "No more heap", h->kind, n);
    if (heap_lock(h) != 0)
        goto done;
    q = (char *)h->ptr + h->used;
    p = q;
    h->used += n;
    h->avail -= n;
done:
    return p;
}
