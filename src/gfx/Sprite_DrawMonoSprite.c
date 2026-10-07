/* PORT: this file's own declarations of GRPort_GetWin_1068_968c removed: the header declares it from the definition (W1 step 3). */
/* Sprite_DrawMonoSprite: candidate for 1068:87ad (712 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite_DrawMonoSprite
 * name-source: sibling-build
 * size-source: 712, not the census 703. The trailing 9 bytes are the dead second epilogue, and
 * 1068:8a75 starts immediately after them.
 *
 * `Sprite_DrawSprite` (1068:84e8) WITH A MODE ARGUMENT: the same scaled blit, argument for argument,
 * except that the blitter it ends in is `1090:0348` rather than `1090:0215` and takes one more word --
 * the caller's `mode` -- pushed deepest. Its callers are `1068:790c` and `1068:794d`.
 *
 * Everything else reads as the twin: -1 takes the sprite's own colour (+0x18); a sprite whose scaled
 * size passes 0x400 is refused with "Error: Sprite size exceeds scale table size" and an empty
 * rectangle; the drawn rectangle is clipped against the port's own (+8), and what the clip cut off
 * becomes the source offset. The rectangle is written back through `out`, which is also returned.
 *
 * This is a twin transfer: the source is `matched/Sprite_DrawSprite.c` with the extra parameter
 * threaded through, and it graded RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-27 from sprite_blit_mode: CHAIN 1 step 4. Mac Sprite::DrawMonoSprite(GRPort&, short, short, short, char), 658 bytes, against 712. Caller: Mac DrawMono (1068:794d after step 2) is its only caller, on both builds. Width: this+port+3 shorts+char = 6, matching (sprite, port, colour, px, py, mode). The extra trailing mode word is exactly the char DrawMono adds. Callees: step_table_by (ScaleMult) x6, as on Mac. Layout: it follows 84e8 (DrawSprite), as DrawMonoSprite follows DrawSprite. Const witness: no pair. mac_bind's align proposal (-> ScaleMult, ratio 0.25) is rejected.
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

RECT *Sprite_DrawMonoSprite(RECT *out, Sprite *sprite, GRPort *port,
                                       int colour, int px, int py, unsigned char mode)
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
        asm_draw_mono_sprite((long)sprite->bits, port_surface_1068_989f(port), (long)g_scale_map, col, ax_, ay_,
                      port_stride_1068_986d(port)
                        * (MSGrafOrient((GrafRec *)GRPort_GetWin_1068_968c(port)) ? 1 : -1),
                      port_height_1068_9886(port),
                      l, t, dx, dy, inset_x, inset_y, sx, sy, mode);
    }
    out->left = r.left;
    out->top = r.top;
    out->right = r.right;
    out->bottom = r.bottom;
    return out;
}
