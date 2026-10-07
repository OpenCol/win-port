/* Sprite_Init_wh: candidate for 1068:66dc (159 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite_Init_wh
 * name-source: sibling-build
 *
 * Builds a w x h sprite of one flat colour: a GRPort on the stack, sized, filled, and the
 * whole of it extracted into the caller's Sprite -- then the Sprite's rectangle at +8 is
 * zeroed, which is what marks it as having no source rectangle of its own.
 *
 * The port is a local object with a destructor, so the function carries Borland's exception
 * block; the block sits below it in the frame and the port is the only local.
 *
 * The colour is a BYTE widened on the way in (`MOV AL,[bp+0xe] / MOV AH,0`), so the
 * parameter is `unsigned char` and `GRPort_FillColor_all` takes an `int`.
 *
 * `Sprite_Extract` is reached by the five-byte NOP form and is `extern`; the two GRPort
 * helpers are reached by the four-byte `PUSH CS / CALL NEAR` form and are stubs here
 * (rule 47). The constructor and destructor calls are the compiler's own.
 *
 * RENAMED 2026-09-27 from sprite_make_solid: the Mac build's Sprite_Init_wh, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Builds a w x h sprite of one colour (a stack GRPort, sized, filled, extracted): Mac Sprite::Init(short, short, unsigned char), this + w + h + colour, ratio 0.98. Sprite_Init is already the (unsigned short) overload, so the name carries an overload suffix the way GRPort_Init_wh does. Plain Sprite_Init would collide.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

extern "C" void Sprite_Extract(Sprite *spr, GRPort *src, int x, int y, int w, int h);

/* same object: 4-byte `push cs; call ` */

extern "C" extern "C" void GRPort_FillColor_all(GRPort *p, int colour);

extern "C" void Sprite_Init_wh(Sprite *s, int w, int h, unsigned char colour)
{
    GRPort port;

    GRPort_Init_wh(&port, w, h);
    GRPort_FillColor_all(&port, colour);
    Sprite_Extract(s, &port, 0, 0, w, h);
    SetRect(&s->area, 0, 0, 0, 0);
}
