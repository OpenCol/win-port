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
        rect_frames = (int *)MSHUnlock(hrect_frames);
    if (hrect_frames != 0)
        MSDisposeHandle(hrect_frames);
    if (res != 0)
        MSReleaseResource_1068_9549(res);
}
