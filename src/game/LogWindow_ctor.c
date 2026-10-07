/* LogWindow_ctor: candidate for 1060:a847 (53 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: LogWindow::LogWindow(char*)
 *
 * An empty constructor for a 2-byte object with a destructor. The object is the game's
 * warning sink: its one caller, `create_log_window`, builds it at DGROUP:0x2ac0,
 * which is the address `GRPort_CopyToPort` already calls `g_error_log` and hands to
 * `report_error_1068_963d` alongside "Warning: Source Port not locked in CopyToPort".
 *
 * THE PARAMETER IS REAL AND THE BODY IGNORES IT. This function reads only `[bp+6]`, so its
 * own bytes are exactly those of a constructor with no parameters -- §119's blind spot.
 * The caller settles it: it pushes eight bytes and clears eight, which is `this` and one
 * more pointer. A parameter that is never read costs nothing here and one `PUSH DS /
 * PUSH offset` pair there, and that pair is in the shipped bytes.
 */

#include "colonization.h"

#include "classes.h"

LogWindow::LogWindow(char *name)
{
}
