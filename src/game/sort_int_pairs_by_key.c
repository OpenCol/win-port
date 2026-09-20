/* sort_int_pairs_by_key -- shipped at 1040:75f7.
 *
 * Ported by tools/port.py from matched/game/sort_int_pairs_by_key.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void sort_int_pairs_by_key(col_i16 n, col_i16 *payload, col_i16 *key)
{
    col_i16 swapped = 1;
    col_i16 i;
    col_i16 t;

    do {
        swapped = 0;
        for (i = 0; !swapped && i < n - 1; i++) {
            if (key[i] > key[i + 1]) {
                t = key[i];
                key[i] = key[i + 1];
                key[i + 1] = t;
                t = payload[i];
                payload[i] = payload[i + 1];
                payload[i + 1] = t;
                swapped = 1;
            }
        }
    } while (swapped);
}
