/* MSReleaseResource_1010_2d8e: candidate for 1010:2d8e (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `MSReleaseResource`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-27 from free_resource_1010_2d8e: Per-module forwarder to MSReleaseResource (1078), 28-32 bytes, one call. It takes the body's name with its address suffix. mac_bind proposed the bare name for 1068:952d/9549; the corpus convention for forwarders (named for the routine they reach) gives this form. RESOURCE BLOCK: Windows 1078 open_resource_dll (00ef), free_resource (0186), MSGetResource (019c), MSGetNamedResource (0277), lock_resource (0386), unlock_resource (03a0). The Mac MS file has MSOpenResFile, MSReleaseResource, MSGetResource, MSGetNamedResource, MSLockResource, MSUnlockResource. The order is EXACT, anchored on the two already-bound names. The per-module forwarders take MSName_<seg>_<off>, as the existing MSGetResource_1068_94c7 family does.
 */

#include "colonization.h"

extern void MSReleaseResource(void *p);

void MSReleaseResource_1010_2d8e(void *p)
{
    MSReleaseResource(p);
}
