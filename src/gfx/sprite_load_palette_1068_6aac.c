/* sprite_load_palette_1068_6aac: candidate for 1068:6aac (701 bytes).  Verdict: RELOCATABLE.
 * symbol: sprite_load_palette_1068_6aac
 * name-source: descriptive
 *
 * Loads a SPRT resource into a Sprite and installs the resource's RGB palette data.
 * `start` and `count` override the two palette-range words carried by the resource;
 * passing -1 for either uses both resource values.  The one caller is a convenience
 * wrapper that supplies -1 for both.
 *
 * Unlike `Sprite_Init`, this variant copies the header fields verbatim rather than
 * passing the words through byte-swap helpers.  It also leaves the pixel payload's
 * first two words available as the default palette range, then passes the following
 * bytes to `CTable_Fill`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal;

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);
extern void *MSGetResource_1068_94c7(void *type, int id);
extern void report_error_1068_94ea(void *ctx, char *msg, long v);
extern void *MSLockResource_1068_94f8(void *h);
extern void MSUnlockResource_1068_952d(void *res);
extern void MSReleaseResource_1068_9549(void *res);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

void sprite_load_palette_1068_6aac(Sprite *sprite, register int res_id,
                                       register int start, int count,
                                       struct logpal *pal)
{
    void *h;
    long type;
    unsigned char *p;
    void *bits;
    unsigned long size;
    int resource_start;
    int resource_count;

    type = g_sprt_palette_type;
    h = MSGetResource_1068_94c7(&type, res_id);
    if (h == 0) {
        report_error_1068_94ea(&g_error_log, "Error: SPRT resource not found - ",
                        (long)(unsigned)res_id);
        return;
    }
    p = (unsigned char *)MSLockResource_1068_94f8(h);
    MSCopyMem(p, &sprite->hx, 2L);
    p += 2;
    MSCopyMem(p, &sprite->hy, 2L);
    p += 2;
    MSCopyMem(p, &sprite->bounds, 8L);
    p += 8;
    MSCopyMem(p, &sprite->area, 8L);
    p += 8;
    MSCopyMem(p, &sprite->key, 1L);
    p += 1;
    MSCopyMem(p, &size, 4L);
    p += 4;
    if (sprite->bits != 0)
        Sprite_Unlock(sprite);
    if (sprite->handle != 0)
        sprite->handle = MSDisposeHandle(sprite->handle);
    sprite->handle = MSAllocateHandle(size);
    bits = MSHLock(sprite->handle);
    MSCopyMem(p, bits, size);
    bits = MSHUnlock(sprite->handle);
    MSCopyMem(p, &resource_start, 2L);
    p += 2;
    MSCopyMem(p, &resource_count, 2L);
    p += 2;
    if (start != -1 && count != -1)
        CTable_Fill((CTable *)pal, start, count, p);
    else
        CTable_Fill((CTable *)pal, resource_start, resource_count, p);
    MSUnlockResource_1068_952d(h);
    p = 0;
    MSReleaseResource_1068_9549(h);
    Sprite_Lock(sprite);
}
