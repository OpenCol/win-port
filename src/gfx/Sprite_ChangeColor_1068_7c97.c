/* Sprite_ChangeColor_1068_7c97 -- shipped at 1068:7c97.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_ChangeColor_1068_7c97.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src, transparent -> colour
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char Sprite_IsLocked(Sprite *s);
col_i16 rect_height_1068_96c8(RECT *r);
void Sprite_Lock(Sprite *s);
void Sprite_Unlock(Sprite *s);
void empty_1068_963d(void *ctx, char *msg);

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);











void Sprite_ChangeColor_1068_7c97(Sprite *s, unsigned char *map, col_i16 from, col_i16 unused1)
{
    col_i16 rows;
    col_i16 locked = 1;
    unsigned char *p;
    col_i16 n;

    if (!Sprite_IsLocked(s)) {
        locked = 0;
        Sprite_Lock(s);
        empty_1068_963d(&(*g_error_ctx), "Warning: Sprite not locked in ChangeColor");
    }
    rows = rect_height_1068_96c8(&(*((RECT *)((unsigned char *)(s) + 0x8))));
    p = s->src;
    while (rows-- != 0) {
        p += 2;
        n = *(col_i16 *)p;
        p += 2;
        while (n-- != 0) {
            if (*p - from >= 0)
                *p = map[*p - from];
            p += 1;
        }
    }
    if (s->colour - from >= 0)
        s->colour = map[s->colour - from];
    if (locked == 0)
        Sprite_Unlock(s);
}
