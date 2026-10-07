/* make_data_path: candidate for 1040:5e7f (79 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * dst = name, or the data directory joined with name when the directory flag is set.
 * Returns dst.
 */

#include "colonization.h"

extern char g_data_dir[];   /* SEG29:0x0000 */

char * make_data_path(char *dst, char *name)
{
    if (g_use_data_dir == 0)
        STRCPY(dst, name);
    else
        join_path(dst, g_data_dir, name);
    return dst;
}
