/* Sprite_ChangeColor_1068_7c97: candidate for 1068:7c97 (316 bytes).  Verdict: RELOCATABLE.
 * symbol: Sprite_ChangeColor_1068_7c97
 *
 * Remaps the sprite's pixels through a colour table: every pixel value at or above `from`
 * becomes map[value - from] (row data as in Sprite_ChangeColor: per row a word skipped, a
 * pixel count, the pixels), and the transparent colour at +0x18 likewise. An unlocked sprite
 * is locked first (with a warning) and unlocked at the end. The module's small helpers are
 * defined here because the shipped calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern char Sprite_IsLocked(Sprite *s);

extern void Sprite_Lock(Sprite *s);

extern void Sprite_Unlock(Sprite *s);

extern void report_error_1068_963d(void *ctx, char *msg);

void Sprite_ChangeColor_1068_7c97(Sprite *s, unsigned char *map, int from, int unused1)
{
    int rows;
    int locked = 1;
    unsigned char *p;
    int n;

    if (!Sprite_IsLocked(s)) {
        locked = 0;
        Sprite_Lock(s);
        report_error_1068_963d(&g_error_log, "Warning: Sprite not locked in ChangeColor");
    }
    rows = rect_height_1068_96c8(&s->area);
    p = s->bits;
    while (rows-- != 0) {
        p += 2;
        n = *(short *)p;
        p += 2;
        while (n-- != 0) {
            if (*p - from >= 0)
                *p = map[*p - from];
            p += 1;
        }
    }
    if (s->key - from >= 0)
        s->key = map[s->key - from];
    if (locked == 0)
        Sprite_Unlock(s);
}
