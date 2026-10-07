/* install_fonts: candidate for 1078:1512 (159 bytes).  Verdict: RELOCATABLE.
 *
 * AddFontResource(name); when it adds fonts, a 0x186-byte record is allocated holding the
 * file name and its first three characters at +0x102, the families are enumerated into it
 * (the check against the resource count, `!= n`, is an empty `if`), and the handle returned; 0 when
 * nothing was added.
 */

#include "colonization.h"

#include "records.h"
extern int enumerate_font_families(FontFile *lparam);

unsigned install_fonts(const char *name)
{
    unsigned h = 0;
    char *f;
    int n;

    if ((n = AddFontResource(name)) != 0) {
        h = MSAllocateHandle(0x186L);
        f = (char *)MSHLock(h);
        STRCPY(f, name);
        STRNCPY(f + 0x102, name, 3);
        if (enumerate_font_families((FontFile *)f) != n)
            ;
        MSHUnlock(h);
    } else
        ;
    return h;
}
