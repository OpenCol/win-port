/* building.h -- the colony building ids, from the game's own BUILDING resources.
 *
 * Source: BUILDING0..BUILDING41, whose index IS the id the code passes. Each resource opens with
 * the building's name in `^{...}^`, and that name is what these macros spell -- so the table is
 * re-derived from the resources by tools/check_headers.py rather than transcribed by hand, which
 * is the bar every table here has to meet (a wrong value cannot be caught by a byte match).
 *
 * WHO USES THEM. `building_available(b)` and `current_colony_building_count(b)` are
 * called with bare ids throughout -- 0x25, 0x14, 0x10, 0xc -- and those are what these name.
 *
 * TWO CAUTIONS, both from the resources themselves:
 *   - 9 and 10 are both called TOWN HALL; 10 is written `^^{TOWN HALL}^` where 9 is `^{TOWN
 *     HALL}^`. They are distinct ids and the second takes a _2 suffix, the same convention
 *     terrain.h uses for its repeated forest block. What tells them apart is NOT established.
 *   - BUILDING10, 11, 30 and 31 carry a name and no description, so nothing here says what they
 *     do -- only what the game calls them.
 */
/* ---- THE 12-BYTE RECORD, SEG20:0xd422, one per id -------------------------------------------
 * The data segment ships as zeros. `load_data_tables` fills six of the fields from the BUILDING
 * resource, whose columns are `name, hammers, tools, kind, min_pop, f9`:
 *
 *     Stockade,   64,  0, 3, 3,  0
 *     Fortress,  320, 20, 3, 8, 15
 *
 * and `building_table_init` fills the other three by hand, one call per building.
 *
 *   +0   name        text id
 *   +2   f2          `building_table_init` passes -1 for this on all 42 buildings, and
 *                    `can_build_item` reads it into a variable it then overwrites. A second
 *                    prerequisite slot that the shipped data never uses.
 *   +3   prev_level  the level BELOW in this building's family, -1 at the head. It is the
 *                    prerequisite: `can_build_item` refuses unless the colony already has it.
 *   +4   next_level  the level ABOVE, -1 at the top. `building_levels_built` counts with it.
 *   +5   kind        BUILDING column 3. `building_table_set_fields` copies it into the
 *                    by-family table in segment 21 keyed on +6. Not established further.
 *   +6   family      0..14, the family index: `building_table_init` counts `++i` at each head,
 *                    so the 42 buildings fall into 15 families.
 *   +7   tools       tools to build
 *   +8   min_pop     colonists required -- 8 for a Fortress, which is the game's own rule
 *   +9   f9          BUILDING column 5: 0/5/10/15, rising with the level. Not established.
 *   +0xa hammers     hammers to build, an int because a Fortress costs 320
 *
 * WHICH WAY THE CHAIN RUNS was worth getting right: six files called +3 `next` and six called
 * +4 `slot`, and both were backwards. The init writes the Stockade family as `(0,-1,-1,1,++i)`,
 * `(1,0,-1,2,i)`, `(2,1,-1,-1,i)`, which settles it -- and `building_family_base`,
 * filed as `building_top_level`, follows +3 and therefore returns the HEAD.
 *
 * THE TOWN HALLS, which the caution above says nothing distinguishes: the init does. Id 9 is a
 * family head with no level above it, while 10, 11, 30 and 31 form a chain hanging off it
 * (10's prev is 9). So 9 is the one a colony is founded with and 10 is the first that can be
 * built on -- which is as much as the table says, and more than was known.
 */

/* ---- WHERE A COLONY RECORDS WHICH IT HAS ----------------------------------------------------
 * The six bytes at Colony+0x84 are a bitmap of these ids, one bit per building, and
 * `colony_flag_get(colony, bit)` / `colony_flag_set(bit, on)` are how the code reads
 * and writes it -- so those two take a BUILDING_* here, not an opaque flag number. Four things
 * establish it, none of them a guess:
 *
 *   - `create_colony` founds a colony by clearing SIX bytes of that region -- 48 bits,
 *     the smallest whole number that holds 42 buildings -- and then setting exactly two: 9 and
 *     0x23, which are BUILDING_TOWN_HALL and BUILDING_CARPENTERS_SHOP, the pair a new colony
 *     starts with;
 *   - `building_class_value(int b)` passes its building straight to the getter;
 *   - a fisherman (JOB_FISHERMAN, 8) is only assigned where bit 6 is set, and 6 is BUILDING_DOCKS;
 *   - bit 0x14 doubles the bell production at g_prod[18], and 20 is BUILDING_NEWSPAPER.
 *
 * The call sites read as the game's rules once the names are in: `resolve_raid` asks for
 * BUILDING_STOCKADE, BUILDING_FORT and BUILDING_FORTRESS, and `ship_evades_blockade` asks for the
 * fortress that fires on passing ships.
 *
 * THE ACCESSORS ARE NOT RENAMED, and that is deliberate. `Colony.flags` is 0x16 bytes, not 6, so
 * the same two functions can address 176 bits of which only the first 48 are shown to be
 * buildings. Calling them `colony_has_building` would claim the whole array. The constants at the
 * call sites carry the meaning instead, which is exactly as much as the evidence supports.
 *
 * ONE THING THAT DOES NOT FIT. `colony_draw_site` pairs 0xf with 0x11 --
 * BUILDING_WAREHOUSE with BUILDING_STABLES -- where the drawing logic reads as though it wants
 * the warehouse and its expansion (0x10). Not explained here.
 */
#ifndef BUILDING_H
#define BUILDING_H

#define BUILDING_STOCKADE                   0
#define BUILDING_FORT                       1
#define BUILDING_FORTRESS                   2
#define BUILDING_ARMORY                     3
#define BUILDING_MAGAZINE                   4
#define BUILDING_ARSENAL                    5
#define BUILDING_DOCKS                      6
#define BUILDING_DRYDOCKS                   7
#define BUILDING_SHIPYARD                   8
#define BUILDING_TOWN_HALL                  9
#define BUILDING_TOWN_HALL_2                10
#define BUILDING_COLONIAL_ASSEMBLY          11
#define BUILDING_SCHOOLHOUSE                12
#define BUILDING_COLLEGE                    13
#define BUILDING_UNIVERSITY                 14
#define BUILDING_WAREHOUSE                  15
#define BUILDING_WAREHOUSE_EXPANSIONS       16
#define BUILDING_STABLES                    17
#define BUILDING_CUSTOM_HOUSE               18
#define BUILDING_PRINTING_PRESS             19
#define BUILDING_NEWSPAPER                  20
#define BUILDING_WEAVERS_HOUSE              21
#define BUILDING_WEAVERS_SHOP               22
#define BUILDING_TEXTILE_MILL               23
#define BUILDING_TOBACCONISTS_HOUSE         24
#define BUILDING_TOBACCONISTS_SHOP          25
#define BUILDING_CIGAR_FACTORY              26
#define BUILDING_RUM_DISTILLERS_HOUSE       27
#define BUILDING_RUM_DISTILLERY             28
#define BUILDING_RUM_FACTORY                29
#define BUILDING_CAPITOL                    30
#define BUILDING_CAPITOL_EXPANSION          31
#define BUILDING_FUR_TRADERS_HOUSE          32
#define BUILDING_FUR_TRADERS_SHOP           33
#define BUILDING_COAT_FACTORY               34
#define BUILDING_CARPENTERS_SHOP            35
#define BUILDING_LUMBER_MILL                36
#define BUILDING_CHURCH                     37
#define BUILDING_CATHEDRAL                  38
#define BUILDING_BLACKSMITHS_HOUSE          39
#define BUILDING_BLACKSMITHS_SHOP           40
#define BUILDING_IRON_WORKS                 41

#define BUILDING_COUNT                 42

#endif
