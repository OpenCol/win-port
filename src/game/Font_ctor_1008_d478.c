/* Font_ctor_1008_d478: candidate for 1008:d478 (61 bytes, 9 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::Font()
 *
 * The constructor beside Font_dtor_1008_d4b5: a 4-byte object whose font handle is zeroed.
 */

#include "colonization.h"

#include "classes.h"

Font::Font()
{
    hfont = 0;
}
