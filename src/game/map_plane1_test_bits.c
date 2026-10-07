/* map_plane1_test_bits: candidate for 1018:6417 (83 bytes).  Verdict: RELOCATABLE.
 *
 * 1 when (x, y) is on the map and the plane-1 byte there has any of the mask bits.
 */

#include "colonization.h"

extern unsigned char feature_get(int x, int y);

int map_plane1_test_bits(int x, int y, unsigned char mask)
{
    int r = 0;

    if (!on_map(x, y))
        goto done;
    if (feature_get(x, y) & mask)
        r = 1;
done:
    return r;
}
