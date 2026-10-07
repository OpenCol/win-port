/* Flic_DecodeFrame: candidate for 1068:1b11 (474 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_DecodeFrame
 * name-source: sibling-build
 *
 * Walks the records of a sprite resource -- each a 6-byte header (long size, short kind)
 * followed by its body -- and dispatches on the kind: 4 and 11 unpack a run-length palette
 * into the sprite's colour table (marking it loaded), 7, 15 and 16 draw through three
 * positioned blitters, 12 through MSDecodeDeltaFli, 13 fills the port with the background
 * colour. A dense `switch` on kinds 4..16 with a jump table; other kinds are skipped.

 * SIZE: the census says 448 and the function is 474. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from sprite_draw_records: walks one FLIC frame's chunks -- each a 6-byte header (long size,
 * short type) and its body -- for Flic_NextFrame, its only caller. The kinds it handles are exactly the FLIC frame chunk types: 4 COLOR_256, 7 DELTA_FLC (SS2),
 * 11 COLOR (64-level, hence the x4 unpack), 12 DELTA_FLI (LC), 13 BLACK, 15 BYTE_RUN, 16 FLI_COPY.
 *
 * RENAMED 2026-09-27 from flic_draw_chunks: Mac Flic::DecodeFrame(unsigned char*, FRAMEHEAD*), 450 bytes, against 474. Caller: Flic::NextFrame on both. SEQUENCE: Mac MSCopyMem, MSDecodeColor256, MSDecodeDeltaFlc, MSDecodeColor64, MSDecodeDeltaFli, FillColor, MSDecodeByteRun, MSDecodeLiteral is Windows MSCopyMem, unpack_palette_runs, flic_decode_ss2, unpack_palette_runs_x4, flic_decode_lc, GRPort_FillColor_all, flic_decode_brun, flic_decode_copy, 8 of 8. It is the FLIC chunk switch 4/7/11/12/13/15/16. Layout: Mac DiskFile ctor/dtor, DecodeFrame, Flic ctor, dtor, Init; Windows RGBTree_*, 1b11, Flic_ctor, Flic_dtor, Flic_Init. Arity: this+2 = (flic, data, frame).
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

extern void MSDecodeColor256(unsigned char *src, unsigned char *dst);
extern void MSDecodeColor64(unsigned char *src, unsigned char *dst);
extern void MSDecodeDeltaFlc(unsigned char *src, unsigned char *dst, int x, int y, int stride, unsigned char add);
extern void MSDecodeByteRun(unsigned char *src, unsigned char *dst, int x, int y, int stride, unsigned char add);
extern void MSDecodeLiteral(unsigned char *src, unsigned char *dst, int x, int y, int stride, unsigned char add);
extern void MSDecodeDeltaFli(unsigned char *src, unsigned char *dst, int stride, unsigned char add);
extern void GRPort_FillColor_all(GRPort *s, int color);

void Flic_DecodeFrame(Flic *flic, unsigned char *data, FlicFrame *frame)
{
    RecHdr hdr;
    unsigned char *body;
    int i;

    for (i = 0; frame->chunks > i; i++) {
        MSCopyMem(data, &hdr, 6L);
        body = data + 6;
        data += hdr.size;
        switch (hdr.kind) {
        case 4:
            MSDecodeColor256(body, flic->rgb);
            flic->dirty = 1;
            break;
        case 7:
            MSDecodeDeltaFlc(body, flic->bits, flic->width, flic->height, flic->stride, flic->pal_first);
            break;
        case 11:
            MSDecodeColor64(body, flic->rgb);
            flic->dirty = 1;
            break;
        case 12:
            MSDecodeDeltaFli(body, flic->bits, flic->stride, flic->pal_first);
            break;
        case 13:
            GRPort_FillColor_all((GRPort *)flic, g_draw_color);
            break;
        case 15:
            MSDecodeByteRun(body, flic->bits, flic->width, flic->height, flic->stride, flic->pal_first);
            break;
        case 16:
            MSDecodeLiteral(body, flic->bits, flic->width, flic->height, flic->stride, flic->pal_first);
            break;
        default:
            break;
        }
    }
}
