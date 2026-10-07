/* create_module_gamewin_1040_b5d0: candidate for 1040:b5d0 (56 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STCON_()
 * name-source: descriptive
 *
 * Constructs module 1040's one file-scope object, a GameWin at SEG29:0x11ea, and
 * `destroy_module_gamewin_1040_b608` at the next address tears it down. `show_ship` and `build_check`
 * are what use it. Segment 29 is the module's text and resource arena -- the data path at
 * 0, the loaded text at 0x3c, the CSV cursor at 0x8c, the string table at 0x10dc -- and
 * this window sits at the end of it.
 *
 * NOTHING IN THE SOURCE IS THIS FUNCTION. It is `_STCON_`, the translation unit's static
 * constructor, and Borland writes it from a file-scope object definition and nothing else,
 * which is why this file has no function in it. The exception block is how a constructor
 * that throws unwinds what is already built.
 *
 * `` on the definition is what puts the object in its own segment: without it the
 * constructor is handed `PUSH DS` (one byte) where the shipped code has `PUSH SEG obj`
 * (three).
 *
 * The destructor is defined in-file as well, for `destroy_module_gamewin_1040_b608`'s call (the same text).
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */
/* PORT: an in-file stub of GameWin::GameWin removed: src/game/GameWin_ctor_1008_d8ee.c defines it (W2). */
/* PORT: this copy of GameWin::~GameWin removed: src/game/GameWin_dtor_1008_d965.c defines it once (W2). */
GameWin g_opening_win;      /* SEG29:0x11ea */
