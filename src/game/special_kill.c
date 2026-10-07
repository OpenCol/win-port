/* special_kill: candidate for 1020:23f5 (4197 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: special_kill
 * name-source: sibling-build
 * size-source: 4197, not the census 4186. The census cuts the shared epilogue in half; the last
 * nine bytes after it are an unreachable SECOND epilogue and `1020:345d` starts right after them.
 *
 * WHAT HAPPENS TO THE UNIT THAT LOST A BATTLE. Every combat in the game ends here: the caller
 * passes the loser, the winner (or -1 when there is none -- a storm or a bombardment), whether to
 * show messages, and the square the fight was over. The routine returns 1 when the loser was
 * destroyed and the record freed, 0 when it survived in some other shape.
 *
 * FIVE OUTCOMES, tried in this order.
 *
 * CAPTURED (LOOTCAPTURE, WAGONCAPTURE, COLONISTCAPTURE, COLONISTCAPTURE2). A treasure train, a
 * colonist or a wagon train changes hands instead of dying: it is unlinked, given the winner's
 * nation and placed on the winner's square with its orders cleared. Capture needs the winner's
 * type to be capture-capable (+6 of its 14-byte record), the winner to be a European nation, and
 * the winner not to be a ship and the fight not to be at sea. The GALLEON cargo-room test that
 * follows (`type.holds - unit.cargo_count >= loser_type.size`) never decides anything: it needs `atsea`. A captured colonist whose speciality is 0x15 becomes
 * 0x1c and gets the second message. The SENTRY for a capture at sea (line after the messages) is unreachable for the same reason.
 *
 * DEMOTED (DEMOTE). On land, against a land winner, the ladder is dragoons->soldiers,
 * soldiers->colonists, continentals->colonists, continental cavalry->continental army and
 * cavalry->regulars. A unit demoted to a colonist whose speciality is 0x18 becomes a missionary
 * instead. ARTILLERY (ARTILLERY, ARTILLERY2) is not on the ladder: it takes the damage bit 0x80
 * the first time and is destroyed the second.
 *
 * BOARDED (CARGOCAPTURE). Ship against ship, the winner takes cargo while it has room:
 * `capture_cargo` picks a slot, the goods move across, and each load is announced with the amount,
 * the two nations and the goods name.
 *
 * DAMAGED (SHIPDAMAGE) or SUNK (SHIPSUNK). A losing ship rolls `rand_range(1, winner.bb +
 * loser.bc)` and is damaged when the roll falls in the loser's half. That verdict is then
 * overridden several times, and the two arms differ: an ARMED ship type (attack at +6 set)
 * looks at how many the nation already has against its colony count; an unarmed one looks at
 * a budget -- `g_nation_cargo_capacity` less the frigates it owns times SEG20:0x7bbd, less the privateers, less the
 * ship's own holds -- and is damaged when that falls under `minimax(g_nation_population[nation] >> 2, 3, 6)`.
 * A caravel from turn 0x50 on is not damaged unless the budget test just after re-damages it, a man-o'-war of the rebelling nation with fewer than
 * two left always is, and a loss with no winner always is. A damaged ship is sent to the nearest
 * colony of its owner with the harbour flag (bit 7) -- nearest by `map_dist`, ignoring
 * distance 0 -- or, with no such colony, to its own Europe square at `nation - 0x14`. Repair turns
 * are `type.defence` less the winner's `f5` (doubled when a land unit did the damage), with a floor of
 * four for a frigate and eight for a man-o'-war.
 *
 * THE KING'S ARMY. A sunk unit carrying bit 0x40 -- one of the ships of the royal expeditionary
 * force -- lowers the nation's TAX RATE at nation+1 by the tax_cut of the matching entry of the
 * six-record table at SEG20:0xd83d, at most the whole rate.
 *
 * TWO SHIPPED BUGS in that last block. The search loop leaves `i` one PAST the record it found,
 * and it is `i`, not the found index, that is used to read the cap -- so the wrong record is read
 * every time, and the record for the last type in the table reads off the end. And the five text
 * arguments the block fills in (the difficulty word, the king's name, the ship type, the tax
 * cut and the new tax rate) are never used: no message is shown before the unit is freed.
 *
 * WHAT THE BYTES SETTLED: the speciality byte at +0x17 is SIGNED and its two tests are written
 * `(int)u.specialty == 0x15` -- without the cast Borland folds the comparison into one byte
 * (`80 bf 17 00 15`); with it the shipped `8a 87 .. 98 3d 15 00` comes out. The same explicit
 * `(int)` is needed on `g_nation_armed_ships[nation] > 8`. The three mid-function exits are `goto done`, not
 * `return ret`: Borland inlines an epilogue at every `return` and only a jump reaches the shared
 * one. The colony search uses `continue` for its first two filters (short JCC over a JMP) and a
 * structured `if` for the last two (rule 12). And `i` must NOT be declared `register`: an explicit
 * `register` on it takes SI away from the `loser` parameter, which the shipped keeps there.
 *
 * `g_unit_types[UNIT_FRIGATE].holds` IS A LEAD, NOT A FINDING. Nothing in the image stores to 0x7bbd and this multiply is
 * its only read, which would make the term constant -- except that the neighbour test fails:
 * there is no absolute store anywhere within 0x20 either side, which is what a region filled by
 * one block load looks like, and the write would then go through a base register where the scan
 * cannot see it. It is also declared SEG20, not DGROUP, so it is outside what
 * `tools/check_dead_dgroup.py` can speak to. Finding the loader would settle it.
 *
 * RENAMED 2026-09-26 from resolve_combat_loss: the Mac build's special_kill(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller combat_fight; caller resolve_raid; callee capture_cargo; callee minimax; callee on_map), the runner-up gets 5, and the arity agrees (5).
 *
 * A LIKELY SHIPPED SLIP in the lent-frigate refund: the search loop's `i++` runs once more after
 * the match, so the tax cut is read from g_recruit[i] = the NEXT record (the table setup writes
 * 2,2,4,6,3,8), past the table's end for the last entry; and `idx > 0` skips record 0. The bytes
 * fix both; `g_recruit[idx]` and `idx >= 0` read like what was meant.
 */

#include "colonization.h"

#include "control.h"
#include "jobs.h"
#include "gameflag.h"
#include "units.h"
#include "nations.h"
#include "sounds.h"

#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
                                  /* 6 bytes at SEG20:0xd83c -- the base `setup_purchase`
                                     writes, one byte BEFORE the 0xd83d the readers used */

extern Nation g_nations[];               /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int terrain_is_ocean(int x, int y);

extern void pick_up_unit(int u);
extern void set_owner(int u, int owner);
extern void put_down_unit(int u, int x, int y);

extern void popup_say(int slot, int word);
extern int popm(char *name, int n);
extern void sound_play(int n);
extern int unload_cargo(int u, int slot);
extern int stow_cargo(int u, int goods, int amount);
extern int cargo_check(int u, int need, int last_ship);

extern int colony_has_building(int colony, int bit);
extern int map_dist(int x0, int y0, int x1, int y1);
extern void new_mood(int mood);
extern void delete_unit(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void set_text_arg_char_1020_a973(int slot, char v);

int special_kill(register int loser, int winner, int show, int x, int y)
{
    int ret = 0;
    int wnat;
    int lnat;
    int slot;
    int goods;
    int best;
    int word;
    int dist;
    int bestdist;
    int cx;
    int cy;
    int wx;
    int wy;
    int damaged;
    int str;
    int guns;
    int defense;
    int t;
    int roll;
    int naval = 0;
    int atsea = 0;
    int cancap = 1;
    int capturable;
    int idx;
    int tax_cut;
    int dmg;
    int wtype;
    int demote;
    int i;

    if (loser >= 0x12c)
        return 0;
    lnat = NATION(g_units[loser]);
    if (winner < 0) {
        wtype = UNIT_PRIVATEER;
        goto ship;
    }
    wnat = NATION(g_units[winner]);
    wtype = g_units[winner].type;
    capturable = (g_units[loser].type == UNIT_TREASURE_TRAIN ||
                  g_units[loser].type == UNIT_COLONISTS ||
                  g_units[loser].type == UNIT_WAGON_TRAIN);
    wx = g_units[winner].x;
    wy = g_units[winner].y;
    if (on_map(wx, wy))
        atsea |= terrain_is_ocean(wx, wy);
    if (on_map(x, y))
        atsea |= terrain_is_ocean(g_units[loser].x, g_units[loser].y);
    if (lnat >= NATION_EUROPEAN_COUNT) {
        if ((g_units[loser].flags2 & 0x10) != 0 || rand_range(0, 1) != 0) {
            if (g_units[loser].type == UNIT_ARMED_BRAVES ||
                g_units[loser].type == UNIT_MOUNTED_WARRIORS)
                g_tribes[lnat - 4].muskets++;
            if (g_units[loser].type == UNIT_MOUNTED_BRAVES ||
                g_units[loser].type == UNIT_MOUNTED_WARRIORS)
                g_tribes[lnat - 4].horses += 0x19;
        }
    }
    if (g_unit_types[wtype].attack == 0)
        capturable = 0;
    if ((g_units[winner].type >= UNIT_SHIP_FIRST && g_units[winner].type <= UNIT_SHIP_LAST) ||
        atsea != 0)
        capturable = 0;
    if (capturable != 0 && on_map(wx, wy) && atsea != 0) {
        if (wtype != UNIT_GALLEON)
            cancap = 0;
        else if (g_unit_types[g_units[winner].type].holds - g_units[winner].cargo_count <
                 g_unit_types[g_units[loser].type].size)
            cancap = 0;
    }
    if (capturable != 0 && wnat < NATION_EUROPEAN_COUNT && cancap != 0) {
        pick_up_unit(loser);
        set_owner(loser, wnat);
        put_down_unit(loser, wx, wy);
        g_units[loser].orders = ORDERS_NONE;
        if (show) {
            popup_say(0, get_adjective(lnat));
            popup_say(1, get_adjective(wnat));
            set_text_arg_i_1020_a926(0, (unsigned char)g_units[loser].spec * 100);
            switch (g_units[loser].type) {
            case UNIT_TREASURE_TRAIN:
                popm("LOOTCAPTURE", 1);
                break;
            case UNIT_WAGON_TRAIN:
                popm("WAGONCAPTURE", 1);
                break;
            case UNIT_COLONISTS:
                if ((int)g_units[loser].spec == JOB_SOLDIER) {
                    g_units[loser].spec = JOB_COLONIST_ALT;
                    popm("COLONISTCAPTURE2", 1);
                } else
                    popm("COLONISTCAPTURE", 1);
                break;
            }
        }
        if (atsea != 0)
            g_units[loser].orders = ORDERS_SENTRY;
        return 0;
    }
    if ((g_units[winner].type < UNIT_SHIP_FIRST || g_units[winner].type > UNIT_SHIP_LAST) &&
        atsea == 0) {
        demote = -1;
        if (g_units[loser].type == UNIT_DRAGOONS)
            demote = UNIT_SOLDIERS;
        if (g_units[loser].type == UNIT_SOLDIERS)
            demote = UNIT_COLONISTS;
        if (g_units[loser].type == UNIT_CONTINENTAL_ARMY)
            demote = UNIT_COLONISTS;
        if (g_units[loser].type == UNIT_CONTINENTAL_CAVALRY)
            demote = UNIT_CONTINENTAL_ARMY;
        if (g_units[loser].type == UNIT_CAVALRY)
            demote = UNIT_REGULARS;
        if (demote >= 0) {
            if (demote == 0 && (int)g_units[loser].spec == JOB_MISSIONARY)
                demote = UNIT_MISSIONARIES;
            if (show) {
                popup_say(0, get_adjective(lnat));
                popup_say(1, g_unit_types[g_units[loser].type].name);
                popup_say(2, g_unit_types[demote].name);
                popm("DEMOTE", 1);
            }
            g_units[loser].type = demote;
            goto done;
        }
        if (g_units[loser].type == UNIT_ARTILLERY) {
            if (!(g_units[loser].flags2 & 0x80)) {
                if (show) {
                    popup_say(0, get_adjective(lnat));
                    popup_say(1, g_unit_types[g_units[loser].type].name);
                    popm("ARTILLERY", 1);
                }
                g_units[loser].flags2 |= 0x80;
                goto done;
            } else if (show) {
                popup_say(0, get_adjective(lnat));
                popup_say(1, g_unit_types[g_units[loser].type].name);
                popm("ARTILLERY2", 1);
            }
        }
    }
    if (g_units[loser].type >= UNIT_SHIP_FIRST && g_units[loser].type <= UNIT_SHIP_LAST &&
        g_units[winner].type >= UNIT_SHIP_FIRST && g_units[winner].type <= UNIT_SHIP_LAST) {
        if (show)
            sound_play(SOUND_BOARD);
        while (cargo_check(winner, 1, 1) != 0) {
            slot = capture_cargo(loser, winner);
            if (slot < 0)
                break;
            goods = unload_cargo(loser, slot);
            stow_cargo(winner, goods, g_cargo_amount);
            if (show && g_cargo_captured != 0) {
                set_text_arg_i_1020_a926(0, g_cargo_amount);
                popup_say(0, get_adjective(lnat));
                popup_say(1, g_goods_name_word[goods]);
                popup_say(2, get_adjective(wnat));
                popup_say(3, g_unit_types[wtype].name);
                popm("CARGOCAPTURE", 0);
            }
        }
        g_units[loser].cargo_count = 0;
    }
ship:
    damaged = 0;
    if (g_units[loser].type >= UNIT_SHIP_FIRST && g_units[loser].type <= UNIT_SHIP_LAST) {
        damaged = 1;
        naval = 1;
        popup_say(0, get_adjective(lnat));
        popup_say(1, g_unit_types[g_units[loser].type].name);
        if (g_unit_types[wtype].guns != 0) {
            guns = g_unit_types[wtype].guns;
            defense = g_unit_types[g_units[loser].type].hull;
            t = guns + defense;
            roll = rand_range(1, t);
            if (roll > defense)
                damaged = 0;
            if (g_unit_types[g_units[loser].type].attack != 0) {
                if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                    str = g_unit_tally[lnat][g_units[loser].type];
                    if (g_nation_colony_count[lnat] < str || (int)g_nation_armed_ships[lnat] > 8)
                        damaged = 0;
                    if (g_units[loser].type == UNIT_FRIGATE && wtype == UNIT_FRIGATE &&
                        g_unit_tally[wnat][UNIT_FRIGATE] < (unsigned char)str)
                        damaged = 0;
                    if (str < 2 && g_nation_colony_count[lnat] != 0)
                        damaged = 1;
                }
            } else {
                str = g_nation_cargo_capacity[lnat] - g_unit_tally[lnat][UNIT_FRIGATE] * g_unit_types[UNIT_FRIGATE].holds -
                      g_unit_tally[lnat][UNIT_PRIVATEER];
                str -= g_unit_types[g_units[loser].type].holds;
                if (g_nation_ship_count[lnat] - g_nation_armed_ships[lnat] > 8)
                    damaged = 0;
                if (g_units[loser].type == UNIT_CARAVEL && g_game.turn_counter >= 0x50)
                    damaged = 0;
                t = g_nation_population[lnat] >> 2;
                t = minimax(t, 3, 6);
                if (str < t)
                    damaged = 1;
            }
            if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0 && g_game.mother_country_idx == lnat &&
                g_units[loser].type == UNIT_MAN_O_WAR && g_unit_tally[lnat][UNIT_MAN_O_WAR] < 2)
                damaged = 1;
            if (winner < 0)
                damaged = 1;
            if (damaged == 0) {
                g_nation_cargo_capacity[lnat] -= g_unit_types[g_units[loser].type].holds;
                g_unit_tally[lnat][g_units[loser].type]--;
                if (g_unit_types[g_units[loser].type].attack != 0)
                    g_nation_armed_ships[lnat]--;
            }
        }
    }
    if (damaged != 0) {
        best = 0x3e7;
        bestdist = 0x270f;
        for (i = 0; i < g_game.colony_count; i++) {
            if (g_colonies[i].nation != (unsigned char)lnat)
                continue;
            if (colony_has_building(i, 7) == 0)
                continue;
            dist = map_dist(x, y, g_colonies[i].x, g_colonies[i].y);
            if (dist <= bestdist && dist != 0) {
                bestdist = dist;
                best = i;
                cx = g_colonies[i].x;
                cy = g_colonies[i].y;
            }
        }
        if (best == 0x3e7) {
            cx = lnat - 0x14;
            cy = lnat - 0x14;
            if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0 && g_game.player_nation == lnat) {
                damaged = 0;
                goto out;
            }
            word = g_nation_name_word[lnat];
            if (g_game.mother_country_idx == lnat)
                word = g_nation_name_word[g_game.player_nation];
            popup_say(2, word);
        } else
            set_name_slot_1020_a94a(2, g_colonies[best].name);
        g_units[loser].flags2 |= 0x80;
        g_units[loser].counter = 0;
        g_units[loser].cargo_count = 0;
        g_units[loser].orders = ORDERS_NONE;
        g_units[loser].dest_x = g_nations[lnat].start_x;
        g_units[loser].dest_y = g_nations[lnat].start_y;
        dmg = g_unit_types[wtype].defence;
        if (g_units[winner].type < UNIT_SHIP_FIRST || g_units[winner].type > UNIT_SHIP_LAST)
            dmg = dmg * 2;
        if (g_unit_types[g_units[loser].type].defence > dmg)
            g_units[loser].counter = g_unit_types[g_units[loser].type].defence - dmg;
        if (g_units[loser].type == UNIT_FRIGATE)
            g_units[loser].counter = max_int_1020_a887(4, g_units[loser].counter);
        if (g_units[loser].type == UNIT_MAN_O_WAR)
            g_units[loser].counter = max_int_1020_a887(8, g_units[loser].counter);
        pick_up_unit(loser);
        put_down_unit(loser, cx, cy);
        if (show)
            popm("SHIPDAMAGE", 0);
    }
out:
    if (damaged == 0) {
        if (naval != 0) {
            popup_say(2, get_adjective(wnat));
            popup_say(3, g_unit_types[wtype].name);
            if (show) {
                sound_play(SOUND_SHIPSINK);
                if (lnat < NATION_EUROPEAN_COUNT && g_nation_text[lnat].control == CONTROL_HUMAN)
                    new_mood(1);
                if (wnat < NATION_EUROPEAN_COUNT && g_nation_text[wnat].control == CONTROL_HUMAN)
                    new_mood(4);
                popm("SHIPSUNK", 0);
            }
        }
        if ((g_units[loser].flags2 & 0x40) != 0) {
            idx = -1;
            for (i = 0; idx < 0 && i < 6; i++)
                if (g_units[loser].type == g_recruit[i].type)
                    idx = i;
            if (idx > 0) {
                tax_cut = *min_char_ptr_1020_b2f3((char *)&g_nations[lnat].tax,
                                            &g_recruit[i].tax_cut);
                if (tax_cut > 0) {
                    g_nations[lnat].tax = g_nations[lnat].tax - tax_cut;
                    if (lnat < NATION_EUROPEAN_COUNT && g_nation_text[lnat].control == CONTROL_HUMAN) {
                        popup_say(0, g_difficulty_words[g_game.difficulty]);
                        set_name_slot_1020_a94a(1, g_nation_text[lnat].leader);
                        popup_say(2, g_unit_types[g_units[loser].type].name);
                        set_text_arg_i_1020_a926(0, tax_cut);
                        set_text_arg_char_1020_a973(1, g_nations[lnat].tax);
                    }
                }
            }
        }
        delete_unit(loser);
        ret = 1;
    }
done:
    return ret;
}
