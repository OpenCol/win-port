/* show_nation_message -- shipped at 1048:1a8c.
 *
 * Ported by tools/port.py from matched/game/show_nation_message.c.
 *
 * The file's own Message dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_CancelMessage(Message *m, col_i16 a);
void msg_add_nation_word(Message *m, col_i16 word);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void msg_activate_redraw(Message *m, col_i16 kind, col_i32 arg);

/* `set_current_nation` is the 5-byte `NOP; PUSH CS; CALL ` form -- TLINK converting a
 * call -- so it stays `extern`; an in-file definition emits the 4-byte form and misses.
 * `msg_add_nation_word` is the 4-byte form and is stubbed. */
extern void set_current_nation(col_i16 n);


void show_nation_message(Message *m, col_i16 word, col_i32 arg)
{
    set_current_nation(m->nation);
    ColChild_CancelMessage(m, 1);
    msg_add_nation_word(m, word);
    msg_activate_redraw(m, 3, arg);
}
