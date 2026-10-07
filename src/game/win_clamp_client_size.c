/* win_clamp_client_size: candidate for 1060:a395 (135 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Clamps a proposed client size into the window's own minimum and maximum and applies it.
 * `get_min_track_size_1060_af51` supplies the lower pair and `get_max_track_size_1060_af79` the
 * upper, and each axis is then squeezed from below and above in turn before
 * `win_set_client_size_1060_aecf` gets the result.
 *
 * Four clamps, all written as bare `if`s with no `else` -- `if (w < lo) w = lo; if (w > hi)
 * w = hi;` -- which is why each is a direct conditional jump over a single `MOV` and not a
 * call to a `min`/`max` helper. This module has both helpers and does not use them here.
 *
 * The two axes live in SI and DI for the whole function, which is what makes the four
 * comparisons register-to-memory and the four assignments memory-to-register.
 */

#include "colonization.h"

#include "classes.h"
extern void get_min_track_size_1060_af51(void *r, int *pa, int *pb);
extern void get_max_track_size_1060_af79(void *r, int *pa, int *pb);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void win_set_client_size_1060_aecf(void *r, int cw, int ch);
void win_clamp_client_size(GameWin *o, register int w, register int h)
{
    int minw, minh;
    int maxw, maxh;

    get_min_track_size_1060_af51((EventWin *)(((char *)o + 0x26)), &minw, &minh);
    get_max_track_size_1060_af79((EventWin *)(((char *)o + 0x26)), &maxw, &maxh);
    if (w < minw)
        w = minw;
    if (w > maxw)
        w = maxw;
    if (h < minh)
        h = minh;
    if (h > maxh)
        h = maxh;
    win_set_client_size_1060_aecf((DialogWin *)(((char *)o + 0x26)), w, h);
}
