/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 92 of the imported file. */
/* PORT: this file's own declarations of MSOpenFile removed: the header declares it from the definition (W1 step 3). */
/* Flic_Init: candidate for 1068:1f42 (458 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Init
 *
 * Opens a FLIC animation: the file (DiskFile at +0xa6) by name -- a miss is reported twice
 * ("Error: FLIC file not found. " and the name) -- the name kept at +0x132, the 128-byte
 * header read to +0x26, the port sized to the header's width and height through the
 * virtual resize, FLI (0xaf11: frame offset 128, speed converted from 1/70 s to ms) or FLC
 * (0xaf12) positioning, a 310000-byte frame buffer allocated and locked, the frame
 * bookkeeping reset and the first frame prepared (Flic_NextFrame). The module's file
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

extern int MSReadFile(MSFile *file, void *buf, long n);
extern int MSSeekFile(MSFile *file, long pos);

extern void Flic_NextFrame(Flic *flic);

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

void Flic_Init(Flic *flic, char *name)
{
    RECT rc;

    if (file_MSOpenFile_1068_95d5(&flic->file, name) == 0) {
        report_error_1068_963d(&g_error_log, "Error: FLIC file not found. ");
        report_error_1068_963d(&g_error_log, name);
        return;
    }
    STRCPY(flic->name, name);
    save_read_field_1068_95fb(&flic->file, flic->header, 128L);
    SetRect(&rc, 0, 0, flic->width, flic->height);
    flic->ResizePort(&rc);
    if (flic->magic == 0xaf11) {
        flic->frame_offset = 128;
        MSSeekFile_1068_964b(&flic->file, flic->frame_offset - 128);
        flic->speed = flic->speed * 1000 / 70;
    } else if (flic->magic == 0xaf12)
        MSSeekFile_1068_964b(&flic->file, flic->frame_offset - 128);
    flic->hbuf = MSAllocateHandle(310000L);
    flic->buf = (char *)MSHLock(flic->hbuf);
    flic->pos = (unsigned char*)(flic->buf);
    flic->frame_no = flic->rect_index = 0;
    flic->timer = -1;
    Flic_NextFrame(flic);
    flic->frame2 = flic->pos;
}

