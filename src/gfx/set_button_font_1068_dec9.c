/* set_button_font_1068_dec9: candidate for 1068:dec9 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Segment 1068's copy of `set_button_font_1050_85b5`: stores a 32-bit value into the text
 * layer's font slot at SEG34:0x2aca. The pair here writes 0x2aca and 0x2ace, where the
 * segment-1050 run of four writes 0x2ace, 0x2aca, 0x2ad2 and 0x2ad6.
 *
 * The store is one 32-bit `MOV` through ES, which is what any ``-qualified four-byte
 * object produces in a -WS build -- a `long` and a pointer alike, which is why this file
 * said `long` until 2026-09-24 and nothing objected. `MSInitButton` is the only reader and it
 * hands the value to `font_handle_1070_9806`, so the object is a pointer and is spelled one
 * here now.
 */

#include "colonization.h"

void set_button_font_1068_dec9(void *font)
{
    g_button_font = font;
}
