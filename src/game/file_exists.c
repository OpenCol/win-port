/* file_exists: candidate for 1040:5f55 (89 bytes).  Verdict: RELOCATABLE.
 * symbol: file_exists
 *
 * 1 when the data file opens (open_data_file with the mode string at
 * DGROUP:0x1d58); the file is closed again.  open_data_file is -called
 * and defined in-file as a stub.
 */

#include "colonization.h"

extern char g_stub_s[];

int file_exists(char *name)
{
    register int r = 0;
    void *f = 0;

    f = open_data_file(name, "rb");
    if (f != 0) {
        FCLOSE((FILE *)f);
        r = 1;
    }
    return r;
}
