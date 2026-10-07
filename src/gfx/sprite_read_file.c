/* sprite_read_file: candidate for 1068:743e (444 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_read_file
 * name-source: descriptive
 *
 * Reads a sprite from its own file into a Sprite. The header fields come first, each through
 * `save_read_field_1068_95fb` with its own length -- the two `int`s at +0x10 and +0x12, the two
 * 8-byte blocks at +0 and +8, one byte at +0x18 -- then a 4-byte pixel length into a local.
 *
 * An already-locked sprite is unlocked first, an old handle disposed, a new one allocated for
 * the pixel length and locked, the pixels read straight into it, and the sprite locked again.
 * `MSHUnlock`'s result after the read is stored in `p` and dropped; `MSHLock`'s is the buffer the pixels are read into.
 *
 * THE LOCK FIELD AT +0x1c IS A `` POINTER, and the test against zero is what says so: under
 * `-3` a `` pointer, a `long` and an `unsigned long` all compare with a single `CMP DWORD`,
 * and only a `` pointer falls back to `MOV AX,ES:[BX+0x1c] / OR AX,ES:[BX+0x1e]` -- one
 * `LES`, which also rules out two `int` members, since a second member access reloads the
 * pointer at `-Od` (rule 62).
 *
 * A local `DiskFile`, so the function carries Borland's exception block and the destructor runs
 * on both exits; the class tower is `DiskFile_ctor.c`'s.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

void sprite_read_file(Sprite *sprite, char *name)
{
    DiskFile f;
    long size;
    void *p;

    if (file_MSOpenFile_1068_95d5(&f, name) == 0)
        return;
    save_read_field_1068_95fb(&f, &sprite->hx, 2L);
    save_read_field_1068_95fb(&f, &sprite->hy, 2L);
    save_read_field_1068_95fb(&f, &sprite->bounds, 8L);
    save_read_field_1068_95fb(&f, &sprite->area, 8L);
    save_read_field_1068_95fb(&f, &sprite->key, 1L);
    save_read_field_1068_95fb(&f, &size, 4L);
    if (sprite->bits != 0)
        Sprite_Unlock(sprite);
    if (sprite->handle != 0)
        sprite->handle = MSDisposeHandle(sprite->handle);
    sprite->handle = MSAllocateHandle(size);
    p = MSHLock(sprite->handle);
    save_read_field_1068_95fb(&f, p, size);
    p = MSHUnlock(sprite->handle);
    Sprite_Lock(sprite);
}

