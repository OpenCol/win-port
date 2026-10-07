/* europe_area_hit_region: candidate for 1048:7653 (218 bytes, 0 callers).  Verdict: RELOCATABLE.
 *
 * Which of seven fixed rectangles the pointer is in, as a region code: 11, 5, 0, 1, 2, 3
 * or 4 in test order, 15 when none. No caller in the shipped binary.
 *
 * RENAMED 2026-09-26 from main_screen_hit_region: Returns a region code for which of seven fixed rectangles is hit (11, 5, 0..4, else 15). Its rectangles 0..4 are exactly the Europe window's inMarketBox..4 rectangles (e.g. region 0 = x 0..0x262, y 0x198..0x1c9; region 1 = 0x11c..0x1bf x 0x102..), and it sits in the Europe module next to europe_new_wars, i.e. a dead (0 callers, stubbed test) variant of EuropeMouseArea. 'main_screen' names the wrong window.
 */

#include "colonization.h"

int europe_area_hit_region(void)
{
    int r = 15;

    if (empty_1040_7d3b(0x131, 0x99, 0xf, 0x15))
        r = 11;
    else if (empty_1040_7d3b(0x119, 0x3f, 0x25, 0x20))
        r = 5;
    else if (empty_1040_7d3b(0, 0x198, 0x262, 0x31))
        r = 0;
    else if (empty_1040_7d3b(0x11c, 0x102, 0xa3, 0x90))
        r = 1;
    else if (empty_1040_7d3b(0x8f, 0x102, 0x8b, 0x79))
        r = 2;
    else if (empty_1040_7d3b(2, 0x102, 0x8b, 0x79))
        r = 3;
    else if (empty_1040_7d3b(0x1bf, 0x102, 0xc1, 0x93))
        r = 4;
    return r;
}
