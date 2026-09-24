/* anim_advance_frames -- shipped at 1068:268c.
 *
 * Ported by tools/port.py from matched/gfx/anim_advance_frames.c.
 *
 * The file's own Flic dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void flic_step_frame(Flic *f);
void palette_set_range(void *pal, col_i16 first, col_i16 count, void *rgb);

/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */


void anim_advance_frames(Flic *f, col_i16 n, void *pal)
{
    f->f23e = f->f23a;
    f->frame = 0;
    while (f->frame != n) {
        flic_step_frame(f);
        f->frame++;
    }
    if (f->dirty != 0) {
        palette_set_range(pal, f->first, f->count, ((char *)((unsigned char *)(f) + 0x252)));
        f->dirty = 0;
    }
}
