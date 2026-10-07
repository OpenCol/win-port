/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 96, 98 of the imported file. */
/* flic_load_resource: candidate for 1068:210c (467 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: flic_load_resource
 *
 * Loads a FLIC animation from the FLC resource `id` (type at DGROUP:0x2af6): the resource
 * handle (+0x232) and its locked data (+0x23a), the name "<id>.FLC", the 128-byte header
 * copied to +0x26, the port sized through the virtual resize, FLI speed converted from
 * 1/70 s to ms (FLC left alone), the data pointer advanced past the header by the frame
 * offset, the frame bookkeeping reset and the first frame prepared (Flic_NextFrame). A missing
 * resource is reported ("Error: FLIC resource not found - ", id). The module's resource
 * wrappers are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

extern void *MSGetResource(char *type, int id);
extern void *MSLockResource(void *h);

extern void Flic_NextFrame(Flic *flic);

extern "C" void *MSGetResource_1068_94c7(void *type, int id);

extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

extern "C" void *MSLockResource_1068_94f8(void *h);

void flic_load_resource(Flic *flic, int id)
{
    RECT rc;
    void *type;

    type = g_flic_resource_type;
    flic->res = MSGetResource_1068_94c7(&type, id);
    if (flic->res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: FLIC resource not found - ", (long)(unsigned)id);
        return;
    }
    flic->buf = (char *)MSLockResource_1068_94f8(flic->res);
    LTOA1(id, flic->name, 10);
    STRCAT(flic->name, ".FLC");
    flic->res_id = id;
    MSCopyMem(flic->buf, flic->header, 128L);
    SetRect(&rc, 0, 0, flic->width, flic->height);
    flic->ResizePort(&rc);
    if (flic->magic == 0xaf11) {
        flic->frame_offset = 128;
        flic->speed = flic->speed * 1000 / 70;
    } else if (flic->magic == 0xaf12)
        ;
    flic->buf += flic->frame_offset;
    flic->pos = (unsigned char*)(flic->buf);
    flic->frame_no = flic->rect_index = 0;
    flic->pos = (unsigned char*)(flic->buf);
    Flic_NextFrame(flic);
    flic->frame2 = flic->pos;
}

