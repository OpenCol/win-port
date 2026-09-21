/* begin_game -- shipped at 1008:2927.
 *
 * Ported by tools/port.py from matched/game/begin_game.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far * far g_cur_nation_rec;            DGROUP:0x4cc4
 *   extern unsigned char far g_goods_value[];            SEG30:0x0010
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern char far g_loaded_flag;                       DGROUP:0x008b
 *
 * The file's own Nation, GameCursor dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1008_ce9c(col_i16 a, col_i16 b);

extern void set_current_nation(col_i16 n);

/* same object: 4-byte `push cs; call ` */


void begin_game(GameCursor *p)
{
    col_i16 nation;
    register col_i16 g;

    g_game_in_progress = 1;
    if (g_loaded_flag != 0) {
        for (nation = 0; nation < 4; nation++) {
            set_current_nation(nation);
            for (g = 0; g < 0x10; g++)
                ((unsigned char *)g_goods_value)[nation * 16 + g] =
                    (unsigned char)max_int_1008_ce9c(0, ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g] - 1);
        }
    }
    p->f6 = 0;
    p->f2 = 0;
    p->f0 = -1;
}
