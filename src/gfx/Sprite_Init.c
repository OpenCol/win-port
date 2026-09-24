/* Sprite_Init -- shipped at 1068:6859.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_Init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern long g_sprt_type;                             DGROUP:0x2b12
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src, ox -> hx, oy -> hy
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1068_96c8(RECT *r);
col_i16 swap_word_identity_1068_9518(col_i16 a);
col_i32 identity_ptr_1068_9991(col_i32 v);
void * MSGetNamedResource_1068_94c7(void *type, col_i16 id);
void * lock_resource_1068_94f8(void *h);
void copy_rect(RECT *src, RECT *dst);
void empty_1068_94ea(void *ctx, char *msg, col_i32 v);
void free_resource_1068_9549(void *h);
void remap_pairs(void *p, col_i16 n);
void unlock_resource_1068_952d(void *h);

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void MSCopyMem(void *src, void *dst, col_i32 n);
extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

/* same object: 4-byte `push cs; call ` (rule 47) */











void Sprite_Init(Sprite *s, register col_i16 res)
{
    void *h;
    col_i32 type;
    char *p;
    void *bits;
    col_i32 size;

    type = g_sprt_type;
    h = MSGetNamedResource_1068_94c7(&type, res);
    if (h == 0) {
        empty_1068_94ea(&(*g_error_ctx), "Error: SPRT resource not found - ", (col_i32)(col_u16)res);
        return;
    }
    p = (char *)lock_resource_1068_94f8(h);
    s->hx = swap_word_identity_1068_9518(*(col_i16 *)p);
    p += 2;
    s->hy = swap_word_identity_1068_9518(*(col_i16 *)p);
    p += 2;
    copy_rect((RECT *)p, &(*((RECT *)((unsigned char *)(s) + 0x0))));
    p += 8;
    copy_rect((RECT *)p, &(*((RECT *)((unsigned char *)(s) + 0x8))));
    p += 8;
    s->colour = *p;
    p += 1;
    size = identity_ptr_1068_9991(*(col_i32 *)p);
    p += 4;
    if (s->src != 0)
        Sprite_Unlock(s);
    if (s->handle != 0)
        s->handle = MSDisposeHandle(s->handle);
    s->handle = MSAllocateHandle(size);
    bits = MSHLock(s->handle);
    MSCopyMem(p, bits, size);
    remap_pairs(bits, rect_height_1068_96c8(&(*((RECT *)((unsigned char *)(s) + 0x8)))));
    bits = MSHUnlock(s->handle);
    unlock_resource_1068_952d(h);
    p = 0;
    free_resource_1068_9549(h);
    Sprite_Lock(s);
}
