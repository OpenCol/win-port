/* MSReleaseResource_1040_a9d8: candidate for 1040:a9d8 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 28, not the census 28. The trailing bytes are the dead second epilogue.
 *
 * This module's -callable forwarder to `MSReleaseResource`, named for the routine it reaches, as
 * the rest of the wrapper layer is. It is one of a run of same-sized forwarders back to back
 * -- 1040:a99c, a9bc and a9d8 -- which is what a module's private copy of a small resource API looks like.
 *
 * RENAMED 2026-09-27 from free_resource_1040_a9d8: Per-module forwarder to MSReleaseResource (1078), 28-32 bytes, one call. It takes the body's name with its address suffix. mac_bind proposed the bare name for 1068:952d/9549; the corpus convention for forwarders (named for the routine they reach) gives this form. RESOURCE BLOCK: Windows 1078 open_resource_dll (00ef), free_resource (0186), MSGetResource (019c), MSGetNamedResource (0277), lock_resource (0386), unlock_resource (03a0). The Mac MS file has MSOpenResFile, MSReleaseResource, MSGetResource, MSGetNamedResource, MSLockResource, MSUnlockResource. The order is EXACT, anchored on the two already-bound names. The per-module forwarders take MSName_<seg>_<off>, as the existing MSGetResource_1068_94c7 family does.
 */

#include "colonization.h"

extern void MSReleaseResource(void *p);

void MSReleaseResource_1040_a9d8(void *res)
{
    MSReleaseResource(res);
}
