/* PORT: this file's own declarations of strings_init, text_item_binary removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 290 of the imported file. */
/* load_data: candidate for 1008:9c64 (2934 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_data
 * name-source: sibling-build
 *
 * THE WHOLE RULES FILE, read in one pass. Thirty-seven TEXT resources -- NAMES, SEASONS,
 * UNFORESTED, FORESTED, OTHER, RESOURCE, COUNTRY, CARGO, BUILDING, UNITS, TRIBES, FATHERS and the
 * rest -- are opened in turn and their CSV lines poured straight into the runtime tables at
 * SEG20. Almost the entire function is one shape repeated: open a resource, then a counted loop
 * that reads a line and assigns its fields. Nothing is validated and nothing is looked up by
 * name; the record COUNT for each table is a literal in the code, so a data file with one row too
 * many is silently ignored and one row too few leaves the tail of the table as it was.
 *
 * ONE RESOURCE IS OPTIONAL. Only COLORS has its return value tested; every other `text_open`
 * is called for effect. The COLORS section fills the colour table at DGROUP:0x8e (the palette
 * indices ReportWin_Score and others draw text in), and index 6 of that run is SKIPPED -- nine
 * values written into ten bytes.
 *
 * TWO PASSES DO MORE THAN COPY. After FORESTED, each of the eight rows just read is copied UP
 * eight places (`g_terrain_rows[i + 16] = g_terrain_rows[i + 8]`), so rows 16..23 keep a pristine
 * forested copy that nothing in the file writes. And after CARGO the function computes, for each
 * of twenty-nine terrain rows, the largest of `cargo[j].b[1] * row.f7[j]` over the eight goods
 * j = 1..8 and stores it in the row's byte +6 -- the one field `load_terrain` never
 * reads from the file. That is the tile's best yield, derived and not authored.
 *
 * THE ORDERS TABLE takes a keyboard key out of prose: the second CSV field is scanned past its
 * leading spaces and the first non-space character is kept. The scan is `while (*p != 0 && *p ==
 * ' ')`, whose first half adds nothing -- a space is never the terminator. The first non-space
 * character ends it, and so does the NUL: a field that is ALL spaces stops there and stores 0.
 *
 * THE LAST TABLE sizes itself: the count of PEDIA entries is parsed out of a text buffer in
 * SEG29 with `ATOL_1008_d87e` and the loop runs to that, the only bound here that is not a literal.
 *
 * RENAMED 2026-09-26 from load_data_tables: the Mac build's load_data(void), bound by call-graph agreement (6 of its Mac call edges already bound land here, runner-up 2); the arity agrees and the Mac body is 1.01x this one.
 */

#include "colonization.h"

#include "fathers.h"

#include "records.h"

   /* @CLASS: "Name, transportation costs" */

/* Two resource-file names are globals, not literals: both "NAMES" opens push DGROUP:0xa4
   and "LABELS" pushes 0xaa, one copy each in this module's initialised data, after
   g_game_txt ("GAME", 0x9e). Literals would compile to the same bytes, with one copy
   per use and in the wrong place in DGROUP (tools/measure_data_coverage.py). */
extern char g_names_txt[];               /* DGROUP:0xa4, "NAMES" */
extern char g_labels_txt[];              /* DGROUP:0xaa, "LABELS" */

extern Nation g_nations[];             /* SEG20:0xc2f4; 0xc326 is +0x32,
                                              .start_x and .start_y */

extern int text_open(void *unused, char *name);
extern char *text_get(void);
extern char *text_item_get(void);
extern int text_item_string(void);
extern int text_string(void);
extern int text_item_number(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void load_terrain(int row);

void load_data(void)
{
    register int i;
    int nation;
    int best;
    int j;
    int v;
    char *p;

    strings_init(0x1af4);
    text_open(g_names_txt, "SEASONS");
    for (i = 0; i < 2; i++) {
        text_get();
        g_season_word[i] = text_item_string();
    }
    text_open(0, "UNFORESTED");
    for (i = 0; i < 8; i++)
        load_terrain(i);
    text_open(0, "FORESTED");
    for (i = 0; i < 8; i++) {
        load_terrain(i + 8);
        g_terrain_rows[i + 16] = g_terrain_rows[i + 8];
    }
    text_open(0, "OTHER");
    for (i = 0; i < 5; i++)
        load_terrain(i + 0x18);
    text_open(0, "OTHER_NAMES");
    for (i = 0; i < 5; i++) {
        text_get();
        g_feature_words[i] = text_item_string();
    }
    text_open(0, "RESOURCE");
    for (i = 0; i < 0xe; i++) {
        text_get();
        g_resource_name[i] = text_item_string();
        g_resource_value[i] = text_item_number();
    }
    text_open(0, "COUNTRY");
    for (i = 0; i < 4; i++) {
        text_get();
        g_nation_name[i] = text_item_string();
        g_nation_colour[i] = text_item_number();
    }
    text_open(0, "NATIONALITY");
    for (i = 0; i < 4; i++)
        g_nation_adjective[i] = text_string();
    text_open(0, "NATIONABBREV");
    for (i = 0; i < 4; i++)
        g_nation_abbrev[i] = text_string();
    text_open(0, "HOMEPORT");
    for (i = 0; i < 4; i++)
        g_nation_name_word[i] = text_string();
    text_open(0, "COLONYNAME");
    for (i = 0; i < 4; i++)
        STRCPY(g_nation_text[i].country, text_get());
    text_open(0, "LEADERNAME2");
    for (i = 0; i < 4; i++) {
        text_get();
        STRCPY(g_nation_text[i].leader, text_item_get());
        g_leader_traits[i].a = text_item_number();
        g_leader_traits[i].b = text_item_number();
        g_leader_traits[i].c = text_item_number();
    }
    text_open(0, "MISSION");
    for (i = 0; i < 4; i++)
        g_mission_word[i] = text_string();
    text_open(0, "DIFFICULTY");
    for (i = 0; i < 5; i++)
        g_difficulty_words[i] = text_string();
    text_open(0, "CLASS");
    for (i = 0; i < 8; i++) {
        text_get();
        g_class[i].name = text_item_string();
        g_class[i].transport_cost = text_item_number();
    }
    text_open(0, "BUILDING");
    for (i = 0; i < 0x2a; i++) {
        text_get();
        g_buildings[i].name = text_item_string();
        g_buildings[i].hammers = text_item_number();
        g_buildings[i].tools = text_item_number();
        g_buildings[i].kind = text_item_number();
        g_buildings[i].min_pop = text_item_number();
        g_buildings[i].upkeep = text_item_number();
    }
    text_open(0, "SCENARIO");
    text_get();
    text_item_get();
    for (nation = 0; nation < 4; nation++) {
        g_nations[nation].start_x = text_item_number();
        g_nations[nation].start_y = text_item_number();
    }
    text_open(0, "JOB");
    for (i = 0; i < 0x1c; i++) {
        text_get();
        g_spec_recs[i].name = text_item_string();
        g_spec_recs[i].expert = text_item_string();
        g_spec_recs[i].school = (unsigned char)text_item_number();
        g_spec_recs[i].price = text_item_number();
    }
    text_open(0, "CARGO");
    for (i = 0; i < 0x14; i++) {
        text_get();
        g_goods_name_word[i] = text_item_string();
        if (i < 0x10) {
            g_goods[i].start_lo = text_item_number();
            g_goods[i].start_hi = text_item_number();
            g_goods[i].price_min = text_item_number();
            g_goods[i].price_max = text_item_number();
            g_goods[i].spread = text_item_number();
            g_goods[i].up = text_item_number();
            g_goods[i].down = text_item_number();
            g_goods[i].drift = text_item_number();
            g_goods[i].shift = text_item_number();
        }
    }
    for (i = 0; i < 0x1d; i++) {
        best = 0;
        for (j = 1; j < 9; j++) {
            v = (signed char)g_goods[j].start_hi * g_terrain_rows[i].yield[j];
            if (v > best)
                best = v;
        }
        g_terrain_rows[i].best_goods_value = best;
    }
    text_open(0, "UNITS");
    for (i = 0; i < 0x17; i++) {
        text_get();
        g_unit_types[i].name = text_item_string();
        g_unit_types[i].icon = text_item_number();
        g_unit_types[i].moves = (unsigned char)text_item_number() * 3;
        g_unit_types[i].attack = text_item_number();
        g_unit_types[i].defence = text_item_number();
        g_unit_types[i].holds = text_item_number();
        g_unit_types[i].size = text_item_number();
        g_unit_types[i].hammers = text_item_number();
        g_unit_types[i].tools = text_item_number();
        g_unit_types[i].guns = text_item_number();
        g_unit_types[i].hull = text_item_number();
        g_unit_types[i].bits = text_item_binary();
    }
    text_open(g_names_txt, "ORDERS");
    for (i = 0; i < 0xd; i++) {
        text_get();
        g_order_word[i] = text_item_string();
        p = text_item_get();
        while (*p != 0 && *p == ' ')
            p++;
        g_order_letter[i] = *p;
    }
    text_open(0, "ACTIONS");
    for (i = 0; i < 0xa; i++)
        g_action_word[i] = text_string();
    text_open(0, "VALUES");
    for (i = 0; i < 4; i++)
        g_value_word[i] = text_string();
    text_open(0, "ATTITUDE");
    for (i = 0; i < 5; i++)
        g_attitude_word[i] = text_string();
    text_open(0, "ATTITUDINAL");
    for (i = 0; i < 5; i++)
        g_attitudinal_word[i] = text_string();
    text_open(0, "LEVELS");
    for (i = 0; i < 5; i++) {
        text_get();
        g_level[i].name = text_item_string();
        g_level[i].settlement = text_item_string();
        g_level[i].settlements = text_item_string();
    }
    text_open(0, "TRIBES");
    for (i = 0; i < 8; i++) {
        text_get();
        g_tribe[i].name = text_item_string();
        g_tribe[i].adjective = text_item_string();
        g_tribe[i].treasure = text_item_string();
        text_item_number();
        g_tribe_colour[i] = text_item_number();
    }
    text_open(0, "FOUNDING");
    for (i = 0; i < 6; i++)
        g_founding_word[i] = text_string();
    text_open(0, "FATHERS");
    for (i = 0; i < FATHER_COUNT; i++) {
        text_get();
        g_fathers[i].name = text_item_string();
        g_fathers[i].cat = text_item_number();
        g_fathers[i].weight[0] = text_item_number();
        g_fathers[i].weight[1] = text_item_number();
        g_fathers[i].weight[2] = text_item_number();
    }
    if (text_open(0, "COLORS") == 0) {
        text_get();
        g_colour_table[0] = text_item_number();
        g_colour_table[1] = text_item_number();
        g_colour_table[2] = text_item_number();
        g_colour_table[3] = text_item_number();
        g_colour_table[4] = text_item_number();
        g_colour_table[5] = text_item_number();
        g_colour_table[7] = text_item_number();
        g_colour_table[8] = text_item_number();
        g_colour_table[9] = text_item_number();
    }
    text_open(g_labels_txt, "INFO");
    for (i = 0; i < 4; i++)
        g_info_label[i] = text_string();
    text_open(0, "MISC");
    for (i = 0; i < 0xdd; i++)
        g_misc_words[i] = text_string();
    text_open(0, "ROUTE");
    for (i = 0; i < 9; i++)
        g_route_label[i] = text_string();
    text_open(0, "CMISC");
    for (i = 0; i < 3; i++)
        g_cmisc_word[i] = text_string();
    text_open(0, "CTITLE");
    for (i = 0; i < 0xa; i++)
        g_ctitle_word[i] = text_string();
    text_open(0, "CMESSAGE");
    for (i = 0; i < 0x13; i++)
        g_colony_message_words[i] = text_string();
    text_open(0, "EUROLABEL");
    for (i = 0; i < 3; i++)
        g_eurolabel_word[i] = text_string();
    text_open((void*)("PEDIA"), "MISCELLANEOUS");
    text_get();
    g_pedia_count = ATOL_1008_d87e(g_text_buf);
    for (i = 0; i < g_pedia_count; i++)
        g_pedia_word[i] = text_string();
}

