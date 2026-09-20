/* sort_pairs_by_key -- shipped at 1040:7560.
 *
 * Ported by tools/port.py from matched/game/sort_pairs_by_key.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void sort_pairs_by_key(col_i16 n, char *a, char *b)
{
    col_i16 swapped = 1;
    register col_i16 j;
    register char t;

    do {
        swapped = 0;
        for (j = 0; !swapped && j < n - 1; j++) {
            if (b[j] > b[j + 1]) {
                t = b[j];
                b[j] = b[j + 1];
                b[j + 1] = t;
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                swapped = 1;
            }
        }
    } while (swapped);
}
