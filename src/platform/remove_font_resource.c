/* remove_font_resource: candidate for 1078:16b5 (138 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Gives back a loaded font resource: lock the handle, delete every HFONT the record holds
 * and null each slot, hand the record itself to `RemoveFontResource` -- the record BEGINS
 * with the file name, which is why the same pointer serves as the LPSTR -- then unlock and
 * free the handle.
 *
 * The layout follows from the two offsets and the API. `RemoveFontResource` takes the
 * pointer unchanged, so bytes 0..0xff are the name; the count is the word at +0x100; the
 * handles start at +0x106, indexed by `i` doubled.
 *
 * The loop test is `count > i`, not `i < count`: the shipped code loads the count from the
 * record and compares it against SI, which is the order the source has it in.
 *
 * The index is recomputed three times per iteration -- `MOV AX,SI / ADD AX,AX / LES BX / ADD
 * BX,AX` before the test, before the delete and before the store -- because nothing survives
 * a call and `-Od` does not hoist.
 */

#include "colonization.h"

void remove_font_resource(int h)
{
    FontRes *p;
    register int i;

    p = (FontRes *)MSHLock(h);
    for (i = 0; p->count > i; i++) {
        if (p->obj[i] != 0) {
            DeleteObject(p->obj[i]);
            p->obj[i] = 0;
        }
    }
    RemoveFontResource(p->name);
    MSHUnlock(h);
    MSDisposeHandle(h);
}
