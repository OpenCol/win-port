/* sprite_extract_bounds: candidate for 1068:76e2 (328 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_extract_bounds
 * name-source: descriptive
 *
 * Finds a sprite's rectangle by scanning OUTWARD from one pixel and then extracts it. Four
 * identical loops walk right, left, down and up from `(x, y)` for as long as the pixel is NOT
 * the key colour; each stops one short of the pixel that is, and those four numbers are the
 * rectangle handed to `Sprite_ExtractSprite`. So the key colour is the frame drawn around each
 * sprite in the sheet, and the caller need only name a point inside one.
 *
 * The vertical steps add `port_stride_1068_986d(p)` -- the port's row stride -- and it is
 * called again on every iteration rather than fetched once.
 *
 * Every walk is through a `` pointer, `ADD`/`ADC` then `SHL` by the imported `__AHSHIFT`:
 * a sprite sheet is bigger than a segment.
 *
 * THREE THINGS THE FIRST ATTEMPT HAD BACKWARDS, all at one instruction. The comparison is
 * eight bits, so the pixel pointer is `unsigned char *` and not `char *` -- a signed
 * pixel widens both sides through `CBW` first. The loop runs while the pixel DIFFERS, not
 * while it matches. And `transparent` is widened on the way into `Sprite_ExtractSprite`, which is
 * rule 55: the callee's prototype takes an `int` even though this function's own parameter is
 * an `unsigned char`.
 *
 * That last call also settles `Sprite_Extract_key_rect`, whose six words of arguments
 * were six ints. They are `(s, src, transparent, r)` -- the shape the other three
 * `Sprite_Extract*` files already declare, and one a pass-through forwarder can never show.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
extern short Sprite_ExtractSprite(Sprite *s, GRPort *port, int key, RECT *src);
extern void Sprite_Lock(Sprite *s);

/* same object: 4-byte `push cs; call ` */
extern "C" unsigned char *GRPort_PixelMap(GRPort *port, int x, int y);
extern "C" int port_stride_1068_986d(GRPort *p);

void sprite_extract_bounds(Sprite *sprite, GRPort *p, unsigned char transparent,
                                         unsigned char key, register int x, int y)
{
    RECT r;
    unsigned char *q;
    unsigned char *start;
    register int i;

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
        q += port_stride_1068_986d(p);
        i++;
    }
    r.bottom = i - 1;
    q = start;
    i = y;
    while (*q != key) {
        q -= port_stride_1068_986d(p);
        i--;
    }
    r.top = i + 1;
    sprite->handle = Sprite_ExtractSprite(sprite, p, transparent, &r);
    Sprite_Lock(sprite);
}
