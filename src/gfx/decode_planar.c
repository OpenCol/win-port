/* decode_planar: candidate for 1068:3dc5 (507 bytes).  Verdict: RELOCATABLE.
 *
 * Decodes one row of a Macintosh image. In byte mode the PackBits decoder is repeated
 * inline (the same code as unpack_bits, mode 0 raw and mode 1 packed). Otherwise the row
 * is cleared and each of `depth` bitplanes is unpacked into a 0x50-byte buffer and OR'd
 * into the row bit by bit, MSB first, 1 << plane; a mask plane (byte +9 == 1) is unpacked
 * and discarded. One int and one register hold the run count / current byte and the
 * repeat value / plane index in the two branches.
 */

#include "colonization.h"

#include "records.h"

extern void unpack_bits(unsigned char *dst, Bmhd *h, unsigned char *src, short len);

void decode_planar(unsigned char *dst, Bmhd *h, unsigned char *src, short len, int bytemode)
{
    int v;
    int bit;
    unsigned char *p;
    unsigned char buf[0x50];
    int i;
    int run;

    if (bytemode != 0) {
        i = 0;
        switch (h->compression) {
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
        for (i = 0; (int)h->w > i; i++)
            dst[i] = 0;
        for (v = 0; h->nplanes > v; v++) {
            unpack_bits(buf, h, src, len);
            p = buf;
            bit = 1 << v;
            for (i = 0; (int)h->w > i; i++) {
                if (i % 8 == 0) {
                    run = *p;
                    p++;
                }
                if (run & 0x80)
                    dst[i] |= bit;
                run += run;
            }
        }
        if (h->masking == 1)
            unpack_bits(buf, h, src, len);
    }
}
