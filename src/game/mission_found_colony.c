/* mission_found_colony -- shipped at 1028:ab1f.
 *
 * Ported by tools/port.py from matched/game/mission_found_colony.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern unsigned far g_game_flags;                    SEG20:0x7784
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *   extern int far g_colony_c24;                         DGROUP:0x0c24
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *   extern char far g_colony_window;                     SEG20:0x25be
 *
 * The file's own Unit, NationRec52, Nation, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void next_colony_name(col_i16 nation, char *out);

#include "control.h"
#include "units.h"
#include "woodcut.h"
#include "nations.h"
#include "sounds.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern unsigned char * map_plane0_ptr(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 m);
extern col_i16 prompt_text_loop(char *rec, char *topic, col_i16 n, char *dflt);
extern void unit_refresh_move_points(col_i16 u);
extern col_i16 create_colony(col_i16 nation, col_i16 x, col_i16 y, col_i16 unit);

extern void colony_flag_set(col_i16 item, col_i16 v);
extern void redraw_map_1040_11b7(col_i16 a);
extern void play_sound_effect(col_i16 id);
extern void woodcut_show(col_i16 hint);
extern void colony_window_open(void *w, col_i16 colony);

/* same object: 4-byte `push cs; call ` */


void mission_found_colony(register col_i16 u)
{
    col_i32 plane0;
    col_i16 x;
    col_i16 y;
    col_i16 terr;
    col_i16 col;
    col_i16 step;
    char name[0x50];

    step = 3;
    x = g_units[u].x;
    y = g_units[u].y;
    plane0 = map_plane0_ptr(x, y);
    terr = terrain_at(x, y);
    step = 1;
    g_units[u].counter = 0;
    g_units[u].orders = ORDERS_NONE;
    if (g_turn_nation == g_view_nation
        || (g_reveal_all != 0
            && (g_game_flags & (g_turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
        map_scroll_to_show(x, y, x, y, 1);
    next_colony_name(g_turn_nation, name);
    if (g_turn_nation < NATION_EUROPEAN_COUNT && g_nation52[g_turn_nation].control == CONTROL_HUMAN) {
        g_text_value = 5;
        if (prompt_text_loop(&(*g_text_ctx), "COLONY", 0x17, name) != 0)
            goto done;
        g_text_value = -1;
    }
    unit_refresh_move_points(u);
    g_nations[g_turn_nation].founded = g_turn_counter;
    col = create_colony(g_turn_nation, x, y, u);
    if (col < 0)
        goto done;
    if (g_turn_nation >= NATION_EUROPEAN_COUNT || g_nation52[g_turn_nation].control != CONTROL_HUMAN)
        strcpy(((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)), name);
    colony_flag_set(0x20, 1);
    colony_flag_set(0x18, 1);
    colony_flag_set(0x15, 1);
    colony_flag_set(0x1b, 1);
    colony_flag_set(0x27, 1);
    if (g_turn_nation < NATION_EUROPEAN_COUNT && g_nation52[g_turn_nation].control == CONTROL_HUMAN)
        strcpy(((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)), g_name_slots[0]);
    if (g_turn_nation == g_view_nation
        || (g_reveal_all != 0
            && (g_game_flags & (g_turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
        redraw_map_1040_11b7(1);
    if (g_turn_nation < NATION_EUROPEAN_COUNT && g_nation52[g_turn_nation].control == CONTROL_HUMAN) {
        play_sound_effect(SOUND_HAMMERS);
        woodcut_show(WOODCUT_BUILDING_A_COLONY);
        g_colony_mode = 0;
        g_colony_c24 = 0;
        colony_window_open(&(*g_colony_window), col);
    }
done:
    ;
}
