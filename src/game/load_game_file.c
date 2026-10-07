/* load_game_file: candidate for 1008:9056 (2930 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_game_file
 * name-source: sibling-build
 *
 * READING A SAVED GAME. Sixty-odd `save_read_field` calls in a row, every one of them followed by
 * the same `if (... == 0) goto done;`, restoring the whole world: the globals block, the four
 * nations, every colony, every unit, every settlement, the four map planes, the trade routes, and
 * three dozen loose counters and tables. A save file is simply the game's memory written out in
 * this order, and this function is the order.
 *
 * FOUR WAYS TO FAIL, and the error code is a register variable that starts at 1 and is only ever
 * cleared on the last line of the successful path. 2 is a bad magic string or a save from a NEWER
 * build, 3 is a save from an older one, 4 is a map whose area does not match the map already
 * loaded, and 1 -- the initial value -- is everything else, including a short read anywhere in
 * the sixty fields and a failed map allocation. So a truncated file and an out-of-memory failure
 * are the same error to the caller.
 *
 * THE MAP IS ALLOCATED ONLY ONCE. If `g_map_bytes` is already set the file's dimensions must match
 * it exactly; if it is zero the planes are allocated here and a flag is raised so that the exit
 * path can UNDO the allocation if anything later goes wrong. That is the only cleanup this
 * function does, and it is conditional on both the flag and a non-zero error.
 *
 * EVERY MULTI-BYTE FIELD GOES THROUGH A SWAP HOOK after it is read -- `swap_word_identity` for scalars and
 * one of eight record-specific helpers for the tables -- kept from the big-endian Macintosh
 * original; on Windows `swap_word_identity` returns its argument, so nothing is actually swapped.
 *
 * TWO SHIPPED ODDITIES. The map dimensions are copied into `g_map_size.w` TWICE when the map is
 * being allocated for the first time -- once inside the branch and once after it -- and the second
 * copy is what the other branch needs. And the last loop swaps sixteen words of one table
 * against sixteen BYTES of another in the same body, passing each byte through the word swapper
 * and storing the low half back, which is a no-op done sixteen times.
 *
 * WHAT THE BYTES SETTLED: `identity_ptr_1008_d74e` takes and returns a LONG, not a pointer --
 * the argument goes across in a single `PUSH DWORD` where a pointer would be two word pushes.
 * And `STRCMP` is called with the expected magic FIRST and the buffer second.
 *
 * `g_rec6_78e0` IS FOUR `Rec6` RECORDS, ONE PER NATION, and the layout says so three ways
 * (2026-09-25). `rec6_swap_words` walks exactly four 6-byte records and is called on it, which
 * is the 0x18 this reads. Its address is where the nation records END: `g_nation_text` is
 * SEG20:0x7810 for 0xd0 bytes, and 0x7810 + 0xd0 = 0x78e0, so the two are contiguous here and
 * in the save file, where they are consecutive fields. 0xd0 is 4 x 52, which is why
 * `g_nation_text` is the SAME array viewed from +0x30 rather than a separate one -- 0x7810 + 0x30
 * = 0x7840 -- and why the apparent overlap with this array is not one.
 *
 * `Rec6` is the six-byte shape of `Pickup` (`{colony, value, loads, troops}`), the record `g_cargo_pickups` holds sixteen of at
 * SEG24:0x676. THE NAME STAYS AN ADDRESS because nothing in the corpus READS this array: the
 * save pair is its only appearance, and a file that hands a block to `save_read_field` learns
 * nothing about what is in it. The shape is established; the meaning is not.
 *
 * RENAMED 2026-09-27 from load_saved_game: the Mac build's load_game_file, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Reads the saved game (sixty save_read_field calls plus byte-swaps): Mac load_game(FSSpec*), ratio 1.07, 1 arg. load_game is already the Windows name of the Mac load_game(void) command that calls this, so it needs an overload suffix; plain load_game would collide.
 */

#include "colonization.h"

#include "sounds.h"
#include "gameflag.h"
#include "w16types.h"
/* PORT: this file's own DialogWin, DiskFile, MenuClass removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* DialogWin : EventWin, flattened for C */
#include "records.h"


typedef struct { char pad[1]; } MainWin;

extern MainWin g_main_win;                 /* SEG20:0x5ca */


extern GameState g_game;   /* SEG20:0x7782 */

extern Nation g_nations[];               /* SEG20:0xc2f4, the whole block */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern int allocate_map_memory(int big);
extern void map_free(void);
extern void feature_free(void);
extern void continent_free(void);
extern void sound_play(int id);
extern void LoadTray(void);
extern void random_activate(int seed);
extern void revolution_setup(void);
extern void anti_revolution(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_disable_1008_d766(void *w);
extern "C" void win_enable_1008_d809(void *w);
extern "C" void win_redraw_menu_bar_1008_d7b3(void *w);
extern "C" void MSDeleteMenuItem_1008_d791(void *m, int pos, int item);

extern "C" void nation52_swap_word_at_32(void *nation_text);
extern "C" void rec6_swap_words(void *p);
extern "C" void colonies_swap_words(void *colony, int colony_count);
extern "C" void units_swap_words(void *u, int unit_count);
extern "C" void nations_swap_words(void *nations);
extern "C" void records18_swap_words(void *settlements, int settlement_count);
extern "C" void records_swap_words(void *tribes);
extern "C" void SetupDitherMap(void);

int load_game_file(char *name)
{
    register int i;
    register int err = 1;
    DiskFile f;
    int ver;
    int alloc;
    long area;
    long p;
    int dims[2];
    char magic[0x50];

    alloc = 0;
    win_disable_1008_d766(&g_main_win);
    if (g_game.game_flags & GAMEFLAG_CHEAT_MENU) {
        MSDeleteMenuItem_1008_d791(&g_menu_owner, 7, -1);
        win_redraw_menu_bar_1008_d7b3(&g_main_win);
    }
    if (file_MSOpenFile_1008_d7e3(&f, name) == 0) {
        win_enable_1008_d809(&g_main_win);
        return 0;
    }
    if (save_read_field(&f, magic, 0xaL) == 0)
        goto done;
    if (STRCMP(g_save_magic, magic) != 0) {
        err = 2;
        goto done;
    }
    if (save_read_field(&f, &ver, 2L) == 0)
        goto done;
    ver = swap_word_identity(ver);
    if (ver > g_save_version) {
        err = 2;
        goto done;
    }
    if (ver < g_save_version) {
        err = 3;
        goto done;
    }
    if (save_read_field(&f, dims, 4L) == 0)
        goto done;
    dims[0] = swap_word_identity(dims[0]);
    dims[1] = swap_word_identity(dims[1]);
    area = (long)dims[0] * (long)dims[1];
    if (g_map_bytes != 0) {
        if (g_map_bytes != area) {
            err = 4;
            goto done;
        }
    } else {
        MSCopyMem(dims, &g_map_size.w, 4L);
        if (allocate_map_memory(0) != 0) {
            err = 1;
            goto done;
        }
        alloc = 1;
    }
    MSCopyMem(dims, &g_map_size.w, 4L);
    if (save_read_field(&f, &g_game, (long)sizeof(GameState)) == 0)
        goto done;
    globals_swap_words(&g_game);
    if (save_read_field(&f, g_nation_text, 0xd0L) == 0)
        goto done;
    nation52_swap_word_at_32(g_nation_text);
    if (save_read_field(&f, &g_rec6_78e0, 0x18L) == 0)
        goto done;
    rec6_swap_words(&g_rec6_78e0);
    if (g_game.colony_count != 0)
        if (save_read_field(&f, g_colonies, (unsigned)(g_game.colony_count * 0xca)) == 0)
            goto done;
    colonies_swap_words(g_colonies, g_game.colony_count);
    if (g_game.unit_count != 0)
        if (save_read_field(&f, g_units, (unsigned)(g_game.unit_count * 0x1c)) == 0)
            goto done;
    units_swap_words(g_units, g_game.unit_count);
    if (save_read_field(&f, g_nations, 0x4f0L) == 0)
        goto done;
    nations_swap_words(g_nations);
    if (g_game.settlement_count != 0)
        if (save_read_field(&f, g_settlements, (unsigned)(g_game.settlement_count * 0x12)) == 0)
            goto done;
    records18_swap_words(g_settlements, g_game.settlement_count);
    if (save_read_field(&f, g_tribes, 0x270L) == 0)
        goto done;
    records_swap_words(g_tribes);
    if (save_read_field(&f, g_leader_traits, 0xcL) == 0)
        goto done;
    if (save_read_field(&f, g_nation_unit_count, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_colony_count, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_free_colonists, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_pop_by_nation, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_population, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_land_strength, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_cargo_capacity, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_ship_count, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_military_power, 8L) == 0)
        goto done;
    for (i = 0; i < 4; i++)
        g_military_power[i] = swap_word_identity(g_military_power[i]);
    if (save_read_field(&f, g_nation_armed_ships, 4L) == 0)
        goto done;
    if (save_read_field(&f, &g_ai_military_target, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_strength, 4L) == 0)
        goto done;
    if (save_read_field(&f, g_unit_tally, 0x4cL) == 0)
        goto done;
    if (save_read_field(&f, g_region_settlements, 0x10L) == 0)
        goto done;
    if (save_read_field(&f, g_region_flags, 0x10L) == 0)
        goto done;
    if (save_read_field(&f, g_region_unit_count, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_region_colony_count, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_region_strength, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_region_population, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_nation_region, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_region_attack, 0x40L) == 0)
        goto done;
    if (save_read_field(&f, g_avg_colony_size, 8L) == 0)
        goto done;
    for (i = 0; i < 4; i++)
        g_avg_colony_size[i] = swap_word_identity(g_avg_colony_size[i]);
    if (save_read_field(&f, &g_show_icon_counts, 1L) == 0)
        goto done;
    if (save_read_field(&f, g_tribe_strength, 8L) == 0)
        goto done;
    if (save_read_field(&f, g_tribe_pop, 8L) == 0)
        goto done;
    if (save_read_field(&f, g_tribe_settlement_count, 8L) == 0)
        goto done;
    if (save_read_field(&f, g_tribe_region, 0x80L) == 0)
        goto done;
    if (save_read_field(&f, &g_cursor_x, 2L) == 0)
        goto done;
    g_cursor_x = swap_word_identity(g_cursor_x);
    if (save_read_field(&f, &g_cursor_y, 2L) == 0)
        goto done;
    g_cursor_y = swap_word_identity(g_cursor_y);
    if (save_read_field(&f, &g_zoom, 2L) == 0)
        goto done;
    g_zoom = swap_word_identity(g_zoom);
    if (save_read_field(&f, &g_map_center_x, 2L) == 0)
        goto done;
    g_map_center_x = swap_word_identity(g_map_center_x);
    if (save_read_field(&f, &g_map_center_y, 2L) == 0)
        goto done;
    g_map_center_y = swap_word_identity(g_map_center_y);
    if (save_read_field(&f, g_plane0, g_map_bytes) == 0)
        goto done;
    if (save_read_field(&f, g_plane1, g_map_bytes) == 0)
        goto done;
    if (save_read_field(&f, g_plane2, g_map_bytes) == 0)
        goto done;
    if (save_read_field(&f, g_plane3, g_map_bytes) == 0)
        goto done;
    if (save_read_field(&f, g_coarse_grid_sea, 0x10eL) == 0)
        goto done;
    if (save_read_field(&f, g_coarse_grid_land, 0x10eL) == 0)
        goto done;
    if (save_read_field(&f, g_region_farmland, 0x20L) == 0)
        goto done;
    if (save_read_field(&f, g_region_size, 0x20L) == 0)
        goto done;
    for (i = 0; i < 0x10; i++) {
        g_region_size[i] = swap_word_identity(g_region_size[i]);
        g_region_farmland[i] = swap_word_identity(g_region_farmland[i]);
    }
    if (save_read_field(&f, &p, 4L) == 0)
        goto done;
    p = identity_ptr_1008_d74e(p);
    if (save_read_field(&f, &g_seed_base, 4L) == 0)
        goto done;
    g_seed_base = identity_ptr_1008_d74e(g_seed_base);
    if (save_read_field(&f, &g_scenery_seed, 2L) == 0)
        goto done;
    g_scenery_seed = swap_word_identity(g_scenery_seed);
    if (save_read_field(&f, g_routes, 0x378L) == 0)
        goto done;
    g_sound_music = g_game.hint_flags & 2;
    g_sound_event_music = g_game.hint_flags & 4;
    g_sound_effects = g_game.hint_flags & 8;
    if (g_sound_event_music == 0 || g_sound_music == 0 || g_sound_effects == 0)
        sound_play(SOUND_STOP_MUSIC);
    if (g_sound_music != 0)
        LoadTray();
    random_activate(g_random_seed);
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        revolution_setup();
    else
        anti_revolution();
    err = 0;
    win_enable_1008_d809(&g_main_win);
    SetupDitherMap();
done:
    MSCloseFile_1008_d85e(&f);
    win_enable_1008_d809(&g_main_win);
    if (alloc != 0 && err != 0) {
        map_free();
        feature_free();
        continent_free();
        g_map_bytes = 0;
    }
    return err;
}

