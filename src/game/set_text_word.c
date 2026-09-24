/* set_text_word -- shipped at 1050:5f55.
 *
 * Ported by tools/port.py from matched/game/set_text_word.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot_from_text(col_i16 slot, col_i16 word);

void set_text_word(col_i16 slot, col_i16 i)
{
    set_name_slot_from_text(slot, g_misc_words[i]);
}
