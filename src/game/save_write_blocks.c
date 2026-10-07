/* save_write_blocks: candidate for 1038:ca86 (167 bytes, 0 callers).  Verdict: RELOCATABLE.
 *
 * Writes three blocks -- two of 0x10e bytes from segment 27 and one of 0x20 from
 * segment 20 (SEG20:0xda50) -- with fwrite, each failure a `goto`, and closes the file. The file
 * pointer is initialised to NULL and never opened, so every call takes the first exit:
 * dead code left from a removed fopen. No caller in the shipped binary.
 */

#include "colonization.h"

int save_write_blocks(void)
{
    int err = 1;
    FILE *fp = 0;

    if (fp == 0)
        goto done;
    if (fwrite_rtl(g_coarse_grid_land, 0x10e, 1, fp) == 0)
        goto done;
    if (fwrite_rtl(g_coarse_grid_sea, 0x10e, 1, fp) == 0)
        goto done;
    if (fwrite_rtl(g_region_farmland, 0x20, 1, fp) == 0)
        goto done;
    err = 0;
done:
    if (fp != 0)
        FCLOSE(fp);
    return err;
}
