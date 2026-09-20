/* decode_planar -- shipped at 1068:3dc5.
 *
 * Ported by tools/port.py from matched/gfx/decode_planar.c.
 *
 * The file's own Hdr dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void unpack_bits(unsigned char *dst, Hdr *h, unsigned char *src, col_i16 len);

void decode_planar(unsigned char *dst, Hdr *h, unsigned char *src, col_i16 len, col_i16 bytemode)
{
    col_i16 v;
    col_i16 bit;
    unsigned char *p;
    unsigned char buf[0x50];
    col_i16 i;
    col_i16 run;

    if (bytemode != 0) {
        i = 0;
        switch (h->mode) {
        case 0:
            for (; i < len; i++)
                dst[i] = *src++;
            break;
        case 1:
            while (i < len) {
                run = (signed char)*src++;
                if (run >= 0) {
                    while (run-- >= 0)
                        dst[i++] = *src++;
                } else if (run >= -127) {
                    v = (signed char)*src++;
                    while (run++ <= 0)
                        dst[i++] = v;
                } else
                    ;
            }
            break;
        }
    } else {
        for (i = 0; h->width > i; i++)
            dst[i] = 0;
        for (v = 0; h->depth > v; v++) {
            unpack_bits(buf, h, src, len);
            p = buf;
            bit = 1 << v;
            for (i = 0; h->width > i; i++) {
                if (i % 8 == 0) {
                    run = *p;
                    p++;
                }
                if (run & 0x80)
                    dst[i] |= bit;
                run += run;
            }
        }
        if (h->has_mask == 1)
            unpack_bits(buf, h, src, len);
    }
}
