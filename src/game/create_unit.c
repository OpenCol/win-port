/* create_unit: candidate for 1038:2246 (748 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: create_unit
 *
 * Creates a unit: enforces the two unit limits, allocates the next slot in the unit array,
 * initialises every field of the record, puts it on the map and reveals what it can see.
 * 38 call sites, the busiest unmatched function in the game's own code.
 *
 * TWO LIMITS, and they are not the same limit:
 *   - 292 (0x124) is the hard ceiling, and it applies only when the request is NOT from a
 *     human-controlled European power -- a native nation (4 and above), a computer power or a
 *     defeated one. So the last eight slots are reserved for the human player.
 *   - 300 (0x12c) is the array's real end, and any European power (0..3, whatever its control) is also capped at 200
 *     (0xc8) units of its own through the per-nation counter at DGROUP:0x4b44.
 * Failing the second limit shows the TOOMANYUNITS text -- but only to a human-controlled European
 * power, so a computer or native nation hitting the ceiling fails silently. Either failure returns -1.
 *
 * THE STRING IS THE PROOF. The shipped call pushes `DS` and the immediate `0x1985`, and
 * DGROUP:0x1985 reads `"TOOMANYUNITS"`. That is the whole reading of the limit branch in
 * one datum, and it is why the two ceilings can be told apart from each other.
 *
 * The record is initialised in the shipped order, which is not tidy and is preserved
 * because the bytes are the evidence: type, nation, +5, +7 = 0x58, +4, orders, cargo
 * count, then amount[6], [0], [1], [2] = 0xff. A NATIVE unit (nation 4 and above) then
 * overwrites amount[2..3] as a 16-bit value from SEG20:0x7790 -- the TURN COUNTER
 * (`GameBrains_DoEndofTurn` increments it once a turn). A native unit is stamped with the turn
 * it appeared, which is not
 * established here; the byte store of 0xff and the word store are at one offset, so the
 * source needs a cast or a union, and the cast is what this file uses.
 *
 * The colony under the unit goes in +6 (0xff for none). Then the unit-type table decides
 * the rest: a type with cargo HOLDS (`g_unit_types[type].holds`, +7 of the 14-byte record,
 * the same field `get_production_cost` reads) gets +0x17 = 0, and a Wagon Train (type 0xc)
 * also clears amount[4]; a type without holds gets +0x17 = 0x1c, or 0x15 for types 7 and
 * 9. Type 2 gets 100 in amount[5]. Both links are -1, the position is set to (0xff, 0xff)
 * and `put_down_unit` moves it to the real square, so the unit is never briefly at a stale
 * one. `explore_square` then reveals what the new unit can see.
 *
 * Unit types 11..17 are Artillery, Wagon Train, Caravel, Merchantman, Galleon, Privateer
 * and Frigate (docs/findings/pedia-tables.md), which fixes 0xc as the Wagon Train and puts
 * the ships in 0xd..0x12 -- the band `explore_area` calls a ship.
 *
 * NAME: the Macintosh build's `create_unit(short, short, short, short)`. Four arguments,
 * and this is the function the whole game calls to make a unit. The address suffix is
 * needed because this build already spells `CheatMenuColonist` at 1048:bbe2 -- but that one is
 * the `WM_COMMAND` handler for the **'Create Unit'** item on the Cheat menu, named after
 * its menu caption, so the collision is between a menu item and a core routine rather than
 * between two readings of the same thing.
 *
 * `show_text_dialog_1038_865b` and `put_down_unit` are defined here as stubs because the
 * shipped calls to them are the compiler's own 4-byte PUSH CS / CALL NEAR, which says they
 * were compiled into this object; `find_colony` and `explore_square` are real 
 * calls and stay `extern`.
 *
 * RENAMED 2026-09-27 from create_unit_1038_2246: String witness: TOOMANYUNITS is in Mac create_unit(short,short,short,short). Call graph: Mac create_unit has 19 callers, and every one appears among 2246's 20 Windows callers: allied_invasion, combat_fight, europe_new_wars, europe_purchase, foreign_country, give_me_liberty, indian_village, king_invasion, lost_city_exploration, negotiate_tribe, new_turn_bookkeeping, new_turn_colony, produce_something, set_job, tory_unrest, try_to_purchase and unit_from_docks. The remaining three are CheatMenuColonist (bbe2), start_new_game (c44a) and start_indians. Mac callees: find_colony, put_down_unit and explore_square. 2246 calls colony_at (becoming find_colony), put_down_unit and explore_square. Arity: 4 shorts against int create_unit_1038_2246(int type, int nation, int x, int y). Size is 706 Mac against 748 Windows. The body is the unit allocator (INC [0x779e], the 28-byte stride). The record already says mac_rename parked this pairing as a false friend only because create_unit was taken by bbe2, and moving bbe2 removes that obstacle. Apply this only after 1048:bbe2 has moved. create_unit is a prefix of create_unit_1038_2246, so rename with exact-name patterns.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "units.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */

extern void put_down_unit(int u, int x, int y);

int create_unit(int type, int nation, int x, int y)
{
    int colony;
    register int u;
    register int ux;

    ux = x;
    u = -1;
    if ((nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) && g_game.unit_count >= 0x124)
        goto done;
    if (g_game.unit_count >= 0x12c || (nation < NATION_EUROPEAN_COUNT && g_nation_unit_count[nation] > 0xc8)) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
            show_text_dialog_1038_865b("TOOMANYUNITS");
        goto done;
    }
    u = g_game.unit_count;
    g_game.unit_count++;
    g_nation_unit_count[nation] = g_nation_unit_count[nation] + 1;
    g_units[u].type = (unsigned char)type;
    g_units[u].flags = (unsigned char)nation;
    g_units[u].moves_used = 0;
    g_units[u].mission = 0x58;
    g_units[u].flags2 = 0;
    g_units[u].orders = ORDERS_NONE;
    g_units[u].cargo_count = 0;
    g_units[u].hold.amount[6] = 0;
    g_units[u].hold.amount[0] = 0;
    g_units[u].hold.amount[1] = 0;
    g_units[u].hold.amount[2] = 0xff;
    if (nation >= NATION_EUROPEAN_COUNT)
        *(short *)&g_units[u].hold.amount[2] = g_game.turn_counter;
    g_units[u].home = 0xff;
    colony = find_colony(ux, y);
    if (colony >= 0)
        g_units[u].home = (unsigned char)colony;
    if (g_unit_types[g_units[u].type].holds > 0) {
        g_units[u].spec = JOB_FARMER;
        if (g_units[u].type == UNIT_WAGON_TRAIN)
            g_units[u].hold.amount[4] = 0;
    } else {
        g_units[u].spec = JOB_COLONIST_ALT;
        if (g_units[u].type == UNIT_CONTINENTAL_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
            g_units[u].spec = JOB_SOLDIER;
    }
    if (g_units[u].type == UNIT_PIONEERS)
        g_units[u].hold.amount[5] = 0x64;
    g_units[u].link = -1;
    g_units[u].link2 = -1;
    g_units[u].x = 0xff;
    g_units[u].y = 0xff;
    put_down_unit(u, ux, y);
    explore_square(u);
done:
    return u;
}
