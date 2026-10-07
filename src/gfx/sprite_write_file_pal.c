/* sprite_write_file_pal: candidate for 1068:6da8 (510 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_write_file_pal
 * name-source: descriptive
 *
 * The write side of `sprite_read_file_pal`, field for field: the two `int`s at +0x10
 * and +0x12, the two 8-byte blocks at +0 and +8, the byte at +0x18, then the pixel length --
 * which comes from `MSBlockSize` on the sprite's own handle rather than from the caller -- and
 * then the pixels themselves, with the sprite locked around the write.
 *
 * The palette follows: a handle of `count * 3`, `MSCopyRawColors` filling it from `pal`
 * starting at `first`, and the triples written out. Unlike the reader, this one writes no first
 * index and no count of its own, so the pair is NOT symmetric: a file this writer produces
 * cannot be read back by that reader, which looks for two words this one never puts in.
 * Nothing in the image calls it, which is probably why nobody noticed.
 *
 * `count` is a register parameter in SI and the palette handle a plain local Borland promotes
 * into DI -- the same ordering the reader needed (rule 53).
 *
 * A local `DiskFile`, so the function carries Borland's exception block and the destructor runs
 * on both exits.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
struct logpal;

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);
extern void MSCopyRawColors(struct logpal *pal, unsigned char *dst, int first, int count);

/* same object: 4-byte `push cs; call ` */

void sprite_write_file_pal(Sprite *sprite, char *name, int first,
                                         register int count, void *pal)
{
    DiskFile f;
    long size;
    void *rgb;
    int h;

    size = MSBlockSize(sprite->handle);
    if (MSCreateFile_1068_9565(&f, name) == 0)
        return;
    MSWriteFile_1068_958b(&f, &sprite->hx, 2L);
    MSWriteFile_1068_958b(&f, &sprite->hy, 2L);
    MSWriteFile_1068_958b(&f, &sprite->bounds, 8L);
    MSWriteFile_1068_958b(&f, &sprite->area, 8L);
    MSWriteFile_1068_958b(&f, &sprite->key, 1L);
    MSWriteFile_1068_958b(&f, &size, 4L);
    Sprite_Lock(sprite);
    MSWriteFile_1068_958b(&f, sprite->bits, size);
    Sprite_Unlock(sprite);
    h = MSAllocateHandle((long)(count * 3));
    rgb = MSHLock(h);
    MSCopyRawColors((logpal *)(identity_ptr_1068_9625(pal)), (unsigned char *)rgb, first, count);
    MSWriteFile_1068_958b(&f, rgb, (long)(count * 3));
    MSCloseFile_1068_95b5(&f);
    rgb = MSHUnlock(h);
    MSDisposeHandle(h);
}

