/* strings_init: candidate for 1040:76af (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Allocates the string pool at SEG29:0x10dc (`heap_create` kind 2, `size` bytes) and
 * resets the string count at DGROUP:0x1cf4 to zero. It opens a four-function module on
 * that pool, back to back: this, `strings_shutdown` (1040:76d5), `strings_store`
 * (1040:76f0), which copies a string in and returns `g_text_count++`, and
 * `strings`, which reads one back.
 *
 * `heap_create` is reached by the four-byte `push cs; call ` with no NOP, so it is
 * defined here as a stub (rule 47). The count is stored DS-relative, the same 
 * declaration `strings_store` uses.
 *
 * RENAMED 2026-09-26 from text_pool_alloc: the Mac build's strings_init(int), bound by call-graph agreement (2 vs 1) and by call-sequence alignment (1.0 aligned gaps in load_data leave it as the lone unbound callee, none against); the arity agrees.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int heap_create(MemBlock *heap, int kind, unsigned size);

void strings_init(unsigned size)
{
    heap_create(&g_text_pool, 2, size);
    g_text_count = 0;
}
