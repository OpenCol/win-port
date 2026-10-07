/* close_resource_dll: candidate for 1078:03b6 (83 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSOpenResFile`'s inverse: find the library handle in the table at DGROUP:0x56e6,
 * shift every entry after it down one, drop the count at 0x3c76 and hand the handle back to
 * KERNEL. A handle that is not in the table is not freed -- the search runs off the end, the
 * `i < count` test fails and the whole body is skipped, so calling it twice is harmless.
 *
 * The shift reads ONE PAST the last live entry: the loop runs while `i < count` and reads
 * `[i + 1]`, so the final iteration copies `g_resource_libs[count]`, a word that has never
 * been written. It is then immediately outside the table as as the count is concerned,
 * so the stale read is harmless -- but it is in the shipped code and not an artefact of this
 * reconstruction, which is why the bound is written as it is rather than `count - 1`.
 *
 * `g_resource_libs[i + 1]` is folded into the displacement -- `MOV BX,SI / ADD BX,BX` and
 * then `[BX+0x56e8]`, the base plus two -- rather than incrementing the index first.
 *
 * The comparisons are `JC`, unsigned, so the index and the count are both unsigned; this is
 * the same pair of globals `MSGetNamedResource` walks and it spells them the
 * same way.
 */

#include "colonization.h"

void close_resource_dll(unsigned h)
{
    unsigned i;

    for (i = 0; g_resource_libs[i] != h && i < g_resource_lib_count; i++)
        ;
    if (i < g_resource_lib_count) {
        for (; i < g_resource_lib_count; i++)
            g_resource_libs[i] = *(g_resource_libs + i + 1);
        g_resource_lib_count--;
        FreeLibrary(h);
    }
}
