/* init_water_sprites: candidate for 1008:5056 (333 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Cuts 36 sprites out of picture resource 0xcc, one step of `init_sprites`, the start-up
 * graphics loader. The picture is loaded into the screen surface at SEG20:0xdea0 in the game
 * palette and locked; then, with extraction mode 0x31, a 2x2 block at (0xcf, 0xb7) fills
 * entries 150..153 of the 32-byte sprite table at SEG20:0x4526, and two rows of sixteen at
 * y = 0x52 and 0x73, from x = 0xd, fill entries 0..15 and 16..31. Every sprite is 32x32 on a
 * 33-pixel grid. The surface is unlocked and its port reinitialized at the end.
 *
 * WHICH ENTRIES ARE FILLED IS NOT SOMETHING THE COMPARATOR CHECKS: the table's address, with
 * the constant index folded in, is one fixup -- 0x57e6 for [150], 0x4726 for `[i + 16]`. The
 * indices were read off the shipped addresses (0x4526 + n * 0x20) (§119, RE_STATUS 250).
 *
 * The loops' increments are `i++, x += 0x21` in the `for` header: the shipped increments SI
 * before adding to DI. `sprite_extract_from_sheet` and `GRPort_Init_wh_1008_d6e7` come through
 * the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from load_sprite_sheet_cc: the Mac build's init_water_sprites(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between init_cloak_sprites and init_sprites, which are already bound; the arity agrees and the Mac body is 1.37x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *port, int res, int w, int h, struct logpal *pal);
extern void MSLockGraf(GRPort *port);
extern void MSUnlockGraf(GRPort *port);

/* same object: 4-byte `push cs; call ` */
extern void sprite_extract_from_sheet(void *s, int mode, int x, int y, int w, int h);

void init_water_sprites(void)
{
    unsigned char mode;
    int y;
    register int i;
    register int x;

    mode = 0x31;
    GRPort_LoadPicture(&g_sprite_sheet, 0xcc, 0x28, 0x66, (struct logpal *)&g_game_palette);
    MSLockGraf(&g_sprite_sheet);
    x = 0xcf;
    y = 0xb7;
    sprite_extract_from_sheet(&g_art_base[178], mode, x, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[179], mode, x + 0x21, y, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[180], mode, x, y + 0x21, 0x20, 0x20);
    sprite_extract_from_sheet(&g_art_base[181], mode, x + 0x21, y + 0x21, 0x20, 0x20);
    x = 0xd;
    y = 0x52;
    for (i = 0; i < 16; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 28], mode, x, y, 0x20, 0x20);
    x = 0xd;
    y += 0x21;
    for (i = 0; i < 16; i++, x += 0x21)
        sprite_extract_from_sheet(&g_art_base[i + 16 + 28], mode, x, y, 0x20, 0x20);
    MSUnlockGraf(&g_sprite_sheet);
    GRPort_Init_wh_1008_d6e7(&g_sprite_sheet);
}
