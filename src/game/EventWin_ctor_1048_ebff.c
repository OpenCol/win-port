/* EventWin_ctor_1048_ebff: candidate for 1048:ebff (207 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventWin::EventWin()
 *
 * The constructor of EventWin (0x8c bytes) as compiled into module 1048: Screen base (13
 * bytes) constructed through the shared Screen::Screen(), the EventTable member at +0xd
 * through the module's own copy, a byte, then three longs and nine shorts initialised. Base and
 * member constructor calls are the compiler's.
 */

#include "colonization.h"

#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

EventWin::EventWin()
{
    menu = 0;
    menu_handler = 0;
    command_handler = 0;
    min_track_w = 0;
    min_track_h = 0;
    max_track_w = 0x4000;
    max_track_h = 0x4000;
    ldown = 0;
    rdown = 0;
    running = 0;
    hpage = 1;
    vpage = 1;
}
