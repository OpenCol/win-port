/* sprite_write_file: candidate for 1068:6fc2 (372 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_write_file
 * name-source: descriptive
 *
 * `sprite_read_file` run backwards: the same six header fields in the same order,
 * the pixel length taken from `MSBlockSize` on the sprite's handle, and the pixels written with
 * the sprite locked around them. No palette -- that is the neighbour at 1068:6da8.
 *
 * Nothing but the `DiskFile` lives in the frame beside the length, so the function pushes
 * neither SI nor DI.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

/* same object: 4-byte `push cs; call ` */

void sprite_write_file(Sprite *spr, char *name)
{
    DiskFile f;
    long size;

    size = MSBlockSize(spr->handle);
    if (MSCreateFile_1068_9565(&f, name) == 0)
        return;
    MSWriteFile_1068_958b(&f, &spr->hx, 2L);
    MSWriteFile_1068_958b(&f, &spr->hy, 2L);
    MSWriteFile_1068_958b(&f, &spr->bounds, 8L);
    MSWriteFile_1068_958b(&f, &spr->area, 8L);
    MSWriteFile_1068_958b(&f, &spr->key, 1L);
    MSWriteFile_1068_958b(&f, &size, 4L);
    Sprite_Lock(spr);
    MSWriteFile_1068_958b(&f, spr->bits, size);
    Sprite_Unlock(spr);
    MSCloseFile_1068_95b5(&f);
}

