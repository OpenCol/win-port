/* set_check_color_on_1010_e423: candidate for 1010:e423 (27 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores a byte into DGROUP:0x2add, the first of the two colour bytes beside
 * `g_line_color_index` at 0x2ada that `ColonyWin_ColonyClickUnits` also writes. Its twin at 1010:e43e
 * writes 0x2ade, and 1008:d086 / 1008:d0a1 are a SECOND pair of setters for the same two
 * bytes, which is why this one keeps its address.
 *
 * `checkbox_wndproc` is what says the bytes are PALETTE INDEXES and not flags: it passes
 * 0x2add to `palette_get_entry_rgb` and hands the result to `SetTextColor` when the item is
 * enabled, and 0x2ade when it is not, with 0 meaning plain black. `radio_wndproc` does the
 * same with 0x2adb and 0x2adc.
 *
 * "on" and "off" here are that enabled/disabled pair. The GLOBALS spell it out and the
 * FUNCTIONS cannot: a stem over 32 characters is truncated in the PUBDEF and the symbol is
 * then never found, and `set_check_color_disabled_1010_e43e` is 34, which graded DIFFERS for
 * that alone. All four setters use the short form so that the two pairs share a stem and the
 * address suffix on this one still distinguishes something (check_naming counts a suffix as
 * redundant when nothing else carries the stem).
 *
 * The explicit `MOV AX,<seg> / MOV ES,AX` before the store is what a ``-qualified global
 * produces in a -WS build.
 */

#include "colonization.h"

void set_check_color_on_1010_e423(char color)
{
    g_check_color_enabled = color;
}
