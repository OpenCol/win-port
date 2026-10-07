/* find_city: candidate for 1048:a253 (299 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Find City command. `FINDCITY` asks for a name into the buffer at SEG31:0xa4, the
 * colonies are searched case-insensitively, and the map jumps to the first match; a miss
 * shows `NOCITY` with the name the player typed.
 *
 * A colony only counts if the player may know about it: nation index >= 4 (no player
 * nation at all), or the colony's square is visible to the player in map plane 3, or
 * SEG20:0x77a4 is set -- a reveal-everything flag. An `||` chain, not an if/else ladder;
 * the three arms all assign the same value and Borland compiles the ladder eleven bytes
 * longer, which showed as a `JMP` against a short one at the loop's entry.
 *
 * The loop STOPS at the first match: its condition is `found < 0 && c < g_game.colony_count`,
 * and the `OR DI,DI / JNL` ahead of the count test is the whole tell.
 *
 * `STRICMP` is called with the TYPED name first and the colony's second. The order does not
 * matter to a case-insensitive comparison, so nothing about the function's behaviour says
 * which way round it is -- only the push order does.
 *
 * Two per-module declarations. `g_cur_colony_rec` is `Colony * `: the shipped code
 * reaches the POINTER through an explicit SEG34 immediate. And `g_game.view_nation` at
 * SEG20:0x7798 is an `int` here, compared as a word with a SIGNED `JGE`, where the
 * `unsigned char` five other files declare gives a byte compare and `JAE`. The same
 * per-module split §114 recorded for `g_game.player_nation`.
 *
 * The fourth argument to `popup_ask` is the empty string: it lands at DGROUP:0x2266,
 * which is one past the end of `"FINDCITY"` at 0x225d.
 *
 * RENAMED 2026-09-27 from find_colony: String witness: FINDCITY, NOCITY and GAME are in Mac find_city(void), and the Mac find_colony has COLONYFLAG instead. Call graph: Mac find_city is called by MainWin::MainMenuProc and calls popup_ask, set_colony, stricmp and GameBrains::SetViewMode. a253 is called by MainWin_MainMenuProc (menu &View id 303, 'Find Colony') and calls popup_ask, set_colony, STRICMP and GameBrains_SetViewMode, plus set_center, site_get and ask_text_resource. Arity: (void) on both sides. Size is 300 Mac against 299 Windows. Why it went wrong: the name comes from the menu caption 'Find Colony', and the Mac symbol find_colony(short, short) was taken as corroboration without checking its arity. That record also says the binding was not established. The developers' own resource ids (FINDCITY and NOCITY) use 'city'.
 */

#include "colonization.h"

#include "nations.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern int popup_ask(void *ctx, void *name, int max_len, void *dflt);

extern void set_colony(int colony);
extern unsigned char site_get(int x, int y);
extern void GameBrains_SetViewMode(void *ctx);
extern void set_center(int x, int y, int mode);
extern GameBrains g_game_brains;            /* SEG20:0x40d6, declared */

extern void set_name_slot_1048_de36(int slot, char *s);

void find_city(void)
{
    int found;
    register int colony;

    found = -1;
    if (popup_ask(g_game_txt, "FINDCITY", 0x17, "") == 0) {
        for (colony = 0; found < 0 && colony < g_game.colony_count; colony++) {
            set_colony(colony);
            if (STRICMP(g_name_slots[0], g_cur_colony_rec->name) == 0) {
                if (g_game.view_nation >= NATION_EUROPEAN_COUNT
                    || (site_get(g_cur_colony_rec->x, g_cur_colony_rec->y)
                        & (0x10 << g_game.view_nation))
                    || g_game.reveal_all != 0)
                    found = colony;
            }
        }
        if (found >= 0) {
            GameBrains_SetViewMode(&g_game_brains);
            set_center(g_cur_colony_rec->x, g_cur_colony_rec->y, 0);
        } else {
            set_name_slot_1048_de36(0, g_name_slots[0]);
            ask_text_resource("NOCITY");
        }
    }
}
