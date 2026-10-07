/* Font_ctor_1068_dd36: candidate for 1068:dd36 (61 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::Font()
 *
 * The constructor beside Font_dtor_1068_dd73: a 4-byte object whose font handle is zeroed.
 */

#include "colonization.h"

#include "classes.h"

Font::Font()
{
    hfont = 0;
}
