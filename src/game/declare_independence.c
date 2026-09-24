/* declare_independence -- shipped at 1038:7bc2.
 *
 * Ported by tools/port.py from matched/game/declare_independence.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_rebel_sentiment;                    SEG20:0x77d2
 *   extern int far g_player_nation;                      SEG20:0x779a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void declaration_of_independence(void);
void set_text_arg_i_1038_86cc(col_i16 a, col_i16 v);

#include "gameflag.h"



extern col_i16 text_message_show(char *msg, col_i16 n);

extern void set_nation_name_slot(col_i16 slot, col_i16 flag, col_i16 nation);
/* same object: 4-byte `push cs; call ` (rule 47) */

void declare_independence(void)
{
    col_i16 r;

    if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
        text_message_show("ALREADYREVOLUTION", 1);
    } else if (g_rebel_sentiment < 0x32) {
        if (g_rebel_sentiment == 0) {
            text_message_show("NONETOOTORY", 1);
        } else {
            set_text_arg_i_1038_86cc(0, g_rebel_sentiment);
            text_message_show("TOOTORY", 1);
        }
    } else {
        set_nation_name_slot(0, 0, g_player_nation);
        r = text_message_show("DECLARE", 1);
        if (r != 2)
            goto done;
        declaration_of_independence();
    }
done:
    ;
}
