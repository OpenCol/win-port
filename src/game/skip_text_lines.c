/* skip_text_lines -- shipped at 1040:7087.
 *
 * Ported by tools/port.py from matched/game/skip_text_lines.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * next_text_line(void);
col_i16 load_text_resource(void *ctx, char *name);
void empty_1040_6ce6(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 skip_text_lines(void *ctx, char *name, col_i16 n)
{
    col_i16 err = 1;
    col_i16 i;

    if (load_text_resource(ctx, name))
        goto done;
    for (i = 0; i <= n; i++)
        next_text_line();
    err = 0;
done:
    empty_1040_6ce6();
    return err;
}
