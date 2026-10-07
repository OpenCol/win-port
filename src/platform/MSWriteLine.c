/* MSWriteLine: candidate for 1070:814d (136 bytes).  Verdict: RELOCATABLE.
 *
 * Writes a string and a CR/LF to an MSFile; 0 when the file has no handle or is not open
 * for writing (reported), else the two write results ANDed.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSWriteFile(MSFile *file, char *buf, unsigned long n);
extern void report_error_1070_969a(void *ctx, char *msg);

int MSWriteLine(MSFile *file, char *s)
{
    int r;

    if (file->handle == 0)
        return 0;
    if (file->writable == 0) {
        report_error_1070_969a(&g_error_log, "Error: Tried to write to a read only file.");
        return 0;
    }
    r = MSWriteFile(file, s, (unsigned long)STRLEN(s));
    r &= MSWriteFile(file, "\r\n", 2L);
    return r;
}
