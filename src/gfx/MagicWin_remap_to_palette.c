/* MagicWin_remap_to_palette: candidate for 1068:ab60 (162 bytes, 5 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_remap_to_palette
 *
 * Builds the remap table from the object's own palette (the port at +0x26) to a target
 * palette (the pointer at +0xea) for `count` entries from `first`, then rewrites the
 * pixels of the port at +0xc4 through it. A temporary handle of `count` bytes holds the
 * table.
 *
 * THE OBJECT IS A MagicWin, not a Sprite: the port at +0xc4 is MagicWin's GRPort member, the
 * palette slot at +0xea sits between it and the rectangle at +0xee, and all five callers are
 * in the MagicWin module (1068:a698..b188) -- the four 202-byte loaders store the palette
 * pointer at +0xea and the range at +0xf6/+0xf8 before calling this.
 *
 * RENAMED 2026-09-27 from sprite_remap_to_palette: the receiver is a MagicWin, not a Sprite: port at +0xc4, palette at +0xea, and every caller sits in MagicWin's module 1068:a698..b188 (521cb981e). The Mac build has no such method; the name stays descriptive.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *get_ctable_1068_d77f(void *port);
extern void palette_remap_table(struct logpal *a, unsigned char *out, struct logpal *b, short first, short n);
#include "records.h"
extern void port_remap_pixels(Port *port, unsigned char *table, short base, short count);

void MagicWin_remap_to_palette(MagicWin *w, short first, short count)
{
    short handle;
    unsigned char *p;

    handle = MSAllocateHandle((long)count);
    p = (unsigned char *)MSHLock(handle);
    palette_remap_table((struct logpal *)get_ctable_1068_d77f(&(DialogWin &)*w), p, (struct logpal *)w->pal, first, count);
    port_remap_pixels((Port *)&w->port, p, first, count);
    p = (unsigned char *)MSHUnlock(handle);
    handle = MSDisposeHandle(handle);
}
