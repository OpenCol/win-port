/* PieceDraw: candidate for 1040:215a (1747 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1747, not the census 1647. The trailing 100 bytes are two jump tables -- four entries
 * for the `kind` switch and forty-six for the status letter -- which Borland puts in the code segment
 * after the epilogue; `PieceDraw_noport` starts right after them.
 *
 * THE UNIT ICON WITH ITS FLAG, the one nine callers use: the colony window's squares and carriers,
 * the map, the Europe dock, two report pages and the unit panel. It composes the icon in the scratch
 * port at SEG28:0x66 and blits it through `draw_sprite_with_shadow`.
 *
 * WHICH UNIT: `pick_ship_in_chain` answers both the sprite to draw and, in `shown`, the
 * unit whose flag and status letter are used -- for a stack that is the ship rather than its cargo.
 *
 * THE FLAG is one of the eighteen 32-byte sprites at SEG20:0x72c6, chosen by a status LETTER and
 * recoloured to the nation's colour (natives use SEG20:0xd9ca). The letter is normally the order's
 * own letter from SEG20:0xd974, but:
 *   - a foreign ship shows its cargo count as a digit instead, and a foreign privateer with
 *     reveal-all off shows `X` and flies a black flag;
 *   - with SEG20:0x7785 bit 5 and debug bit 3 set, a computer nation's unit shows the raw byte at
 *     unit+7, or `E` when that byte is 0x80 or more;
 *   - a damaged unit (bit 7 of +4, artillery excepted) shows the turns left before it is repaired,
 *     halved on the map, as a digit, or `+` at ten and over.
 * Letters `7`, `8`, `9` and `E` have no flag of their own and fall to the default, flag 0.
 *
 * WHERE THE FLAG SITS depends on the unit's `kind`: 0xa,0 for a wagon, treasure train or artillery;
 * 0x14,0 for a galleon, privateer, frigate or man-o'-war; 0,0 for a caravel, a merchantman and the
 * mounted units (scouts, dragoons, the cavalries, mounted braves and warriors); and 0x14,0x13 for
 * everything else. The first and third also draw the flag OVER the unit sprite rather than under it.
 *
 * A damaged unit drawn at `mode` 0x64 gets the boycott cross at SEG20:0x6946 over it at 7/8 scale.
 *
 * SHIPPED DEAD CODE, a lot of it. `tw` and `off` are read and never written, so the centring
 * `tx += (span - tw) >> 1` and the `dx = tx + off` that follows are both nonsense -- but harmless,
 * because every arm of the `kind` switch overwrites `dx`. `stacked` is computed through two chain
 * calls and then tested by an `if` with an empty body. The whole `flags & 0x20` tail recomputes
 * `colour` after the icon is already drawn and nothing reads it again -- and `flags & 0x20` was
 * cleared in the first statement of the function, so that tail always runs and the `e`/`f` block
 * above it never does. Six more locals are written once and never read, including the two-character
 * string the letter is copied into.
 *
 * WHAT THE BYTES SETTLED: the repair countdown is halved when the unit IS on the map, not when it is
 * off it. `e` and `f` are assigned in that order (two statements, not a chained assignment). The
 * `kind` switch needs `case 0:` beside `default:` for its table to start at 0, and lays its arms out
 * in the order 2, 1, 3, 0/default. `glyph` and `type` are the two `register` locals, SI and DI in
 * declaration order (rule 53).
 *
 * RENAMED 2026-09-26 from draw_unit_icon: the Mac build's PieceDraw(GRPort&, short, short, short, short, short, short), bound by call-sequence alignment (8.0 aligned gaps in PediaWin_DoUnitArticle, ColonyWin_DrawColonyMap, ColonyWin_DrawColonyShips leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.96x this one.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "nations.h"
#include "control.h"
#include "dbgopt.h"
#include "units.h"
#include "classes.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

#include "icons.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_FillColor_all(GRPort *p, int colour);
extern int top_unit(int u);
extern int next_unit(int u);

extern void Sprite_CopyTo(Sprite *art, Sprite *dest);
extern void Sprite_ChangeColor(Sprite *s, unsigned char from, unsigned char to);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, int x, int y, int w, int h);
extern void Sprite_SetHotSpot(Sprite *s, int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int pick_ship_in_chain(int u, int flag, int *out);
extern void draw_sprite_with_shadow(void *dst, void *rec, int x, int y, int flags);

void PieceDraw(GRPort *port, int unit, int flags, int x, int y,
                                  int span, int mode)
{
    int tw;
    int unused6;
    int off;
    int centre;
    int e;
    int f;
    int tx;
    int dx;
    int dy;
    int g;
    int h;
    int pic;
    int stacked;
    int shown;
    int kind;
    int nation;
    int over;
    int orders;
    int repair_left;
    int damaged;
    int hidden;
    char colour;
    char colour2;
    char ch;
    char label[9];
    int scale_num;
    int scale_den;
    RECT r1;
    RECT r2;
    RECT r3;
    register int glyph;
    register int type;

    unused6 = 0;
    hidden = 0;
    flags &= ~0x20;
    GetGlobalSpriteScale(&scale_num, &scale_den);
    SetGlobalSpriteScale(1, 1);
    GRPort_FillColor_all(&g_icon_port, 0x31);
    pic = pick_ship_in_chain(unit, flags & 0x40, &shown);
    stacked = (flags & 0x80) && next_unit(top_unit(unit)) >= 0;
    kind = 0;
    type = g_units[shown].type;
    if (g_units[shown].type >= UNIT_SHIP_FIRST && g_units[shown].type <= UNIT_SHIP_LAST) {
        if (type == UNIT_GALLEON || type == UNIT_PRIVATEER || type == UNIT_FRIGATE || type == UNIT_MAN_O_WAR)
            kind = 1;
        else
            kind = 3;
    } else if (type == UNIT_MOUNTED_BRAVES || type == UNIT_MOUNTED_WARRIORS || type == UNIT_SCOUTS || type == UNIT_DRAGOONS || type == UNIT_CONTINENTAL_CAVALRY
               || type == UNIT_CAVALRY)
        kind = 3;
    else if (type == UNIT_WAGON_TRAIN || type == UNIT_TREASURE_TRAIN || type == UNIT_ARTILLERY)
        kind = 2;
    nation = NATION(g_units[shown]);
    orders = g_units[shown].orders;
    if (nation >= NATION_EUROPEAN_COUNT)
        orders = ORDERS_NONE;
    ch = g_order_letter[orders];
    if (g_units[shown].type >= UNIT_SHIP_FIRST && g_units[shown].type <= UNIT_SHIP_LAST
        && g_game.view_nation != nation) {
        ch = g_units[shown].cargo_count + '0';
        if (type == UNIT_PRIVATEER && g_game.reveal_all == 0) {
            hidden = 1;
            ch = 'X';
        }
    }
    if (nation < NATION_EUROPEAN_COUNT && (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN)
        && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) && (g_debug_flags & DBG_FOREIGN_AI_PLANNING_MODES)) {
        ch = g_units[shown].mission;
        if ((unsigned char)g_units[shown].mission >= 0x80)
            ch = 'E';
    }
    if (nation < NATION_EUROPEAN_COUNT)
        colour = g_nation_colour[nation];
    else
        colour = (g_tribe_colour - 4)[nation];
    colour2 = colour;
    if (hidden != 0)
        colour2 = 0;
    damaged = (g_units[shown].flags2 & 0x80) && g_units[shown].type != UNIT_ARTILLERY;
    if (damaged != 0) {
        repair_left = g_unit_types[g_units[shown].type].defence - g_units[shown].counter;
        if (on_map(g_units[shown].x, g_units[shown].y))
            repair_left = (repair_left + 1) >> 1;
        if (repair_left < 10)
            ch = repair_left + '0';
        else
            ch = '+';
    }
    switch (ch) {
    case '-':
        glyph = 0;
        break;
    case 'S':
        glyph = 1;
        break;
    case 'T':
        glyph = 2;
        break;
    case 'G':
        glyph = 3;
        break;
    case 'L':
        glyph = 4;
        break;
    case 'F':
        glyph = 5;
        break;
    case 'B':
        glyph = 6;
        break;
    case 'P':
        glyph = 7;
        break;
    case 'R':
        glyph = 8;
        break;
    case '+':
        glyph = 9;
        break;
    case 'X':
        glyph = 0xa;
        break;
    case '0':
        glyph = 0xb;
        break;
    case '1':
        glyph = 0xc;
        break;
    case '2':
        glyph = 0xd;
        break;
    case '3':
        glyph = 0xe;
        break;
    case '4':
        glyph = 0xf;
        break;
    case '5':
        glyph = 0x10;
        break;
    case '6':
        glyph = 0x11;
        break;
    default:
        glyph = 0;
        break;
    }
    Sprite_CopyTo((Sprite *)&g_flag_art[glyph], &g_flag_sprite);
    Sprite_ChangeColor(&g_flag_sprite, 0x51, colour2);
    label[0] = ch;
    label[1] = 0;
    tx = x;
    if (tw < span)
        tx += (span - tw) >> 1;
    if (flags & 0x20) {
        e = 4;
        f = 4;
    }
    centre = (g_tile_w >> 1) + x;
    dx = tx + off;
    switch (kind) {
    case 2:
        dx = 0xa;
        dy = 0;
        g = 7;
        h = 3;
        over = 1;
        break;
    case 1:
        dx = 0x14;
        dy = 0;
        over = 0;
        break;
    case 3:
        dx = 0;
        dy = 0;
        over = 1;
        break;
    case 0:
    default:
        dx = 0x14;
        dy = 0x13;
        over = 0;
        break;
    }
    Sprite_Draw(&r1, &g_flag_sprite, &g_icon_port, dx, dy);
    Sprite_Draw(&r2, &g_icon_sprites[pic], &g_icon_port, 0x10, 0);
    Sprite_Extract_mask(&g_icon_sprite, &g_icon_port, 0x31, 0, 0, 0x20, 0x20);
    Sprite_SetHotSpot(&g_icon_sprite, -0x10, 0);
    SetGlobalSpriteScale(scale_num, scale_den);
    if (over != 0) {
        if (pic == 6 || pic == 0xf || pic == 0x10)
            draw_sprite_with_shadow(port, &g_icon_sprites[pic], x, y, 8);
        else
            draw_sprite_with_shadow(port, &g_icon_sprites[pic], x, y, 1);
    }
    if (stacked != 0)
        ;
    if (over == 0) {
        if (pic == 6 || pic == 0xf || pic == 0x10)
            draw_sprite_with_shadow(port, &g_icon_sprites[pic], x, y, 8);
        else
            draw_sprite_with_shadow(port, &g_icon_sprites[pic], x, y, 1);
    }
    draw_sprite_with_shadow(port, &g_icon_sprite, x, y, 2);
    if (!(flags & 0x20)) {
        if (orders == ORDERS_SENTRY || orders == ORDERS_FORTIFIED) {
            if (nation < NATION_EUROPEAN_COUNT)
                colour += 0xf8;
            else
                colour = 8;
        } else
            colour = 0;
        if (hidden != 0)
            colour = 0xf;
        if (damaged != 0)
            colour = nation == NATION_SPAIN ? 0xc : 0xf;
    }
    if (damaged != 0 && mode == 0x64) {
        SetGlobalSpriteScale(7, 8);
        Sprite_Draw(&r3, &g_icon_sprites[ICON_BOYCOTT], port, x, y + 4);
        SetGlobalSpriteScale(1, 1);
    }
}
