/* build_region_rects: candidate for 1068:2f3d (616 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: build_region_rects
 * size-source: 616, not the census 429: the code runs to 1068:3191 and its 10-entry jump table
 *   follows. As with `CheatMenuColonist`, one pad byte the object does not emit sits in front of the
 *   table, so the 617-byte gap is one more than compares.
 *
 * Rebuilds an object's region rectangles from a packed region resource. The port is cleared to
 * 0 and the resource's chunks are played into the object's byte grid at +0x18 in turn -- each
 * chunk is a 32-bit length, a type word and its data -- through the three RLE mask routines
 * (types 7, 12, 15) or a fill with 0xFF (types 13 and 16); other types are skipped. Then every
 * cell still 0xFF seeds `grow_region_rect`, which clears the cells it absorbs, and each
 * rectangle goes into the array at +0x55c with the current value of +0x564 in the parallel word
 * array at +0x560. The number found is added to +0x568, and the smallest and largest counts seen
 * are kept at +0x56a and +0x56c.
 *
 * The chunks are only walked when the resource is more than its 4-byte header. The bodies are in
 * memory in the source's order -- 7, 15, 13, 16, 12 -- and not in value order (rule 36).
 *
 * THE MODULE IS C++, and one statement is what says so. `region->rects[region->rect_index] = grow_region_rect(...)`
 * returns a RECT by value into the array slot. Compiled as C, Borland then works out
 * `region->rects + region->rect_index * 8` a second time after the call -- the value of the assignment expression,
 * thrown away -- 21 bytes the shipped function does not have; `*(region->rects + region->rect_index) =` and a
 * `(void)` cast do the same. Compiled with `-P` the second computation is gone and every byte
 * matches. `save_frr_file`, two functions earlier, builds a DiskFile, so the module
 * being C++ was already on record.
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

typedef struct { long size; int type; char data[1]; } FlicChunk;

extern void GRPort_FillColor_all(GRPort *p, int color);
extern RECT grow_region_rect(Flic *g, int x0, int y0);

void build_region_rects(Flic *region, unsigned char *data, FlicFrame *hdr)
{
    long n;
    unsigned char *p;
    unsigned char *row;
    int i;
    int y;
    int found;
    FlicChunk *rec;
    register int x;

    found = 0;
    p = data;
    GRPort_FillColor_all(region, 0);
    n = hdr->size - 4;
    if (n > 0) {
        for (i = 0; hdr->chunks > i; i++) {
            rec = (FlicChunk *)p;
            p += rec->size;
            switch (rec->type) {
            case 7:
                blit_rle_mask_words(region->bits, rec->data, region->stride);
                break;
            case 15:
                mask_runs_fill(region->bits, rec->data, region->w, region->h, region->stride);
                break;
            case 13:
                GRPort_FillColor_all(region, 0xff);
                break;
            case 16:
                GRPort_FillColor_all(region, 0xff);
                break;
            case 12:
                blit_rle_mask(region->bits, rec->data, region->stride);
                break;
            default:
                break;
            }
        }
    }
    for (y = 0; (unsigned)region->height > y; y++) {
        row = region->bits + (long)y * region->stride;
        for (x = 0; (unsigned)region->width > x; x++) {
            if (row[x] == 0xff) {
                region->rects[region->rect_index] = grow_region_rect(region, x, y);
                region->rect_frames[region->rect_index] = region->frame_no;
                region->rect_index++;
                found++;
            }
        }
    }
    region->count += found;
    if (region->least > found)
        region->least = found;
    if (region->most < found)
        region->most = found;
}

