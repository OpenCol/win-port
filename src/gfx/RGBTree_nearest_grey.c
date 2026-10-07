/* RGBTree_nearest_grey: candidate for 1068:1a0b (262 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The seed RGBTree_build starts from: scans the packed RGB triples and returns the index
 * of the entry nearest mid-grey (0x80, 0x80, 0x80), squared distance in 32-bit, initial
 * best 0x400. Takes the tree as `this` but reads only its count. Previously named
 * nearest_palette_entry.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

short RGBTree_nearest_grey(RGBTree *tree, unsigned char *pal)
{
    short i;
    short r;
    short g;
    short b;
    short best;
    long dist;
    long best_dist = 0x400L;

    for (i = 0; tree->count * 3 > i; i += 3) {
        r = pal[i];
        g = pal[i + 1];
        b = pal[i + 2];
        dist = (long)(r - 0x80) * (r - 0x80) + (long)(g - 0x80) * (g - 0x80)
             + (long)(b - 0x80) * (b - 0x80);
        if (dist < best_dist) {
            best_dist = dist;
            best = i / 3;
        }
    }
    return best;
}
