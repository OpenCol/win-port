/* units.h -- unit type ids and the unit record.
 *
 * Source: the game's UNIT0..UNIT23 TEXT resources, whose index is the type byte at Unit+2.
 * The code agrees at its anchors: 0xb is ARTILLERY (colony_build_complete), 0xd..0x12 are the
 * ships (dock_draw_unit, recruit_unit). tools/check_headers.py re-reads the
 * resources.
 *
 * UNIT23 is a second, lower-case "Colonists" entry; nothing here shows the code uses 23 as a
 * type, so it is not named.
 */
#ifndef UNITS_H
#define UNITS_H

#define UNIT_COLONISTS              0
#define UNIT_SOLDIERS               1
#define UNIT_PIONEERS               2
#define UNIT_MISSIONARIES           3
#define UNIT_DRAGOONS               4
#define UNIT_SCOUTS                 5
#define UNIT_REGULARS               6
#define UNIT_CONTINENTAL_CAVALRY    7
#define UNIT_CAVALRY                8
#define UNIT_CONTINENTAL_ARMY       9
#define UNIT_TREASURE_TRAIN         10
#define UNIT_ARTILLERY              11
#define UNIT_WAGON_TRAIN            12
#define UNIT_CARAVEL                13
#define UNIT_MERCHANTMAN            14
#define UNIT_GALLEON                15
#define UNIT_PRIVATEER              16
#define UNIT_FRIGATE                17
#define UNIT_MAN_O_WAR              18
#define UNIT_BRAVES                 19
#define UNIT_ARMED_BRAVES           20
#define UNIT_MOUNTED_BRAVES         21
#define UNIT_MOUNTED_WARRIORS       22

#define UNIT_SHIP_FIRST             UNIT_CARAVEL
#define UNIT_SHIP_LAST              UNIT_MAN_O_WAR

/* ---- the orders byte, Unit+8 ---------------------------------------------------------------
 * 0, 1 and 5 are the three the UNITOPTIONS menu sets ("Clear orders.", "Sentry / Board ship.",
 * "Fortify."; unit_orders_menu, colony_click_ship). The others are named from the one function
 * that sets each.
 */
#define ORDERS_NONE                 0
#define ORDERS_SENTRY               1       /* the menu's "Sentry / Board ship" */
#define ORDERS_TRADE_ROUTE          2       /* begin_trade_route sets it, route_delete clears it */
#define ORDERS_FORTIFY              5       /* fortify_unit */
#define ORDERS_FORTIFIED            6       /* unit_set_orders_6; defence_bonus tests it */
#define ORDERS_BUILD_COLONY         7       /* build_colony sets it before mission_found_colony; TOONEARBUILD tests it */
#define ORDERS_PLOW                 8       /* plow_or_clear_tile */
#define ORDERS_BUILD_ROAD           9       /* build_road */
#define ORDERS_MISSION              11      /* unit_set_mission */

/* ORDERS 3, 10 AND 12 OCCUR AND ARE NOT NAMED. Each is now understood well enough to say what
 * it does, and none is understood well enough to name, so the evidence is recorded instead.
 *
 * 11 IS NAMED and the other three are not, for one reason: `unit_set_mission` writes
 * it, stores a mission code in Unit+7 and a destination in +9/+10, and
 * `unit_run_mission` guards its trespass charge on "a unit already ON a mission".
 * A setter whose whole body is that, plus a reader that says the same, is the bar the other
 * orders here do not clear.
 *
 *   3   `arrived_unit_mission` sets it on the unit at SEG20:0x7794 and stores a
 *       destination in +9/+10, like a mission but with no mission code. The dispatcher has no
 *       case for it, so whatever steps a unit with these orders is not unit_run_mission.
 *  10   dispatched to `mission_goto_step`, which takes one step toward the destination
 *       at +9/+10 and clears the orders on arrival. NOTHING IN THE CORPUS EVER WRITES 10 -- the
 *       setter is in a function nobody has reconstructed -- so "goto" would rest entirely on the
 *       name someone already gave the worker, which is not evidence.
 *  12   `ai_unit_turn` sets it with a target in +9/+10 when it picks a direction.
 *       `mission_goto_step` sets DGROUP:0x10a4 to -1 for it, which is what makes that order's
 *       move invisible, and the arrival tidy-up clears every order except 2 and 12. Reading it
 *       as "the AI is moving this unit" fits everything seen and is still a guess.
 */

/* AN OPEN QUESTION ABOUT 7, 8 AND 9, recorded because it looks like a contradiction and is not
 * settled. These three names rest on the commands that WRITE the byte -- `plow_or_clear_tile`
 * does `orders = 8`, `build_road` does `orders = 9`, `build_colony` sets 7 -- which is the strongest
 * evidence available and is why they stand.
 *
 * But `unit_run_mission` dispatches the same byte to workers whose names do not line
 * up: 7 goes to `clear_forest`, 8 to `mission_work_step` and 9 to `mission_found_colony`. Read
 * naively that would put founding a colony on 9 and plowing on 7, which is the rotation of these
 * three names, and it is wrong -- the setters are unambiguous.
 *
 * The likelier reading is that a WORKER is misnamed rather than an order. `clear_forest` is
 * documented as "a pioneer's turn of clearing OR PLOWING", which fits order 7 only if founding a
 * colony clears its site first; and `mission_found_colony` on order 9 would have to be doing road
 * work. Nobody has read those two functions against their callers. Until someone does, the orders
 * keep the names their setters give them and this note keeps the discrepancy visible. */

/* ---- the 14-byte UNIT TYPE record, SEG20:0x7ac8, one per type ---------------------------------
 * NOT the unit. This is the per-TYPE table `g_unit_types[]`, and `load_data_tables` fills it by
 * parsing the UNITS resource (TEXT 685), a CSV whose columns are
 *
 *     name, icon, moves, attack, defence, holds, ...
 *
 * The resource is the ground truth and it can be read: Artillery is `10, 1, 7, 5, 0, ...` and
 * Man-O-War `128, 5, 24, 24, 6, ...`, which are the game's own 1 move / attack 7 / defence 5 and
 * 5 / 24 / 24. The loader does NOT read the columns in order, which is what made these hard:
 * it stores column 2 into +4, column 3 into +6 and column 4 into +5, so +5 is DEFENCE and +6
 * is ATTACK even though +5 is filled last.
 *
 *   +0   name      the text id
 *   +2   icon
 *   +4   moves     IN THIRDS: the loader multiplies the column by 3. `unit_move_points` adds
 *                  another 3 for a ship whose nation has FATHER_FERDINAND_MAGELLAN, and
 *                  Magellan's documented effect is +1 naval movement. That is what pins the
 *                  scale, and what showed the old name `rating` was a movement allowance.
 *   +5   defence
 *   +6   attack
 *   +7   holds     cargo slots; 0 for Artillery, 2 for a Caravel, 6 for a Man-O-War
 *   +8   size      HOW MANY HOLDS THIS UNIT TAKES UP when something carries it -- the mirror of
 *                  +7, not another word for it. `europe_voyage_click` is the pair in one place:
 *                  `space = holds - cargo_count;` then `if (size <= space) space -= size;`, and
 *                  `move_reason` passes it to `stack_cargo_room` as the room to find. 99 means
 *                  "cannot be carried" and is the value every ship and the wagon train have,
 *                  which is how `move_reason` tells a land unit apart (`< 99`). The resource
 *                  confirms it from the other side: Treasure is 6 here, and needing six holds is
 *                  why a treasure train wants a galleon. Two files called this `cargo`, the same
 *                  word four files used for +7, so one name meant both what a ship can carry and
 *                  what a passenger costs.
 *   +9..+0xd  four more columns and a bit string. Not named: nothing reads them in the corpus.
 *
 * DEFENCE IS ALSO THE REPAIR TIME. A damaged ship's `counter` (Unit+0x16) rises by one a turn,
 * twice that in a colony, and `nation_turn` clears the damaged flag when `defence - counter <= 0`
 * while `draw_unit_icon` draws the same difference on the icon. Two files had read that byte as a
 * refit timer and as movement; it is the defence value doing double duty.
 */

/* ---- Unit+0x16 IS ONE COUNTER DOING FIVE JOBS -------------------------------------------------
 * Whatever the unit is currently waiting out, it waits it out here, and five files each named the
 * byte after their own job -- `work`, `sail_turns`, `idle`, `restless`, `counter`. None of them is
 * wrong and none covers the others, so the name is the general one:
 *
 *   a pioneer      turns of work left on a clearing, a road or a plow (`clear_forest`,
 *                  `mission_work_step`, `mission_found_colony`)
 *   a ship at sea  turns until it reaches Europe (`set_sail_for_europe`, `sail_countdown`)
 *   a damaged ship the repair timer, which is why the unit type's DEFENCE doubles as repair time:
 *                  `nation_turn` clears the damaged flag when `defence - counter <= 0` and
 *                  `draw_unit_icon` draws that same difference on the icon
 *   a convert      turns before faith lapses (`converts_lose_faith`)
 *   a native       turns idle before `tribe_joins_tory_side` acts
 *
 * The same shape as Unit+0x17 below, and the same remedy: one covering name, with the readings
 * written down here rather than argued over in fifteen typedefs.
 */

/* ---- Unit+0x17 IS TWO THINGS, depending on what the unit is -----------------------------------
 * 51 files call it `spec` and 28 call it `route`, and both are right.
 *
 *   a colonist   the SPECIALTY, a whole byte, since jobs run 0..27 and JOB_COLONIST_ALT is 0x1c.
 *                jobs.h documents it; `visit_tribe_village` stores JOB_CONVERT here and
 *                `resolve_attack` turns a dead missionary into one.
 *   a treasure   the VALUE IN HUNDREDS. `sell_treasure_to_agents` reads the byte and multiplies
 *                by 100, so one byte covers a treasure up to 25,500 gold.
 *   a carrier    the TRADE ROUTE, split into nibbles by four dedicated accessors:
 *                `unit_route_get` reads `& 0xf`, `unit_route_flag_get` reads `>> 4`, and the two
 *                setters write each half back. A route index cannot be a specialty, because a
 *                nibble cannot hold 27.
 *
 * No unit has two of these. `create_unit` gives a specialty to land units with NO cargo holds,
 * which is exactly the set that has neither a trade route nor treasure to carry. The byte is
 * reused, which is what a 28-byte record does when it runs out of room.
 *
 * `PediaWin_DoUnitArticle` called it `icon` and that was simply wrong: it stores JOB_COLONIST
 * and JOB_COLONIST_ALT, which is the portrait fold jobs.h already describes.
 *
 * tools/check_field_names.py knows this one is deliberate; see KNOWN_TWO_MEANINGS there.
 */

/* ---- the mission byte, Unit+7 ---------------------------------------------------------------
 * A SECOND STANDING-ORDER BYTE, and the reason this note is here is that it was easy to confuse
 * with the one above. `unit_set_mission(u, m, x, y)` writes the mission code here,
 * ORDERS_MISSION into +8 and a destination into +9/+10; the AI passes codes 0x21..0x57 and
 * `unit_run_mission` reads +7 against 0x45 while switching on +8.
 *
 * The codes are NOT named. Roughly thirty distinct values reach this byte from `ai_unit_turn`
 * alone and nothing in the game's data lists them, so there is no table to check a name against.
 *
 * ONE FILE HAD THESE TWO BYTES ONE APART. `ai_survey_and_assign` called +7 `orders` and +8
 * `state`, which is why `.orders == 0x41` appeared in a corpus whose orders run 0..12. It was
 * the only file of the eighteen that named +7 `orders`, and the value ranges settle which
 * reading is right. Nothing was mis-swept: every value it holds there is outside the ORDERS_*
 * map, so the map refused them all -- luck, and the reason the field names are now consistent.
 */

/* A unit whose x is UNIT_EUROPE_X + its nation is in that nation's Europe (crosses_required). */
#define UNIT_EUROPE_X               0xec

/* ---- the flags byte, Unit+3 ----------------------------------------------------------------
 * The low nibble is the owning nation. 111 sites mask it out and compare the result against a
 * nation: `(g_units[u].flags & 0xf) == g_player_nation_byte` and `!= (unsigned char)nation` are
 * the two shapes, and `compose_unit_icon` indexes a per-nation table with it. Four bits hold all
 * twelve nations, which tools/check_headers.py checks against nations.h rather than trusting.
 *
 * THE HIGH NIBBLE IS WHO HAS SPOTTED THE UNIT, one bit per European nation, laid out exactly like
 * the map's plane 3 (terrain.h's MAP3_NATION_BIT). It reads that way in every site:
 *   - `draw_map_overview` and `draw_active_unit_cell` both draw a unit only
 *     `if (((0x10 << viewer) & g_units[u].flags) || g_reveal_all != 0)` -- the viewer's bit OR
 *     fog-of-war off. Nothing else in the record could make reveal-all the alternative.
 *   - `draw_units_in_rect`, `draw_info_window` and `unit_move_to_square` gate the same way on
 *     `g_view_nation`, and `fort_opens_fire` will not fire at a unit the viewer cannot see.
 *   - `unit_nation_flag_set(u, n)` is the setter and does nothing but `|= 0x10 << n`. Its callers
 *     are the moments a nation learns a unit exists: `colony_tile_flags` sets the colony owner's
 *     bit on every unit in the colony's tiles, and `intervention_force_arrives` and
 *     `royal_expedition_lands` set it on the units they create.
 * So one byte carries the owner and the observers, which is why neither `nation` nor `flags`
 * alone was a good name for it and why eleven files disagreed with 191 about which it was.
 */
#define UNIT_NATION_MASK            0x0f
/* The SAME bit as terrain.h's MAP3_NATION_BIT, spelled the way the unit sites spell it. The two
 * forms are kept apart on purpose: `0x10 << n` and `(1 << n) << 4` are different expressions and
 * need not compile to the same instructions, and every macro here has to reproduce the bytes of
 * the site it replaces. Use this one at unit-flag sites and MAP3_NATION_BIT on the map planes. */
#define UNIT_SEEN_BY(n)             (0x10 << (n))

#define UNIT_RECORD_SIZE            0x1c    /* the unit array at SEG20:0xa208 */
#define UNIT_NONE                   (-1)

#endif
