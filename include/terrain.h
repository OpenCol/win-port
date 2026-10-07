/* terrain.h -- terrain ids and the four map planes.
 *
 * Sources: docs/findings/terrain-table.md (the ids, from the game's own TERRAIN0..TERRAIN28
 * TEXT resources) and docs/findings/map-planes.md (what each plane's bits are known to mean).
 * tools/check_headers.py re-reads the resources and fails if an id here disagrees with them.
 *
 * Only what is established is named. Where the findings say "not established", the macro is
 * named after the bit and not after a guess.
 */
#ifndef TERRAIN_H
#define TERRAIN_H

/* ---- terrain ids: the low five bits of a plane 0 byte, when MAP0_HILLY is clear ----------- */
#define TERRAIN_TUNDRA              0
#define TERRAIN_DESERT              1
#define TERRAIN_PLAINS              2
#define TERRAIN_PRAIRIE             3
#define TERRAIN_GRASSLAND           4
#define TERRAIN_SAVANNAH            5
#define TERRAIN_MARSH               6
#define TERRAIN_SWAMP               7
#define TERRAIN_BOREAL_FOREST       8       /* the resource spells it BORREAL */
#define TERRAIN_SCRUB_FOREST        9
#define TERRAIN_MIXED_FOREST        10
#define TERRAIN_BROADLEAF_FOREST    11
#define TERRAIN_CONIFER_FOREST      12
#define TERRAIN_TROPICAL_FOREST     13
#define TERRAIN_WETLAND_FOREST      14
#define TERRAIN_RAIN_FOREST         15
/* 16..23 repeat the names of 8..15. What tells the two bands apart is NOT established. */
#define TERRAIN_BOREAL_FOREST_2     16
#define TERRAIN_SCRUB_FOREST_2      17
#define TERRAIN_MIXED_FOREST_2      18
#define TERRAIN_BROADLEAF_FOREST_2  19
#define TERRAIN_CONIFER_FOREST_2    20
#define TERRAIN_TROPICAL_FOREST_2   21
#define TERRAIN_WETLAND_FOREST_2    22
#define TERRAIN_RAIN_FOREST_2       23

/* ---- the forest range ----------------------------------------------------------------------
 * 8..23 are every terrain whose resource name contains FOREST, and nothing outside that range
 * does -- which tools/check_headers.py re-derives from the TERRAIN resources rather than from
 * this comment. So a test like `terrain >= 8 && terrain <= 0x17` asks "is this forest?", and
 * naming its ends TERRAIN_BOREAL_FOREST and TERRAIN_RAIN_FOREST_2 would answer a question nobody
 * asked. `MainWin_CheckMenuItems` is the witness: it relabels an order "Clear Forest" across
 * exactly this range, written as the two bands 8..0xf and 0x10..0x17.
 */
#define TERRAIN_FOREST_FIRST        TERRAIN_BOREAL_FOREST     /* 8 */
#define TERRAIN_FOREST_LAST         TERRAIN_RAIN_FOREST_2     /* 23 */
#define TERRAIN_FOREST_2_FIRST      TERRAIN_BOREAL_FOREST_2   /* 16, where the second band starts */

#define TERRAIN_ARCTIC              24
#define TERRAIN_OCEAN               25
#define TERRAIN_SEA_LANE            26
#define TERRAIN_MOUNTAINS           27      /* never stored in the five-bit field: see MAP0_HILLY */
#define TERRAIN_HILLS               28
#define TERRAIN_COUNT               29

/* ---- plane 0: the raw terrain byte (terrain_type, 1040:42b5) --------------------- */
#define MAP0_TERRAIN_MASK   0x1f    /* a terrain id only when MAP0_HILLY is clear */
#define MAP0_HILLY          0x20    /* hills or mountains; the low five bits are then not an id */
#define MAP0_RIVER          0x40    /* a river: "Minor River" / "Major River" (label_map) */
#define MAP0_MOUNTAINS      0x80    /* with MAP0_HILLY: mountains rather than hills */
#define MAP0_MAJOR_RIVER    0x80    /* the same bit with MAP0_RIVER: a major river rather than a minor one */

/* ---- plane 1: a bitfield written through feature_set ------------------------------
 * The names come from the square description (label_map), which appends the
 * game's own MISC words "Road" for bits 0x0a and "Plowed" for 0x40, and from the writers.
 */
#define MAP1_UNIT           0x01    /* set by put_down_unit; a BURNED settlement clears it */
#define MAP1_SETTLEMENT     0x02    /* colony or village: gates square_nation, counts as "Road", BURNED clears it */
#define MAP1_DEPLETED       0x04    /* set by colony_depletion */
#define MAP1_ROAD           0x08    /* "Road"; orders_road refuses a square with it or a settlement */
#define MAP1_CLAIM_SETTLED  0x10    /* set once land is paid for or taken: buy_this, orders_road,
                                       orders_clear, set_field's set_indian */
#define MAP1_BIT5           0x20    /* set by 1020:09ea; meaning not established */
#define MAP1_PLOWED         0x40    /* "Plowed" */

/* ---- plane 2: a nation index in each nibble ----------------------------------------------- */
#define MAP2_NIBBLE_NONE    15      /* the high-nibble reader maps this to -1 */

/* ---- plane 3: the high nibble is a per-European-nation bitmask ---------------------------- */
#define MAP3_NATION_BIT(n)  ((1 << (n)) << 4)

#endif
