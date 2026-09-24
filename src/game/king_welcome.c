/* king_welcome -- shipped at 1048:6b6f.
 *
 * Ported by tools/port.py from matched/game/king_welcome.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_difficulty_words[];                 SEG20:0xd8fe
 *   extern char far g_nation_text[][0x34];               SEG20:0x7810
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern char g_text_king_welcome[];                   DGROUP:0x2146
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ask_text_resource(char *name);
void dialog_set_option_text(col_i16 i, char *s);

extern void set_name_slot_from_text(col_i16 a, col_i16 word);

/* same object: 4-byte `push cs; call ` (rule 47) */



void king_welcome(void)
{
    set_name_slot_from_text(0, g_difficulty_words[g_difficulty]);
    dialog_set_option_text(1, (&g_nation_text[g_cur_nation]));
    ask_text_resource(g_text_king_welcome);
}
