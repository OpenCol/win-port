/* EventWin_ctor_1050_956d: candidate for 1050:956d (207 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventWin::EventWin()
 *
 * The constructor of EventWin (0x8c bytes) as compiled into module 1050: Screen base (13
 * bytes) constructed through the shared Screen::Screen(), the EventTable member at +0xd
 * through the module's own copy, then three longs and nine shorts initialised; the byte at +0x69 is not written. Base and
 * member constructor calls are the compiler's.
 */

#include "colonization.h"

#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

/* PORT: this copy of EventWin::EventWin removed: src/game/EventWin_ctor_1008_d500.c defines it once (W2). */
