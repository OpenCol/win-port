/* EuropeWin_DrawTitle: candidate for 1048:8ded (435 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawTitle
 * name-source: sibling-build
 *
 * Builds the Europe screen's status line and hands it to `win_set_caption_1048_d48b` to draw. It is one
 * 0x50-byte buffer assembled piece by piece: the nation's name, a comma, the ruler's name from
 * the second per-nation table at SEG20:0xd896, a full stop and two spaces, the season word from
 * DGROUP:0x4b34 indexed by SEG20:0x778e, a comma, the year, a full stop, the tax heading, the
 * tax rate with a per-cent sign, two spaces, the gold heading and the treasury.
 *
 * `w->msg_timer != -1` is the guard, and it returns EARLY with its own epilogue -- an actual
 * `return` and not a `goto` to the end (rule 44).
 *
 * The tax rate comes from `Nation+1` through the pointer at DGROUP:0x4cc4, which is the
 * same byte §274's `free_galleon2` doubles to price the King's galleon.
 *
 * `num[0] = 0` is written and never read: `LTOA1` overwrites the buffer whole, and nothing
 * between the clear and the call touches it.
 *
 * RENAMED 2026-09-26 from europe_draw_status: the Mac build's EuropeWin::DrawTitle(void): this only, assembling the nation/ruler/season line and setting it as the window's caption -- the title.
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern char *strings(int n);

extern void comma_string(char *buf);
extern void period_string(char *buf);
extern void percent_string(char *buf);
extern void space_string(char *buf);

extern void say_my_bucks(int nation, char *buf);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" void win_set_caption_1048_d48b(EuropeWin *w, char *s);

void EuropeWin_DrawTitle(EuropeWin *w)
{
    char buf[0x50];
    char num[0x14];

    if (w->msg_timer != -1)
        return;
    set_europe(w->nation);
    buf[0] = 0;
    num[0] = 0;
    STRCPY(buf, strings(g_nation_name_word[g_cur_nation]));
    comma_string(buf);
    STRCAT(buf, strings(g_nation_name[g_cur_nation]));
    period_string(buf);
    STRCAT(buf, strings(g_season_word[g_game.season]));
    comma_string(buf);
    LTOA1(g_game.year, num, 10);
    STRCAT(buf, num);
    period_string(buf);
    say_string(buf, g_ctitle_word[CTITLE_TAX]);
    say_number(buf, g_cur_nation_rec->tax);
    percent_string(buf);
    space_string(buf);
    space_string(buf);
    say_string(buf, g_ctitle_word[CTITLE_GOLD]);
    space_string(buf);
    say_my_bucks(g_cur_nation, buf);
    win_set_caption_1048_d48b(w, buf);
}
