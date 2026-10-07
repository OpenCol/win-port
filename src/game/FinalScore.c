/* FinalScore: candidate for 1050:7eec (175 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the current nation's name into a local buffer, gathers NINE more values out of the
 * game globals into nine more locals, and then shows the Hall of Fame with the name and a 1.
 *
 * THE NINE ARE THE HALL OF FAME ENTRY, built on the stack. The locals are laid out first-declared
 * highest, so the name buffer is lowest and the nine follow it in memory at exactly the FameEntry
 * offsets: +0x18 nation, +0x1a indep, +0x1c granted, +0x1e year, +0x20 season, +0x22
 * difficulty, +0x24 raw_score (g_score_total), +0x26 rating (what Fame returns), +0x28 rank
 * (Fame's out-parameter, copied). `HallOfFame(name, 1)` receives the address of the buffer --
 * the record's start. (Until 2026-09-27 this header called them nine dead stores.)
 *
 * The locals are declared in the reverse of their frame order, first-declared highest, which
 * puts the buffer LAST despite being written first.
 *
 * `STRCPY`'s source is `0x7810 + 0x34 * nation`, the 0x34-byte record array
 * `get_america_name` also indexes.
 *
 * RENAMED 2026-09-26 from hall_of_fame_entry: the Mac build's FinalScore(void), bound by call-sequence alignment (3.5 aligned gaps in GameBrains_DoEndofTurn, MainWin_MainMenuProc, end_of_game_check leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.17x this one.
 */

#include "colonization.h"

#include "gameflag.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` */
extern int Fame(int show, int *out);
extern void HallOfFame(FameEntry *entry, char show);

void FinalScore(void)
{
    int rank_out;
    FameEntry e;

    STRCPY(e.name, g_nation_text[g_game.player_nation].leader);
    e.nation = g_game.player_nation;
    e.indep = g_game.game_flags & GAMEFLAG_INDEPENDENCE;
    e.granted = g_game.game_flags & GAMEFLAG_REVOLUTION_WON;
    e.year = g_game.year;
    e.season = g_game.season;
    e.difficulty = g_game.difficulty;
    e.rating = Fame(1, &rank_out);
    e.raw_score = g_score_total;
    e.rank = rank_out;
    HallOfFame(&e, 1);
}
