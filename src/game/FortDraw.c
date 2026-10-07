/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of Sprite_CopyOnto removed: the header declares it from the definition (W1 step 3). */
/* FortDraw: candidate for 1040:3546 (1255 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: FortDraw
 * name-source: sibling-build
 * size-source: 1255, not the census 1247 -- the switch's 8-byte jump table follows the code.
 *
 * DRAWS ONE COLONY'S MAP ICON at (x, y) and, optionally, its population and name. `zoom` is the
 * sprite scale in eighths (`SetGlobalSpriteScale(zoom, 8)`); the map, the reports and the colony
 * window all call it.
 *
 * THE PICTURE is built in a local 32x32 `Sprite`, filled with colour 0x31:
 *   - the colony picture for its fortification level (0..3: buildings 0, 1 and 2 -- stockade, fort, fortress -- each add
 *     one), from the art at SEG20:0x6266;
 *   - the nation's flag (art 0x77 + nation) overlaid at an offset that depends on the level.
 * During the revolution the Crown's colonies show the player's flag, and the player's own show
 * the rebel flag (art 0x83).
 *
 * WHAT ANOTHER NATION SEES. When the viewer (SEG20:0x7798) is not the owner and the map is not
 * revealed (0x77a4), the population shown is the owner's colony as last seen by the viewer (+0xba
 * by nation), at least 1. The last-seen level at +0xbe is read too, but by then the picture is drawn.
 *
 * THE TEXT is in the colour at DGROUP:0x337f, or 0xc if flag 4 of +0x1c is set (0xfa if flag 2 is
 * as well). The population sits in the middle of the square and the name below it, in one of three
 * fonts by zoom (4-5, 6-9, 10-12), drawn into the backdrop port at SEG28:0x0. No text appears at
 * other zooms.
 *
 * DEAD STORES, all in the shipped: the level is remapped `(level + 3) & 3` after its last use, and
 * `tile_w`, the flag offset (`fx`, `fy`) and the name position (`nx`, `ny`) are computed and never read.
 *
 * It compiled RELOCATABLE on the first build: `c` and `zoom` as register parameters (loaded before
 * `__InitExceptBlock`, rule 69), the Sprite declared last (rule 72), and `TextColor(char)` (rule 71).
 *
 * RENAMED 2026-09-26 from draw_colony: the Mac build's FortDraw(GRPort&, short, short, short, short, short, short), bound by call-sequence alignment (2.5 aligned gaps in ReportWin_ReportsColony, StatWin_Stats, combat_analysis leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.96x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "colflag.h"
#include "gameflag.h"
/* PORT: this file's own Font, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-
#include "records.h"

#include "icons.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void Sprite_Init_wh(Sprite *s, int w, int h, unsigned char colour);
extern int colony_has_building(int colony, int bit);
extern void Sprite_CopyTo(Sprite *art, Sprite *dest);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);


extern "C" void FortDraw(GRPort *port, register int colony, int x, int y, register int zoom,
                                int show_pop, int show_name)
{
    int nation;
    int level;
    int px;
    int py;
    int tile_w;
    int shown;
    int art;
    int ox;
    int oy;
    int nx;
    int ny;
    int fx;
    int fy;
    int pop;
    char colour;
    int den;
    RECT r;
    char buf[20];
    Sprite spr;

    Sprite_Init_wh(&spr, 0x20, 0x20, 0x31);
    nation = g_colonies[colony].nation;
    level = 0;
    pop = g_colonies[colony].pop;
    if (colony_has_building(colony, 0))
        level++;
    if (colony_has_building(colony, 1))
        level++;
    if (colony_has_building(colony, 2))
        level++;
    Sprite_CopyTo(&g_icon_sprites[ICON_COLONY + level], &spr);
    switch (level) {
    case 0:
        ox = 7;
        oy = 0;
        break;
    case 1:
        ox = 8;
        oy = 3;
        break;
    case 2:
        ox = 8;
        oy = 5;
        break;
    case 3:
        ox = 8;
        oy = 7;
        break;
    }
    if (g_game.view_nation != nation && g_game.reveal_all == 0) {
        level = g_colonies[colony].hist_bld[g_game.view_nation];
        pop = g_colonies[colony].hist_pop[g_game.view_nation];
        if (pop == 0) {
            pop = 1;
            g_colonies[colony].hist_pop[g_game.view_nation] = 1;
        }
    }
    level = (level + 3) & 3;
    if (zoom < 100) {
        x -= 2 >> g_zoom;
        tile_w = g_tile_w;
    } else
        tile_w = 16;
    px = x;
    py = y;
    fx = x + 6;
    fy = y + 1;
    shown = nation;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_game.mother_country_idx == nation)
        shown = g_game.player_nation;
    art = shown + 0x77;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_game.player_nation == nation) {
        art = 0x83;
        fx -= 2;
        fy--;
    }
    Sprite_CopyOnto(&g_icon_sprites[art], &spr, ox, oy);
    den = 8;
    SetGlobalSpriteScale(zoom, den);
    Sprite_Draw(&r, &spr, port, px, py);
    colour = g_text_colour;
    if (g_colonies[colony].status & COLONY_REBEL_MAJORITY) {
        colour = 0xc;
        if (g_colonies[colony].status & COLONY_REBEL_UNANIMOUS)
            colour = 0xfa;
    }
    TextColor(colour);
    if (show_pop != 0) {
        LTOA1(pop, buf, 10);
        if (zoom >= 4 && zoom < 6)
            GRPort_String(&g_map_port, (unsigned *)&g_info_font_small, buf, (g_tile_w >> 1) + px, (g_tile_h >> 1) + py, 0x10);
        if (zoom >= 6 && zoom < 10)
            GRPort_String(&g_map_port, (unsigned *)&g_font_tiny, buf, (g_tile_w >> 1) + px, (g_tile_h >> 1) + py, 0x10);
        if (zoom >= 10 && zoom < 13)
            GRPort_String(&g_map_port, (unsigned *)&g_report_font_small, buf, (g_tile_w >> 1) + px, (g_tile_h >> 1) + py, 0x10);
    }
    if (show_name != 0) {
        nx = x + 2;
        ny = y + 0x10;
        TextColor(g_text_colour);
        if (zoom >= 4 && zoom < 6)
            GRPort_String(&g_map_port, (unsigned *)&g_font_tiny, g_colonies[colony].name, (g_tile_w >> 1) + px, g_tile_h + py, 0x10);
        if (zoom >= 6 && zoom < 10)
            GRPort_String(&g_map_port, (unsigned *)&g_report_font_small, g_colonies[colony].name, (g_tile_w >> 1) + px, g_tile_h + py, 0x10);
        if (zoom >= 10 && zoom < 13)
            GRPort_String(&g_map_port, (unsigned *)&g_title_font, g_colonies[colony].name, (g_tile_w >> 1) + px, g_tile_h + py, 0x10);
    }
    SetGlobalSpriteScale(1, 1);
}
