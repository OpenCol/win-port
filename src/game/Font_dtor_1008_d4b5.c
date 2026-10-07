/* Font_dtor_1008_d4b5: candidate for 1008:d4b5 (75 bytes, 10 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::~Font()
 *
 * The destructor of Font, as compiled into module 1008: release the font handle at +0
 * when set. Identical copies live in 1050 and 1068.
 */

#include "colonization.h"

#include "classes.h"

Font::~Font()
{
    if (hfont != 0)
        MSReleaseFont(hfont);
}
