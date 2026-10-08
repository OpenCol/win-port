/* PORT: integer <-> pointer through uintptr_t/intptr_t: a resource id, a 16-bit handle token or a parameter block, the same bits on i386 and on 64-bit Windows (W9). */
/* MSLockResource: candidate for 1078:0386 (26 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * LockResource(h).
 *
 * The parameter is FOUR bytes and the body cannot say so: it pushes only `[bp+0x6]`, which
 * is the same instruction for an `int` and for the low half of a pointer. All 7 call
 * sites push a segment and then an offset -- the -pointer shape of rule 25 -- and clean
 * up with `ADD SP,4`, so the argument is a pointer whose OFFSET half is the handle.
 * `tools/signatures.py` is what settles it; `bytematch` grades both spellings RELOCATABLE.
 *
 * RENAMED 2026-09-27 from lock_resource: Mac MSLockResource(ResourceStruct**). Its forwarders' callers are CTable_Init, Sprite_Init, load_picture_resource (1068:94f8), PostLowMemReport (1068:da48), Opening_ctor (1010:2d39), Popup_start (1050:8dc2), text_open and draw_text_resource (1040:a99c) and build_opening_art (1060:88a7). They cover Mac MSLockResource's 8 callers. Arity 1. RESOURCE BLOCK: Windows 1078 open_resource_dll (00ef), free_resource (0186), MSGetResource (019c), MSGetNamedResource (0277), lock_resource (0386), unlock_resource (03a0). The Mac MS file has MSOpenResFile, MSReleaseResource, MSGetResource, MSGetNamedResource, MSLockResource, MSUnlockResource. The order is EXACT, anchored on the two already-bound names. The per-module forwarders take MSName_<seg>_<off>, as the existing MSGetResource_1068_94c7 family does.
 */

#include "colonization.h"

void *MSLockResource(void *p)
{
    return LockResource((int)(intptr_t)p);
}
