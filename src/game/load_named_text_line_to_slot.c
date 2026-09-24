/* load_named_text_line_to_slot -- shipped at 1030:4b8b.
 *
 * Ported by tools/port.py from matched/game/load_named_text_line_to_slot.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern char g_text_prefix[];                         DGROUP:0x14fa
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot(col_i16 i, char *s);

extern col_i16 load_text_resource(void *ctx, char *name);
extern char * next_text_line(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void empty_1040_6ce6(void);



void load_named_text_line_to_slot(col_i16 slot, char *name, col_i16 n)
{
    col_i16 i;
    char *line;
    char buf[0x50];

    line = buf;
    strcpy(buf, g_text_prefix);
    strcat(buf, name);
    if (load_text_resource(&(*g_text_ctx), buf))
        goto done;
    for (i = 0; i <= n; i++)
        line = next_text_line();
done:
    set_name_slot(slot, line);
    empty_1040_6ce6();
}
