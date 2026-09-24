/* fortify_unit -- shipped at 1008:0278.
 *
 * Ported by tools/port.py from matched/game/fortify_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern char g_str_havetreaty[];                      DGROUP:0x015e
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
#include "sounds.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))





extern col_i16 square_european_nation(col_i16 x, col_i16 y);
extern char nation_pair_flags_get(col_i16 a, col_i16 b);
extern col_i16 get_adjective(col_i16 n);
extern void set_name_slot_from_text(col_i16 a, col_i16 word);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void play_sound_effect(col_i16 n);
extern void unit_refresh_move_points(col_i16 u);

void fortify_unit(void)
{
    col_i16 u;
    col_i16 nation;
    col_i16 other;
    col_i16 x;
    col_i16 y;
    col_i16 nx;
    col_i16 ny;
    col_i16 found;
    register col_i16 n;
    register col_i16 i;

    u = g_selected_unit;
    nation = NATION(g_units[u]);
    x = g_units[u].x;
    y = g_units[u].y;
    found = 0;
    other = -1;
    for (i = 0; i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        n = square_european_nation(nx, ny);
        if (n < 0 || n == nation)
            continue;
        if ((nation_pair_flags_get(nation, n) & 0x40) == 0)
            continue;
        found = 1;
        other = n;
        break;
    }
    if (other < 0 || nation == n)
        goto fortify;
    set_name_slot_from_text(0, get_adjective(n));
    if (text_message_show(g_str_havetreaty, 1) != 2)
        goto out;
fortify:
    play_sound_effect(SOUND_COCKGUN);
    g_units[u].orders = ORDERS_FORTIFY;
    g_units[u].counter = 0;
    unit_refresh_move_points(u);
out:
    ;
}
