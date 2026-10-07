/* Flic_ctor: candidate for 1068:1cec (261 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic::Flic()
 *
 * The constructor of Flic (0x578 bytes, over GRPort, vtable at DGROUP:0x2b30) with a DiskFile
 * member at +0xa6: the frame, timing and buffer fields initialised -- 0x236 = -1, 0x552 = 10,
 * 0x554 = 236, the rest 0, with the long at +0x23a cleared twice as shipped.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

Flic::Flic()
{
    f24e = f250 = 0;
    res_id = -1;
    buf = 0;
    dirty = 0;
    pal_first = 0xa;
    pal_count = 0xec;
    loop_hook = 0;
    buf = 0;
    hbuf = 0;
    rects = 0;
    hrects = 0;
    rect_frames = 0;
    hrect_frames = 0;
    res = 0;
}
