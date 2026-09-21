/* msg_add_nation_word -- shipped at 1048:1a52.
 *
 * Ported by tools/port.py from matched/game/msg_add_nation_word.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_colony_message_words[];             SEG20:0xd996
 *
 * The file's own Message dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_AddToMessage(Message *m, col_i16 idx);

extern void set_current_nation(col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */


void msg_add_nation_word(Message *m, col_i16 i)
{
    set_current_nation(m->nation);
    ColChild_AddToMessage(m, g_colony_message_words[i]);
}
