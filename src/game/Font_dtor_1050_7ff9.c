/* Font_dtor_1050_7ff9: candidate for 1050:7ff9 (75 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::~Font()
 *
 * The destructor of Font, as compiled into module 1050: release the font handle at +0
 * when set. A copy of Font_dtor_1008_d4b5.
 */

#include "colonization.h"

#include "classes.h"

Font::~Font()
{
    if (hfont != 0)
        MSReleaseFont(hfont);
}
