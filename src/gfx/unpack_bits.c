/* unpack_bits -- shipped at 1068:3fc0.
 *
 * Ported by tools/port.py from matched/gfx/unpack_bits.c.
 *
 * The file's own Hdr dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void unpack_bits(unsigned char *dst, Hdr *h, unsigned char *src, col_i16 len)
{
    col_i16 i = 0;
    col_i16 n;
    col_i16 v;

    switch (h->mode) {
    case 0:
        for (; i < len; i++)
            dst[i] = *src++;
        break;
    case 1:
        while (i < len) {
            n = (signed char)*src++;
            if (n >= 0) {
                while (n-- >= 0)
                    dst[i++] = *src++;
            } else if (n >= -127) {
                v = (signed char)*src++;
                while (n++ <= 0)
                    dst[i++] = v;
            } else
                ;
        }
        break;
    }
}
