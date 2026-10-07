/* foreign_city: candidate for 1020:969e (348 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * After unit u attacks colony `colony`: scouts run txt_LOSTOURSCOUTS, units whose type-info
 * byte is set run foreign_cargo. If no unit was destroyed, a player at war of independence
 * (bit 0 of SEG20:0x7784) whose European unit attacked a foreign AI colony other than the
 * former mother country (SEG20:0x77d4) triggers NOWARSDURINGREV. The unit's rating is refreshed
 * whenever anything happened.
 *
 * RENAMED 2026-09-26 from foreign_attack_revolution_check: the Mac build's foreign_city(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller move_unit; callee end_unit_turn; callee set_colony), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "gameflag.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_colony(int colony);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int foreign_scout(int u);
extern int foreign_cargo(int u);

extern void end_unit_turn(int u);

int foreign_city(int u, int colony)
{
    int count;
    int r = 0;

    count = g_game.unit_count;
    set_colony(colony);
    if (g_units[u].type == UNIT_SCOUTS)
        r = foreign_scout(u);
    else if (g_unit_types[g_units[u].type].holds != 0)
        r = foreign_cargo(u);
    else
        ;
    if (g_game.unit_count != count)
        return r;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && r == 0
        && (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
        && g_cur_colony_rec->nation != ((unsigned char)g_game.mother_country_idx)
        && (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) < 4
        && g_nation_text[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)].control == CONTROL_HUMAN) {
        show_text_dialog_1020_aa02("NOWARSDURINGREV");
        r = 1;
    }
    if (r != 0)
        end_unit_turn(u);
    return r;
}
