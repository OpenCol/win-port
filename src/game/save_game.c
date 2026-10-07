/* save_game: candidate for 1008:a7f6 (2627 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: save_game
 * name-source: sibling-build
 *
 * WRITING A SAVED GAME, the exact mirror of `load_game_file` and in the same order. The
 * shape of every block is swap, write, SWAP BACK -- the byte-swapping helpers are run in place over
 * the live game state, the file is written from it, and the state is put straight again, so a save
 * leaves the world momentarily big-endian and then repairs it. Nothing is buffered and nothing is
 * copied aside.
 *
 * IT CHECKS NOTHING. `file_MSCreateFile_1008_d89e` returns a value and it is discarded; so is every
 * `save_write_field`. The function always returns 0, which is what its callers read as success. A
 * full disk produces a truncated file and a cheerful return.
 *
 * THE HEADER IS TWELVE BYTES, written as three fields: nine bytes of magic, one byte 0x1a -- a DOS
 * end-of-file marker, so `TYPE` on the save stops at the first line -- and the swapped version
 * word. The loader reads the first ten back as a single string and compares them, so the 0x1a is
 * part of the magic as as it is concerned.
 *
 * A SHIPPED BUG: the four bytes written just after the random seed come from a LOCAL THAT IS NEVER
 * ASSIGNED. It is passed through `identity_ptr_1008_d74e`, written to the file, passed through
 * again, and that is its whole life -- so every save file carries four bytes of whatever was on the
 * stack. It does no harm because the loader reads them into a local of its own and discards them,
 * but two save files of the same game will not compare equal.
 *
 * AND ONE CURIOSITY: `random_activate` is called in the MIDDLE of the writing, from the seed
 * the file has already recorded, so saving RESETS the generator. The same thing happens at the end
 * of loading, which is presumably the point -- save and load leave the sequence in the same place.
 *
 * The byte-swap loop over the two sixteen-entry tables passes each BYTE of one of them through the
 * word swapper and stores the low half back, a no-op done sixteen times, exactly as the loader does.
 *
 * RENAMED 2026-09-27 from save_game_to_file: the Mac build's save_game, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Writes the saved game block by block (swap, write, swap back): Mac save_game(FSSpec*) is the same Flip* /MSWriteFile sequence; 1 arg (name vs FSSpec*), ratio 1.10; perform_save is the caller on both sides.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern Nation g_nations[];               /* SEG20:0xc2f4, the whole block */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern void random_activate(int seed);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void nation52_swap_word_at_32(void *nation_text);
extern "C" void rec6_swap_words(void *p);
extern "C" void colonies_swap_words(void *colony, int colony_count);
extern "C" void units_swap_words(void *u, int unit_count);
extern "C" void nations_swap_words(void *nations);
extern "C" void records18_swap_words(void *settlements, int settlement_count);
extern "C" void records_swap_words(void *tribes);

int save_game(char *name)
{
    register int i;
    register int err = 0;
    DiskFile f;
    int ver;
    long p;
    char eof;

    file_MSCreateFile_1008_d89e(&f, name);
    ver = swap_word_identity(g_save_version);
    save_write_field(&f, g_save_magic, 9L);
    eof = 0x1a;
    save_write_field(&f, &eof, 1L);
    save_write_field(&f, &ver, 2L);
    g_map_size.w = swap_word_identity(g_map_size.w);
    g_map_size.h = swap_word_identity(g_map_size.h);
    save_write_field(&f, &g_map_size.w, 4L);
    g_map_size.w = swap_word_identity(g_map_size.w);
    g_map_size.h = swap_word_identity(g_map_size.h);
    globals_swap_words(&g_game);
    save_write_field(&f, &g_game, (long)sizeof(GameState));
    globals_swap_words(&g_game);
    nation52_swap_word_at_32(g_nation_text);
    save_write_field(&f, g_nation_text, 0xd0L);
    nation52_swap_word_at_32(g_nation_text);
    rec6_swap_words(&g_rec6_78e0);
    save_write_field(&f, &g_rec6_78e0, 0x18L);
    rec6_swap_words(&g_rec6_78e0);
    if (g_game.colony_count != 0) {
        colonies_swap_words(g_colonies, g_game.colony_count);
        save_write_field(&f, g_colonies, (unsigned)(g_game.colony_count * 0xca));
        colonies_swap_words(g_colonies, g_game.colony_count);
    }
    if (g_game.unit_count != 0) {
        units_swap_words(g_units, g_game.unit_count);
        save_write_field(&f, g_units, (unsigned)(g_game.unit_count * 0x1c));
        units_swap_words(g_units, g_game.unit_count);
    }
    nations_swap_words(g_nations);
    save_write_field(&f, g_nations, 0x4f0L);
    nations_swap_words(g_nations);
    if (g_game.settlement_count != 0) {
        records18_swap_words(g_settlements, g_game.settlement_count);
        save_write_field(&f, g_settlements, (unsigned)(g_game.settlement_count * 0x12));
        records18_swap_words(g_settlements, g_game.settlement_count);
    }
    records_swap_words(g_tribes);
    save_write_field(&f, g_tribes, 0x270L);
    records_swap_words(g_tribes);
    save_write_field(&f, g_leader_traits, 0xcL);
    save_write_field(&f, g_nation_unit_count, 4L);
    save_write_field(&f, g_nation_colony_count, 4L);
    save_write_field(&f, g_nation_free_colonists, 4L);
    save_write_field(&f, g_pop_by_nation, 4L);
    save_write_field(&f, g_nation_population, 4L);
    save_write_field(&f, g_nation_land_strength, 4L);
    save_write_field(&f, g_nation_cargo_capacity, 4L);
    save_write_field(&f, g_nation_ship_count, 4L);
    for (i = 0; i < 4; i++)
        g_military_power[i] = swap_word_identity(g_military_power[i]);
    save_write_field(&f, g_military_power, 8L);
    for (i = 0; i < 4; i++)
        g_military_power[i] = swap_word_identity(g_military_power[i]);
    save_write_field(&f, g_nation_armed_ships, 4L);
    save_write_field(&f, &g_ai_military_target, 4L);
    save_write_field(&f, g_nation_strength, 4L);
    save_write_field(&f, g_unit_tally, 0x4cL);
    save_write_field(&f, g_region_settlements, 0x10L);
    save_write_field(&f, g_region_flags, 0x10L);
    save_write_field(&f, g_region_unit_count, 0x40L);
    save_write_field(&f, g_region_colony_count, 0x40L);
    save_write_field(&f, g_nation_region_strength, 0x40L);
    save_write_field(&f, g_region_population, 0x40L);
    save_write_field(&f, g_nation_region, 0x40L);
    save_write_field(&f, g_region_attack, 0x40L);
    for (i = 0; i < 4; i++)
        g_avg_colony_size[i] = swap_word_identity(g_avg_colony_size[i]);
    save_write_field(&f, g_avg_colony_size, 8L);
    for (i = 0; i < 4; i++)
        g_avg_colony_size[i] = swap_word_identity(g_avg_colony_size[i]);
    save_write_field(&f, &g_show_icon_counts, 1L);
    save_write_field(&f, g_tribe_strength, 8L);
    save_write_field(&f, g_tribe_pop, 8L);
    save_write_field(&f, g_tribe_settlement_count, 8L);
    save_write_field(&f, g_tribe_region, 0x80L);
    g_cursor_x = swap_word_identity(g_cursor_x);
    save_write_field(&f, &g_cursor_x, 2L);
    g_cursor_x = swap_word_identity(g_cursor_x);
    g_cursor_y = swap_word_identity(g_cursor_y);
    save_write_field(&f, &g_cursor_y, 2L);
    g_cursor_y = swap_word_identity(g_cursor_y);
    g_zoom = swap_word_identity(g_zoom);
    save_write_field(&f, &g_zoom, 2L);
    g_zoom = swap_word_identity(g_zoom);
    g_map_center_x = swap_word_identity(g_map_center_x);
    save_write_field(&f, &g_map_center_x, 2L);
    g_map_center_x = swap_word_identity(g_map_center_x);
    g_map_center_y = swap_word_identity(g_map_center_y);
    save_write_field(&f, &g_map_center_y, 2L);
    g_map_center_y = swap_word_identity(g_map_center_y);
    save_write_field(&f, g_plane0, g_map_bytes);
    save_write_field(&f, g_plane1, g_map_bytes);
    save_write_field(&f, g_plane2, g_map_bytes);
    save_write_field(&f, g_plane3, g_map_bytes);
    save_write_field(&f, g_coarse_grid_sea, 0x10eL);
    save_write_field(&f, g_coarse_grid_land, 0x10eL);
    for (i = 0; i < 0x10; i++) {
        g_region_size[i] = swap_word_identity(g_region_size[i]);
        g_region_farmland[i] = swap_word_identity(g_region_farmland[i]);
    }
    save_write_field(&f, g_region_farmland, 0x20L);
    save_write_field(&f, g_region_size, 0x20L);
    for (i = 0; i < 0x10; i++) {
        g_region_size[i] = swap_word_identity(g_region_size[i]);
        g_region_farmland[i] = swap_word_identity(g_region_farmland[i]);
    }
    random_activate(g_random_seed);
    p = identity_ptr_1008_d74e(p);
    save_write_field(&f, &p, 4L);
    p = identity_ptr_1008_d74e(p);
    g_seed_base = identity_ptr_1008_d74e(g_seed_base);
    save_write_field(&f, &g_seed_base, 4L);
    g_seed_base = identity_ptr_1008_d74e(g_seed_base);
    g_scenery_seed = swap_word_identity(g_scenery_seed);
    save_write_field(&f, &g_scenery_seed, 2L);
    g_scenery_seed = swap_word_identity(g_scenery_seed);
    save_write_field(&f, g_routes, 0x378L);
    MSCloseFile_1008_d85e(&f);
    err = 0;
    return err;
}

