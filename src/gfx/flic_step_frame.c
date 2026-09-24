/* flic_step_frame -- shipped at 1068:258e.
 *
 * Ported by tools/port.py from matched/gfx/flic_step_frame.c.
 *
 * The file's own FlicFrame, MSFile, Flic dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: pos -> f23e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 save_read_field_1068_95fb(MSFile *f, void *buf, col_i32 n);
void sprite_draw_records(Flic *f, unsigned char *data, FlicFrame *hdr);

extern void MSCopyMem(void *src, void *dst, col_i32 n);

/* same object: 4-byte `push cs; call ` (rule 47) */

void flic_step_frame(Flic *f)
{
    FlicFrame hdr;

    if ((*((col_i16 *)((unsigned char *)(f) + 0x236))) != -1) {
        MSCopyMem(f->f23e, &hdr, 16L);
        f->f23e += 16;
        (*((col_i32 *)((unsigned char *)(f) + 0x24a))) -= 16;
    } else {
        save_read_field_1068_95fb(&(*((MSFile *)((unsigned char *)(f) + 0xa6))), &hdr, 16L);
        save_read_field_1068_95fb(&(*((MSFile *)((unsigned char *)(f) + 0xa6))), f->f23e, hdr.size - 16);
    }
    if (hdr.size - 16 > 0)
        sprite_draw_records(f, f->f23e, &hdr);
    if ((*((col_i16 *)((unsigned char *)(f) + 0x236))) != -1)
        f->f23e += hdr.size - 16;
}
