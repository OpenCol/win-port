/* strings_shutdown: candidate for 1040:76d5 (27 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * RENAMED from `strings_shutdown`. The block at SEG29:0x10dc is the STRING POOL -- its allocator
 * `strings_init` resets `g_text_count` beside it, `strings_store` copies strings into it,
 * and `strings` reads them back -- and this frees that pool and touches nothing else.
 * The old name came from the one caller, `clean_up`, which frees the pool during the
 * same teardown as the map planes; it described the context, not the function.
 *
 * Frees the block at SEG29:0x10dc through `heap_destroy`, which is reached by the four-byte
 * `push cs / call ` and so is defined here as a stub (rule 47).
 *
 * RENAMED 2026-09-26 from text_pool_free: the Mac build's strings_shutdown(void), bound by layout (between strings_init and strings_store on both sides); the arity agrees and the Mac body is 1.63x this one.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void heap_destroy(MemBlock *block);

void strings_shutdown(void)
{
    heap_destroy(&g_text_pool);
}
