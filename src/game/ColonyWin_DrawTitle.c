/* ColonyWin_DrawTitle: candidate for 1010:a4c5 (756 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawTitle
 * name-source: sibling-build
 *
 * The colony window's status line, and the counterpart of the Europe screen's
 * `EuropeWin_DrawTitle`: "Jamestown.  Spring, 1623.  <heading> <treasury>", built into an
 * 0x50-byte buffer and handed to `game_win_set_text_1010_e756`. The heading is the same word slot
 * (SEG20:0xd984) the Europe line uses, and `say_my_bucks` appends the owner's gold. Nothing is
 * drawn while the window's word at +0x776 is not -1.
 *
 * FOR A COLONY NO HUMAN PLAYS, THE LINE IS A DEBUG READOUT. When the owner is a native nation or a
 * European whose control byte at SEG20:0x7841 is non-zero -- and neither DGROUP:0xa08 nor the byte at
 * DGROUP:0x8a is set -- the name and date are replaced by the colony's flags byte (+0x1b) in binary,
 * the four signed bytes at +0x8c..+0x8f with the second shown as a goods NAME, the MISC word
 * "Location", the colony square's land region (`land_region_at`), a colon, and the byte the
 * owner's row of the 16-column table at SEG24:0x500 holds for that region. What those values are is
 * not established here; the display says only that they are per colony, and that one is a goods id.
 *
 * RENAMED 2026-09-26 from colony_draw_status: the Mac build's ColonyWin::DrawTitle(void): this only, building the colony/season/treasury line and setting it as the caption, exactly as EuropeWin_DrawTitle does for Europe.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "misc.h"
#include "labels.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);

extern void space_string(char *buf);

extern int land_region_at(int x, int y);
extern void colon_string(char *buf);

extern void period_string(char *buf);
extern void comma_string(char *buf);
extern char *strings(int n);
extern void say_my_bucks(int nation, char *buf);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColonyWin_DrawTitle(ColonyWin *w)
{
    char buf[0x50];
    register int i;
    register int region;

    if (w->msg_timer != -1)
        return;
    set_colony(w->colony);
    if ((g_colonies[w->colony].nation >= NATION_EUROPEAN_COUNT
         || g_nation_text[g_colonies[w->colony].nation].control != CONTROL_HUMAN)
        && g_scout_visit == 0 && g_debug_mode == 0) {
        buf[0] = 0;
        say_binary(buf, g_colonies[w->colony].ai_flags);
        space_string(buf);
        for (i = 0; i < 4; i++) {
            if (i == 1)
                say_string(buf, g_goods_name_word[g_colonies[w->colony].wanted_goods]);
            else
                say_number(buf, (&g_colonies[w->colony].ai_idle_turns)[i]);
            space_string(buf);
        }
        say_string(buf, g_misc_words[MISC_LOCATION]);
        space_string(buf);
        region = land_region_at(g_colonies[w->colony].x, g_colonies[w->colony].y);
        say_number(buf, region);
        colon_string(buf);
        say_number(buf, g_region_state[g_colonies[w->colony].nation][region]);
    } else {
        STRCPY(buf, g_colonies[w->colony].name);
        period_string(buf);
        space_string(buf);
        say_string(buf, g_season_word[g_game.season]);
        comma_string(buf);
        say_number(buf, g_game.year);
        period_string(buf);
    }
    STRCAT(buf, strings(g_ctitle_word[CTITLE_GOLD]));
    space_string(buf);
    say_my_bucks(g_colonies[w->colony].nation, buf);
    game_win_set_text_1010_e756(w, buf);
}
