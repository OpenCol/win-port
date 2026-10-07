/* MSWriteFile: candidate for 1070:80db (108 bytes).  Verdict: RELOCATABLE.
 *
 * _hwrite(handle, buf, n); 0 for no handle, for a file not opened for writing (reported:
 * "Error: Tried to write to a read only file.") or on failure, else 1.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_969a(void *ctx, char *msg);

int MSWriteFile(MSFile *file, void *buf, long n)
{
    if (file->handle == 0)
        return 0;
    if (file->writable == 0) {
        report_error_1070_969a(&g_error_log, "Error: Tried to write to a read only file.");
        return 0;
    }
    if (_hwrite(file->handle, buf, n) == -1L)
        return 0;
    else
        return 1;
}
