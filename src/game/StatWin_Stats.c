/* PORT: this file's own declarations of ColChild_IsSmallDesktop, FortDraw, GRPort_String, MSGetTextLength_1058_be29, TeepeeDraw, copy_rect_to_screen_1058_c340, set_window_caption, sprite_width_1058_c203 removed: the header declares them from the definition (W1 step 3). */
/* StatWin_Stats: candidate for 1058:943c (9452 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: StatWin_Stats
 * name-source: sibling-build
 * size-source: 9452, and the census does not have this function at all. It is the one -model
 * entry `docs/findings/census-gaps.md` reports missing: Ghidra never listed it, so `re_ready.py`
 * never offered it and `frontier.py` reported 100% of the census matched while this was open.
 *
 * THE INFORMATION PANEL -- the strip that says what the cursor is over. It is redrawn from
 * scratch on every move: the frame is blitted back over the client rectangle, then the lines are
 * written down the panel one after another, each one advancing `y0` by the font's height.
 *
 * WHAT IT PUTS THERE, in order:
 *
 *   1. THE DATE AND THE PURSE. Season word, year, then the treasury and the tax rate as a
 *      percentage, both read from the nation record of whoever is looking.
 *   2. THE SELECTED UNIT, when there is one and the view is not locked. Its icon at 32x32, its
 *      remaining moves as thirds (`5 1/3` comes out as "5 1 /3" -- the whole number, the
 *      remainder and the literal "/3"), the cursor square, the nation adjective and the unit
 *      type, the colonist's profession, a pioneer's tool count, a treasure train's value, and
 *      its standing orders with the destination square.
 *   3. THE SQUARE ITSELF. Terrain kind through `InfoTerrain`, then a river through
 *      `InfoTerrain2` (kind 2 with MAP0_MAJOR_RIVER, else 3), then, in brackets, the road, the
 *      plough, the resource decoration and a lost city rumour. Unexplored squares get kind 4 and nothing else.
 *   4. THE COLONY, if one is there and this nation knows about it: its picture, its name, and --
 *      only for our own colonies or in god mode -- up to five goods icons, chosen by
 *      `value * amount` and sorted, scaled down a step at a time until the row fits the panel
 *      width. Then the good the colony is exporting.
 *   5. THE NATIVE SETTLEMENT, if one is there: the picture, the tribe name, the settlement
 *      level, the mission's owner, and the settlement's disposition.
 *   6. THE STACK. Every unit on the square except the selected one, each with an icon and two
 *      lines, until the panel runs out of height. Foreign units get a single entry with the
 *      nation and the type, and only when this nation has seen that square.
 *   7. THE DEBUG LINE. With bit 0 of the debug flags set, one line per tribe with its attitude.
 *
 * Every line height is FLOATING POINT: `(double)fw / fh * 32.0 + k`, where `fw`/`fh` are 6x8 on
 * a 640-wide desktop and 8x8 otherwise. That is the only floating-point arithmetic in the
 * function and it is what `FTOL` is called fourteen times for.
 *
 * WHAT THE BYTES SETTLED:
 *
 *   - THE THREE SPRITE PORTS HAVE TO BE DECLARED IN AN INNER BLOCK. Borland allocates the
 *     int-to-double conversion temporary after the scalar locals and before the aggregates, so
 *     three function-scope `char[8]`s push it below them and move every slot from `font` down by
 *     two. Declared at the top of the colony block instead, the temporary lands at `BP-0x58` and
 *     the ports at `BP-0x60`, `BP-0x68` and `BP-0x70`, which is what shipped. Reading the `Reg:`
 *     and `BP:` records out of a `-v` build is the quick way to see this.
 *   - `fld1` IS THE INTEGER CONSTANT 1. `... * 32.0 + 1 + 8.0` emits `FMUL / FLD1 / FADDP /
 *     FADD`, and writing the two constants as one `9.0` does not.
 *   - `OffsetRect` IS ``, so its arguments push left to right; `GRPort_CopyToPort` takes the
 *     destination rectangle FIRST and the window second, which only the push order says.
 *   - THE NATION NIBBLE IS ANDed IN AL. `(unsigned char)(g_units[u].flags & 0xf)` gives
 *     `AND AL,0xF / MOV AH,0`; without the cast Borland promotes first and the AND is a word.
 *   - THREE BLOCKS NEST WHERE THEY LOOK LIKE SIBLINGS. The village and the unit stack are inside
 *     `if (known)`, and the export good is inside the goods display -- each of those `goto`-
 *     looking long conditionals is a structured `if` whose false arm happens to land on the next
 *     section (rule 12).
 *   - `move_rate` AND `continent_at` RETURN `unsigned char` HERE, and `rand_range`-style
 *     sign questions decide a `CBW` against a `MOV AH,0` at eight separate sites.
 *
 * `g_266a` IS READ HERE AND STORED TO NOWHERE, and the neighbour test is inconclusive: 0x2664,
 * 0x2666 and 0x2668 below it are written absolutely but nothing above it within 0x20 is, so a
 * block load reaching up to it cannot be ruled out (`tools/check_dead_dgroup.py`). If it really
 * is never written this guard is decided, since DGROUP ships the cell zero. Treat it as a lead;
 * the address stays until a setter is found or ruled out.
 *
 * RENAMED 2026-09-26 from draw_info_window: the Mac build's StatWin::Stats(void): this only, redrawing the whole panel, and called by the window's repaint handler; of StatWin's three Mac methods the other two are Init and LeftMouseClick, both bound by their install sites.
 */

#include "colonization.h"

#include "gameflag.h"
#include "goods.h"
#include "w16types.h"
#include "dbgopt.h"
#include "terrain.h"
#include "nations.h"
#include "jobs.h"
#include "units.h"
#include "misc.h"
#include "labels.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, StatWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"



class DlgControl;






#include "records.h"

extern Nation g_nations[];               /* SEG20:0xc2f4, 0x13c bytes each */

extern GameState g_game;   /* SEG20:0x7782 */

#pragma option -a

#pragma option -a-

#include "icons.h"

extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern int top_unit_in_square(int x, int y);
extern void sort_stack_by_class(int u);
extern int top_unit(int u);
extern int next_unit(int u);
extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern unsigned char continent_at(int x, int y);
extern char owner_of(int x, int y);
extern unsigned char site_get(int x, int y);

extern int terrain_is_ocean(int x, int y);
extern int resource_at(int x, int y);
extern int lost_city_at(int x, int y);

extern int find_village(int x, int y);
extern int can_see(int colony, int nation);
extern unsigned char move_rate(int u);
extern int get_unit_route(int u);
extern int job_of_unit(int u);
extern int pissed(int tribe, int n);
extern void set_tribe(int tribe);
extern char * get_america_name(int t);
extern int village_anger(int v, int *count);

extern char *strings(int n);
extern int get_name(int nation);
extern void sort_16x16(int n, int *payload, int *key);
extern void SetGlobalSpriteScale(int num, int den);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y,
                                         int span, int mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void comma_string(char *buf);
extern void percent_string(char *buf);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int font_height_1058_be4f(Font *font);


extern "C" void say_special(char *buf, int u, int flag);

extern "C" int StatWin_Stats(StatWin *w)
{
    register int x1;
    register int u;
    int own_gold;
    RECT r1;
    RECT r2;
    int fw;
    int fh;
    int scale;
    int y2;
    int moves;
    int thirds;
    int u0;
    int drew;
    int known;
    int col;
    int i;
    int icon;
    int nat;
    int village_level;
    int vill;
    int deco;
    int to_x;
    int to_y;
    int shown;
    int goods;
    int amount;
    int mission_nation;
    int terr;
    int sprbase;
    int wsp;
    int sel_ok;
    int any;
    int amt;
    char plane0;
    int x0;
    int y0;
    Font *font;
    char buf[0x50];
    int value[16];
    int order[16];

    own_gold = 0;
    any = 0;
    x0 = 1;
    y0 = 1;
    if (ColChild_IsSmallDesktop(w)) {
        font = &g_info_font_small;
        fw = 6;
        fh = 8;
    } else {
        font = &g_info_font_large;
        fw = 8;
        fh = 8;
    }
    GRPort_SetClip(w, &w->rect);
    r1 = w->rect;
    r2 = r1;
    OffsetRect(&r1, -w->rect.left, -w->rect.top);
    GRPort_CopyToPort(&w->picture, w, &r1, &r2);
    if (g_266a != 0)
        copy_rect_to_screen_1058_c340(w, &w->rect);
    u0 = top_unit_in_square(g_cursor_x, g_cursor_y);
    sort_stack_by_class(u0);
    u0 = top_unit(u0);
    known = (g_game.view_nation >= NATION_EUROPEAN_COUNT
        || ((0x10 << g_game.view_nation)
        & site_get(g_cursor_x, g_cursor_y))
        || g_game.reveal_all != 0);
    TextColor(0xfa);
    buf[0] = 0;
    say_string(buf, g_season_word[g_game.season]);
    space_string(buf);
    say_number(buf, g_game.year);
    GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0, w->rect.top + y0, 5);
    y0 += font_height_1058_be4f(font) + 2;
    buf[0] = 0;
    say_string(buf, g_ctitle_word[CTITLE_GOLD]);
    say_bucks(buf, g_nations[own_gold != 0 ? g_game.player_nation : g_game.view_nation].gold);
    space_string(buf);
    space_string(buf);
    say_string(buf, g_ctitle_word[CTITLE_TAX]);
    space_string(buf);
    say_number(buf, (int)g_nations[own_gold != 0 ? g_game.player_nation
        : g_game.view_nation].tax);
    percent_string(buf);
    GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0, w->rect.top + y0, 5);
    y0 += font_height_1058_be4f(font) + 6;
    if (own_gold != 0)
        goto after_tile;
    sel_ok = (g_game.view_mode == 0);
    if ((unsigned char)(g_units[g_game.selected_unit].flags & UNIT_NATION_MASK) != (unsigned char)g_game.view_nation
        && g_game.reveal_all == 0)
        sel_ok = 0;
    if (sel_ok != 0) {
        u = g_game.selected_unit;
        PieceDraw(w, u, 0, w->rect.left + x0 + 0x14,
            w->rect.top + y0 - 3, 0x20, 0x64);
        x1 = x0 + 0x2a;
        buf[0] = 0;
        say_string(buf, g_info_label[INFO_LABEL_MOVES]);
        space_string(buf);
        moves = move_rate(u) - g_units[u].moves_used;
        moves = max_int_1058_c226(0, moves);
        thirds = moves % 3;
        moves = moves / 3;
        if (moves != 0 || thirds == 0) {
            say_number(buf, moves);
            if (thirds != 0)
                space_string(buf);
        }
        if (thirds != 0) {
            say_number(buf, thirds);
            STRCAT(buf, "/3");
        }
        TextColor(0xfa);
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
            w->rect.top + y0, 5);
        y2 = font_height_1058_be4f(font) + y0;
        buf[0] = 0;
        say_string(buf, g_info_label[INFO_LABEL_LOCAT]);
        space_string(buf);
        open_string(buf);
        say_number(buf, g_cursor_x);
        comma_string(buf);
        say_number(buf, g_cursor_y);
        close_string(buf);
        if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) && (g_game.reveal_all != 0 || g_game.view_override != 0)) {
            space_string(buf);
            say_number(buf, continent_at(g_cursor_x, g_cursor_y));
        }
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
            w->rect.top + y2, 5);
        y0 += (int)((double)fw / fh * 32.0 + 6.0);
        buf[0] = 0;
        if (g_game.reveal_all != 0 && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU)) {
            say_number(buf, u);
            space_string(buf);
        }
        say_string(buf, g_nation_abbrev[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]);
        space_string(buf);
        say_string(buf, g_unit_types[g_units[u].type].name);
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
            w->rect.top + y0, 5);
        y0 += font_height_1058_be4f(font) + 2;
        if (job_of_unit(u) >= 0) {
            buf[0] = 0;
            say_special(buf, u, 1);
            TextColor(0xfb);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        if (g_units[u].type == UNIT_PIONEERS) {
            buf[0] = 0;
            open_string(buf);
            say_number(buf, g_units[u].hold.w.tools);
            space_string(buf);
            say_string(buf, g_goods_name_word[GOODS_TOOLS]);
            close_string(buf);
            TextColor(0xfb);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        if (g_units[u].type == UNIT_TREASURE_TRAIN) {
            buf[0] = 0;
            open_string(buf);
            say_string(buf, g_ctitle_word[CTITLE_GOLD]);
            space_string(buf);
            amt = (unsigned char)g_units[u].spec;
            say_number(buf, amt * 0x64);
            close_string(buf);
            TextColor(0xfb);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        buf[0] = 0;
        say_string(buf, g_order_word[g_units[u].orders]);
        space_string(buf);
        if (g_units[u].orders == 3)
            say_where(buf, g_units[u].dest_x, g_units[u].dest_y,
            (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), 1);
        else if (g_units[u].orders == ORDERS_TRADE_ROUTE) {
            say_number(buf, get_unit_route(u) + 1);
            open_string(buf);
            say_where(buf, g_units[u].dest_x, g_units[u].dest_y,
                (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), 1);
            close_string(buf);
        }
        TextColor(0xfb);
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
            w->rect.top + y0, 5);
        y0 += font_height_1058_be4f(font) + 2;
    } else {
        TextColor(0xfa);
        buf[0] = 0;
        say_string(buf, g_info_label[INFO_LABEL_LOCAT]);
        space_string(buf);
        open_string(buf);
        say_number(buf, g_cursor_x);
        comma_string(buf);
        say_number(buf, g_cursor_y);
        close_string(buf);
        if (g_game.reveal_all != 0 || g_game.view_override != 0) {
            space_string(buf);
            say_number(buf, continent_at(g_cursor_x, g_cursor_y));
        }
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
            w->rect.top + y0, 5);
        y0 += font_height_1058_be4f(font) + 2;
        if (known != 0 && terrain_is_ocean(g_cursor_x, g_cursor_y) == 0) {
            nat = owner_of(g_cursor_x, g_cursor_y);
            buf[0] = 0;
            if (nat < 0)
                say_string(buf, g_misc_words[MISC_WILDERNESS]);
            else if (nat < 4)
                STRCAT(buf, (char *)get_america_name(nat));
            else {
                say_string(buf, g_tribe[nat - 4].adjective);
                space_string(buf);
                say_string(buf, g_misc_words[MISC_LAND]);
            }
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
    }
    plane0 = map_get(g_cursor_x, g_cursor_y);
    if (known == 0) {
        x0 += w->rect.left;
        y0 += w->rect.top;
        InfoTerrain2(w, 4, &x0, &y0);
        x0 -= w->rect.left;
        y0 -= w->rect.top;
    } else {
        terr = terrain_at(g_cursor_x, g_cursor_y);
        if (terr == 0x19 && continent_at(g_cursor_x, g_cursor_y) != 1)
            terr = -1;
        x0 += w->rect.left;
        y0 += w->rect.top;
        InfoTerrain(w, terr, &x0, &y0);
        x0 -= w->rect.left;
        y0 -= w->rect.top;
        if (plane0 & 0x40) {
            x0 += w->rect.left;
            y0 += w->rect.top;
            if (plane0 & 0x80)
                InfoTerrain2(w, 2, &x0, &y0);
            else
                InfoTerrain2(w, 3, &x0, &y0);
            x0 -= w->rect.left;
            y0 -= w->rect.top;
        }
        if (feature_get(g_cursor_x, g_cursor_y) & (MAP1_SETTLEMENT | MAP1_ROAD)) {
            buf[0] = 0;
            open_string(buf);
            say_string(buf, g_misc_words[MISC_ROAD]);
            close_string(buf);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        if (feature_get(g_cursor_x, g_cursor_y) & MAP1_PLOWED) {
            buf[0] = 0;
            open_string(buf);
            say_string(buf, g_misc_words[MISC_PLOWED]);
            close_string(buf);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        deco = resource_at(g_cursor_x, g_cursor_y);
        if (deco != -1) {
            buf[0] = 0;
            open_string(buf);
            say_string(buf, g_resource_name[deco]);
            close_string(buf);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
        if (lost_city_at(g_cursor_x, g_cursor_y)) {
            buf[0] = 0;
            open_string(buf);
            say_string(buf, g_misc_words[MISC_LOST_CITY_RUMOR]);
            close_string(buf);
            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                w->rect.top + y0, 5);
            y0 += font_height_1058_be4f(font) + 2;
        }
    }
    y0 += 8;
    if (sel_ok != 0 && g_unit_types[g_units[g_game.selected_unit].type].holds > 0
        && g_units[g_game.selected_unit].cargo_count != 0) {
        buf[0] = 0;
        say_string(buf, g_info_label[INFO_LABEL_WITH]);
        TextColor(0xfa);
        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
            w->rect.top + y0, 5);
        x1 = MSGetTextLength_1058_be29(font, buf) + x0 + 0xa;
        InfoCargo(w, g_game.selected_unit, w->rect.left + x1, w->rect.top + y0 - 7);
        y0 += (int)((double)fw / fh * 33.0);
    }
    if (known != 0) {
        col = find_colony(g_cursor_x, g_cursor_y);
        if (col >= 0 && can_see(col, g_game.view_nation)) {
            char spr1[8];
            char spr2[8];
            char spr3[8];

            x1 = x0 + 0x24;
            nat = g_colonies[col].nation;
            FortDraw(w, col, w->rect.left + x0 + 2, w->rect.top + y0, fw, 0, 0);
            TextColor(0xfa);
            GRPort_String(w, (unsigned *)font, g_colonies[col].name,
                w->rect.left + x1, w->rect.top + y0 + 0xa, 5);
            y0 += (int)((double)fw / fh * 32.0 + 1 + 8.0);
            if (g_colonies[col].nation == (unsigned char)g_game.view_nation
                || g_game.reveal_all != 0) {
                buf[0] = 0;
                say_string(buf, g_info_label[INFO_LABEL_WITH_2]);
                for (i = 0; i < 0x10; i++) {
                    order[i] = (unsigned char)i;
                    sprbase = g_goods_value[nat][i] << 4;
                    if (i == 0xf)
                        sprbase = 1;
                    if (i == 0xe)
                        sprbase = 1;
                    if (i == 8 && g_colonies[col].stock[i] < 0x96)
                        sprbase = 0;
                    value[i] = min_int_1058_be68(g_colonies[col].stock[i], 0x64)
                        * sprbase;
                }
                sort_16x16(0x10, order, value);
                scale = 8;
                while (1) {
                    shown = 0;
                    for (i = 0xf; i >= 0; i--) {
                        goods = order[i];
                        amount = g_colonies[col].stock[goods];
                        if (shown < 5 && amount > 0) {
                            any = 1;
                            if (shown == 0)
                                x1 = MSGetTextLength_1058_be29(font, buf) + x0 + 0xa;
                            wsp = sprite_width_1058_c203(
                                &g_icon_sprites[sprbase + goods]);
                            if (goods != 0xa && goods != 0xf)
                                wsp = (int)((double)wsp * (scale / 8.0) + 0.5);
                            wsp = (int)((double)wsp / 2.0);
                            x1 += wsp + 5;
                            sprbase = 0x27;
                            if (amount >= 0x64)
                                sprbase = 0x17;
                            if (goods != 0xa && goods != 0xf)
                                x1 += wsp + 1;
                            else
                                x1 += sprite_width_1058_c203(
                                &g_icon_sprites[sprbase + goods]) / 2 + 1;
                            shown++;
                        }
                        if (any == 0)
                            break;
                    }
                    if (rect_width_1058_bd2d(&w->rect) < x1 + 0xa && scale > 1)
                        scale--;
                    else
                        break;
                }
                shown = 0;
                SetGlobalSpriteScale(scale, 8);
                for (i = 0xf; i >= 0; i--) {
                    goods = order[i];
                    amount = g_colonies[col].stock[goods];
                    if (shown < 5 && amount > 0) {
                        if (shown == 0) {
                            TextColor(0xfa);
                            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                                w->rect.top + y0 + 4, 5);
                            x1 = MSGetTextLength_1058_be29(font, buf) + x0 + 0xa;
                        }
                        wsp = sprite_width_1058_c203(&g_icon_sprites[sprbase + goods]);
                        if (goods != 0xa && goods != 0xf)
                            wsp = (int)((double)wsp * (scale / 8.0) + 0.5);
                        wsp = (int)((double)wsp / 2.0);
                        x1 += wsp + 5;
                        sprbase = 0x27;
                        if (amount >= 0x64)
                            sprbase = 0x17;
                        if (goods != 0xa && goods != 0xf) {
                            Sprite_Draw((RECT *)&spr1, &g_icon_sprites[sprbase + goods], w,
                                w->rect.left + x1, w->rect.top + y0 - 4);
                            x1 += wsp + 1;
                        } else {
                            SetGlobalSpriteScale(1, 1);
                            Sprite_Draw((RECT *)&spr2, &g_icon_sprites[sprbase + goods], w,
                                w->rect.left + x1, w->rect.top + y0 - 4);
                            x1 += sprite_width_1058_c203(
                                &g_icon_sprites[sprbase + goods]) / 2 + 1;
                            SetGlobalSpriteScale(scale, 8);
                        }
                        shown++;
                    }
                }
                SetGlobalSpriteScale(fw, fh);
                if (shown != 0)
                    y0 += (int)((double)fw / fh * 32.0 + 1);
                if (g_game.reveal_all != 0 || g_game.view_override >= 0) {
                    if ((int)g_colonies[col].wanted_goods >= 0) {
                        buf[0] = 0;
                        say_string(buf, g_misc_words[MISC_ASK]);
                        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                            w->rect.top + y0 + 4, 5);
                        x1 = MSGetTextLength_1058_be29(font, buf) + x0 + 1;
                        goods = (int)g_colonies[col].wanted_goods;
                        Sprite_Draw((RECT *)&spr3, &g_icon_sprites[ICON_GOODS + goods], w,
                            w->rect.left + x1, w->rect.top + y0);
                        x1 += 0x21;
                        y0 += (int)((double)fw / fh * 32.0 + 1);
                    }
                }
            }
        }
        vill = find_village(g_cursor_x, g_cursor_y);
        if (vill >= 0) {
            x1 = x0 + 0x24;
            nat = g_settlements[vill].owner - 4;
            TeepeeDraw(w, vill, w->rect.left + x0, w->rect.top + y0, fw);
            village_level = g_tribes[nat].level;
            mission_nation = g_settlements[vill].mission & 0xf;
            if (g_settlements[vill].flags & 4)
                village_level = 4;
            GRPort_String(w, (unsigned *)font, strings(g_tribe[nat].adjective),
                w->rect.left + x1, w->rect.top + y0 + 4, 5);
            GRPort_String(w, (unsigned *)font, strings(g_level[village_level].settlement),
                w->rect.left + x1, w->rect.top + y0 + 0x13, 5);
            y0 += (int)((double)fw / fh * 32.0 + 16.0 + 1);
            if ((int)g_settlements[vill].mission >= 0) {
                buf[0] = 0;
                if (g_game.view_nation != mission_nation)
                    say_string(buf, get_name(mission_nation));
                else if (g_settlements[vill].mission & 0x10)
                    say_string(buf, g_misc_words[MISC_EXPERT]);
                else
                    say_string(buf, get_name(mission_nation));
                space_string(buf);
                say_string(buf, g_misc_words[MISC_MISSION]);
                GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                    w->rect.top + y0, 5);
                y0 += font_height_1058_be4f(font) + 2;
            }
            mission_nation = village_anger(vill, 0);
            if (mission_nation >= 0) {
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_ALARMED_BY]);
                space_string(buf);
                say_string(buf, get_name(mission_nation));
                GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                    w->rect.top + y0, 5);
                y0 += font_height_1058_be4f(font) + 2;
            }
        }
        x1 = x0;
        if (u0 >= 0) {
            if ((unsigned char)(g_units[u0].flags & UNIT_NATION_MASK) == (unsigned char)g_game.view_nation
                || g_game.reveal_all != 0) {
                drew = 0;
                for (u = u0; u >= 0; u = next_unit(u)) {
                    if (g_game.view_mode == 1 || g_game.selected_unit != u) {
                        if (rect_height_1058_bd4c(&w->rect)
                            - (int)((double)fw / fh * 32.0) <= y0) {
                            TextColor(0xf9);
                            goto stack_done;
                        }
                        icon = 0;
                        SetGlobalSpriteScale(fw, fh);
                        PieceDraw(w, u, icon, w->rect.left + x1 + 0x10,
                            w->rect.top + y0, 0x20, 0x64);
                        x1 += 0x22;
                        if (g_units[u].type == UNIT_PIONEERS) {
                            buf[0] = 0;
                            if ((int)g_units[u].spec == JOB_PIONEER) {
                                say_string(buf, g_misc_words[MISC_EXPERT]);
                                space_string(buf);
                            }
                            say_number(buf, g_units[u].hold.w.tools);
                            space_string(buf);
                            say_string(buf, g_goods_name_word[GOODS_TOOLS]);
                            TextColor(0xfb);
                            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                                w->rect.top + y0 + 4, 5);
                        } else if (job_of_unit(u) >= 0) {
                            buf[0] = 0;
                            say_special(buf, u, 0);
                            TextColor(0xfb);
                            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                                w->rect.top + y0 + 4, 5);
                        } else if (g_units[u].type == UNIT_TREASURE_TRAIN) {
                            buf[0] = 0;
                            say_string(buf, g_ctitle_word[CTITLE_GOLD]);
                            space_string(buf);
                            say_number(buf, (unsigned char)g_units[u].spec * 0x64);
                            TextColor(0xfb);
                            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                                w->rect.top + y0 + 4, 5);
                        } else if (g_unit_types[g_units[u].type].holds > 0
                                && g_units[u].cargo_count != 0)
                                InfoCargo(w, u, w->rect.left + x1, w->rect.top + y0);
                            else {
                                buf[0] = 0;
                                say_string(buf, g_unit_types[g_units[u].type].name);
                                TextColor(0xfb);
                                GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                                    w->rect.top + y0 + 4, 5);
                            }
                            y2 = font_height_1058_be4f(font) + y0 + 2;
                            to_x = g_units[u].dest_x;
                            to_y = g_units[u].dest_y;
                            buf[0] = 0;
                            if (g_game.reveal_all != 0 && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU)) {
                                say_number(buf, u);
                                space_string(buf);
                            }
                            if (g_units[u].orders == 3 || g_units[u].orders == ORDERS_MISSION
                                || g_units[u].orders == 0xc || g_units[u].orders == ORDERS_TRADE_ROUTE)
                                say_where(buf, to_x, to_y, (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), 1);
                            else
                                say_string(buf, g_order_word[g_units[u].orders]);
                            if (g_units[u].orders == ORDERS_TRADE_ROUTE) {
                                space_string(buf);
                                open_string(buf);
                                say_number(buf, get_unit_route(u) + 1);
                                close_string(buf);
                            }
                            TextColor(0xfb);
                            GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                                w->rect.top + y2, 5);
                            x1 = x0;
                            y0 += (int)((double)fw / fh * 32.0 + 4.0);
                        }
                    }
stack_done:
                    if (drew != 0)
                        y0 += (int)((double)fw / fh * 32.0 + 4.0);
                } else if (UNIT_SEEN_BY(g_game.view_nation) & g_units[u0].flags) {
                    icon = 0xa0;
                    PieceDraw(w, u0, icon, w->rect.left + x1 + 0x10,
                        w->rect.top + y0, 0x20, 0x64);
                    x1 += 0x24;
                    u = u0;
                    nat = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
                    buf[0] = 0;
                    if (g_units[u].type != UNIT_PRIVATEER) {
                        say_string(buf, get_adjective(nat));
                        TextColor(0xfa);
                        GRPort_String(w, (unsigned *)font, buf, w->rect.left + x1,
                            w->rect.top + y0 + 4, 5);
                    }
                    TextColor(0xfb);
                    GRPort_String(w, (unsigned *)font,
                        strings(g_unit_types[g_units[u].type].name),
                        w->rect.left + x1, w->rect.top + y0 + 0x13, 5);
                    y0 += (int)((double)fw / fh * 32.0 + 2.0);
                }
            }
        }
        y0 += 0x11;
        if (g_debug_flags & DBG_ANGER_FRICTION_LEVELS) {
            for (i = 0; i < 8; i++) {
                set_tribe(i);
                if (!(g_tribes[i].flags & 0x80)) {
                    buf[0] = 0;
                    say_string(buf, get_name(g_cur_tribe_nation));
                    space_string(buf);
                    say_number(buf, pissed(i, g_game.view_nation));
                    GRPort_String(w, (unsigned *)font, buf, w->rect.left + x0,
                        w->rect.top + y0, 5);
                    y0 += font_height_1058_be4f(font) + 2;
                }
            }
        }
after_tile:
        GRPort_ResetClip(w);
        if (g_game.end_of_turn_pending != 0) {
            if (g_cycle_flag != 0)
                set_window_caption(w, strings(g_misc_words[MISC_END_OF_TURN]));
            else
                set_window_caption(w, "Info Window");
        }
        copy_rect_to_screen_1058_c340(w, &w->rect);
    }
