/* Sprite_ChangeColor: candidate for 1068:7bb5 (226 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces colour `from` with `to` in every pixel of the sprite's row data (pointer at
 * +0x1c: per row a word skipped, a pixel count, then the pixels), locking the sprite first
 * when it is not already (and reporting "Warning: Sprite not locked in ChangeColor").
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern char Sprite_IsLocked(Sprite *s);
extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);
extern void report_error_1068_963d(void *ctx, char *msg);

void Sprite_ChangeColor(Sprite *s, unsigned char from, unsigned char to)
{
    int n;
    int rows;
    int locked = 1;
    unsigned char *p;

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
            if (*p == (unsigned char)from)
                *p = to;
            p += 1;
        }
    }
    if (locked == 0)
        Sprite_Unlock(s);
}
