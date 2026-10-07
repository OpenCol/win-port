/* dump_font_09_sprites: candidate for 1008:3d06 (2363 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: dump_font_09_sprites
 * name-source: descriptive
 *
 * A BUILD TOOL LEFT IN THE SHIPPED BINARY. Nothing calls it. It loads picture 0x5a, cuts fifty-three
 * sprites out of it and writes each one to its own file: `dec-09au.spr` through `dec-09zu.spr`, then
 * `dec-09al.spr` through `dec-09zl.spr`, then one more called `dec-09sq.spr`. That is the whole
 * function -- fifty-three `Sprite_Extract` / `sprite_write_file` pairs in a row, 1,069
 * instructions of straight line and not one branch.
 *
 * THE PICTURE IS A FONT SHEET in four bands, and the bands are the only structure here. Every glyph
 * is 0x36 tall and every band shares one y: 0x8b for the upper-case a-o, 0xcb for p-z, 0x116 for the
 * lower-case a-o and 0x155 for p-z and the odd one out. Each glyph's x and width are written out
 * individually, which is how a proportional font gets cut: the sheet was measured by hand once and
 * the numbers typed in.
 *
 * `dec-09sq.spr` is the fifty-third and is 0x3c wide where the widest letter is 0x27 -- it sits at
 * the end of the lower-case row, past `z`, and is the only file whose name is not a letter and a
 * case.
 *
 * WHAT THE BYTES SETTLED: the first extract is the only one that passes x and width through
 * VARIABLES; the other fifty-two pass literals. The two variables are assigned once, used once and
 * never touched again, so they are the last remnant of whatever loop or table this was before it was
 * unrolled. `y` and the height are register variables and are shared by every call, which is why the
 * four band changes are a single `MOV SI`.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */

#pragma option -a

#pragma option -a-

struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int id, int a, int b, struct logpal *pal);
extern void Sprite_Extract(Sprite *spr, GRPort *src, int x, int y, int w, int h);
extern void sprite_write_file(Sprite *spr, char *name);

void dump_font_09_sprites(void)
{
    register int y;
    register int h;
    GRPort port;
    Sprite spr;
    int x;
    int w;

    GRPort_LoadPicture(&port, 0x5a, 0xa, 0xec, (struct logpal *)&g_game_palette);
    x = 0x13;
    y = 0x8b;
    w = 0x1b;
    h = 0x36;
    Sprite_Extract(&spr, &port, x, y, w, h);
    sprite_write_file(&spr, "dec-09au.spr");
    Sprite_Extract(&spr, &port, 0x33, y, 0x17, h);
    sprite_write_file(&spr, "dec-09bu.spr");
    Sprite_Extract(&spr, &port, 0x4f, y, 0x26, h);
    sprite_write_file(&spr, "dec-09cu.spr");
    Sprite_Extract(&spr, &port, 0x7a, y, 0x1f, h);
    sprite_write_file(&spr, "dec-09du.spr");
    Sprite_Extract(&spr, &port, 0x9e, y, 0x1a, h);
    sprite_write_file(&spr, "dec-09eu.spr");
    Sprite_Extract(&spr, &port, 0xbd, y, 0x1e, h);
    sprite_write_file(&spr, "dec-09fu.spr");
    Sprite_Extract(&spr, &port, 0xe0, y, 0x25, h);
    sprite_write_file(&spr, "dec-09gu.spr");
    Sprite_Extract(&spr, &port, 0x10a, y, 0x19, h);
    sprite_write_file(&spr, "dec-09hu.spr");
    Sprite_Extract(&spr, &port, 0x128, y, 0x15, h);
    sprite_write_file(&spr, "dec-09iu.spr");
    Sprite_Extract(&spr, &port, 0x142, y, 0x13, h);
    sprite_write_file(&spr, "dec-09ju.spr");
    Sprite_Extract(&spr, &port, 0x15a, y, 0x20, h);
    sprite_write_file(&spr, "dec-09ku.spr");
    Sprite_Extract(&spr, &port, 0x17f, y, 0x1b, h);
    sprite_write_file(&spr, "dec-09lu.spr");
    Sprite_Extract(&spr, &port, 0x19f, y, 0x22, h);
    sprite_write_file(&spr, "dec-09mu.spr");
    Sprite_Extract(&spr, &port, 0x1c6, y, 0x23, h);
    sprite_write_file(&spr, "dec-09nu.spr");
    Sprite_Extract(&spr, &port, 0x1ee, y, 0x17, h);
    sprite_write_file(&spr, "dec-09ou.spr");
    y = 0xcb;
    Sprite_Extract(&spr, &port, 0x13, y, 0x1e, h);
    sprite_write_file(&spr, "dec-09pu.spr");
    Sprite_Extract(&spr, &port, 0x36, y, 0x1c, h);
    sprite_write_file(&spr, "dec-09qu.spr");
    Sprite_Extract(&spr, &port, 0x57, y, 0x1b, h);
    sprite_write_file(&spr, "dec-09ru.spr");
    Sprite_Extract(&spr, &port, 0x77, y, 0x14, h);
    sprite_write_file(&spr, "dec-09su.spr");
    Sprite_Extract(&spr, &port, 0x90, y, 0x23, h);
    sprite_write_file(&spr, "dec-09tu.spr");
    Sprite_Extract(&spr, &port, 0xb8, y, 0x27, h);
    sprite_write_file(&spr, "dec-09uu.spr");
    Sprite_Extract(&spr, &port, 0xe4, y, 0x19, h);
    sprite_write_file(&spr, "dec-09vu.spr");
    Sprite_Extract(&spr, &port, 0x102, y, 0x1e, h);
    sprite_write_file(&spr, "dec-09wu.spr");
    Sprite_Extract(&spr, &port, 0x125, y, 0x1f, h);
    sprite_write_file(&spr, "dec-09xu.spr");
    Sprite_Extract(&spr, &port, 0x149, y, 0x1d, h);
    sprite_write_file(&spr, "dec-09yu.spr");
    Sprite_Extract(&spr, &port, 0x16b, y, 0x1f, h);
    sprite_write_file(&spr, "dec-09zu.spr");
    y = 0x116;
    Sprite_Extract(&spr, &port, 0x13, y, 0xe, h);
    sprite_write_file(&spr, "dec-09al.spr");
    Sprite_Extract(&spr, &port, 0x26, y, 0x10, h);
    sprite_write_file(&spr, "dec-09bl.spr");
    Sprite_Extract(&spr, &port, 0x3b, y, 0xb, h);
    sprite_write_file(&spr, "dec-09cl.spr");
    Sprite_Extract(&spr, &port, 0x4b, y, 0x10, h);
    sprite_write_file(&spr, "dec-09dl.spr");
    Sprite_Extract(&spr, &port, 0x60, y, 0xf, h);
    sprite_write_file(&spr, "dec-09el.spr");
    Sprite_Extract(&spr, &port, 0x74, y, 0xf, h);
    sprite_write_file(&spr, "dec-09fl.spr");
    Sprite_Extract(&spr, &port, 0x88, y, 0x10, h);
    sprite_write_file(&spr, "dec-09gl.spr");
    Sprite_Extract(&spr, &port, 0x9d, y, 0xe, h);
    sprite_write_file(&spr, "dec-09hl.spr");
    Sprite_Extract(&spr, &port, 0xb0, y, 0xb, h);
    sprite_write_file(&spr, "dec-09il.spr");
    Sprite_Extract(&spr, &port, 0xc0, y, 0xc, h);
    sprite_write_file(&spr, "dec-09jl.spr");
    Sprite_Extract(&spr, &port, 0xd1, y, 0x13, h);
    sprite_write_file(&spr, "dec-09kl.spr");
    Sprite_Extract(&spr, &port, 0xe9, y, 0xf, h);
    sprite_write_file(&spr, "dec-09ll.spr");
    Sprite_Extract(&spr, &port, 0xfd, y, 0x18, h);
    sprite_write_file(&spr, "dec-09ml.spr");
    Sprite_Extract(&spr, &port, 0x11a, y, 0x13, h);
    sprite_write_file(&spr, "dec-09nl.spr");
    Sprite_Extract(&spr, &port, 0x133, y, 0xd, h);
    sprite_write_file(&spr, "dec-09ol.spr");
    y = 0x155;
    Sprite_Extract(&spr, &port, 0x13, y, 0x17, h);
    sprite_write_file(&spr, "dec-09pl.spr");
    Sprite_Extract(&spr, &port, 0x2f, y, 0x15, h);
    sprite_write_file(&spr, "dec-09ql.spr");
    Sprite_Extract(&spr, &port, 0x49, y, 0xe, h);
    sprite_write_file(&spr, "dec-09rl.spr");
    Sprite_Extract(&spr, &port, 0x5c, y, 0xe, h);
    sprite_write_file(&spr, "dec-09sl.spr");
    Sprite_Extract(&spr, &port, 0x6f, y, 0x10, h);
    sprite_write_file(&spr, "dec-09tl.spr");
    Sprite_Extract(&spr, &port, 0x84, y, 0x13, h);
    sprite_write_file(&spr, "dec-09ul.spr");
    Sprite_Extract(&spr, &port, 0x9c, y, 0x10, h);
    sprite_write_file(&spr, "dec-09vl.spr");
    Sprite_Extract(&spr, &port, 0xb1, y, 0x10, h);
    sprite_write_file(&spr, "dec-09wl.spr");
    Sprite_Extract(&spr, &port, 0xc6, y, 0x12, h);
    sprite_write_file(&spr, "dec-09xl.spr");
    Sprite_Extract(&spr, &port, 0xdd, y, 0x14, h);
    sprite_write_file(&spr, "dec-09yl.spr");
    Sprite_Extract(&spr, &port, 0xf6, y, 0x13, h);
    sprite_write_file(&spr, "dec-09zl.spr");
    Sprite_Extract(&spr, &port, 0x10e, y, 0x3c, h);
    sprite_write_file(&spr, "dec-09sq.spr");
}

