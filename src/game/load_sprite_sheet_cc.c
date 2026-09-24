/* load_sprite_sheet_cc -- shipped at 1008:5056.
 *
 * Ported by tools/port.py from matched/game/load_sprite_sheet_cc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_screen_surface;                    SEG20:0xdea0
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern char far g_art_base[][0x20];                  SEG20:0x41a6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GRPort_Init_wh_1008_d6e7(void *p);
void sprite_extract_to_screen(void *s, col_i16 mode, col_i16 x, col_i16 y, col_i16 w, col_i16 h);

extern void load_picture_resource(void *port, col_i16 res, col_i16 w, col_i16 h, void *pal);
extern void MSLockGraf(void *p);
extern void MSUnlockGraf(void *p);

/* same object: 4-byte `push cs; call ` */



void load_sprite_sheet_cc(void)
{
    unsigned char mode;
    col_i16 y;
    register col_i16 i;
    register col_i16 x;

    mode = 0x31;
    load_picture_resource(&g_screen_surface, 0xcc, 0x28, 0x66, &(*g_game_palette));
    MSLockGraf(&g_screen_surface);
    x = 0xcf;
    y = 0xb7;
    sprite_extract_to_screen((&g_art_base[178]), mode, x, y, 0x20, 0x20);
    sprite_extract_to_screen((&g_art_base[179]), mode, x + 0x21, y, 0x20, 0x20);
    sprite_extract_to_screen((&g_art_base[180]), mode, x, y + 0x21, 0x20, 0x20);
    sprite_extract_to_screen((&g_art_base[181]), mode, x + 0x21, y + 0x21, 0x20, 0x20);
    x = 0xd;
    y = 0x52;
    for (i = 0; i < 16; i++, x += 0x21)
        sprite_extract_to_screen((&g_art_base[i + 28]), mode, x, y, 0x20, 0x20);
    x = 0xd;
    y += 0x21;
    for (i = 0; i < 16; i++, x += 0x21)
        sprite_extract_to_screen((&g_art_base[i + 16 + 28]), mode, x, y, 0x20, 0x20);
    MSUnlockGraf(&g_screen_surface);
    GRPort_Init_wh_1008_d6e7(&g_screen_surface);
}
