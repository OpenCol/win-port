/* request_or_demand -- shipped at 1030:4c15.
 *
 * Ported by tools/port.py from matched/game/request_or_demand.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern char g_text_request_demand[];                 DGROUP:0x1500
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot(col_i16 i, char *s);

extern col_i16 load_text_resource(void *ctx, char *name);
extern char * next_text_line(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void empty_1040_6ce6(void);



void request_or_demand(col_i16 slot, col_i16 flag)
{
    col_i16 i;
    col_i16 n;
    char *line;
    char buf[0x50];

    line = buf;
    if (load_text_resource(&(*g_text_ctx), g_text_request_demand))
        goto done;
    n = flag ? 1 : 2;
    for (i = 0; i < n; i++)
        line = next_text_line();
done:
    set_name_slot(slot, line);
    empty_1040_6ce6();
}
