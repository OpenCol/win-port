/* close_log_file: candidate for 1038:d175 (72 bytes).  Verdict: RELOCATABLE.
 *
 * Leftover of a log-file closer: the result starts at 1 and is at once set
 * to 0, the file pointer is null, DGROUP:0x1c68 is cleared, and FCLOSE
 * would run on a non-null pointer.  Returns 0.
 */

#include "colonization.h"

int close_log_file(void)
{
    register int r = 1;
    void *f = 0;

    r = 0;
    g_map_load_error = 0;
    if (f != 0)
        FCLOSE((FILE *)f);
    return r;
}
