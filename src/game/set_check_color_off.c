/* set_check_color_off: candidate for 1008:d0a1 (27 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores one byte at SEG34:0x2ade, one of FOUR identical setters back to back at 1008:d050,
 * d06b, d086 and d0a1 writing 0x2adb, 0x2adc, 0x2add and 0x2ade. The four are flags the text
 * layer tests beside its font slots: `radio_wndproc` reads 0x2adb and 0x2adc,
 * `checkbox_wndproc` reads 0x2add and 0x2ade, and each is a `CMP BYTE ...,0` before the
 * value is used.
 *
 * The parameter is passed as a word and only its low half is stored (`MOV DL,[bp+6]`), which
 * is a `char` parameter and §119's width blind spot the other way round: here the callee's
 * own bytes settle it.
 */

#include "colonization.h"

void set_check_color_off(char disabled)
{
    g_check_color_disabled = disabled;
}
