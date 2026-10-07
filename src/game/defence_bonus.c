/* defence_bonus -- shipped at 1020:8017.
 *
 * Ported by tools/port.py from matched/game/defence_bonus.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern Tribe far g_tribes[];                         SEG20:0xd182
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_terrain_row_bytes[][0x10]; SEG20:0x78f0
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int g_combat_flags[2];                        DGROUP:0x4bda
 *   extern int g_terrain_defence;                        DGROUP:0x4be6
 *   extern int g_defence_village;                        DGROUP:0x0dc0
 *   extern int g_defence_colony;                         DGROUP:0x0dc2
 *
 * The file's own Unit, Tribe, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_fort_level(void);
col_i16 unit_combat_strength(col_i16 u, col_i16 attacking);

#include "nations.h"
#include "tribes.h"
#include "control.h"
#include "settle.h"
#include "gameflag.h"
#include "units.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */










extern col_i16 square_native_nation(col_i16 x, col_i16 y);
extern col_i16 village_at(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);
extern col_i16 colony_at(col_i16 x, col_i16 y);
extern void set_current_colony(col_i16 c);
extern col_i16 terrain_at(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */



col_i16 defence_bonus(register col_i16 u, col_i16 attacker)
{
    col_i16 str;
    col_i16 terr;
    col_i16 village;
    col_i16 tribe;
    col_i16 x;
    col_i16 y;
    col_i16 open;
    col_i16 an;
    register col_i16 bonus;

    x = g_units[u].x;
    y = g_units[u].y;
    an = NATION(g_units[attacker]);
    str = unit_combat_strength(u, 0);
    bonus = 0;
    g_terrain_defence = 0;
    if (square_native_nation(x, y) >= 0) {
        bonus = 2;
        g_defence_village = village = village_at(x, y);
        tribe = g_settlements[village].owner - 4;
        if (g_tribes[tribe].level >= LEVEL_ADVANCED) {
            bonus += 2;
            g_combat_flags[1] |= 0x10;
        }
        if (g_settlements[village].flags & SETTLEMENT_CAPITAL) {
            bonus = bonus * 2;
            g_combat_flags[1] |= 0x20;
        }
        g_combat_flags[1] |= 8;
    } else if (square_european_nation(x, y) >= 0) {
        bonus += 2;
        g_defence_colony = colony_at(x, y);
        set_current_colony(g_defence_colony);
        bonus += colony_fort_level() * 2;
        g_combat_flags[1] |= 0x40;
    } else {
        terr = terrain_at(x, y);
        if (NATION(g_units[u]) >= 4
            || (an < NATION_EUROPEAN_COUNT && ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0 || an >= NATION_EUROPEAN_COUNT || g_nation52[an].control != CONTROL_HUMAN))) {
            bonus += g_terrain_row_bytes[terr][0xb];
            g_combat_flags[1] |= 0x80;
        } else {
            open = 1;
            if (an < NATION_EUROPEAN_COUNT && g_nation52[an].control == CONTROL_HUMAN) {
                if (square_european_nation(x, y) >= 0
                    || square_european_nation(g_units[attacker].x,
                                              g_units[attacker].y) >= 0) {
                    open = 0;
                    bonus += g_terrain_row_bytes[terr][0xb];
                    g_combat_flags[1] |= 0x80;
                }
            } else {
            }
            if (open != 0) {
                g_terrain_defence = g_terrain_row_bytes[terr][0xb];
                g_combat_flags[0] |= 0x80;
            }
        }
    }
    if (g_units[u].orders == ORDERS_FORTIFIED
        && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
        && bonus < 5) {
        bonus += 2;
        g_combat_flags[1] |= 0x2000;
    }
    str = (bonus + 4) * str >> 2;
    return str;
}
