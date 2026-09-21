/* MSWriteFile -- shipped at 1070:80db.
 *
 * Ported by tools/port.py from matched/platform/MSWriteFile.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_969a(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal _hwrite(col_i16 h, void *buf, col_i32 n);
#endif

/* OFSTRUCT at +4 */

/* same object: 4-byte `push cs; call ` (rule 47) */



col_i16 MSWriteFile(MSFile *f, void *buf, col_i32 n)
{
    if (f->handle == 0)
        return 0;
    if (f->writable == 0) {
        empty_1070_969a(&(*g_error_ctx), "Error: Tried to write to a read only file.");
        return 0;
    }
    if (_hwrite(f->handle, buf, n) == -1L)
        return 0;
    else
        return 1;
}
