/* MSDisposeButton -- shipped at 1070:1d94.
 *
 * Ported by tools/port.py from matched/platform/MSDisposeButton.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_969a(void *ctx, char *msg);

void MSDisposeButton(col_u16 h)
{
    if (h != 0)
        ;
    else
        empty_1070_969a(&(*g_error_ctx), "Error: Tried to dispose NULL Button in MSDisposeButton");
}
