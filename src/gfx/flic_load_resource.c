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
class Flic : public GRPort {
public:
    char header[4];                     /* +0x26 */
    unsigned magic;                     /* +0x2a */
    unsigned short frames;              /* +0x2c */
    short width, height;                /* +0x2e */
    char pad_32[4];
    long speed;                         /* +0x36 */
    char pad_3a[2];
    long created;                       /* +0x3c */
    char pad_40[0x76 - 0x40];
    long frame_offset;                  /* +0x76 */
    char pad_7a[0xa6 - 0x7a];
    DiskFile file;                      /* +0xa6 */
    char name[0x232 - 0x132];           /* +0x132 */
    void *res;                      /* +0x232 */
    short res_id;                       /* +0x236 */
    short hbuf;                         /* +0x238 */
    char *buf;                     /* +0x23a */
    unsigned char *pos;            /* +0x23e */
    void *frame2;                   /* +0x242 */
    char pad_246[4];
    long left;                          /* +0x24a */
    short f24e, f250;                   /* +0x24e */
    unsigned char rgb[0x300];           /* +0x252 */
    short pal_first;                    /* +0x552 */
    short pal_count;                    /* +0x554 */
    unsigned char dirty;                /* +0x556 */
    char f557;
    short hrects, hrect_frames;         /* +0x558 */
    RECT *rects;                    /* +0x55c */
    int *rect_frames;               /* +0x560 */
    short frame_no, rect_index;         /* +0x564 */
    unsigned count, least, most;        /* +0x568 */
    short timer;                        /* +0x56e */
    void *port;                     /* +0x570 */
    void (*loop_hook)(void);        /* +0x574 */
    Flic();
    ~Flic();
};

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

