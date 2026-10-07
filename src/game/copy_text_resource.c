/* copy_text_resource: candidate for 1040:7a2a (184 bytes).  Verdict: RELOCATABLE.
 *
 * Copies up to n bytes of the named resource into dst: the name goes into a
 * 32-byte stack buffer, the resource is found with the key at DGROUP:0x1cff,
 * locked, copied with STRNCPY, unlocked and freed.  0 when not found.
 *
 * RENAMED 2026-09-26 from load_text_resource_1040_7a2a: it copies up to n bytes of a named resource into a caller buffer (find, lock, STRNCPY, unlock, free); the address suffix only told it apart from 1040:6cf4, which is now text_open, the Mac build's name for the reader this was never a copy of.
 */

#include "colonization.h"

/* The resource type is a 4-character code, a Mac ResType, DEFINED as `char[4]`: segment 9's
   _DATA holds 'STRN' and 'TEXT' at odd addresses (0x1cfb, 0x1cff), where Borland 4.52 would
   word-align a long or a pointer (tools/check_module_data.py, 2026-10-02). It is read whole. */

extern void *MSLockResource(void *h);

extern void MSUnlockResource(void *h);
extern void MSReleaseResource(void *h);

int copy_text_resource(char *name, char *dst, int n)
{
    void *h;
    char *p;
    long key = *(long *)g_restype_text_copy;
    char buf[32];

    memcpy_rtl(buf, name, STRLEN(name) + 1);
    h = MSGetNamedResource((unsigned char *)&key, buf);
    if (h == 0)
        return 0;
    p = (char *)MSLockResource(h);
    STRNCPY(dst, p, n);
    MSUnlockResource(h);
    MSReleaseResource(h);
    return 1;
}
