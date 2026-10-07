/* PORT: this file's own declarations of port_height_1008_d720, port_width_1008_d707 removed: the header declares them from the definition (W1 step 3). */
/* init_sprites: candidate for 1008:51d4 (10584 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: init_sprites
 * name-source: sibling-build
 *
 * EVERY SPRITE THE GAME OWNS, CUT OUT ONCE AT START-UP. This is the largest function in the
 * binary and it is almost entirely straight-line: three picture resources are loaded into the
 * screen surface one after another and 217 separate `sprite_extract_from_sheet` calls, plus
 * fifteen loops, copy 32x32 (and, at the end, 16x16 and 12x13) rectangles out of them into the
 * game's art table. Ninety-six `Sprite_SetHotSpot` calls then place the anchor points, and
 * twenty-nine `GRPort_PixelMap` reads sample single pixels out of the sheet into the colour-key
 * bytes at SEG20:0xde82 -- the terrain and unit colours the renderer later substitutes.
 *
 * THE SHEETS. Resource 0xc9 holds the terrain and the unit icons, and is read in a grid of
 * 33-pixel steps; 0xcb and 0xfc hold the two bars that are extracted whole, using the surface's
 * own width and height rather than a constant. `init_cloak_sprites` and
 * `init_water_sprites` finish the job for the coastline pieces and resource 0xcc.
 *
 * THE COMPOSITED SPRITES are what the two local `Sprite` arrays are for. A four-entry array
 * holds one set of overlays and a thirty-two entry array the other; for each pair the function
 * draws the base into an off-screen 16x16 `GRPort` port, draws the overlay on top, and then
 * extracts the result as a MASKED sprite. That is how the game gets a unit-on-terrain or a
 * flag-on-unit image without compositing it at draw time -- every combination is baked here.
 * Both arrays are built by `VNEWVF` and torn down by `VDELF`, which is what Borland emits for
 * an array of objects with a constructor; the function's whole exception frame exists for them.
 *
 * WHAT THE BYTES SETTLED, three spellings.
 *   The art table has to be based at SEG20:0x41a6 (`g_art_base`, the name the corpus already
 *   uses) and not at the 0x4526 that `init_water_sprites` uses, because this function reaches
 *   twenty-eight entries BELOW that point and Borland does not fold a NEGATIVE constant index
 *   into the relocation -- it emits `MOV AX,seg / ADD AX,-0x380` at run time instead.
 *   The composited-sprite arguments need an explicit `(void *)` cast. Without it Borland
 *   pushes SS BEFORE computing the element's offset; with it, the offset is computed first and
 *   `PUSH SS / PUSH AX` follows, which is what the shipped does. The two orders are the same
 *   three instructions in a different sequence.
 *   And `Sprite_Extract_mask` takes its mode as a `char` while `sprite_extract_from_sheet` takes
 *   an `int`: the shipped pushes the byte slot straight as a word (`PUSH WORD PTR [BP-0x4C9]`)
 *   at the first and widens it (`MOV AL / MOV AH,0 / PUSH AX`) at the second, in the same
 *   statement pair, so the two prototypes genuinely differ.
 *
 * HOW IT WAS RECONSTRUCTED: by transcription, not by reading. The body is regular enough that a
 * throwaway script turned the disassembly into the call sequence and the fifteen loops, and only
 * five statements -- the two that consume `get_w_2`/`get_w_4` results and the three composited
 * draws -- had to be written by hand.
 *
 * RENAMED 2026-09-26 from load_all_sprite_sheets: the Mac build's init_sprites(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee Draw; callee PixelMap; callee SetHotSpot), the runner-up gets 2, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */

#pragma option -a

#pragma option -a-

struct logpal;
extern int GRPort_LoadPicture(GRPort *port, int res, int w, int h, struct logpal *pal);
extern void MSLockGraf(GRPort *port);
extern void MSUnlockGraf(GRPort *port);
extern char *GRPort_PixelMap(GRPort *port, int x, int y);

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void Sprite_Extract_mask(Sprite *s, void *src, unsigned char mode,
                                    int x, int y, int w, int h);
extern void Sprite_SetHotSpot(Sprite *s, int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void sprite_extract_from_sheet(void *s, int mode, int x, int y, int w, int h);

extern "C" void init_cloak_sprites(void);
extern "C" void init_water_sprites(void);

extern "C" void init_sprites(void)
{
    register int x;
    register int y;
    GRPort port;
    Sprite sa[4];
    Sprite sb[0x20];
    int i;
    unsigned char mode;
    RECT r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;

    mode = 0x31;
    x = 0x12;
    y = 0x27;
    GRPort_LoadPicture(&g_sprite_sheet, 0xc9, 0x28, 0x60, (struct logpal *)&g_game_palette);
    MSLockGraf(&g_sprite_sheet);
    sprite_extract_from_sheet(&g_art_base[0], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[8], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[16], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[1], mode, x, y, 0x20, 0x20);
    y += 0x21;
    sprite_extract_from_sheet(&g_art_base[9], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[17], mode, x, y, 0x20, 0x20);
    y -= 0x21;
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[2], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[10], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[18], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[3], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[11], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[19], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[4], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[12], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[20], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[5], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[13], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[21], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[6], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[14], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[22], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[7], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[15], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[23], mode, x, y, 0x20, 0x20);
    x = 0x141;
    sprite_extract_from_sheet(&g_art_base[24], mode, x, y, 0x20, 0x20);
    y += 0x21;
    sprite_extract_from_sheet(&g_art_base[25], mode, x, y, 0x20, 0x20);
    y += 0x21;
    sprite_extract_from_sheet(&g_art_base[26], mode, x, y, 0x20, 0x20);
    x = 0x22;
    y = 0x37;
    g_terrain_colour[0] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[8] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[16] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[1] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[9] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[17] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[2] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[10] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[18] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[3] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[11] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[19] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[4] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[12] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[20] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[5] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[13] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[21] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[6] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[14] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[22] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x += 0x21;
    g_terrain_colour[7] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[15] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    g_terrain_colour[23] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x = 0x141;
    g_terrain_colour[24] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    y += 0x21;
    g_terrain_colour[25] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    y += 0x21;
    g_terrain_colour[26] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x = 0x3e;
    y = 0x135;
    g_terrain_colour[27] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x = 0x18;
    y = 0x159;
    g_terrain_colour[28] = *GRPort_PixelMap(&g_sprite_sheet, x, y);
    x = 0x7;
    y = 0xe8;
    for (i = 0x0; i < 0x10; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 28], mode, x, y, 0x20, 0x20);
    x = 0x7;
    y += 0x21;
    for (i = 0x0; i < 0x10; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 44], mode, x, y, 0x20, 0x20);
    x = 0x7;
    y += 0x21;
    for (i = 0x0; i < 0x10; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 60], 0x51, x, y, 0x20, 0x20);
    x = 0x7;
    y += 0x21;
    for (i = 0x0; i < 0x10; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 76], mode, x, y, 0x20, 0x20);
    x = 0x7;
    y += 0x21;
    for (i = 0x0; i < 0x10; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 92], mode, x, y, 0x20, 0x20);
    x = 0x7;
    y += 0x21;
    for (i = 0x0; i < 0x9; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 108], mode, x, y, 0x20, 0x20);
    x = 0x177;
    y = 0x1b6;
    sprite_extract_from_sheet(&g_art_base[172], mode, 0x177, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[173], mode, 0x1a0, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[174], mode, 0x1c6, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[175], mode, 0x1ee, y, 0x20, 0x20);
    x = 0x177;
    y = 0x191;
    sprite_extract_from_sheet(&g_art_base[168], mode, 0x177, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[169], mode, 0x19f, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[170], mode, 0x1c6, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[171], mode, 0x1ee, y, 0x20, 0x20);
    x = 0x229;
    y = 0x18;
    mode = 0x87;
    for (i = 0x0; i < 0x4; i++, y += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 132], mode, x, y, 0x20, 0x20);
    mode = 0x31;
    GRPort_Init_wh(&port, 0x20, 0x20);
    x = 0;
    y = 0;
    for (y = 0; y < 0x8; y++) {
        for (i = 0x0; i < 0x4; i++) {
            Sprite_Draw(&r0, &g_art_base[y], &port, 0x0, 0x0);
            Sprite_Draw(&r1, &g_art_base[i + 132], &port, 0x0, 0x0);
            Sprite_Extract_mask(&g_art_base[x++ + 217], &port, mode, 0x0, 0x0, 0x20, 0x20);
        }
    }
    for (i = 0x0; i < 0x4; i++) {
        Sprite_Draw(&r2, &g_art_base[24], &port, 0x0, 0x0);
        Sprite_Draw(&r3, &g_art_base[i + 132], &port, 0x0, 0x0);
        Sprite_Extract_mask(&g_art_base[x++ + 217], &port, mode, 0x0, 0x0, 0x20, 0x20);
    }
    for (i = 0x0; i < 0x4; i++) {
        Sprite_Draw(&r4, &g_art_base[25], &port, 0x0, 0x0);
        Sprite_Draw(&r5, &g_art_base[i + 132], &port, 0x0, 0x0);
        Sprite_Extract_mask(&g_art_base[x++ + 217], &port, mode, 0x0, 0x0, 0x20, 0x20);
    }
    for (i = 0x0; i < 0x4; i++) {
        Sprite_Draw(&r6, &g_art_base[26], &port, 0x0, 0x0);
        Sprite_Draw(&r7, &g_art_base[i + 132], &port, 0x0, 0x0);
        Sprite_Extract_mask(&g_art_base[x++ + 217], &port, mode, 0x0, 0x0, 0x20, 0x20);
    }
    sprite_extract_from_sheet(&g_art_base[117], mode, 0xe9, 0xbd, 0x20, 0x20);
    x = 0x44;
    y = 0x9c;
    sprite_extract_from_sheet(&g_art_base[118], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[119], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[120], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[121], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[122], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[123], mode, x, y, 0x20, 0x20);
    x = 0x12f;
    y = 0x9b;
    sprite_extract_from_sheet(&g_art_base[124], mode, 0x12f, 0x9b, 0x20, 0x20);
    x = 0x65;
    y = 0xbd;
    sprite_extract_from_sheet(&g_art_base[125], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[126], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[127], mode, x, y, 0x20, 0x20);
    x = 0x12f;
    y = 0x9b;
    sprite_extract_from_sheet(&g_art_base[129], mode, 0x12f, 0xbc, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[130], mode, 0x150, 0xbc, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[131], mode, 0x171, 0xbc, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[177], mode, 0x130, 0x18d, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[176], mode, 0x248, 0xe8, 0x20, 0x20);
    x = 0x22b;
    y = 0x12a;
    sprite_extract_from_sheet(&g_art_base[178], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[179], mode, x + 0x21, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[180], mode, x, y + 0x21, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[181], mode, x + 0x21, y + 0x21, 0x20, 0x20);
    mode = 0x87;
    x = 0x223;
    y = 0x1b2;
    sprite_extract_from_sheet(sa, mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sa[1], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sa[3], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sa[2], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x = 0x7;
    y = 0x1b6;
    sprite_extract_from_sheet(sb, mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[1], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[3], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[2], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[4], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[5], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[7], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[6], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[8], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[9], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[11], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[10], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[12], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[13], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[15], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[14], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[16], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[17], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[19], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[18], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[20], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[21], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[23], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[22], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[24], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[25], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[27], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[26], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    x += 0x24;
    sprite_extract_from_sheet(&sb[28], mode, x, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[29], mode, x + 0x11, y, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[31], mode, x, y + 0x11, 0x10, 0x10);
    sprite_extract_from_sheet(&sb[30], mode, x + 0x11, y + 0x11, 0x10, 0x10);
    mode = 0x31;
    GRPort_Init_wh(&port, 0x10, 0x10);
    x = 0;
    y = 0;
    for (y = 0; y < 0x20; y++) {
        for (i = 0x0; i < 0x4; i++) {
            Sprite_Draw(&r8, (Sprite *)&sa[i], &port, 0x0, 0x0);
            Sprite_Draw(&r9, (Sprite *)&sb[y], &port, 0x0, 0x0);
            Sprite_Extract_mask(&g_art_base[y + 136], &port, mode, 0x0, 0x0, 0x10, 0x10);
        }
    }
    sprite_extract_from_sheet(&g_art_base[262], mode, 0x297, 0x35, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[263], mode, 0x297, 0xcb, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[264], mode, 0x2b8, 0xaa, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[265], mode, 0x297, 0xaa, 0x20, 0x20);
    x = 0x293;
    y = 0x136;
    sprite_extract_from_sheet(&g_art_base[272], mode, 0x399, 0x156, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[273], mode, 0x3ba, 0x156, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[274], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[275], mode, x, y, 0x20, 0x20);
    x = 0x293;
    y = 0x111;
    sprite_extract_from_sheet(&g_art_base[329], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[330], mode, x, y, 0x20, 0x20);
    x += 0x21;
    sprite_extract_from_sheet(&g_art_base[331], mode, x, y, 0x20, 0x20);
    x = 0x341;
    y = 0xa8;
    sprite_extract_from_sheet(&g_art_base[316], mode, x, y, 0x18, 0x20);
    x += 0x19;
    Sprite_SetHotSpot(&g_art_base[316], 18446744073709486068, 0x0);
    sprite_extract_from_sheet(&g_art_base[317], mode, x, y, 0x18, 0x20);
    x += 0x19;
    Sprite_SetHotSpot(&g_art_base[317], 18446744073709486068, 0x0);
    sprite_extract_from_sheet(&g_art_base[318], mode, x, y, 0x13, 0x20);
    x += 0x14;
    Sprite_SetHotSpot(&g_art_base[318], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[319], mode, x, y, 0x12, 0x20);
    x += 0x13;
    Sprite_SetHotSpot(&g_art_base[319], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[324], mode, x, y, 0x1c, 0x20);
    Sprite_SetHotSpot(&g_art_base[324], 18446744073709486066, 0x0);
    sprite_extract_from_sheet(&g_art_base[385], mode, 0x3be, y, 0x19, 0x20);
    Sprite_SetHotSpot(&g_art_base[385], 18446744073709486068, 0x0);
    sprite_extract_from_sheet(&g_art_base[386], mode, 0x3d8, y, 0x1a, 0x20);
    Sprite_SetHotSpot(&g_art_base[386], 18446744073709486067, 0x0);
    sprite_extract_from_sheet(&g_art_base[279], mode, 0x362, 0x17f, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[326], mode, 0x2fa, 0x17f, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[326], 18446744073709486064, 0x0);
    x = 0x292;
    y = 0x15a;
    for (i = 0x0; i < 0x5; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 375], mode, x, y, 0x20, 0x20);
    x = 0x341;
    y = 0x60;
    sprite_extract_from_sheet(&g_art_base[284], mode, x, y, 0x10, 0x20);
    x += 0x11;
    Sprite_SetHotSpot(&g_art_base[284], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[285], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[285], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[286], mode, x, y, 0x14, 0x20);
    x += 0x15;
    Sprite_SetHotSpot(&g_art_base[286], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[287], mode, x, y, 0x17, 0x20);
    x += 0x18;
    Sprite_SetHotSpot(&g_art_base[287], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[288], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[288], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[289], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[289], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[290], mode, x, y, 0x1a, 0x20);
    x += 0x1b;
    Sprite_SetHotSpot(&g_art_base[290], 18446744073709486067, 0x0);
    sprite_extract_from_sheet(&g_art_base[291], mode, x, y, 0x14, 0x20);
    Sprite_SetHotSpot(&g_art_base[291], 18446744073709486070, 0x0);
    x = 0x341;
    y = 0x81;
    sprite_extract_from_sheet(&g_art_base[292], mode, x, y, 0x1c, 0x20);
    x += 0x1d;
    Sprite_SetHotSpot(&g_art_base[292], 18446744073709486066, 0x0);
    sprite_extract_from_sheet(&g_art_base[293], mode, x, y, 0x12, 0x20);
    x += 0x13;
    Sprite_SetHotSpot(&g_art_base[293], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[294], mode, x, y, 0x9, 0x20);
    x += 0xa;
    Sprite_SetHotSpot(&g_art_base[294], 18446744073709486076, 0x0);
    sprite_extract_from_sheet(&g_art_base[295], mode, x, y, 0x11, 0x20);
    x += 0x12;
    Sprite_SetHotSpot(&g_art_base[295], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[296], mode, x, y, 0x14, 0x20);
    x += 0x15;
    Sprite_SetHotSpot(&g_art_base[296], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[297], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[297], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[298], mode, x, y, 0x18, 0x20);
    x += 0x19;
    Sprite_SetHotSpot(&g_art_base[298], 18446744073709486068, 0x0);
    sprite_extract_from_sheet(&g_art_base[299], mode, x, y, 0xd, 0x20);
    Sprite_SetHotSpot(&g_art_base[299], 18446744073709486074, 0x0);
    x = 0x409;
    y = 0x60;
    sprite_extract_from_sheet(&g_art_base[300], mode, x, y, 0x10, 0x20);
    x += 0x11;
    Sprite_SetHotSpot(&g_art_base[300], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[301], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[301], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[302], mode, x, y, 0x14, 0x20);
    x += 0x15;
    Sprite_SetHotSpot(&g_art_base[302], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[303], mode, x, y, 0x17, 0x20);
    x += 0x18;
    Sprite_SetHotSpot(&g_art_base[303], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[304], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[304], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[305], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[305], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[306], mode, x, y, 0x1a, 0x20);
    x += 0x1b;
    Sprite_SetHotSpot(&g_art_base[306], 18446744073709486067, 0x0);
    sprite_extract_from_sheet(&g_art_base[307], mode, x, y, 0x14, 0x20);
    Sprite_SetHotSpot(&g_art_base[307], 18446744073709486070, 0x0);
    x = 0x409;
    y = 0x81;
    sprite_extract_from_sheet(&g_art_base[308], mode, x, y, 0x1c, 0x20);
    x += 0x1d;
    Sprite_SetHotSpot(&g_art_base[308], 18446744073709486066, 0x0);
    sprite_extract_from_sheet(&g_art_base[309], mode, x, y, 0x12, 0x20);
    x += 0x13;
    Sprite_SetHotSpot(&g_art_base[309], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[310], mode, x, y, 0x9, 0x20);
    x += 0xa;
    Sprite_SetHotSpot(&g_art_base[310], 18446744073709486076, 0x0);
    sprite_extract_from_sheet(&g_art_base[311], mode, x, y, 0x11, 0x20);
    x += 0x12;
    Sprite_SetHotSpot(&g_art_base[311], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[312], mode, x, y, 0x14, 0x20);
    x += 0x15;
    Sprite_SetHotSpot(&g_art_base[312], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[313], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[313], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[314], mode, x, y, 0x18, 0x20);
    x += 0x19;
    Sprite_SetHotSpot(&g_art_base[314], 18446744073709486068, 0x0);
    sprite_extract_from_sheet(&g_art_base[315], mode, x, y, 0xd, 0x20);
    Sprite_SetHotSpot(&g_art_base[315], 18446744073709486074, 0x0);
    x = 0x297;
    y = 0x61;
    sprite_extract_from_sheet(&g_art_base[267], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[267], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[268], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[268], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[269], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[269], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[276], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[276], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[277], mode, x, y, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[277], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[270], mode, 0x2d9, 0xaa, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[270], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[271], mode, 0x2d9, 0xcb, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[271], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[327], mode, 0x2fa, 0xcb, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[327], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[278], mode, 0x2d9, 0xec, 0x20, 0x20);
    Sprite_SetHotSpot(&g_art_base[278], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[371], mode, 0x342, 0xfa, 0xe, 0x20);
    Sprite_SetHotSpot(&g_art_base[371], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[372], mode, 0x351, 0xfa, 0x12, 0x20);
    Sprite_SetHotSpot(&g_art_base[372], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[373], mode, 0x364, 0xfa, 0x1e, 0x20);
    Sprite_SetHotSpot(&g_art_base[373], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[374], mode, 0x383, 0xfa, 0x1e, 0x20);
    Sprite_SetHotSpot(&g_art_base[374], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[387], mode, 0x382, 0xd4, 0x14, 0x20);
    Sprite_SetHotSpot(&g_art_base[387], 18446744073709486070, 0x0);
    sprite_extract_from_sheet(&g_art_base[388], mode, 0x397, 0xd4, 0x1e, 0x20);
    Sprite_SetHotSpot(&g_art_base[388], 18446744073709486067, 0x0);
    sprite_extract_from_sheet(&g_art_base[389], mode, 0x3b6, 0xd4, 0x1f, 0x20);
    Sprite_SetHotSpot(&g_art_base[389], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[390], mode, 0x3d6, 0xd4, 0x12, 0x20);
    Sprite_SetHotSpot(&g_art_base[390], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[391], mode, 0x3e9, 0xd4, 0x1e, 0x20);
    Sprite_SetHotSpot(&g_art_base[391], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[370], mode, 0x350, 0xd4, 0x10, 0x20);
    Sprite_SetHotSpot(&g_art_base[370], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[384], mode, 0x424, 0xa8, 0x1a, 0x20);
    sprite_extract_from_sheet(&g_art_base[392], mode, 0x2ce, 0xcb, 0xa, 0x8);
    sprite_extract_from_sheet(&g_art_base[380], mode, 0x2b8, 0xcb, 0xa, 0x8);
    sprite_extract_from_sheet(&g_art_base[383], mode, 0x2c3, 0xcb, 0xa, 0x8);
    sprite_extract_from_sheet(&g_art_base[381], mode, 0x2b8, 0xd5, 0xa, 0x8);
    sprite_extract_from_sheet(&g_art_base[382], mode, 0x2c3, 0xd5, 0xa, 0x8);
    x = 0x47a;
    y = 0xa9;
    sprite_extract_from_sheet(&g_art_base[361], mode, x, y, 0x12, 0x20);
    Sprite_SetHotSpot(&g_art_base[361], 18446744073709486071, 0x0);
    x = 0x33a;
    y = 0x36;
    sprite_extract_from_sheet(&g_art_base[343], mode, x, y, 0x16, 0x20);
    x += 0x17;
    Sprite_SetHotSpot(&g_art_base[343], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[344], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[344], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[345], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[345], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[346], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[346], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[347], mode, x, y, 0x10, 0x20);
    x += 0x11;
    Sprite_SetHotSpot(&g_art_base[347], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[348], mode, x, y, 0x11, 0x20);
    x += 0x12;
    Sprite_SetHotSpot(&g_art_base[348], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[349], mode, 0x31f, 0x36, 0x17, 0x20);
    Sprite_SetHotSpot(&g_art_base[349], 18446744073709486069, 0x0);
    sprite_extract_from_sheet(&g_art_base[350], mode, x, y, 0x12, 0x20);
    x += 0x13;
    Sprite_SetHotSpot(&g_art_base[350], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[351], mode, x, y, 0x20, 0x20);
    x += 0x21;
    Sprite_SetHotSpot(&g_art_base[351], 18446744073709486064, 0x0);
    sprite_extract_from_sheet(&g_art_base[354], mode, x, y, 0xe, 0x20);
    x += 0xf;
    Sprite_SetHotSpot(&g_art_base[354], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[353], mode, x, y, 0xe, 0x20);
    x += 0xf;
    Sprite_SetHotSpot(&g_art_base[353], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[352], mode, x, y, 0xe, 0x20);
    x += 0xf;
    Sprite_SetHotSpot(&g_art_base[352], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[355], mode, x, y, 0x12, 0x20);
    x += 0x13;
    Sprite_SetHotSpot(&g_art_base[355], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[356], mode, x, y, 0xe, 0x20);
    x += 0xf;
    Sprite_SetHotSpot(&g_art_base[356], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[357], mode, x, y, 0x10, 0x20);
    x += 0x11;
    Sprite_SetHotSpot(&g_art_base[357], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[358], mode, x, y, 0xe, 0x20);
    x += 0xf;
    Sprite_SetHotSpot(&g_art_base[358], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[359], mode, x, y, 0xc, 0x20);
    x += 0xd;
    Sprite_SetHotSpot(&g_art_base[359], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[360], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[360], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[362], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[362], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[363], mode, x, y, 0xf, 0x20);
    x += 0x10;
    Sprite_SetHotSpot(&g_art_base[363], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[364], mode, x, y, 0x1e, 0x20);
    x += 0x1f;
    Sprite_SetHotSpot(&g_art_base[364], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[365], mode, x, y, 0x1d, 0x20);
    x += 0x1e;
    Sprite_SetHotSpot(&g_art_base[365], 18446744073709486066, 0x0);
    sprite_extract_from_sheet(&g_art_base[366], mode, x, y, 0x1e, 0x20);
    x += 0x1f;
    Sprite_SetHotSpot(&g_art_base[366], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[367], mode, x, y, 0xc, 0x20);
    x += 0xd;
    Sprite_SetHotSpot(&g_art_base[367], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[368], mode, x, y, 0xc, 0x20);
    x += 0xd;
    Sprite_SetHotSpot(&g_art_base[368], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[369], mode, x, y, 0xf, 0x20);
    Sprite_SetHotSpot(&g_art_base[369], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[328], mode, 0x361, 0xd4, 0xf, 0x20);
    Sprite_SetHotSpot(&g_art_base[328], 18446744073709486073, 0x0);
    x = 0x48d;
    y = 0xcf;
    sprite_extract_from_sheet(&g_art_base[335], mode, x, y, 0x10, 0x20);
    x += 0x11;
    Sprite_SetHotSpot(&g_art_base[335], 18446744073709486072, 0x0);
    sprite_extract_from_sheet(&g_art_base[336], mode, x, y, 0x13, 0x20);
    x += 0x14;
    Sprite_SetHotSpot(&g_art_base[336], 18446744073709486071, 0x0);
    sprite_extract_from_sheet(&g_art_base[337], mode, x, y, 0x1d, 0x20);
    x += 0x1e;
    Sprite_SetHotSpot(&g_art_base[337], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[338], mode, x, y, 0x1e, 0x20);
    x += 0x1f;
    Sprite_SetHotSpot(&g_art_base[338], 18446744073709486065, 0x0);
    sprite_extract_from_sheet(&g_art_base[339], mode, x, y, 0xe, 0x20);
    Sprite_SetHotSpot(&g_art_base[339], 18446744073709486073, 0x0);
    x = 0x4c1;
    y = 0xa9;
    sprite_extract_from_sheet(&g_art_base[320], mode, x, y, 0xc, 0x20);
    x += 0xd;
    Sprite_SetHotSpot(&g_art_base[320], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[321], mode, x, y, 0xc, 0x20);
    x += 0xd;
    Sprite_SetHotSpot(&g_art_base[321], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[322], mode, x, y, 0xd, 0x20);
    x += 0xe;
    Sprite_SetHotSpot(&g_art_base[322], 18446744073709486074, 0x0);
    sprite_extract_from_sheet(&g_art_base[323], mode, x, y, 0xe, 0x20);
    Sprite_SetHotSpot(&g_art_base[323], 18446744073709486073, 0x0);
    sprite_extract_from_sheet(&g_art_base[280], mode, 0x292, 0x17f, 0x20, 0x20);
    x = 0x2d9;
    y = 0x1a0;
    for (i = 0x0; i < 0x9; i++, x += 0xd)
        sprite_extract_from_sheet(&g_art_base[i + 393], mode, x, y, 0xc, 0xd);
    y += 0xe;
    x = 0x2d9;
    for (i = 0x9; i < 0x12; i++, x += 0xd)
        sprite_extract_from_sheet(&g_art_base[i + 393], mode, x, y, 0xc, 0xd);
    MSUnlockGraf(&g_sprite_sheet);
    GRPort_LoadPicture(&g_sprite_sheet, 0xcb, 0x10, 0x18, (struct logpal *)&g_game_palette);
    MSLockGraf(&g_sprite_sheet);
    sprite_extract_from_sheet(&g_art_base[411], mode, 0x1, 0x0,
                             port_width_1008_d707(&g_sprite_sheet),
                             port_height_1008_d720(&g_sprite_sheet) - 1);
    MSUnlockGraf(&g_sprite_sheet);
    GRPort_LoadPicture(&g_sprite_sheet, 0xfc, 0xef, 0x7, (struct logpal *)&g_game_palette);
    MSLockGraf(&g_sprite_sheet);
    Sprite_Extract_mask(&g_art_base[412], &g_sprite_sheet, mode, 0x0, 0x0,
                        port_width_1008_d707(&g_sprite_sheet),
                        port_height_1008_d720(&g_sprite_sheet));
    GRPort_Init_wh_1008_d6e7(&g_sprite_sheet);
    init_cloak_sprites();
    init_water_sprites();
}
