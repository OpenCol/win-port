/* PORT: this file's own declarations of file_MSOpenFile_1038_e492 removed: the header declares it from the definition (W1 step 3). */
/* map_startup: candidate for 1038:d272 (276 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: map_startup
 *
 * Sizes the map and allocates its four planes. The defaults are written FIRST -- 120 x 75,
 * Colonization's standard map -- and a file is then allowed to overwrite them: four bytes at
 * SEG27:0x3c's name give a new width and height, each passed through
 * `swap_word_identity_1038_e4e2`, and the cell count is recomputed. Any failure on that path
 * simply leaves the defaults standing, which is why the file is opened, read and closed
 * without a single error report.
 *
 * THE HEADER IS READ AS STORED. The two words pass through a swap hook kept from the
 * Macintosh original, but the swapper is an identity function here, so nothing is swapped;
 * it is one of the pair this module keeps for exactly that reason.
 *
 * `allocate_map_memory` returns 0 for SUCCESS, so the error variable is cleared in the `else`.
 * A failure stores 0x13 at DGROUP:0x1c68 and returns 1.
 *
 * DGROUP:0x1ca2 gates the whole file path and NOTHING IN THE IMAGE WRITES IT -- it is zero
 * for the life of the process, so the read always happens and always finds nothing, because
 * the name at SEG27:0x3c is an empty string in the shipped data too. A custom-map loader
 * left connected but with no way to switch it on.
 *
 * `map_check()` is tested with the two-jump form, so the source is `goto` and
 * not a positive `if` around the body (rule 12); it also returns 0 unconditionally, so the
 * allocation always runs.
 *
 * `err` is initialised in its DECLARATION -- the `MOV SI,1` lands before the `6` that arms
 * the exception frame, which is before the DiskFile is constructed. `fh` is NOT initialised,
 * and when 0x1ca2 is non-zero the close at the end tests a register that was never written.
 * That is the shipped code.
 *
 * RENAMED 2026-09-27 from map_load: Mac map_startup(short). The Mac body is the same program: DiskFile, MSOpenFile(map name), the defaults 0x78 x 0x4b written before the read, MSReadFile of 4 bytes, map_check, allocate_map_memory, error 0x13 on failure, MSCloseFile. The Windows file writes 120 x 75, reads 4 bytes, return_zero, map_planes_alloc, sets 0x13 and closes. The 1244/276 size ratio is Mac exception scaffolding, not logic. Caller: start_new_game on both. Arity: 1 (int big). Layout: Mac CODE 20 allocate_map_memory, map_check, load_map_file, map_startup; Windows 1038 map_planes_alloc, return_one_1038_cf4b (0 callers, Windows-only), return_zero_1038_cf6e, map_read_planes, [close_log_file, map_alloc: Windows-only, 0 callers], map_load.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

extern MapSize g_map_size;   /* DGROUP:0x4c40 */

/* same object: 4-byte `push cs; call ` */
extern "C" int read_file_1038_e4b8(MSFile *file, void *buf, long n);

extern "C" int map_check(void);
extern "C" int allocate_map_memory(int big);

int map_startup(int big)
{
    register int err = 1;
    register int fh;
    DiskFile f;

    if (g_no_map_file == 0) {
        fh = file_MSOpenFile_1038_e492((MSFile *)&f, g_map_file_name);
        g_map_size.w = 120;
        g_map_size.h = 75;
        g_map_cells = (long)g_map_size.w * g_map_size.h;
        if (fh != 0) {
            if (read_file_1038_e4b8((MSFile *)&f, &g_map_size.w, 4L) != 0) {
                g_map_size.w = swap_word_identity_1038_e4e2(g_map_size.w);
                g_map_size.h = swap_word_identity_1038_e4e2(g_map_size.h);
                g_map_cells = (long)g_map_size.w * g_map_size.h;
            }
        }
    }
    if (map_check() != 0)
        goto out;
    if (allocate_map_memory(big) != 0)
        g_map_load_error = 0x13;
    else
        err = 0;
out:
    if (fh != 0)
        MSCloseFile_1038_e4f7(&f);
    return err;
}

