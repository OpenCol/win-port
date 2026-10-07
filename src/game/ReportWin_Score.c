/* PORT: this file's own declarations of GRPort_String, MSGetTextLength_1058_be29, font_height_1058_be4f removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 167, 188 of the imported file. */
/* PORT: this file's own declarations of PopGuy, write_in_color removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_Score: candidate for 1058:1dad (4128 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: ReportWin_Score
 * name-source: sibling-build
 * size-source: 4128, not the census 4119. The last nine bytes are an unreachable second epilogue.
 *
 * HOW THE GAME SCORES YOU, and the only report page whose RETURN VALUE the dispatcher keeps. The
 * second argument says whether to draw: the same code computes the number either way, so the score
 * can be asked for without a window.
 *
 * SEVEN TERMS, each drawn as its own line and each added to one total:
 *   population   4 a colonist, but 2 for JOB_COLONIST_ALT and 1 for JOB_INDENTURED_SERVANT,
 *                JOB_CRIMINAL and JOB_CONVERT, which is what the code below says -- counted
 *                over every colony AND every unit on the map, with the portraits laid into a grid
 *                as it goes, so the picture and the arithmetic are the same loop;
 *   fathers      5 for each founding father, listed four across until the column runs past 0x120;
 *   gold         the treasury divided by a thousand, and only when it reaches a thousand;
 *   villages     MINUS the villages-burned count (nation +0x18) times the difficulty plus one -- the
 *                only negative term;
 *   liberty      the Sons of Liberty percentage, straight;
 *   time         twice the turns left, (1780 - turn) * 2, only on a won game;
 *   rebels       the bell total over a hundred, capped at a hundred.
 * The turn number itself is two SIGNED bytes, low plus high times a hundred.
 *
 * THEN THE DIFFICULTY MULTIPLIER. The number of OTHER nations that also rebelled is counted at the
 * top, and the final total is scaled by `(8 + (8 >> rebels)) / 8` -- so alone you keep 2x, with one
 * other rebel 1.5x, with two 1.25x. The same shift is shown to the player as `100 >> rebels` per
 * cent. The multiply is done in 32 bits and shifted back down by three.
 *
 * A SHIPPED BUG: two lines are drawn WITHOUT the draw-flag guard: the bells line (the `rebels` term) and the
 * Continental Congress fathers total. Asking this function for the score alone still paints both.
 *
 * The bar across the bottom is the total shifted right two and clamped to the window width less
 * 0x46, drawn over a full-width bar in the darker colour.
 *
 * WHAT THE BYTES SETTLED: both unit and colony filters are ONE expression ending in `continue`,
 * `if (owner != nation && ((flags & 1) == 0 || owner != rebel)) continue;` -- the shipped jumps are
 * short hops to a shared trampoline, which is rule 12's `continue` and not a structured if. The
 * rebel nation is compared as a BYTE against a colony's owner and as a WORD against the nation
 * number, two declarations of one global (rule 9). And the pen advance is written `x = width + x`
 * in the first two blocks and `x += width` in the other five, which is rule 60 and three bytes each
 * time.
 */

#include "colonization.h"

#include "fathers.h"
#include "gameflag.h"
#include "w16types.h"
#include "nations.h"
#include "units.h"
#include "jobs.h"
#include "misc.h"
#include "goods.h"
#include "classes.h"
#include "records.h"

extern Nation g_nations[];        /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern char *strings(int n);

extern void space_string(char *buf);
extern void colon_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void plus_string(char *buf);
extern void percent_string(char *buf);
extern void set_colony(int colony);
extern void set_europe(int n);
extern int get_specialty(int u);
extern int ColonistIcon(int spec);
extern int job_of_unit(int u);
extern int UnitIcon(int u);
extern int has_father(int nation, int bit);

extern void GRPort_FillColor(GRPort *w, RECT *r, int colour);

/* same object: 4-byte `push cs; call ` (rule 47) */

int ReportWin_Score(ReportWin *w, int draw)
{
    int nation;
    int ci;
    int ui;
    int j;
    int pic;
    int margin;
    int y;
    int x;
    int fy;
    int bit;
    int declaration_year;
    int rebels = 0;
    register int pop = 0;
    register int spec;
    int gold = 0;
    int lib = 0;
    int pct = 0;
    int sol = 0;
    int penalty = 0;
    int fathers = 0;
    int bonus = 0;
    int total = 0;
    RECT r;
    long scaled;
    char buf[0x50];

    declaration_year = g_game.year_lo;
    declaration_year += g_game.year_hi * 0x64;
    for (nation = 0; nation < NATION_EUROPEAN_COUNT; nation++) {
        if (g_game.player_nation == nation)
            continue;
        if (g_nations[nation].flags & 4)
            rebels++;
    }
    nation = g_game.player_nation;
    if (draw != 0) {
        y = 0xf;
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font, strings(g_misc_words[MISC_COLONIZATION_SCORE]),
                             w->rect.left + 0x140, w->rect.top + y, 0x10);
        if (g_game.game_flags & GAMEFLAG_SCORED) {
            y = 0x61;
            TextColor(0xfa);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_small, strings(g_misc_words[MISC_SCORING_COMPLETE]),
                                 w->rect.left + 0x140, w->rect.top + y, 0);
            goto done;
        }
        y += 0x1a;
        buf[0] = 0;
        say_string(buf, g_difficulty_words[g_game.difficulty]);
        space_string(buf);
        STRCAT(buf, g_nation_text[nation].leader);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_OF_THE]);
        space_string(buf);
        say_string(buf, get_true_adjective(nation));
        colon_string(buf);
        say_string(buf, g_season_word[g_game.season]);
        space_string(buf);
        say_number(buf, g_game.year);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + 0x140, w->rect.top + y, 0);
        y += 0x21;
    }
    margin = 0x20;
    y = 0x37;
    g_layout_x = 0x20;
    g_layout_y = y + 0x12;
    for (ci = 0; g_game.colony_count > ci; ci++) {
        set_colony(ci);
        if (g_cur_colony_rec->nation != (unsigned char)nation
            && ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0
                || g_cur_colony_rec->nation != (unsigned char)g_game.mother_country_idx))
            continue;
        for (j = 0; g_cur_colony_rec->pop > j; j++) {
            spec = get_specialty(j);
            pic = ColonistIcon(spec);
            if (draw != 0)
                PopGuy((GRPort *)(w), pic);
            if (spec == JOB_COLONIST_ALT)
                pop += 2;
            else if (spec == JOB_INDENTURED_SERVANT || spec == JOB_CRIMINAL || spec == JOB_CONVERT)
                pop++;
            else
                pop += 4;
        }
    }
    for (ui = 0; g_game.unit_count > ui; ui++) {
        if ((unsigned char)(g_units[ui].flags & UNIT_NATION_MASK) != (unsigned char)nation
            && ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 || g_game.mother_country_idx != nation))
            continue;
        if (job_of_unit(ui) < 0)
            continue;
        pic = UnitIcon(ui);
        if (pic >= 0x4a && pic <= 0x4e) {
            spec = g_units[ui].spec;
            pic = ColonistIcon(spec);
        }
        if (draw != 0)
            PopGuy((GRPort *)(w), pic);
        if (spec == JOB_COLONIST_ALT)
            pop += 2;
        else if (spec == JOB_INDENTURED_SERVANT || spec == JOB_CRIMINAL || spec == JOB_CONVERT)
            pop++;
        else
            pop += 4;
    }
    if (draw != 0) {
        buf[0] = 0;
        say_string(buf, get_true_adjective(nation));
        space_string(buf);
        say_string(buf, g_misc_words[MISC_CITIZENS]);
        colon_string(buf);
        space_string(buf);
        plus_string(buf);
        say_number(buf, pop);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + margin, w->rect.top + y, 5);
    }
    y = g_layout_y + 0x28;
    x = 0x20;
    fy = y + 0x1d;
    for (bit = 0; bit < FATHER_COUNT; bit++) {
        if (has_father(nation, bit)) {
            fathers += 5;
            if (draw != 0) {
                buf[0] = 0;
                say_string(buf, g_fathers[bit].name);
                if (fy <= 0x120) {
                    TextColor(0xfa);
                    GRPort_String((GRPort *)w, (unsigned *)&g_info_font_large, buf, w->rect.left + x,
                                         w->rect.top + fy, 5);
                }
                x += 0x96;
                if (x > 0x258) {
                    x = 0x20;
                    fy += font_height_1058_be4f(&g_info_font_large) + 3;
                }
            }
        }
    }
    buf[0] = 0;
    say_string(buf, get_true_adjective(nation));
    space_string(buf);
    say_string(buf, g_misc_words[MISC_CONTINENTAL_CONGRESS]);
    colon_string(buf);
    plus_string(buf);
    say_number(buf, fathers);
    TextColor(0xc);
    GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + margin, w->rect.top + y, 5);
    if (y <= 0x120)
        y = 0x12c;
    else
        y += 0x21;
    margin = 0x20;
    x = margin;
    set_europe(nation);
    if (g_cur_nation_rec->gold >= 0x3e8L) {
        gold = (int)(g_cur_nation_rec->gold / 0x3e8L);
        if (draw != 0) {
            buf[0] = 0;
            say_string(buf, g_misc_words[MISC_GOLD]);
            colon_string(buf);
            open_string(buf);
            say_bucks(buf, g_cur_nation_rec->gold);
            close_string(buf);
            space_string(buf);
            plus_string(buf);
            say_number(buf, gold);
            x = write_in_color(buf, x, y, g_colour_table[0]);
            TextColor(0xc);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            x = MSGetTextLength_1058_be29(&g_heading_font, buf) + x;
            x += 0x28;
        }
    }
    if (g_cur_nation_rec->villages_burned != 0) {
        penalty = g_cur_nation_rec->villages_burned * (g_game.difficulty + 1) * -1;
        if (draw != 0) {
            buf[0] = 0;
            say_number(buf, g_cur_nation_rec->villages_burned);
            space_string(buf);
            say_string(buf, g_misc_words[MISC_VILLAGES_BURNED]);
            colon_string(buf);
            space_string(buf);
            say_number(buf, penalty);
            TextColor(0xc);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            x = MSGetTextLength_1058_be29(&g_heading_font, buf) + x;
            x += 0x28;
        }
    }
    y += 0x21;
    x = margin;
    if (g_game.rebel_sentiment != 0) {
        lib = g_game.rebel_sentiment;
        if (draw != 0) {
            buf[0] = 0;
            say_string(buf, g_misc_words[MISC_REBEL]);
            space_string(buf);
            say_string(buf, g_misc_words[MISC_SENTIMENT]);
            colon_string(buf);
            space_string(buf);
            plus_string(buf);
            say_number(buf, lib);
            TextColor(0xc);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            x += MSGetTextLength_1058_be29(&g_heading_font, buf);
            x += 0x28;
        }
    }
    if ((g_game.game_flags & GAMEFLAG_REVOLUTION_WON) && declaration_year < 0x6f4) {
        bonus = (0x6f4 - declaration_year) * 2;
        if (draw != 0) {
            buf[0] = 0;
            say_string(buf, g_misc_words[MISC_EARLY_REVOLUTION]);
            space_string(buf);
            open_string(buf);
            say_string(buf, g_season_word[g_game.season]);
            space_string(buf);
            say_number(buf, g_game.year);
            close_string(buf);
            colon_string(buf);
            space_string(buf);
            plus_string(buf);
            say_number(buf, bonus);
            TextColor(0xc);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            x += MSGetTextLength_1058_be29(&g_heading_font, buf);
            x += 0x28;
        }
    }
    if (g_game.rebel_sentiment != 0 || ((g_game.game_flags & GAMEFLAG_REVOLUTION_WON) && declaration_year < 0x6f4))
        y += 0x21;
    x = margin;
    if ((g_game.game_flags & GAMEFLAG_INTERVENTION) && g_cur_nation_rec->bells >= 0x64) {
        sol = (int)((long)g_cur_nation_rec->bells / 0x64L);
        sol = min_int_1058_be68(sol, 0x64);
        buf[0] = 0;
        say_string(buf, g_goods_name_word[PROD_BELLS]);
        colon_string(buf);
        plus_string(buf);
        say_number(buf, sol);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
        x += MSGetTextLength_1058_be29(&g_heading_font, buf);
        x += 0x28;
    }
    if (g_game.game_flags & GAMEFLAG_REVOLUTION_WON) {
        pct = 0x64 >> rebels;
        if (draw != 0) {
            buf[0] = 0;
            say_string(buf, g_misc_words[MISC_INDEPENDENCE]);
            space_string(buf);
            say_string(buf, g_misc_words[MISC_ACHIEVED]);
            if (rebels != 0) {
                space_string(buf);
                open_string(buf);
                say_number(buf, rebels);
                space_string(buf);
                say_string(buf, g_misc_words[MISC_PRIOR_NATIONS]);
                close_string(buf);
            }
            colon_string(buf);
            plus_string(buf);
            say_number(buf, pct);
            percent_string(buf);
            TextColor(0xc);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
            x += MSGetTextLength_1058_be29(&g_heading_font, buf);
            x += 0x28;
        }
    }
    if ((g_game.game_flags & GAMEFLAG_INTERVENTION) || (g_game.game_flags & GAMEFLAG_REVOLUTION_WON))
        y += 0x11;
    total = pop + gold + lib + fathers + sol + penalty + bonus;
    if (pct != 0) {
        pct = 8 >> rebels;
        scaled = (long)total * ((long)pct + 8);
        scaled = scaled >> 3;
        total = (int)scaled;
    }
    x = margin;
    if (draw != 0) {
        buf[0] = 0;
        say_string(buf, g_misc_words[MISC_TOTAL_SCORE]);
        colon_string(buf);
        say_number(buf, total);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 5);
        x += MSGetTextLength_1058_be29(&g_heading_font, buf);
        SetRect(&r, 0x23, rect_height_1058_bd4c(&w->rect) - 0x2d,
                rect_width_1058_bd2d(&w->rect) - 0x23,
                rect_height_1058_bd4c(&w->rect) - 0x1f);
        OffsetRect(&r, w->rect.left, w->rect.top);
        GRPort_FillColor((GRPort *)w, &r, 0xf8);
        SetRect(&r, 0x23, rect_height_1058_bd4c(&w->rect) - 0x2d,
                minimax(total >> 2, 0, rect_width_1058_bd2d(&w->rect) - 0x46) + 0x23,
                rect_height_1058_bd4c(&w->rect) - 0x1f);
        OffsetRect(&r, w->rect.left, w->rect.top);
        GRPort_FillColor((GRPort *)w, &r, 0xfa);
    }
done:
    if (draw != 0)
        ;
    return total;
}
