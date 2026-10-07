/* TeepeeDraw: candidate for 1040:2ec4 (1638 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: TeepeeDraw
 * name-source: sibling-build
 * size-source: 1638, not the census 1630 -- the switch's 8-byte jump table follows the code.
 *
 * DRAWS ONE NATIVE SETTLEMENT'S MAP ICON: `FortDraw`'s neighbour and counterpart.
 *
 * THE PICTURE is the settlement art for the tribe's level (capped at 3), with colour 0x3c recoloured
 * to the tribe's own colour for levels 0 and 1. Settlement flag 4 adds the overlay at SEG20:0x6486.
 *
 * THE ALARM LABEL. `village_anger(s, &count)` returns a European nation (or -1) and a count, and
 * the label is one "!" per four of the count, at least one.
 *   - Shown to that nation (the viewer, SEG20:0x7798): the settlement's alarm toward it (+0xa,
 *     clamped at 0), in 32nds up to 3, or 3 whenever the tribe's alarm is 75+, picks palette entry
 *     6, 7, 8 or 11 on a black box.
 *   - Shown to anyone else: the nation's colour on a box of the same colour, with one mark.
 * Low counts (2 or less) shift the entry down by 6. The label is drawn twice, one pixel apart, from a
 * 12-entry palette held in a local initialised array.
 *
 * THE MISSION CROSS. A settlement with a mission (+5 >= 0) gets a cross of eight lines: a frame in the
 * text colour and a core in the mission's colour. The core colour is meant to be the nation's own
 * colour for an expert mission (bit 0x10) and a fixed colour otherwise, but the nation number is
 * masked with 0xf BEFORE the bit is tested, so the fixed colour is always used.
 *
 * DEAD WORK in the shipped: the map window's zoom is fetched and discarded (`num`), and with debug
 * flag 1 the viewer's alarm is formatted into the buffer and never drawn.
 *
 * REGISTER VARIABLES AND AN INITIALISED ARRAY (rule 74). The palette is a local array with an
 * initialiser, which Borland copies with `REP MOVSW` through SI and DI. With the two text offsets
 * declared `register` at function scope they lost their registers and took frame slots. Declared
 * in the inner block, after the copy, they get SI and DI as in the shipped.
 *
 * RENAMED 2026-09-26 from draw_native_settlement: the Mac build's TeepeeDraw(GRPort&, short, short, short, short), bound by call-sequence alignment (1.5 aligned gaps in StatWin_Stats, combat_analysis leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.86x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "dbgopt.h"
#include "settle.h"
#include "tribes.h"
/* PORT: this file's own Font, GRPort, Sprite, MapWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-
#include "records.h"

#include "icons.h"

   /* MapWin : ColChild, the base as its bytes: 0x788 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int MapWin_GetZoom(void *mw);
extern void SetGlobalSpriteScale(int num, int den);
extern void Sprite_Init_wh(Sprite *s, int w, int h, unsigned char colour);
extern void Sprite_CopyTo(Sprite *art, Sprite *dest);
extern void Sprite_ChangeColor(Sprite *s, unsigned char from, unsigned char to);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int village_anger(int s, int *count);
extern int pissed(int tribe, int i);

extern void GRPort_FillColor(GRPort *p, RECT *r, int colour);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern void GRPort_DrawLine(GRPort *p, short x1, short y1, short x2, short y2);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int MSGetTextLength_1040_44a9(void *font, char *s);
extern "C" int font_height_1040_44cf(void *font);

extern "C" void TeepeeDraw(GRPort *port, int s, int x, int y, int zoom)
{
    int tribe;
    int nat;
    int level;
    int px;
    int py;
    int tile_w;
    int rx;
    int ry;
    int rw;
    int rh;
    int cx;
    int cy;
    int count;
    int n;
    int mood;
    unsigned char fg;
    unsigned char bg;
    RECT r;
    int num;
    int den;
    RECT r1;
    RECT r2;
    Sprite spr;

    num = MapWin_GetZoom(&g_map_window) + 8;
    den = 8;
    SetGlobalSpriteScale(zoom, den);
    tribe = g_settlements[s].owner - 4;
    level = g_tribes[tribe].level;
    Sprite_Init_wh(&spr, 0x20, 0x20, 0x31);
    if (zoom < 100) {
        x -= 2 >> g_zoom;
        tile_w = g_tile_w;
    } else
        tile_w = 16;
    px = x;
    py = y;
    Sprite_CopyTo(&g_icon_sprites[ICON_SETTLEMENT + min_int_1040_4483(3, level)], &spr);
    fg = g_tribe_colour[tribe];
    if (level <= 1)
        Sprite_ChangeColor(&spr, 0x3c, fg);
    Sprite_Draw(&r1, &spr, port, px, py);
    if (g_settlements[s].flags & SETTLEMENT_CAPITAL)
        Sprite_Draw(&r2, &g_icon_sprites[ICON_SETTLEMENT + 7], port, px, py);
    char buf[0x50];
    unsigned char pal[12] = { 0xfa, 0x7c, 0x24, 0x51, 0x0c, 0xf9, 0x6c, 0x7c, 0x24, 0x51, 0x68, 0x3e };
    cx = x + 6;
    nat = village_anger(s, &count);
    n = count;
    if (nat >= 0) {
        if (g_game.view_nation == nat) {
            g_settlements[s].anger[nat] = max_int_1040_445d(0, g_settlements[s].anger[nat]);
            mood = g_settlements[s].anger[nat] >> 5;
            mood = min_int_1040_4483(mood, 3);
            if (pissed(tribe, nat) >= TRIBE_ATTITUDE_WAR)
                mood = 3;
            switch (mood) {
            case 0:
                fg = 6;
                break;
            case 1:
                fg = 7;
                break;
            case 2:
                fg = 8;
                break;
            case 3:
            default:
                fg = 0xb;
                break;
            }
            bg = 0;
        } else {
            n = 1;
            fg = g_nation_colour[nat];
            bg = fg;
        }
        buf[0] = 0;
        while (n >= 0) {
            cy = y + 4;
            if (n <= 2)
                fg += 0xfa;
            STRCAT(buf, "!");
            cx += 2;
            n -= 4;
        }
        rw = MSGetTextLength_1040_44a9(&g_heading_font, buf) + 1;
        rh = font_height_1040_44cf(&g_heading_font);
        rx = x + 10 - (rw >> 1);
        ry = y + 0x10 - (rh >> 1);
        SetRect(&r, rx, ry, rx + rw, ry + rh);
        GRPort_FillColor(port, &r, bg);
        TextColor(pal[fg]);
        register int ox = 0;
        register int oy = 0;
        GRPort_String(port, (unsigned *)&g_heading_font, buf, rect_width_1040_44e8(&r) / 2 + r.left + ox, y + oy + 0x10, 0);
        GRPort_String(port, (unsigned *)&g_heading_font, buf, rect_width_1040_44e8(&r) / 2 + r.left + ox - 1, y + oy + 0x10, 0);
        cx += 3;
    }
    buf[0] = 0;
    if ((int)g_settlements[s].mission >= 0) {
        unsigned char mc[4] = { 0x3b, 0x7c, 0x65, 0x4b };
        cy = y + 6;
        cx++;
        nat = (int)g_settlements[s].mission;
        nat = nat & 0xf;
        fg = (nat & 0x10) ? g_nation_colour[nat] : mc[nat];
        STRCAT(buf, "+");
        rw = 7;
        rh = 0xc;
        rx = x + 0x14 - (rw >> 1);
        ry = y + 0x10 - (rh >> 1);
        SetRect(&r, rx, ry, rx + rw, ry + rh);
        TextColor(g_text_colour);
        GRPort_DrawLine(port, rx + 2, ry, rx + 2, ry + 0xc);
        GRPort_DrawLine(port, rx + 5, ry, rx + 5, ry + 0xc);
        GRPort_DrawLine(port, rx, ry + 4, rx + 8, ry + 4);
        GRPort_DrawLine(port, rx, ry + 7, rx + 8, ry + 7);
        TextColor(fg);
        GRPort_DrawLine(port, rx + 3, ry, rx + 3, ry + 0xc);
        GRPort_DrawLine(port, rx + 4, ry, rx + 4, ry + 0xc);
        GRPort_DrawLine(port, rx, ry + 5, rx + 8, ry + 5);
        GRPort_DrawLine(port, rx, ry + 6, rx + 8, ry + 6);
        cx += 5;
    }
    if (g_debug_flags & DBG_ANGER_FRICTION_LEVELS) {
        nat = g_game.view_nation;
        cx = x + 2;
        cy = y + 9;
        LTOA1(g_settlements[s].anger[nat], buf, 10);
    }
    SetGlobalSpriteScale(1, 1);
}
