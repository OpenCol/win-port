/* unpack_bits: candidate for 1068:3fc0 (274 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Decodes `len` bytes into dst according to a header's mode byte: 0 is a raw copy, 1 is
 * Macintosh PackBits -- a signed count n means "copy n+1 literal bytes" when n >= 0 and
 * "repeat the next byte 1-n times" when n is in -127..-1; -128 is a no-op.
 */

#include "colonization.h"

#include "records.h"

void unpack_bits(unsigned char *dst, Bmhd *h, unsigned char *src, short len)
{
    short i = 0;
    short n;
    short run_byte;

    switch (h->compression) {
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
                run_byte = (signed char)*src++;
                while (n++ <= 0)
                    dst[i++] = run_byte;
            } else
                ;
        }
        break;
    }
}
