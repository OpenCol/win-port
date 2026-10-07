/* init_cloak_sprites: candidate for 1008:4dbe (664 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: init_cloak_sprites
 * name-source: sibling-build
 *
 * Builds all sixteen tile-edge sprites at SEG20:0x7546 -- the table `draw_neighbor_sprite`
 * indexes with `tile_neighbor_clear`'s four-bit mask -- out of one 32x32 scratch port.
 *
 * First the four single-direction pieces: sprite 148 as the ground, one of 104..107 over it,
 * and the result extracted with mask mode 0x31 into the entry for that single bit -- 8, 4,
 * 2 and 1, so the table's own one-bit slots double as the source pieces. Then sixteen
 * passes clear the port to colour 0, draw back whichever of those four the index's bits
 * name, and extract the composite into entry `i`. Entry 0 is a blank, and entries 1, 2, 4
 * and 8 are rewritten with what they already held.
 *
 * WHICH ENTRY EACH CALL WRITES IS NOT SOMETHING THE COMPARATOR CHECKS: a constant array
 * index folds into the one fixup that carries the table's address, so `[8]`, `[4]`, `[2]`
 * and `[1]` were read off the shipped addresses 0x7646, 0x75c6, 0x7586, 0x7566 against the
 * 0x7546 base (§119).
 *
 * THIRTEEN RECTANGLES ARE WRITTEN AND NONE IS READ. Every `Sprite_Draw` wants somewhere to
 * report the rectangle it drew, and this function gives each call its own; they are separate
 * locals rather than an array because the frame fills from `bp` downward in the order the
 * calls use them, which an array would reverse.
 *
 * The port is a local object with a destructor, so the function carries Borland's exception
 * block; it sits between the port and the rectangles in the frame.
 *
 * RENAMED 2026-09-26 from build_edge_sprites: the Mac build's init_cloak_sprites(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller init_sprites; callee Draw), the runner-up gets 1, the arity agrees (0), and the Mac body is 0.93x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern "C" void GRPort_FillColor_all(GRPort *p, int colour);
extern "C" RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern "C" void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, int x, int y, int w, int h);

extern "C" void init_cloak_sprites(void)
{
    GRPort port;
    RECT r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13;
    register int i;

    GRPort_Init_wh(&port, 0x20, 0x20);
    Sprite_Draw(&r1, &g_art_base[176], &port, 0, 0);

    Sprite_Draw(&r2, &g_art_base[176], &port, 0, 0);
    Sprite_Draw(&r3, &g_art_base[132], &port, 0, 0);
    Sprite_Extract_mask((Sprite *)&g_edge_sprites[8], &port, 0x31, 0, 0, 0x20, 0x20);

    Sprite_Draw(&r4, &g_art_base[176], &port, 0, 0);
    Sprite_Draw(&r5, &g_art_base[134], &port, 0, 0);
    Sprite_Extract_mask((Sprite *)&g_edge_sprites[4], &port, 0x31, 0, 0, 0x20, 0x20);

    Sprite_Draw(&r6, &g_art_base[176], &port, 0, 0);
    Sprite_Draw(&r7, &g_art_base[135], &port, 0, 0);
    Sprite_Extract_mask((Sprite *)&g_edge_sprites[2], &port, 0x31, 0, 0, 0x20, 0x20);

    Sprite_Draw(&r8, &g_art_base[176], &port, 0, 0);
    Sprite_Draw(&r9, &g_art_base[133], &port, 0, 0);
    Sprite_Extract_mask((Sprite *)&g_edge_sprites[1], &port, 0x31, 0, 0, 0x20, 0x20);

    for (i = 0; i < 0x10; i++) {
        GRPort_FillColor_all(&port, 0);
        if (i & 8)
            Sprite_Draw(&r10, (Sprite *)&g_edge_sprites[8], &port, 0, 0);
        if (i & 4)
            Sprite_Draw(&r11, (Sprite *)&g_edge_sprites[4], &port, 0, 0);
        if (i & 2)
            Sprite_Draw(&r12, (Sprite *)&g_edge_sprites[2], &port, 0, 0);
        if (i & 1)
            Sprite_Draw(&r13, (Sprite *)&g_edge_sprites[1], &port, 0, 0);
        Sprite_Extract_mask((Sprite *)&g_edge_sprites[i], &port, 0, 0, 0, 0x20, 0x20);
    }
}
