/* create_log_window: candidate for 1060:b0f6 (69 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STCON_()
 * name-source: descriptive
 *
 * Constructs module 1060's two file-scope objects: a LogWindow at DGROUP:0x2ac0 and a Font
 * at DGROUP:0x4e68. `destroy_log_window` at the next address tears both down in
 * reverse order.
 *
 * DGROUP:0x2ac0 is the address `GRPort_CopyToPort` already calls `g_error_log` -- the context it
 * hands `report_error_1068_963d` alongside "Warning: Source Port not locked in CopyToPort". So the
 * game's warning sink is a LogWindow, and this is where it is made.
 *
 * THE CONSTRUCTOR TAKES AN ARGUMENT IT IGNORES, AND THIS IS WHERE THAT IS PROVED.
 * `LogWindow_ctor` (1060:a847) reads only `[bp+6]`, so its own bytes are those of a
 * constructor with no parameters -- §119's blind spot exactly. The shipped call here pushes
 * eight bytes and clears eight, which is `this` and one more pointer, so the parameter
 * is there and the body simply never looks at it. Its header has been corrected.
 *
 * NOTHING IN THE SOURCE IS THIS FUNCTION: it is `_STCON_`, written by Borland from two
 * file-scope definitions.
 * LogWindow::LogWindow and Font::Font are defined in-file first, so the static constructor reaches them by the 4-byte call.
 *
 * The destructors are defined in-file as well, for `destroy_log_window`'s calls (the same text).
 */

#include "colonization.h"

#include "classes.h"

/* PORT: an in-file stub of Font::Font removed: src/game/Font_ctor_1008_d478.c defines it (W2). */

/* PORT: an in-file stub of Font::~Font removed: src/game/Font_dtor_1008_d4b5.c defines it (W2). */

LogWindow g_error_log(g_log_name);        /* DGROUP:0x2ac0 */
Font g_log_font;                        /* DGROUP:0x4e68 */
