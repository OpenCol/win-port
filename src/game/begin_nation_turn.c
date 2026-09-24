/* begin_nation_turn -- shipped at 1008:2be6.
 *
 * Ported by tools/port.py from matched/game/begin_nation_turn.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern char far g_loaded_flag;                       DGROUP:0x008b
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern unsigned char far g_flags_7785;               SEG20:0x7785
 *   extern char far g_info_win[];                        SEG20:0x1566
 *
 * The file's own NationRec52, Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void autosave_to_slot(void);

#include "control.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern void nation_turn(void);
extern void window_relayout(char *w);
extern void ai_nation_turn(col_i16 n);

/* same object: 4-byte `push cs; call ` */


void begin_nation_turn(Ctl *c, register col_i16 n)
{
    if (g_loaded_flag != 0 && g_turn_nation > n)
        goto out;
    g_turn_nation = n;
    if (g_loaded_flag == 0 && g_nation52[n].control != CONTROL_DEFEATED)
        nation_turn();
    if (g_nation52[n].control == CONTROL_AI) {
        if (g_view_nation == n || g_reveal_all != 0)
            window_relayout(g_info_win);
        if (g_loaded_flag == 0 && g_player_nation == n && (g_flags_7785 & 4))
            autosave_to_slot();
        ai_nation_turn(n);
    } else if (g_nation52[n].control == CONTROL_HUMAN) {
        c->done = 1;
        c->f4 = 0x64;
        c->f8 = n;
    }
out:
    ;
}
