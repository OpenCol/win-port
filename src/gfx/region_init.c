/* PORT: rect_frames is i16 * (Borland's int), so its cast says so (W1). */
/* region_init: candidate for 1068:2785 (728 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: region_init
 *
 * Builds an animation's region rectangles, or loads them from their cache. The two arrays are
 * allocated and locked first -- 0x75300 bytes of rectangles at +0x55c (handle +0x558) and 0x1d4c0
 * of words at +0x560 (handle +0x55a) -- with the count at +0x568 cleared and the smallest and
 * largest per-frame counts at +0x56a and +0x56c primed to 32000 and 0. When
 * `load_frr_file` answers nonzero the work is done.
 *
 * Otherwise every FLIC frame is walked, in the two-source shape of `Flic_NextFrame`: out
 * of memory at the pointer +0x23e when +0x236 is not -1, read from the file at +0xa6 into that
 * pointer when it is. Each frame header's type must be 0xF1FA or the routine returns with nothing
 * saved; its body goes to `build_region_rects` with the frame number in +0x564. From a
 * file, the first frame header is read into the frame buffer and copied out, where each later one
 * is read straight into the header. At the end the count
 * is handed with "Total rects calculated:" (DGROUP:0x2bdb) to the empty debug routine, and
 * `save_frr_file` writes the cache.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

extern char load_frr_file(Flic *flic);
extern void build_region_rects(Flic *region, unsigned char *data, FlicFrame *hdr);
extern char save_frr_file(Flic *flic);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

void region_init(Flic *flic)
{
    unsigned char *pos;
    FlicFrame hdr;

    flic->count = 0;
    flic->least = 32000;
    flic->most = 0;
    flic->rect_index = 0;
    flic->hrects = MSAllocateHandle(0x75300L);
    flic->rects = (RECT *)MSHLock(flic->hrects);
    flic->hrect_frames = MSAllocateHandle(0x1d4c0L);
    flic->rect_frames = (i16 *)MSHLock(flic->hrect_frames);
    if (load_frr_file(flic))
        return;
    if (flic->res_id == -1)
        save_read_field_1068_95fb(&flic->file, flic->pos, 16L);
    pos = flic->pos;
    MSCopyMem(pos, &hdr, 16L);
    if (flic->res_id == -1) {
        save_read_field_1068_95fb(&flic->file, flic->pos, hdr.size - 16);
        pos = flic->pos;
    } else {
        pos += 16;
    }
    if (hdr.type != 0xf1fa)
        return;
    flic->frame_no = 0;
    build_region_rects(flic, pos, &hdr);
    if (flic->res_id != -1)
        pos += hdr.size - 16;
    for (flic->frame_no = 1; flic->frame_no < flic->frames; flic->frame_no++) {
        if (flic->res_id == -1) {
            save_read_field_1068_95fb(&flic->file, &hdr, 16L);
            save_read_field_1068_95fb(&flic->file, flic->pos, hdr.size - 16);
            pos = flic->pos;
        } else {
            MSCopyMem(pos, &hdr, 16L);
            pos += 16;
        }
        if (hdr.type != 0xf1fa)
            return;
        build_region_rects(flic, pos, &hdr);
        if (flic->res_id != -1)
            pos += hdr.size - 16;
    }
    report_error_1068_94ea(&g_error_log, "Total rects calculated:", (long)flic->count);
    save_frr_file(flic);
}
