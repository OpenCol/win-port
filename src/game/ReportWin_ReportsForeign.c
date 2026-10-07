/* ReportWin_ReportsForeign: candidate for 1058:1354 (2180 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 2180, not the census 2171. The trailing 9 bytes are the dead second epilogue.
 *
 * THE FOREIGN AFFAIRS REPORT, case 8 of the report dispatcher at 1058:0738. One block of 0x64 pixels
 * per European nation, four of them, separated by a rule drawn right across the page.
 *
 * After independence (SEG20:0x7784 bit 0) the page does not exist: FOREIGNNOTAVAIL is shown and the
 * report window closes.
 *
 * EACH BLOCK opens with the leader's name from SEG20:0x7810 and the nation's own name, prefixed with
 * one more word when the nation has declared independence (bit 2 of its record). A nation that has
 * been defeated (the one at SEG20:0x77d4) gets a single centred line instead of everything below.
 *
 * THE FIGURES -- colonies, population, military and naval strength and the two remaining counts --
 * are shown only to a player with father flag 4 (Jan de Witt) or with reveal-all on.
 *
 * THE TWO STRENGTHS WERE THE WRONG WAY ROUND HERE, and the game's own labels settle it. Every
 * figure on the row is captioned by a MISC word, and the MISC resource is 221 lines of English:
 *
 *   g_misc_words[95]  "Colonies"          g_nation_colony_count[i]
 *   g_misc_words[97]  "Average Colony"    g_avg_colony_size[i]
 *   g_misc_words[96]  "Population"        g_nation_population[i]
 *   g_misc_words[98]  "Military Power"    g_military_power[i] >> 3
 *   g_misc_words[99]  "Naval Power"       (privateers + frigates) << 3
 *   g_misc_words[100] "Merchant Marine"   g_nation_cargo_capacity[i]
 *
 * So SEG20:0xd9d6 is the MILITARY power -- it was `g_fleet_strength` -- and the naval figure is
 * two bytes of the nation's 19-byte record, added and multiplied by eight. The shift is
 * UNSIGNED, SHR and not SAR.
 *
 * THE RECORD IS `g_unit_tally`, AND THE BASE WAS ELEVEN BYTES OFF. Seven files declared a
 * 19-byte record at SEG20:0xd9eb with numbered fields, but `collect_stats` -- the one
 * function that FILLS it -- writes `g_unit_tally[nation * 0x13 + g_units[u].type]` at
 * SEG20:0xd9de. So 0xd9eb is column 13, not column 0, and every field is a unit type:
 *
 *   +0 CARAVEL  +1 MERCHANTMAN  +2 GALLEON  +3 PRIVATEER  +4 FRIGATE  +5 MAN_O_WAR
 *
 * Which is what makes this line readable, and is its own confirmation: "Naval Power" is
 * privateers plus frigates, the two fighting ships, and the merchantmen and galleons next
 * to them are counted under "Merchant Marine" instead. Read as +3 and +4 of a record based
 * at 0xd9eb it would have been merchantmen plus galleons -- the cargo ships -- under a
 * heading that says naval. `foreign_country` had the right base all along and spells the same
 * two columns `[0x10]` and `[0x11]`.
 *
 * THEN the nation's relations: every other European it has met, in the MISC word for war or peace,
 * coloured 0xff or 0xf9. A defeated nation is skipped here too. Last, for a nation that has not
 * declared independence, the split of its colonists into rebels and royalists, from the percentage at
 * +0x19 of its record.
 *
 * WHAT THE BYTES SETTLED: the three tests in the relations loop are `continue` statements (short JCC
 * over a long JMP, rule 12), not one `&&` chain. `TextColor` takes a `char` in this module, as it does
 * in `ReportWin_ReportsIndian`. `g_military_power` is unsigned -- the shift is SHR, not SAR. And the
 * function ends with an explicit `return;`, which is what puts the dead second epilogue after it;
 * falling off the end of the block emits only one.
 */

#include "colonization.h"

#include "fathers.h"
#include "nations.h"
#include "gameflag.h"
#include "units.h"
#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern Nation g_nations[];               /* SEG20:0xc2f4 */

extern int popm(char *msg, int n);
extern void set_europe(int n);

extern char *strings(int n);
extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x,
                                     int y, int size);
extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);

extern void space_string(char *buf);
extern void colon_string(char *buf);

extern int has_father(int nation, int bit);
extern int get_treaty(int nation_a, int nation_b);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int MSGetTextLength_1058_be29(void *font, const char *s);

void ReportWin_ReportsForeign(ReportWin *w, int nation)
{
    int margin;
    int y;
    int tx;
    int j;
    int y_row;
    int naval_power;
    int at_peace;
    int any;
    int rebels;
    int tories;
    char buf[0x50];
    register int i;
    register int x;

    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        popm("FOREIGNNOTAVAIL", 1);
        ReportWin_Done(w);
        goto out;
    }
    set_europe(nation);
    TextColor(0xa);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, strings(g_misc_words[MISC_FOREIGN_AFFAIRS_REPORT]), w->rect.left + 0x140,
                         w->rect.top + 0xf, 0x10);
    margin = 4;
    y = 0x24;
    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        y_row = i * 0x64 + y - 3;
        TextColor(0xf);
        GRPort_DrawLine((GRPort *)w, w->rect.left, w->rect.top + y_row, w->rect.left + 0x280, w->rect.top + y_row);
    }
    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        y_row = y;
        buf[0] = 0;
        STRCAT(buf, g_nation_text[i].leader);
        say_string(buf, g_misc_words[MISC_S]);
        space_string(buf);
        TextColor(0xe);
        GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + margin, w->rect.top + y, 5);
        x = MSGetTextLength_1058_be29(&g_font_small, buf) + margin + 2;
        buf[0] = 0;
        if (g_nations[i].flags & 4) {
            say_string(buf, g_misc_words[MISC_FREE]);
            space_string(buf);
        }
        say_string(buf, get_adjective(i));
        colon_string(buf);
        TextColor(0xf);
        GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + x, w->rect.top + y, 5);
        y += 0x13;
        if (g_game.mother_country_idx == i) {
            buf[0] = 0;
            y += 0x13;
            say_string(buf, g_misc_words[MISC_WITHDRAWN_FROM_NEW_WORLD]);
            TextColor(7);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + 0x140, w->rect.top + y, 0);
        } else {
            if (has_father(nation, FATHER_JAN_DE_WITT) || g_game.reveal_all != 0) {
                x = margin;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_COLONIES]);
                colon_string(buf);
                say_number(buf, g_nation_colony_count[i]);
                TextColor(0xe);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                x = 0xa0;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_AVERAGE_COLONY]);
                colon_string(buf);
                say_number(buf, g_avg_colony_size[i]);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                x = 0x140;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_POPULATION]);
                colon_string(buf);
                say_number(buf, g_nation_population[i]);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                y += 0x13;
                x = margin;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_MILITARY_POWER]);
                colon_string(buf);
                say_number(buf, g_military_power[i] >> 3);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                x = 0xa0;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_NAVAL_POWER]);
                colon_string(buf);
                naval_power = (g_unit_tally[i][UNIT_PRIVATEER] + g_unit_tally[i][UNIT_FRIGATE]) << 3;
                say_number(buf, naval_power);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                x = 0x140;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_MERCHANT_MARINE]);
                colon_string(buf);
                say_number(buf, g_nation_cargo_capacity[i]);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            }
            y += 0x13;
            x = margin;
            any = 0;
            for (j = 0; j < 4; j++) {
                if (j == i)
                    continue;
                if (!(get_treaty(i, j) & 0x20))
                    continue;
                if (g_game.mother_country_idx == j)
                    continue;
                any = 1;
                buf[0] = 0;
                say_string(buf, g_nation_name[j]);
                colon_string(buf);
                TextColor(0xe);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                tx = MSGetTextLength_1058_be29(&g_heading_font, buf) + x + 2;
                buf[0] = 0;
                at_peace = get_treaty(i, j) & 0x40;
                say_string(buf, g_misc_words[at_peace != 0 ? 0x66 : 0x65]);
                TextColor(at_peace != 0 ? 0xff : 0xf9);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + tx, w->rect.top + y, 5);
                x += 0xa0;
            }
            if (any != 0)
                y += 0x13;
            x = margin;
            if (!(g_nations[i].flags & 4)) {
                rebels = g_nations[i].support * g_nation_population[i] / 100;
                tories = g_nation_population[i] - rebels;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_REBELS]);
                colon_string(buf);
                say_number(buf, rebels);
                TextColor(0xe);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
                x = 0xa0;
                buf[0] = 0;
                say_string(buf, g_misc_words[MISC_TORIES]);
                colon_string(buf);
                say_number(buf, tories);
                GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            }
        }
        y = y_row + 0x64;
    }
out:
    return;
}
