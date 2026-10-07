/* mask_runs_fill: candidate for 1070:8ba7 (293 bytes).  Verdict: RELOCATABLE.
 *
 * Fills a mask from signed run bytes, one source row per destination row of `stride`
 * bytes: a non-negative n writes n bytes of 0xff and skips a source byte; a negative n
 * writes -n bytes of 0xff consuming a source byte each. A row ends when the running
 * width reaches `width`.
 */

#include "colonization.h"

void mask_runs_fill(unsigned char *dst, unsigned char *src, int width, int rows, int stride)
{
    int ww = width;
    int r = rows;
    signed char n;
    unsigned char count;
    int i = 0;
    int x = ww;
    unsigned char *p;
    int w;

    while (--r >= 0) {
        p = dst + (long)i * stride;
        w = n = count = 0;
        src++;
        while ((w += n) < x) {
            n = count = *src;
            src++;
            if ((int)n >= 0) {
                while (count-- != 0) {
                    *p = 0xff;
                    p++;
                }
                src++;
            } else {
                n = count = -n;
                while (count-- != 0) {
                    *p = 0xff;
                    p++;
                    src++;
                }
            }
        }
        i++;
    }
}
