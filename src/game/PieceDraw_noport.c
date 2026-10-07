/* PORT: `&g_flag_art[art]` takes the address: the corpus's char g_flag_art[][0x20] row decayed to its address; the header's element is a Sprite (W1). */
/* PieceDraw_noport: candidate for 1040:282d (1658 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * size-source: 1658, not the census 1558. The two jump tables follow the epilogue: four entries for
 * the flag placement and 46 for the flag letter.
 *
 * BUILDS A UNIT'S MAP ICON in the scratch port at SEG28:0x66 and draws it. The callers are
 * `GenerateUnitStack`, `1038:aba2` and `1040:3a4a`. `flags` is the style word from `GenerateUnitStack`:
 * 0x40 shows the first ship of the unit's chain, 0x80 notes whether anything follows the unit, and
 * 0x20 is cleared on entry.
 *
 * THE FLAG: a letter selects one of the 18 flag sprites at SEG20:0x72c6, and its colour 0x51 is
 * repainted in the owner's colour. The letter is picked in this order:
 *  - the unit's orders, looked up in the table at SEG20:0xd974 (natives always use entry 0);
 *  - for a foreign ship, the number of cargo holds in use, as '0'..'6'. A foreign PRIVATEER shows 'X'
 *    in colour 0, so it has no nationality, unless `g_game.reveal_all` is set;
 *  - with god mode (bit 5 of SEG20:0x7785) and debug bit 3 at DGROUP:0xb6, a European unit shows its
 *    byte +7, or 'E' when that is 0x80 or above;
 *  - a damaged unit (other than artillery) shows the turns left, from its type's +5 less its counter
 *    +0x16, halved and rounded up when the unit is on the map, and '+' when that is 10 or more.
 *
 * PLACEMENT, by unit type: the four large ships have the flag at (20, 0); the caravel, the merchantman,
 * scouts, dragoons, both cavalries and mounted natives at (0, 0); wagon train, treasure and artillery at
 * (10, 0); anything else at (20, 19). The unit sprite goes at (16, 0), the whole 32x32 square is cut out
 * to SEG28:0x26 with its hot spot at (-16, 0), and `map_draw_unit_marks` draws the unit sprite, then
 * the composite. Sprites 6, 15 and 16 are drawn in mode 8, everything else in mode 1.
 *
 * DEAD CODE left in the shipped function:
 *  - the letter goes into a two-character string nobody reads;
 *  - the `stacked` and `h == 100` tests have empty bodies;
 *  - a second colour is worked out at the end and never used;
 *  - several placement locals are written and never read, and `r4` and `r8` are read before any write.
 *
 * `kind`'s switch needs `case 0:` next to `default:`. Without it, three cases compile as a compare
 * chain and not a table. Both callees are in-file stubs (rule 47).
 *
 * THE THREE SEG28 SURFACES, which this function composes a unit icon through:
 *
 *   SEG28:0x26  g_icon_sprite  the composed icon, extracted from the port with colour
 *                              0x31 masked out, given a hot spot of (-16, 0) and drawn
 *   SEG28:0x46  g_flag_sprite  a 12x13 solid-0x31 sprite the nation flag is recoloured in
 *   SEG28:0x66  g_icon_port    a 32x32 GRPort, the scratch surface the two are combined on
 *
 * `PieceInit` builds the flag sprite and the port; 0x31 is both the fill colour
 * and the mask, which is how the unfilled corners of the 32x32 square stay transparent.
 *
 * RENAMED 2026-09-27 from compose_unit_icon: the Mac build's PieceDraw_noport, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: The port-less twin of PieceDraw: it composes the unit icon plus flag in the scratch port and draws it; 6 args, matching Mac PieceDraw(short x6); ratio 0.96; called by MapWin_SlideIcon, the Mac overload's only caller. PieceDraw already names the GRPort& overload, so it needs a suffix; plain PieceDraw would collide.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "control.h"
#include "dbgopt.h"
#include "nations.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

extern int g_stub_a;
extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_FillColor_all(GRPort *p, int color);
extern int top_unit(int u);
extern int next_unit(int u);

extern void Sprite_CopyTo(Sprite *art, Sprite *dest);
extern void Sprite_ChangeColor(Sprite *s, unsigned char from, unsigned char to);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, int x, int y, int w, int h);
extern void Sprite_SetHotSpot(Sprite *s, int x, int y);

extern int pick_ship_in_chain(int u, int flag, int *out);
extern void map_draw_unit_marks(void *sprite, int x, int y, int mode);

void PieceDraw_noport(int u, int flags, int x, int y, int w, int h)
{
    int r4;
    int r6;
    int r8;
    int ra;
    int rc;
    int re;
    int r10;
    int dx;
    int dy;
    int r16;
    int r18;
    int icon;
    int stacked;
    int shown;
    int kind;
    int nation;
    int under;
    int orders;
    int value;
    int damaged;
    int hidden;
    char colour;
    char colour2;
    char letter;
    char text[8];
    int save_a;
    int save_b;
    RECT r44;
    RECT r4c;
    register int type;
    register int art;

    r6 = 0;
    hidden = 0;
    flags &= ~0x20;
    GetGlobalSpriteScale(&save_a, &save_b);
    SetGlobalSpriteScale(1, 1);
    GRPort_FillColor_all(&g_icon_port, 0x31);
    icon = pick_ship_in_chain(u, flags & 0x40, &shown);
    stacked = (flags & 0x80) && next_unit(top_unit(u)) >= 0;
    kind = 0;
    type = g_units[shown].type;
    if (g_units[shown].type >= UNIT_SHIP_FIRST && g_units[shown].type <= UNIT_SHIP_LAST) {
        if (type == UNIT_GALLEON || type == UNIT_PRIVATEER || type == UNIT_FRIGATE || type == UNIT_MAN_O_WAR)
            kind = 1;
        else
            kind = 3;
    } else if (type == UNIT_MOUNTED_BRAVES || type == UNIT_MOUNTED_WARRIORS || type == UNIT_SCOUTS || type == UNIT_DRAGOONS || type == UNIT_CONTINENTAL_CAVALRY || type == UNIT_CAVALRY) {
        kind = 3;
    } else if (type == UNIT_WAGON_TRAIN || type == UNIT_TREASURE_TRAIN || type == UNIT_ARTILLERY) {
        kind = 2;
    }
    nation = (unsigned char)(g_units[shown].flags & UNIT_NATION_MASK);
    orders = g_units[shown].orders;
    if (nation >= NATION_EUROPEAN_COUNT)
        orders = ORDERS_NONE;
    letter = g_order_letter[orders];
    if (g_units[shown].type >= UNIT_SHIP_FIRST && g_units[shown].type <= UNIT_SHIP_LAST && g_game.view_nation != nation) {
        letter = g_units[shown].cargo_count + '0';
        if (type == UNIT_PRIVATEER && g_game.reveal_all == 0) {
            hidden = 1;
            letter = 'X';
        }
    }
    if (nation < NATION_EUROPEAN_COUNT && (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) && (g_debug_flags & DBG_FOREIGN_AI_PLANNING_MODES)) {
        letter = g_units[shown].mission;
        if ((unsigned char)g_units[shown].mission >= 0x80)
            letter = 'E';
    }
    if (nation < NATION_EUROPEAN_COUNT)
        colour = g_nation_colour[nation];
    else
        colour = (g_tribe_colour - 4)[nation];
    colour2 = colour;
    if (hidden)
        colour2 = 0;
    damaged = (g_units[shown].flags2 & 0x80) && g_units[shown].type != UNIT_ARTILLERY;
    if (damaged) {
        value = g_unit_types[g_units[shown].type].defence - g_units[shown].counter;
        if (on_map(g_units[shown].x, g_units[shown].y))
            value = (value + 1) >> 1;
        if (value < 10)
            letter = (char)value + '0';
        else
            letter = '+';
    }
    switch (letter) {
    case '-': art = 0; break;
    case 'S': art = 1; break;
    case 'T': art = 2; break;
    case 'G': art = 3; break;
    case 'L': art = 4; break;
    case 'F': art = 5; break;
    case 'B': art = 6; break;
    case 'P': art = 7; break;
    case 'R': art = 8; break;
    case '+': art = 9; break;
    case 'X': art = 10; break;
    case '0': art = 11; break;
    case '1': art = 12; break;
    case '2': art = 13; break;
    case '3': art = 14; break;
    case '4': art = 15; break;
    case '5': art = 16; break;
    case '6': art = 17; break;
    default: art = 0; break;
    }
    Sprite_CopyTo(&g_flag_art[art], &g_flag_sprite);
    Sprite_ChangeColor(&g_flag_sprite, 0x51, colour2);
    text[0] = letter;
    text[1] = 0;
    r10 = x;
    if (r4 < w)
        r10 += (w - r4) >> 1;
    if (flags & 0x20) {
        rc = 4;
        re = 4;
    }
    ra = (g_tile_w >> 1) + x;
    dx = r10 + r8;
    switch (kind) {
    case 2:
        dx = 10; dy = 0; r16 = 7; r18 = 3; under = 1;
        break;
    case 1:
        dx = 0x14; dy = 0; under = 0;
        break;
    case 3:
        dx = 0; dy = 0; under = 1;
        break;
    case 0:
    default:
        dx = 0x14; dy = 0x13; under = 0;
        break;
    }
    Sprite_Draw(&r44, &g_flag_sprite, &g_icon_port, dx, dy);
    Sprite_Draw(&r4c, &g_icon_sprites[icon], &g_icon_port, 0x10, 0);
    Sprite_Extract_mask(&g_icon_sprite, &g_icon_port, 0x31, 0, 0, 0x20, 0x20);
    Sprite_SetHotSpot(&g_icon_sprite, -16, 0);
    if (under) {
        if (icon == 6 || icon == 15 || icon == 16)
            map_draw_unit_marks(&g_icon_sprites[icon], x, y, 8);
        else
            map_draw_unit_marks(&g_icon_sprites[icon], x, y, 1);
    }
    if (stacked) {
    }
    if (!under) {
        if (icon == 6 || icon == 15 || icon == 16)
            map_draw_unit_marks(&g_icon_sprites[icon], x, y, 8);
        else
            map_draw_unit_marks(&g_icon_sprites[icon], x, y, 1);
    }
    map_draw_unit_marks(&g_icon_sprite, x, y, 2);
    if (!(flags & 0x20)) {
        if (orders == ORDERS_SENTRY || orders == ORDERS_FORTIFIED) {
            if (nation < NATION_EUROPEAN_COUNT)
                colour -= 8;
            else
                colour = 8;
        } else {
            colour = 0;
        }
        if (hidden)
            colour = 15;
        if (damaged)
            colour = (nation == NATION_SPAIN) ? 12 : 15;
    }
    if (damaged && h == 100) {
    }
    SetGlobalSpriteScale(save_a, save_b);
}
