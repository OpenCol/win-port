/* sprite_extract_bounds -- shipped at 1068:76e2.
 *
 * Ported by tools/port.py from matched/gfx/sprite_extract_bounds.c.
 *
 * The file's own Sprite, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_bottom(GRPort *p);
unsigned char * GRPort_PixelMap(GRPort *p, col_i16 x, col_i16 y);

extern col_i16 ExtractSprite(Sprite *s, GRPort *p, col_i16 transparent, RECT *r);
extern void Sprite_Lock(Sprite *s);

/* same object: 4-byte `push cs; call ` */



void sprite_extract_bounds(Sprite *s, GRPort *p, unsigned char transparent,
                                         unsigned char key, register col_i16 x, col_i16 y)
{
    RECT r;
    unsigned char *q;
    unsigned char *start;
    register col_i16 i;

    start = GRPort_PixelMap(p, x, y);
    q = start;
    i = x;
    while (*q != key) {
        q += 1;
        i++;
    }
    r.right = i - 1;
    q = start;
    i = x;
    while (*q != key) {
        q -= 1;
        i--;
    }
    r.left = i + 1;
    q = start;
    i = y;
    while (*q != key) {
        q += rect_bottom(p);
        i++;
    }
    r.bottom = i - 1;
    q = start;
    i = y;
    while (*q != key) {
        q -= rect_bottom(p);
        i--;
    }
    r.top = i + 1;
    s->handle = ExtractSprite(s, p, transparent, &r);
    Sprite_Lock(s);
}
