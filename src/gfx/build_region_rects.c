/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 101, 104, 113 of the imported file. */
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
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

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
                blit_rle_mask_words(region->bits, (unsigned char*)(rec->data), region->stride);
                break;
            case 15:
                mask_runs_fill(region->bits, (unsigned char*)(rec->data), region->w, region->h, region->stride);
                break;
            case 13:
                GRPort_FillColor_all(region, 0xff);
                break;
            case 16:
                GRPort_FillColor_all(region, 0xff);
                break;
            case 12:
                blit_rle_mask(region->bits, (unsigned char*)(rec->data), region->stride);
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

