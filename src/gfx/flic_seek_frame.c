/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 80 of the imported file. */
/* flic_seek_frame: candidate for 1068:268c (137 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: flic_seek_frame
 * name-source: descriptive
 *
 * Rewinds a FLIC and steps it to absolute frame `n`, then installs its palette if the last
 * frame changed one. `Flic_NextFrame` is the single-frame step -- `Flic_Init2` calls it once to decode the
 * first frame -- and +0x564 is the frame counter this drives it with.
 *
 * The dirty flag at +0x556 is what gates the palette, and the function clears it: the
 * decoder sets it when a frame carries a colour chunk, and only then do the `count` entries
 * from `first` at +0x552/+0x554 get pushed into the caller's palette from the RGB block at
 * +0x252.
 *
 * The loop test is `!=`, not `<`. A caller asking for a negative count would run the
 * counter all the way round.
 *
 * The first statement copies a long from +0x23a to +0x23e with one 32-bit `MOV` -- a
 * saved-position pair, restored before the stepping starts.
 *
 * RENAMED 2026-09-26 from anim_advance_frames: The body first rewinds the FLIC: it copies the first-frame data pointer (+0x23a) over the current read position (+0x23e) and zeroes the frame counter (+0x564). Only then does it decode frames with Flic_NextFrame until the counter equals n, and it installs the palette if one changed. So it goes to absolute frame n from the start. 'advance_frames' says it steps n frames forward from where the animation already is; called twice with the same n, it lands on the same frame both times.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, GRPort, DiskFile removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */
typedef struct { char pad[1]; } logpal;

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
extern "C" void CTable_Fill(CTable *pal, int start, int count, void *rgb);
/* same object: 4-byte `push cs; call ` */
extern "C" void Flic_NextFrame(Flic *flic);

void flic_seek_frame(Flic *flic, int n, void *pal)
{
    flic->pos = (unsigned char*)(flic->buf);
    flic->frame_no = 0;
    while (flic->frame_no != n) {
        Flic_NextFrame(flic);
        flic->frame_no++;
    }
    if (flic->dirty != 0) {
        CTable_Fill((CTable *)pal, flic->pal_first, flic->pal_count, flic->rgb);
        flic->dirty = 0;
    }
}
