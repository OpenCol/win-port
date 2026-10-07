/* PORT: this file's own declarations of wing_grab_from_window removed: the header declares it from the definition (W1 step 3). */
/* port_grab_from_window: candidate for 1068:b65b (71 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_grab_from_window
 * name-source: descriptive
 *
 * Grabs the rectangle at +0x10 off the screen and into the port's WinG surface:
 * `wing_grab_from_window` takes the destination surface (the pointer at
 * +0x22, the port's window), the window to read from (the pointer at +6 of the object
 * at +0x26) and the rectangle.
 *
 * The rectangle is the port's SECOND one. A GRPort's drawing rectangle is at +8; this
 * reaches for +0x10, the eight bytes immediately after it, which `MSLockGraf`'s reading of
 * the record leaves unnamed.
 *
 * The three accessors are reached by the four-byte `PUSH CS / CALL NEAR` form, so all three
 * are in this translation unit and appear here as stubs (rule 47); the grab is a call
 * and is `extern`. They are evaluated right to left, which is why the rectangle is fetched
 * first and the destination surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern "C" RECT *port_bounds(GRPort *p);
extern "C" void *get_win_1068_d72a(Screen *o);
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_grab_from_window(MagicWin *p)
{
    wing_grab_from_window(GRPort_GetWin_1068_d91d(p),
                                    (MSWin *)(get_win_1068_d72a(&(DialogWin &)*p)),
                                    port_bounds(p));
}
