/* start_new_game: candidate for 1008:c44a (1615 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * SETS UP A NEW GAME, for `1008:ca99`. `america` is non-zero for the fixed AMERICA map and
 * ZERO for a generated New World -- the opposite of what this header used to say. BEGINMENU,
 * the message `1008:ca99` puts on screen, lists its options in order: "Start a Game in NEW
 * WORLD", "Start a Game in AMERICA", "CUSTOMIZE New World", and `begin_game` passes 1
 * for the second of them and 0 for the other two. `start_indians` is what it decides:
 * with `g_game.america_map` zero it places the eight tribes and their settlements at random,
 * which is what a generated world needs and the historical map does not.
 * Returns 1 when the leader-name dialog (`get_game_options`) is cancelled, and 2 once it
 * has got past that point. The shipped code never sets anything else.
 *
 * RESET: the palette entries 10..245 are saved (and restored on every exit), the map file name is set, and the
 * game globals are cleared:
 *   - the nation on turn and the viewer are 0, trade routes 0, the map not revealed, no view
 *     override, the flags words 0xc600 / 0xe, the tutorial flags 0, and the three sound switches on;
 *   - no rebel sentiment, no Crown, no intervening or mercenary power, and no war started for any nation;
 *   - the sixteen base goods volumes (`europe_adjust_prices`) are each 600..1000 at random;
 *   - the royal forces and the intervention force are 0, until the leader is named.
 *
 * THE CROWN'S FORCES by difficulty: 15 + 8d regulars, 5 + 5d cavalry, 2 + 6d artillery and 2 + 3d
 * men-of-war. The viceroy's introduction plays unless DGROUP:0x8a is set, over SOUND_MUSIC_HORNPIPE.
 *
 * THE MAP. The standard map is 58x72. It is loaded (`map_startup`), and the AMERICA map also has
 * its planes read, with its top and bottom rows forced to arctic (terrain 24). Then the world is generated
 * (`build_map`), the nations and the markets initialised, the coarse grid built, and colony
 * sites scored. The date is 1492, turn 0, spring.
 *
 * `build_map`'s own parameter is `loaded`, and it agrees: with a non-zero argument it
 * skips generation and does only the tail a loaded map needs. AMERICA is the loaded map,
 * so the same value means the same thing at both ends of the call.
 *
 * THE FOUR EUROPEAN NATIONS that are in the game (control byte not 2) each get, at their Europe square
 * `nation - 28`, bound for their start square (+0x32, +0x33):
 *   - a CARAVEL (the Dutch a MERCHANTMAN);
 *   - HARDY PIONEERS on board (sentried), expert pioneers for France;
 *   - SOLDIERS on board (sentried), veterans for Spain and for a human on the two easiest levels.
 * The map centre and cursor are set to the start square, the nation's twelve bytes at +0x34 are
 * cleared, and so is the word at +2 of its 52-byte record at SEG20:0x7840.
 * Then the tribes are loaded, terrain seams built, and the timer waited out before
 * SOUND_MUSIC_JINE_THE_CAVALRY starts.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-27 from new_game_setup: String witness: AMER2.MP is the only literal in Mac start_new_game(short), and on Windows it is here. Call graph: Mac begin_game calls start_new_game, and ca99 (begin_game) calls c44a; it is c44a's only caller. Mac start_new_game's sequence (CopyRawColors, random, get_game_options, MSDoAllTasks, KingHowdy, sound_play, build_check x6, map_startup/load_map_file, buffer_rect_fill x2, random_activate, build_map, europe_init, compute_moves, map_analysis, create_unit x3, start_indians, force_music, Fill) lines up with c44a's callees. Those are CTable_CopyRawColors, rand_range, new_game_player_setup, MSDoAllTasks, KingHowdy, sound_play, build_check x6, map_startup/load_map_file, buffer_rect_fill x2, random_activate x2, build_map, europe_init, compute_moves, map_analysis, create_unit_1038_2246 x3, start_indians, force_music and CTable_Fill. Arity: Mac (short) against int new_game_setup(int america). Size is 1422 Mac against 1615 Windows. The old name was a descriptive paraphrase. Apply this only after 1008:ca99 has moved to begin_game.
 */

#include "colonization.h"

#include "difficul.h"
#include "control.h"
#include "goods.h"
#include "nations.h"
#include "royal.h"
#include "units.h"
#include "jobs.h"
#include "sounds.h"
#include "classes.h"
#include "records.h"

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern void sound_play(int n);
extern int build_check(int flag);
extern int map_startup(int big);
extern int load_map_file(void);
extern void buffer_rect_fill(short h, int x, int y, unsigned n, int rows, char value);
extern void random_activate(int seed);
extern void build_map(int loaded);
extern void map_init(void);
extern void europe_init(void);
extern void compute_moves(void);
extern void map_analysis(void);

extern void start_indians(void);
extern void force_music(int what);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void KingHowdy(void);
extern void SetupDitherMap(void);

int start_new_game(int america)
{
    int r;
    int u;
    int human;
    char pal[0x2c4];
    register int nation;
    register int i;

    r = 1;
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 10, 0xec);
    STRCPY(g_map_file_name, g_default_map_file);
    g_game.america_map = america;
    g_game.turn_nation = 0;
    g_game.view_nation = 0;
    g_game.trade_route_count = 0;
    g_game.reveal_all = 0;
    g_game.view_override = -1;
    g_game.game_flags = 0xc600;
    g_game.hint_flags = 0xe;
    g_game.tutorial_flags = 0;
    g_sound_music = 1;
    g_sound_event_music = 1;
    g_sound_effects = 1;
    g_game.rebel_sentiment = 0;
    g_game.mother_country_idx = -1;
    g_game.intervention_nation = -1;
    g_game.merc_nation = -1;
    g_game.rebel_step = 0;
    for (i = 0; i < 4; i++)
        g_game.war_started[i] = -1;
    for (i = 0; i < GOODS_COUNT; i++)
        g_game.base_sales[i] = rand_range(600, 1000);
    for (i = 0; i < 4; i++) {
        g_game.royal_forces[i] = 0;
        g_game.force[i] = 0;
    }
    memset_rtl(g_game.woodcuts_seen, 0, 4);
    if (get_game_options())
        goto out;
    MSDoAllTasks_1008_ce3f();
    g_game.royal_forces[ROYAL_REGULARS] = (g_game.difficulty << 3) + 15;
    g_game.royal_forces[ROYAL_CAVALRY] = g_game.difficulty * 5 + 5;
    g_game.royal_forces[ROYAL_ARTILLERY] = g_game.difficulty * 6 + 2;
    g_game.royal_forces[ROYAL_MAN_O_WAR] = g_game.difficulty * 3 + 2;
    if (g_debug_mode == 0)
        KingHowdy();
    sound_play(SOUND_MUSIC_HORNPIPE);
    build_check(0);
    r = 2;
    if (america == 0) {
        g_no_map_file = 1;
        g_map_size.w = 0x3a;
        g_map_size.h = 0x48;
        g_map_cells = 0x1050L;
    }
    if (map_startup(0))
        goto out;
    build_check(0);
    if (america != 0) {
        if (load_map_file()) {
            g_84 = g_map_load_error;
            goto out;
        }
        buffer_rect_fill(g_plane0_handle, 0, 0, g_map_size.w, 1, 0x18);
        buffer_rect_fill(g_plane0_handle, 0, g_map_size.h - 1, g_map_size.w, 1, 0x18);
    }
    build_check(0);
    random_activate(g_random_seed);
    build_map(america);
    build_check(0);
    map_init();
    europe_init();
    compute_moves();
    map_analysis();
    build_check(0);
    memset_rtl(g_game.father_owner, -1, 0x19);
    g_game.year_hi = 0;
    g_game.year_lo = rand_range(1, 8);
    g_game.turn_counter = 0;
    g_game.year = 0x5d4;
    g_game.season = 0;
    g_game.turn_nation = g_game.player_nation;
    g_game.view_nation = g_game.player_nation;
    if (g_game.view_override >= 0)
        g_game.view_nation = g_game.view_override;
    random_activate(g_random_seed);
    for (nation = 0; nation < NATION_EUROPEAN_COUNT; nation++) {
        if (g_nation_text[nation].control == CONTROL_DEFEATED)
            continue;
        human = nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN ? 1 : 0;
        for (i = 0; i < 12; i++)
            g_nations[nation].rel[i] = 0;
        u = create_unit(UNIT_CARAVEL, nation, nation - 0x1c, nation - 0x1c);
        g_units[u].orders = ORDERS_NONE;
        g_units[u].dest_x = g_nations[nation].start_x;
        g_units[u].dest_y = g_nations[nation].start_y;
        if (nation == 3)
            g_units[u].type = UNIT_MERCHANTMAN;
        u = create_unit(UNIT_PIONEERS, nation, nation - 0x1c, nation - 0x1c);
        g_units[u].orders = ORDERS_SENTRY;
        g_units[u].dest_x = g_nations[nation].start_x;
        g_units[u].dest_y = g_nations[nation].start_y;
        if (nation == 1)
            g_units[u].spec = JOB_PIONEER;
        u = create_unit(UNIT_SOLDIERS, nation, nation - 0x1c, nation - 0x1c);
        g_units[u].orders = ORDERS_SENTRY;
        g_units[u].dest_x = g_nations[nation].start_x;
        g_units[u].dest_y = g_nations[nation].start_y;
        if ((human && g_game.difficulty <= DIFFICULTY_EXPLORER) || nation == 2)
            g_units[u].spec = JOB_SOLDIER;
        g_cursor_x = g_map_center_x = g_nations[nation].start_x;
        g_cursor_y = g_map_center_y = g_nations[nation].start_y;
        g_nation_text[nation].colony_names_used = 0;
    }
    start_indians();
    SetupDitherMap();
    while (build_check(1) == 0)
        ;
    force_music(SOUND_MUSIC_JINE_THE_CAVALRY);
out:
    CTable_Fill(&g_game_palette, 10, 0xec, pal);
    return r;
}
