/* PORT: this file's own declarations of MSOpenFile removed: the header declares it from the definition (W1 step 3). */
/* Flic_Init2: candidate for 1068:2313 (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Init2
 *
 * Opens a FLIC animation with a given PALETTE RANGE: the first entry and count at +0x552/+0x554
 * are set and Flic_Init does the rest. Flic_Init and the module's file wrappers are defined
 * here because the shipped calls to them are the compiler's own `push cs / call `.
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

extern int MSReadFile(MSFile *file, void *buf, long n);
extern int MSSeekFile(MSFile *file, long pos);

extern void Flic_NextFrame(Flic *flic);

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

extern "C" void Flic_Init(Flic *flic, char *name);

void Flic_Init2(Flic *flic, char *name, int first, int count)
{
    flic->pal_first = first;
    flic->pal_count = count;
    Flic_Init(flic, name);
}

