/* read_text_line: candidate for 1070:81d5 (202 bytes).  Verdict: RELOCATABLE.
 *
 * Reads one line from the file a byte at a time into out (at most max characters, CR
 * dropped, stopped by LF or end of file) and NUL-terminates it. Returns the last read's
 * status, 0 for no handle.
 */

#include "colonization.h"

#include "records.h"

int read_text_line(MSFile *file, char *out, int max)
{
    int n = 0;
    int ok;
    char ch;

    *out = 0;
    if (file->handle == 0)
        return 0;
    ok = _hread(file->handle, &ch, 1L) != -1L;
    while (ok && (int)ch != '\n' && n < max) {
        if ((int)ch != '\r') {
            *out = ch;
            out++;
            n++;
        }
        ok = _hread(file->handle, &ch, 1L) != -1L;
    }
    *out = 0;
    return ok;
}
