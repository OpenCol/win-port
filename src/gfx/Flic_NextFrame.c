/* Flic_NextFrame: candidate for 1068:258e (254 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_NextFrame
 * name-source: sibling-build
 *
 * Decodes ONE FLIC frame. `flic_seek_frame` drives it in a loop and `Flic_Init2` calls
 * it once for the first frame. The 16 bytes it reads first are the FLIC frame header --
 * `long size, short type, short chunks` and eight reserved bytes -- and `Flic_DecodeFrame`
 * is handed it as its `Resource`, whose `count` at +6 is this header's CHUNK COUNT. That is
 * what pins the header's shape: the sub-chunk walker already reads a short at +6 and treats
 * it as a record count, and the FLIC format puts the chunk count exactly there.
 *
 * TWO SOURCES, one shape. When +0x236 is not -1 the frame comes out of the file already
 * resident at the pointer +0x23e, advanced 16 bytes and with +0x24a dropped by 16; when
 * it IS -1 the same 16 bytes and then `size - 16` more are READ FROM THE FILE at +0xa6 into
 * that same pointer, which is a scratch buffer in that case. The body walk that follows is
 * shared, and only the resident case advances the pointer past the body at the end -- which
 * is why that last `if` repeats the test rather than living in the first branch.
 *
 * `pos` is a `` pointer and every move through it is Borland's three-step:
 * `ADD` the offset, `ADC` the carry, then `SHL` by the imported `__AHSHIFT` before adding to
 * the segment. A `` pointer would wrap inside the segment instead, and a FLIC frame can
 * be longer than 64K.
 *
 * `size - 16 > 0` is signed (`JNG`), so a header claiming less than 16 skips the body
 * rather than reading backwards.
 *
 * RENAMED 2026-09-26 from flic_step_frame: the Mac build's Flic::NextFrame(void): this only, decoding the next FLIC frame. The width matches.
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
extern "C" void Flic_DecodeFrame(Flic *flic, unsigned char *data, FlicFrame *hdr);
void Flic_NextFrame(Flic *flic)
{
    FlicFrame hdr;

    if (flic->res_id != -1) {
        MSCopyMem(flic->pos, &hdr, 16L);
        flic->pos += 16;
        flic->left -= 16;
    } else {
        save_read_field_1068_95fb(&flic->file, &hdr, 16L);
        save_read_field_1068_95fb(&flic->file, flic->pos, hdr.size - 16);
    }
    if (hdr.size - 16 > 0)
        Flic_DecodeFrame(flic, flic->pos, &hdr);
    if (flic->res_id != -1)
        flic->pos += hdr.size - 16;
}
