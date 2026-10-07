/* PORT: this file's own declarations of GRPort_String, MSGetTextLength_1058_be29 removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ReportsIndian: candidate for 1058:0cab (1705 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 1705, the census figure, and the gap to the next function is 1705 exactly.
 *
 * THE INDIAN ADVISOR REPORT, case 1 of the report dispatcher at 1058:0738. One block per tribe, eight
 * of them, 0x28 pixels apart: a mood face, the tribe's adjective and its level, and then a line of
 * counts.
 *
 * A TRIBE IS LISTED when the player has met it (pair flag 0x20) or it has been destroyed (bit 7 of
 * Tribe+3). The mood face is `piss_factor` of the tribe's attitude -- five faces, picked
 * from the remainder over 5 and turned round for a positive quarter -- and a destroyed tribe always
 * gets face 3. The name is drawn in the nation's own colour: DGROUP:0x9a for a European, the table at
 * SEG20:0xd9ce indexed by `tribe - 4` for a native.
 *
 * THE COUNTS, for a tribe still alive: its villages (and how many of them hold this player's
 * missionary), the tribe's own muskets plus every armed brave and mounted warrior it has on the map,
 * multiplied by fifty, and the byte at Tribe+8. The village count is always drawn and the other three only when non-zero, 0x70 pixels apart.
 *
 * SHIPPED QUIRK: `face`, the five-way mood index, is computed and clamped and then never used -- the
 * sprite is chosen by `mood`, the quarter, which is 0..3. And a `get_treaty(...) & 0x40`
 * test guards an empty statement.
 *
 * WHAT THE BYTES SETTLED, four declarations and one spelling. `nation` is an int parameter but is cast
 * to `char` for the missionary comparison. `TextColor` takes a `char` in this module, so the colour
 * byte is pushed as a word without promotion (rule 71). The tribe comparisons are byte-wide,
 * `== (unsigned char)tnation`. The name colour is a ternary -- one store after the join. And the fifty
 * times multiply is a COMPOUND assignment: `muskets_total *= 0x32` loads the constant and multiplies memory,
 * where `muskets_total = muskets_total * 0x32` would use the immediate IMUL and be three bytes shorter (rule 78).
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "settle.h"
#include "units.h"
#include "misc.h"
#include "classes.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

#include "icons.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern char *strings(int n);
extern void set_tribe(int tribe);
extern int get_treaty(int nation_a, int nation_b);
extern int pissed(int a, int tribe);
extern int piss_factor(int tension);

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int get_name(int nation);

extern void space_string(char *buf);
extern void colon_string(char *buf);
extern void set_village(int s);

/* same object: 4-byte `push cs; call ` (rule 47) */

void ReportWin_ReportsIndian(ReportWin *w, int nation)
{
    int i;
    int x0;
    int y0;
    int y;
    int tnation;
    int mood;
    int face;
    int villages;
    int missions;
    int muskets_total;
    char colour;
    RECT r;
    char buf[0x50];
    register int s;
    register int x;

    TextColor(0xa);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, strings(g_misc_words[MISC_INDIAN_ADVISOR_REPORT]), w->rect.left + 0x140,
                         w->rect.top + 0xf, 0x10);
    x0 = 0xa;
    y0 = 0x2d;
    for (i = 0; i < 8; i++) {
        set_tribe(i);
        tnation = g_cur_tribe_nation;
        if ((get_treaty(nation, tnation) & 0x20)
            || (g_cur_tribe_rec->flags & 0x80)) {
            x = x0;
            y = y0;
            mood = piss_factor(pissed(i, nation));
            face = (pissed(i, nation) - mood * 0x19) / 5;
            if (mood > 0)
                face = 4 - face;
            face = minimax(face, 0, 4);
            if ((get_treaty(nation, tnation) & 0x40) == 0)
                ;
            if (g_cur_tribe_rec->flags & 0x80)
                mood = 3;
            Sprite_Draw(&r, &g_icon_sprites[ICON_MOOD + mood], (GRPort *)w, w->rect.left + x, w->rect.top + y);
            x += 0x28;
            y += 6;
            colour = tnation < 4 ? g_nation_colour[tnation] : g_tribe_colour[tnation - 4];
            buf[0] = 0;
            say_string(buf, get_name(tnation));
            colon_string(buf);
            if (g_cur_tribe_rec->flags & 0x80) {
                say_string(buf, g_misc_words[MISC_EXTINCT]);
                TextColor(colour);
                GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + x, w->rect.top + y, 0x15);
                x += MSGetTextLength_1058_be29(&g_font_small, buf) + 2;
            } else {
                TextColor(colour);
                GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + x, w->rect.top + y, 0x15);
                x += MSGetTextLength_1058_be29(&g_font_small, buf) + 2;
            }
            if (!(g_cur_tribe_rec->flags & 0x80)) {
                buf[0] = 0;
                say_string(buf, g_level[g_cur_tribe_rec->level].name);
                x = 0x276 - MSGetTextLength_1058_be29(&g_font_small, buf);
                TextColor(colour);
                GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + x, w->rect.top + y, 0x15);
                x = x0 + 0x3c;
                y += 0x14;
                villages = 0;
                missions = 0;
                for (s = 0; s < g_game.settlement_count; s++) {
                    if (g_settlements[s].owner == (unsigned char)tnation) {
                        set_village(s);
                        villages++;
                        if (((int)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == (char)nation)
                            missions++;
                    }
                }
                muskets_total = g_cur_tribe_rec->muskets;
                for (s = 0; s < g_game.unit_count; s++) {
                    if (NATION(g_units[s]) == (unsigned char)tnation
                        && (g_units[s].type == UNIT_ARMED_BRAVES || g_units[s].type == UNIT_MOUNTED_WARRIORS))
                        muskets_total++;
                }
                muskets_total *= 0x32;
                buf[0] = 0;
                say_number(buf, villages);
                space_string(buf);
                if (villages == 1)
                    say_string(buf, g_level[g_cur_tribe_rec->level].settlement);
                else
                    say_string(buf, g_level[g_cur_tribe_rec->level].settlements);
                TextColor(g_draw_color);
                GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + x, w->rect.top + y, 5);
                x += 0x70;
                if (missions != 0) {
                    buf[0] = 0;
                    say_number(buf, missions);
                    space_string(buf);
                    if (missions == 1)
                        say_string(buf, g_misc_words[MISC_MISSION]);
                    else
                        say_string(buf, g_misc_words[MISC_MISSIONS]);
                    TextColor(g_draw_color);
                    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + x, w->rect.top + y, 5);
                }
                x += 0x70;
                if (muskets_total != 0) {
                    buf[0] = 0;
                    say_number(buf, muskets_total);
                    space_string(buf);
                    say_string(buf, g_goods_name_word[GOODS_MUSKETS]);
                    TextColor(g_draw_color);
                    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + x, w->rect.top + y, 5);
                }
                x += 0x70;
                if (g_cur_tribe_rec->horse_herds != 0) {
                    buf[0] = 0;
                    say_number(buf, (int)g_cur_tribe_rec->horse_herds);
                    space_string(buf);
                    say_string(buf, g_misc_words[MISC_HORSE_HERDS]);
                    TextColor(g_draw_color);
                    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + x, w->rect.top + y, 5);
                }
            }
            y0 += 0x28;
        }
    }
}
