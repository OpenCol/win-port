/* text_message_show -- shipped at 1050:64d1.
 *
 * Ported by tools/port.py from matched/game/text_message_show.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_ctx[];                        DGROUP:0x009e
 *   extern int      g_text_value;                        DGROUP:0x251a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 sub_1050_8e85(void *a, void *b, col_i16 c, col_i16 d);

/* same object: the shipped reaches this through the 4-byte `PUSH CS / CALL NEAR`, which is
 * the form Borland emits for a callee in the SAME translation unit -- so it is defined
 * here and not declared extern (rule 47). */


col_i16 text_message_show(void *p, col_i16 value)
{
    g_text_value = value;
    return sub_1050_8e85(g_text_ctx, p, 0, 0);
}
