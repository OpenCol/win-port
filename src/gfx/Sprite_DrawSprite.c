/* PORT: the assembly routines take their pointers as intptr_t, not long: 8 bytes on 64-bit Windows (W9). */
/* Sprite_DrawSprite: candidate for 1068:84e8 (709 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite_DrawSprite
 * name-source: sibling-build
 * size-source: 709, not the census 700. The trailing 9 bytes are the dead second epilogue.
 *
 * Draws a sprite at a scale, and returns the rectangle it drew into. This is the function
 * the five `Sprite_Draw*` wrappers at 1068:7894..798a all reduce to -- 187 call sites for
 * the first of them alone -- so what it is settles what they are.
 *
 * It is a scaled blit, and the scale is global: `g_scale_num` / `g_scale_den` at
 * DGROUP:0x2b0e/0x2b10, applied to the sprite's own extent. A sprite whose scaled size
 * exceeds 0x400 is refused with "Error: Sprite size exceeds scale table size" and an empty
 * rectangle -- the only appearance of that string in the binary, and the reason the scale
 * table is what bounds the sprite.
 *
 * The rest is the clip: build the destination rect from the sprite's scaled corners, offset
 * it to (px, py), intersect with the port's own rect at +8, and hand the survivors to
 * `asm_draw_sprite` with the source pointer, the port's bits, the colour, and how the
 * intersection moved the origin -- so the blitter gets a pre-clipped job.
 *
 * `MSGrafOrient(GRPort_GetWin(port))` picks the SIGN of the destination pitch:
 * `port_stride_1068_986d(port) * (state == 1 ? 1 : -1)`, which is a bottom-up versus top-down DIB.
 *
 * THE LAST TWO ARGUMENTS ARE TWO INTS, NOT A FAR POINTER. `signatures.py` reports
 * `(void *, void *, void *, int, void *)` from three call sites, because two
 * adjacent word pushes look exactly like a pointer -- §119's blind spot. The body reads
 * `[bp+0x14]` and `[bp+0x16]` as separate coordinates.
 *
 * Two spellings the bytes fixed. `g_scale_num`/`g_scale_den` are NEAR in this module where
 * most DGROUP words in SEG14 are . And the colour argument of `asm_draw_sprite` is `char`:
 * the shipped pushes `PUSH WORD [bp-0x17]` with no promotion, which is what a prototype
 * declaring `char` gives, where `int` gives `MOV AL / MOV AH,0 / PUSH AX`.
 *
 * RENAMED 2026-09-27 from sprite_draw_scaled: CHAIN 1 step 3. Mac Sprite::DrawSprite(GRPort&, short, short, short), 644 bytes, against 709. Caller: Mac DrawSprite's only caller is Sprite::Draw, and Mac Draw is DrawSprite(this, &rect, port, -1, x, y). Windows Sprite_Draw (1068:7894) is sprite_draw_scaled(out, s, port, -1, px, py), the same call. Width: this+port+3 shorts = 5, matching (sprite, port, colour, px, py). Callees: ScaleMult x6 on Mac, step_table_by x6 here (step_table_by is ScaleMult, below), plus PixelMap/NextScanLine (inlined surface math here). Layout: ExtractSprite, DrawSprite, [ScaleMult], DrawMonoSprite, CopySprite on Mac; Sprite_ExtractSprite, 84e8, 87ad, sprite_merge here. mac_bind's align proposal (-> DrawMonoSprite) came from aligning Mac DrawMono with the mis-bound 1068:798a and is WRONG.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int MSGrafOrient(GrafRec *graf);

/* same object: 4-byte `push cs; call ` */
extern "C" int ScaleMult(int amount, int start, int exact);

extern "C" int sprite_width_1068_9abd(Sprite *sprite);
extern "C" int sprite_height_1068_9ae0(Sprite *sprite);

extern "C" void *port_surface_1068_989f(GRPort *port);
extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern "C" int port_height_1068_9886(GRPort *p);
extern "C" int port_stride_1068_986d(GRPort *p);
extern "C" GrafRec *GRPort_GetWin_1068_968c(GRPort *p);

RECT *Sprite_DrawSprite(RECT *out, Sprite *sprite, GRPort *port,
                                 int colour, int px, int py)
{
    int l, t;
    int dx, dy;
    int inset_x, inset_y;
    RECT r;
    unsigned char col;
    int sx, sy;
    register int ax_;
    register int ay_;

    if (colour == -1)
        col = sprite->key;
    else
        col = (unsigned char)colour;
    inset_x = sprite->area.left - sprite->bounds.left;
    inset_y = sprite->area.top - sprite->bounds.top;
    if ((int)((long)rect_width_1068_99a9(sprite) * g_scale_num / g_scale_den) > 0x400
        || (int)((long)rect_height_1068_99c8(sprite) * g_scale_num / g_scale_den) > 0x400) {
        report_error_1068_963d(&g_error_log, "Error: Sprite size exceeds scale table size");
        SetRect(&r, 0, 0, 0, 0);
        out->left = r.left;
        out->top = r.top;
        out->right = r.right;
        out->bottom = r.bottom;
        return out;
    }
    sx = ScaleMult(inset_x, 0, 1);
    sy = ScaleMult(inset_y, 0, 1);
    ax_ = ScaleMult(sprite->hx, 0, 0) + px + sx;
    ay_ = ScaleMult(sprite->hy, 0, 0) + py + sy;
    SetRect(&r, 0, 0,
            ScaleMult(sprite_width_1068_9abd(sprite), sx, 0),
            ScaleMult(sprite_height_1068_9ae0(sprite), sy, 0));
    OffsetRect(&r, ax_, ay_);
    if (IntersectRect(&r, &r, (RECT *)((char *)port + 8)) != 0) {
        dx = 0;
        l = rect_width_1068_96a9(&r);
        if (r.left > ax_) {
            dx = r.left - ax_;
            ax_ = r.left;
        }
        dy = 0;
        t = rect_height_1068_96c8(&r);
        if (r.top > ay_) {
            dy = r.top - ay_;
            ay_ = r.top;
        }
        asm_draw_sprite((intptr_t)sprite->bits, port_surface_1068_989f(port), (intptr_t)g_scale_map, col, ax_, ay_,
                      port_stride_1068_986d(port)
                        * (MSGrafOrient((GrafRec *)GRPort_GetWin_1068_968c(port)) ? 1 : -1),
                      port_height_1068_9886(port),
                      l, t, dx, dy, inset_x, inset_y, sx, sy);
    }
    out->left = r.left;
    out->top = r.top;
    out->right = r.right;
    out->bottom = r.bottom;
    return out;
}
