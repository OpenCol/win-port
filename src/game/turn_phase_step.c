/* turn_phase_step -- shipped at 1008:3461.
 *
 * Ported by tools/port.py from matched/game/turn_phase_step.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern int far g_message_pending;                    SEG20:0x77c8
 *   extern int far g_quit_flag;                          DGROUP:0x4b04
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: step -> hwnd
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void advance_turn(Ctl *c);
void begin_nation_turn(Ctl *c, col_i16 n);
void player_turn_step(Ctl *c);
void turn_step_units(Ctl *c);

/* same object: 4-byte `push cs; call ` (rule 47) */





void turn_phase_step(Ctl *c)
{
    if (g_game_in_progress != 0) {
        while (c->done == 0 && g_game_in_progress != 0) {
            switch (c->hwnd) {
            case 0:
                turn_step_units(c);
                c->hwnd++;
                break;
            case 1:
                begin_nation_turn(c, 0);
                c->hwnd++;
                break;
            case 2:
                begin_nation_turn(c, 1);
                c->hwnd++;
                break;
            case 3:
                begin_nation_turn(c, 2);
                c->hwnd++;
                break;
            case 4:
                begin_nation_turn(c, 3);
                c->hwnd++;
                break;
            case 5:
                advance_turn(c);
                g_message_pending = 0;
                c->hwnd = 0;
                break;
            }
        }
        if (g_game_in_progress != 0)
            player_turn_step(c);
    } else {
        g_quit_flag = 1;
    }
}
