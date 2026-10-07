/* PORT: rect_frames is i16 * (Borland's int), so its cast says so (W1). */
/* Flic_dtor: candidate for 1068:1df1 (337 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic::~Flic()
 *
 * The destructor of Flic (over GRPort, vtable at DGROUP:0x2b30) with a DiskFile member at
 * +0xa6: the three locked memory blocks (handles +0x238, +0x558, +0x55a with their pointers at
 * +0x23a, +0x55c, +0x560) are unlocked and disposed, the object at +0x232 released through
 * MSReleaseResource_1068_9549; member and base destruction are the compiler's.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSReleaseResource_1068_9549(void *res);

Flic::~Flic()
{
    if (buf != 0)
        buf = (char *)MSHUnlock(hbuf);
    if (hbuf != 0)
        MSDisposeHandle(hbuf);
    if (rects != 0)
        rects = (RECT *)MSHUnlock(hrects);
    if (hrects != 0)
        MSDisposeHandle(hrects);
    if (rect_frames != 0)
        rect_frames = (i16 *)MSHUnlock(hrect_frames);
    if (hrect_frames != 0)
        MSDisposeHandle(hrect_frames);
    if (res != 0)
        MSReleaseResource_1068_9549(res);
}
