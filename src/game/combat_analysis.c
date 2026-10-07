/* combat_analysis: candidate for 1020:68ba (5473 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: combat_analysis
 * name-source: sibling-build
 *
 * THE COMBAT ODDS WINDOW -- the "analyze" panel that lists, side by side, every modifier that went
 * into an attacker's and a defender's strength. `get_defense` and
 * `get_combat` have already left their reasons in the two bit-words at DGROUP:0x4bda and
 * 0x4bde, one pair per side; this function only turns those bits into lines.
 *
 * IT RUNS THE WHOLE BODY TWICE. The first pass draws nothing and only counts the lines each side
 * produces into a two-entry tally; the second builds a 400x240 window and draws. The tally is never
 * read afterwards -- the window is a fixed size either way -- so the first pass exists purely to
 * keep the counting code and the drawing code in step, and the two passes share every `if` in the
 * function.
 *
 * EACH SIDE gets a column, at +4 and +0xc8 from the left margin, with the unit's icon at the top and
 * then one line per modifier: the label in colour 0xfa on the left, the signed percentage in colour
 * 0xc a hundred and sixty pixels further right, thirty-six (0x24) pixels apart.
 *
 * THE MODIFIERS, in the order the shipped tests them: the unit's own name and strength (a specialist
 * prints its speciality and counts as 1); +1 labelled MUSKETS with the sprite from art 0x26;
 * +50% for a veteran soldier or dragoon; a cargo penalty of `cargo * 100 / 8` (the count at +0xc);
 * -33% and -66% for the two FATIGUE states; +50% for an attack bonus; +50% BOMBARD with a
 * Man-O-War icon, or an artillery icon on a colony square without bit 6 of +0x1c; the two Sons of Liberty terms, `100 - sol%` and `sol%`; the terrain bonus,
 * 25 per point of the terrain's +0xb; the colony's own fortification, `level * 50 + 50`, labelled
 * with the best building it has; the native settlement's, 50 per level with the capital doubling it;
 * -75% for one state and +100% for another; and three more flat +50% terms. With DEBUG (bit 5 of
 * SEG20:0x7785) two extra lines print the raw totals the caller passed in.
 *
 * THE COLONY IS LOOKED UP TWICE more inside the loop, and the second lookup OVERWRITES the caller's
 * colony argument -- so the colony the fortification line uses is whatever `find_colony` last returned
 * for the square, not the one the caller named.
 *
 * WHAT THE BYTES SETTLED: the two flag words are read once per side into locals and then tested a
 * BYTE at a time, which is why every mask above 0xff shows as a test on the odd byte. Both columns
 * pick their constants with ternaries -- `((k == 0) ? 4 : 0xc8) + left` -- and not with an `if`,
 * because the shipped stores the result once after the join. The unit speciality at +0x17 is signed
 * and needs an explicit `(int)`. Two parameters are never read and two locals are written and never
 * read. And `get_colony_defense` is the one same-segment callee reached through the five-byte NOP
 * form, so it is extern where the other six are in-file stubs (rule 47).
 *
 * RENAMED 2026-09-26 from battle_odds: the Mac build's combat_analysis(short, short, short, short, short, short, short, short, short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (callee Draw; callee Modal; callee TextColor; callee say_number; callee say_string), the runner-up gets 4, and the arity agrees (13).
 */

#include "colonization.h"

#include "gameflag.h"
#include "tribes.h"
#include "goods.h"
#include "w16types.h"
#include "jobs.h"
#include "building.h"
#include "units.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern void set_village(int s);

extern char *strings(int n);
extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int size);
extern void PieceDraw(GRPort *port, int unit, int flags, int x, int y,
                                         int span, int mode);
extern int job_name(int spec);

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void plus_string(char *buf);
extern void percent_string(char *buf);
extern void colon_string(char *buf);
extern void space_string(char *buf);
extern void minus_string(char *s);

extern int colony_rebel(void);

extern void FortDraw(GRPort *w, int c, int x, int y, int zoom,
                            int show_pop, int show_name);
extern int has_building(int bit);
extern int best_building(int bit);
extern int get_colony_defense(void);
extern void TeepeeDraw(void *w, int s, int x, int y, int zoom);
extern void EventWin_Modal(EventWin *w);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void PopupBoxDraw(char *win, int x, int y, int w, int h);

extern void show_window_1020_ac0e(void *o);

extern void MSHideWindow_of_1020_ac40(void *o);
extern void win_release_1020_ac61(void *o);

void combat_analysis(int u0, int u1, int unused_a, int unused_b, int x, int y,
                               int nation0, int nation1, int colony, int settlement,
                               int n0, int n1, int n2)
{
    int side_x;
    int wx;
    int wy;
    int left;
    int top;
    int row0;
    int side;
    int unit;
    int nation;
    int fa;
    int fb;
    int terr;
    int bonus;
    int pass;
    int count[2];
    int ww;
    int wh;
    int num;
    int ship_icon;
    int zero;
    RECT r1;
    RECT r2;
    RECT r3;
    char line[0x50];
    char val[0x50];
    register int row;
    register int col;

    if (colony >= 0)
        set_colony(colony);
    if (settlement >= 0)
        set_village(settlement);
    left = 0;
    top = 0;
    count[0] = 0;
    count[1] = 0;
    for (pass = 0; pass < 2; pass++) {
        side_x = left;
        row = top;
        if (pass == 0) {
            count[0]++;
            count[1]++;
        } else {
            ww = 0x190;
            wh = 0xf0;
            wx = 0x140;
            wy = 0x140;
            PopupBoxDraw((char *)&g_win, wx, wy, ww, wh);
            left = 0xa;
            top = 0x1e;
            side_x = left;
            row = top;
            TextColor(0xfa);
            GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, strings(g_misc_words[MISC_COMBAT_ANALYSIS]),
                                 0xc8, 0x14, 0);
            row += 0x14;
        }
        row0 = row;
        for (side = 0; side <= 1; side++) {
            row = row0;
            fa = g_combat_flags[side];
            fb = g_combat_flags2[side];
            side_x = ((side == 0) ? 4 : 0xc8) + left;
            unit = (side == 0) ? u0 : u1;
            nation = (side == 0) ? nation0 : nation1;
            col = side_x;
            zero = 0;
            if (pass != 0)
                PieceDraw((GRPort *)&g_win, unit, 0, col + 0x10, row, 0, 0x64);
            col += 0x21;
            if ((fa & 0x200) != 0) {
                line[0] = 0;
                say_string(line, job_name((int)g_units[unit].spec));
                num = 1;
            } else {
                line[0] = 0;
                say_string(line, g_unit_types[g_units[unit].type].name);
                num = g_combat_str[side];
            }
            val[0] = 0;
            say_number(val, num);
            if (pass != 0) {
                TextColor(0xfa);
                GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                TextColor(0xc);
                GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                row += 0x24;
                col = side_x;
            } else
                count[side]++;
            if ((fa & 0x400) != 0) {
                if (pass != 0)
                    Sprite_Draw(&r1, &g_icon_sprites[0x26], (GRPort *)&g_win, col + 0x10, row + 2);
                col += 0x24;
                line[0] = 0;
                say_string(line, g_goods_name_word[GOODS_MUSKETS]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 1);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 2) != 0 &&
                (g_units[unit].type == UNIT_SOLDIERS || g_units[unit].type == UNIT_DRAGOONS) &&
                (int)g_units[unit].spec == JOB_SOLDIER) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_VETERAN]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 4) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_CARGO]);
                colon_string(line);
                space_string(line);
                say_number(line, g_units[unit].cargo_count);
                val[0] = 0;
                minus_string(val);
                say_number(val, g_units[unit].cargo_count * 0x64 >> 3);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x100) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_FATIGUE]);
                val[0] = 0;
                minus_string(val);
                say_number(val, 0x21);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fb & 8) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_FATIGUE]);
                val[0] = 0;
                minus_string(val);
                say_number(val, 0x42);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 1) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_ATTACK_BONUS]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x8000) != 0) {
                colony = find_colony(x, y);
                ship_icon = 1;
                if (colony >= 0 && !(g_colonies[colony].status & 0x40))
                    ship_icon = 0;
                if (pass != 0) {
                    if (ship_icon != 0)
                        Sprite_Draw(&r2, &g_icon_sprites[g_unit_types[UNIT_MAN_O_WAR].icon], (GRPort *)&g_win, col + 0x10, row);
                    else
                        Sprite_Draw(&r3, &g_icon_sprites[g_unit_types[UNIT_ARTILLERY].icon], (GRPort *)&g_win, col + 0x10, row);
                }
                col += 0x24;
                line[0] = 0;
                say_string(line, g_misc_words[MISC_BOMBARD]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fb & 2) != 0) {
                colony = find_colony(x, y);
                set_colony(colony);
                line[0] = 0;
                say_string(line, g_misc_words[MISC_TORY_UNREST]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x64 - colony_rebel());
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fb & 4) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_REBEL_UNREST]);
                val[0] = 0;
                plus_string(val);
                say_number(val, colony_rebel());
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x80) != 0) {
                if (side == 0)
                    g_combat_flags[1] &= 0xff7f;
                terr = terrain_at(x, y);
                bonus = g_terrain_rows[terr].defence;
                if (bonus != 0) {
                    if (pass != 0)
                        ;
                    col += 0x21;
                    if (side == 0) {
                        line[0] = 0;
                        say_string(line, g_misc_words[MISC_AMBUSH]);
                    } else {
                        line[0] = 0;
                        say_string(line, g_misc_words[MISC_TERRAIN]);
                    }
                    val[0] = 0;
                    plus_string(val);
                    say_number(val, bonus * 0x19);
                    percent_string(val);
                    if (pass != 0) {
                        TextColor(0xfa);
                        GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                        TextColor(0xc);
                        GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                        row += 0x24;
                        col = side_x;
                    } else
                        count[side]++;
                }
            }
            if ((fa & 0x40) != 0) {
                if (pass != 0)
                    FortDraw((GRPort *)&g_win, colony, col, row, 8, 0, 0);
                col += 0x24;
                if (has_building(BUILDING_STOCKADE) != 0) {
                    line[0] = 0;
                    say_string(line, g_buildings[best_building(0)].name);
                } else {
                    line[0] = 0;
                    say_string(line, g_misc_words[MISC_COLONY]);
                }
                val[0] = 0;
                plus_string(val);
                say_number(val, get_colony_defense() * 0x32 + 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 8) != 0) {
                if (pass != 0)
                    TeepeeDraw(&g_win, settlement, col, row, 8);
                col += 0x24;
                bonus = 1;
                if ((fa & 0x10) != 0)
                    bonus++;
                if ((fa & 0x20) != 0) {
                    bonus = bonus * 2;
                    line[0] = 0;
                    say_string(line, g_level[LEVEL_CAPITAL_WORDS].settlement);
                } else {
                    line[0] = 0;
                    say_string(line, g_level[g_cur_tribe_rec->level].settlement);
                }
                val[0] = 0;
                plus_string(val);
                say_number(val, bonus * 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x800) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_ARTILLERY_IN_OPEN]);
                val[0] = 0;
                minus_string(val);
                say_number(val, 0x4b);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fb & 1) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_ARTILLERY_VS_RAID]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x64);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x2000) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_FORTIFIED]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x1000) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_SPAIN_BONUS]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((fa & 0x4000) != 0) {
                line[0] = 0;
                say_string(line, g_misc_words[MISC_DRAKE]);
                val[0] = 0;
                plus_string(val);
                say_number(val, 0x32);
                percent_string(val);
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
            if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) != 0) {
                if ((fa & 0x200) != 0) {
                    line[0] = 0;
                    say_string(line, job_name((int)g_units[unit].spec));
                } else {
                    line[0] = 0;
                    say_string(line, g_unit_types[g_units[unit].type].name);
                }
                if (side == 0) {
                    val[0] = 0;
                    plus_string(val);
                    say_number(val, n0);
                } else {
                    val[0] = 0;
                    plus_string(val);
                    say_number(val, n1);
                }
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
                if (side == 0) {
                    val[0] = 0;
                    plus_string(val);
                    say_number(val, n2);
                } else {
                    val[0] = 0;
                    plus_string(val);
                    say_number(val, n0 + n1);
                }
                line[0] = 0;
                if (pass != 0) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, line, col, row + 6, 5);
                    TextColor(0xc);
                    GRPort_String((GRPort *)&g_win, (unsigned *)&g_report_font_small, val, side_x + 0xa0, row + 6, 6);
                    row += 0x24;
                    col = side_x;
                } else
                    count[side]++;
            }
        }
    }
    copy_all_to_screen_1020_abf0(&g_win);
    show_window_1020_ac0e(&g_win);
    MSDoAllTasks_1020_ac2d();
    EventWin_Modal((EventWin *)((char *)&g_win + 0x26));
    MSHideWindow_of_1020_ac40((DialogWin *)((char *)&g_win + 0x26));
    win_release_1020_ac61(&g_win);
    MSDoAllTasks_1020_ac2d();
}
