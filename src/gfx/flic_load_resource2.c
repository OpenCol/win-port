/* flic_load_resource2: candidate for 1068:22df (52 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: flic_load_resource2
 *
 * Loads a FLIC resource with a given PALETTE RANGE: the first entry and count at +0x552/+0x554
 * are set and flic_load_resource does the rest; it and the module's resource wrappers are
 * defined here because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * (Corrected 2026-09-26: this said a playback WIDTH and HEIGHT. The only caller, ClosingWin_ctor,
 * passes 10 and 0xec -- palette entries 10..245, the 236 a Windows 256-colour palette leaves once its
 * 20 static colours are reserved -- and flic_seek_frame reads +0x552/+0x554 as the palette
 * range it installs after a colour chunk.)
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

extern "C" void flic_load_resource(Flic *flic, int id);

void flic_load_resource2(Flic *flic, int id, int first, int count)
{
    flic->pal_first = first;
    flic->pal_count = count;
    flic_load_resource(flic, id);
}

