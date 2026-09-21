/* MSWriteLine -- shipped at 1070:814d.
 *
 * Ported by tools/port.py from matched/platform/MSWriteLine.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern char g_msg_not_writable[];                    DGROUP:0x3937
 *   extern char g_crlf[];                                DGROUP:0x3962
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MSWriteFile(MSFile *f, char *s, col_u32 n);
void empty_1070_969a(void *ctx, char *msg);

/* same object: 4-byte `push cs; call ` (rule 47) */






col_i16 MSWriteLine(MSFile *f, char *s)
{
    col_i16 r;

    if (f->handle == 0)
        return 0;
    if (f->writable == 0) {
        empty_1070_969a(&(*g_error_ctx), g_msg_not_writable);
        return 0;
    }
    r = MSWriteFile(f, s, (col_u32)strlen(s));
    r &= MSWriteFile(f, g_crlf, 2L);
    return r;
}
