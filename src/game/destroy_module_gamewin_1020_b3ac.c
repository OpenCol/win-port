/* destroy_module_gamewin_1020_b3ac: candidate for 1020:b3ac (29 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STDES_()
 * name-source: descriptive
 *
 * Destroys module 1020's one file-scope object, the GameWin at SEG22:0x0a that
 * `create_module_gamewin_1020_b374` at the previous address builds. This is `_STDES_`,
 * the translation unit's static destructor, and unlike its constructor half it carries no
 * exception block -- there is nothing to unwind on the way out.
 *
 * The `2` is Borland's destructor flag word: destroy the object, do not free its storage.
 * `destroy_map_ports` passes the same 2 four times.
 *
 * The destructor is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 *
 * THIS FILE IS THE CONSTRUCTOR'S TEXT WITH THE SYMBOL CHANGED (2026-09-29), as
 * destroy_game_globals is: Borland writes `_STCON_` and `_STDES_` from the same file-scope
 * definitions, and a merged module (tools/merge_module.py) carries those definitions once and
 * lets the compiler place both. The plain-C spelling this replaced called each destructor by
 * hand, and would have been a second copy beside the compiler's.
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
GameWin g_win;      /* SEG22:0x0a */
