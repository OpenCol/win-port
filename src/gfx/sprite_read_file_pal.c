/* sprite_read_file_pal: candidate for 1068:717e (675 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_read_file_pal
 * name-source: descriptive
 *
 * `sprite_read_file` with a palette on the end: the same header fields, the same
 * pixel handle allocated from the 4-byte length and read into, and then TWO MORE ints -- a
 * first index and a count -- followed by that many RGB triples, which go into the caller's
 * palette through `CTable_Fill`.
 *
 * The caller can override the range: `first` and `count` of -1 (which is what the wrapper
 * `sprite_read_auto_pal` passes) means use the pair the file carries, anything
 * else means put the triples where the caller says instead.
 *
 * THE TRIPLES ARE ALLOCATED FROM ONE COUNT AND READ WITH ANOTHER. The handle is sized
 * `pal_count * 3` from the file, but the read that fills it asks for `count * 3` from the
 * PARAMETER. Through the wrapper `count` is -1, so the read length is -3 sign-extended to a
 * very large unsigned length; every other caller has to pass a count no larger than the
 * file's. It is in the shipped bytes and is left as it is.
 *
 * `count` is a register PARAMETER and takes SI over the handle: `h` is a plain local that
 * Borland promotes, and a promoted local ranks below a register parameter (rule 53). Spelling
 * `h` as `register` gives it SI and moves the parameter to DI.
 *
 * A local `DiskFile`, so the function carries Borland's exception block and the destructor
 * runs on both exits. The lock field at +0x1c is a `` pointer -- rule 62, as in the twin.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, DiskFile, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */
#include "records.h"
/* PORT: an opaque `typedef struct { char pad[1]; } logpal;` removed: the header declares
   struct logpal, which in C++ names the type already (W1). */

#pragma option -a

#pragma option -a-

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

/* same object: 4-byte `push cs; call ` */
extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

extern "C" void CTable_Fill(CTable *pal, int start, int count, void *rgb);

void sprite_read_file_pal(Sprite *sprite, char *name, int first,
                                        register int count, void *pal)
{
    DiskFile f;
    long size;
    void *p;
    void *rgb;
    int pal_first;
    int pal_count;
    int h;

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
    save_read_field_1068_95fb(&f, &pal_first, 2L);
    save_read_field_1068_95fb(&f, &pal_count, 2L);
    h = MSAllocateHandle((long)(pal_count * 3));
    rgb = MSHLock(h);
    save_read_field_1068_95fb(&f, rgb, (long)(count * 3));
    if (first != -1 && count != -1)
        CTable_Fill((CTable *)pal, first, count, (unsigned char *)rgb);
    else
        CTable_Fill((CTable *)pal, pal_first, pal_count, (unsigned char *)rgb);
    rgb = MSHUnlock(h);
    MSDisposeHandle(h);
    MSCloseFile_1068_95b5(&f);
    Sprite_Lock(sprite);
}

