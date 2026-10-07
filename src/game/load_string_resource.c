/* load_string_resource: candidate for 1040:78b8 (370 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * size-source: 370, not the census 362. The trailing 8 bytes are a SECOND epilogue, and
 * they are there because the body ends in an explicit `return;` -- Borland emits an
 * epilogue for the return statement AND one for the end of the function (rule 44).
 *
 * Copies string `index` (1-based) of a STR#-style resource into dst as a Pascal string:
 * a big-endian count, then length-prefixed strings walked by their lengths. A missing
 * resource, a negative or too-large index, leaves dst[0] = 0. The exits are `goto`s.
 */

#include "colonization.h"

/* The resource type is a 4-character code, a Mac ResType, DEFINED as `char[4]`: segment 9's
   _DATA holds 'STRN' and 'TEXT' at odd addresses (0x1cfb, 0x1cff), where Borland 4.52 would
   word-align a long or a pointer (tools/check_module_data.py, 2026-10-02). It is read whole. */

extern void *MSGetResource(char *ctx, int id);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *MSLockResource_1040_a99c(void *res);
extern void MSUnlockResource_1040_a9bc(void *res);
extern void MSReleaseResource(void *res);
extern void MSUnlockResource(void *res);

void load_string_resource(unsigned char *dst, int id, int index)
{
    int idx = index;
    void *res;
    unsigned char *p;
    int count;
    unsigned char n;
    unsigned char hi;
    unsigned char lo;
    void *ctx = *(void **)g_restype_strn;

    if (idx < 0)
        goto fail;
    res = MSGetResource((char *)&ctx, id);
    if (res == 0)
        goto fail;
    p = (unsigned char *)MSLockResource_1040_a99c(res);
    hi = *p++;
    lo = *p++;
    count = (hi << 8) + lo;
    if (count < idx) {
        MSUnlockResource_1040_a9bc(res);
        MSReleaseResource(res);
        goto fail;
    }
    idx--;
    while (idx != 0) {
        n = *p++;
        p += n;
        idx--;
    }
    n = *p++;
    *dst = n;
    dst++;
    memcpy_rtl(dst, p, n);
    MSUnlockResource(res);
    MSReleaseResource(res);
    return;
fail:
    *dst = 0;
    return;
}
