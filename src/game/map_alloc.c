/* map_alloc: candidate for 1038:d1bd (152 bytes).  Verdict: RELOCATABLE.
 *
 * Records the map's width, height and cell count, allocates the planes, and clears
 * them: plane 0 to 0x19, planes 1 and 2 to 0. Returns 1 when allocation fails, else 0.
 * The failure exit is a `goto`: under -Od an if-body holding a jump compiles to
 * `JZ $+2 / JMP`, which the shipped bytes have.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int map_check(void);

int map_alloc(int w, int h)
{
    int ww = w;
    int hh = h;
    int err = 1;

    g_map_size.w = ww;
    g_map_size.h = hh;
    g_map_cells = (long)ww * hh;
    if (map_check())
        goto done;
    memset_rtl(g_plane0, 0x19, (unsigned)g_map_cells);
    memset_rtl(g_plane1, 0, (unsigned)g_map_cells);
    memset_rtl(g_plane2, 0, (unsigned)g_map_cells);
    err = 0;
done:
    return err;
}
