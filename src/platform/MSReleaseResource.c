/* PORT: integer <-> pointer through uintptr_t/intptr_t: a resource id, a 16-bit handle token or a parameter block, the same bits on i386 and on 64-bit Windows (W9). */
/* MSReleaseResource: candidate for 1078:0186 (22 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * FreeResource(h).
 *
 * The parameter is FOUR bytes and the body cannot say so: it pushes only `[bp+0x6]`, which
 * is the same instruction for an `int` and for the low half of a pointer. All 7 call
 * sites push a segment and then an offset -- the -pointer shape of rule 25 -- and clean
 * up with `ADD SP,4`, so the argument is a pointer whose OFFSET half is the handle.
 * `tools/signatures.py` is what settles it; `bytematch` grades both spellings RELOCATABLE.
 *
 * RENAMED 2026-09-27 from free_resource: Mac MSReleaseResource(ResourceStruct**). The union of its forwarders' callers covers Mac MSReleaseResource's callers: CTable::Init, Sprite::Init, GRPort::LoadPicture, Flic::<dtor> (1068:9549), Opening::<dtor> (1010:2d8e), Popup::start (1050:8e1e), text_open and DoFancyText (1040:a9d8, text_open/draw_text_resource). Arity 1. Mac LoadPicture's seq has MSReleaseResource x2, and load_picture_resource calls 1068:9549 x2. RESOURCE BLOCK: Windows 1078 open_resource_dll (00ef), free_resource (0186), MSGetResource (019c), MSGetNamedResource (0277), lock_resource (0386), unlock_resource (03a0). The Mac MS file has MSOpenResFile, MSReleaseResource, MSGetResource, MSGetNamedResource, MSLockResource, MSUnlockResource. The order is EXACT, anchored on the two already-bound names. The per-module forwarders take MSName_<seg>_<off>, as the existing MSGetResource_1068_94c7 family does.
 */

#include "colonization.h"

void MSReleaseResource(void *p)
{
    FreeResource((int)(intptr_t)p);
}
