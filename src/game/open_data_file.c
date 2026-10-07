/* open_data_file: candidate for 1040:5ece (79 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * fopen of a name resolved through make_data_path, in the given mode.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

FILE * open_data_file(char *name, char *mode)
{
    FILE *f;
    char path[0x84];

    make_data_path(path, name);
    f = fopen_rtl(path, mode);
    return f;
}
