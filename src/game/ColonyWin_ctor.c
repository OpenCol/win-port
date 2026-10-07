/* ColonyWin_ctor: candidate for 1008:dd64 (209 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin::ColonyWin()
 * name-source: descriptive
 *
 * The constructor of a 0xa14-byte window class over `ColChild`, vtable at DGROUP:0x108. The
 * body is EMPTY: every instruction in it is the compiler's -- the conditional `operator new`
 * of 0xa14 bytes when `this` arrives null, the base constructor, the vptr store, then each
 * member in declaration order, then the live-object counter.
 *
 * So the whole reconstruction is the MEMBER LIST, and the shipped calls are what give it:
 * an `SRegion` at +0x77a, two `GRPort`s at +0x7a8 and +0x7ce, THREE `SImageButton`s at
 * +0x7f4 built by one `VNEWVF` with element size 0x74 and count 3, TWO `SButton`s at +0x950
 * by another with size 0x22 and count 2, and a fourth lone `SImageButton` at +0x994. Twelve
 * bytes of tail take it to 0xa14.
 *
 * `VNEWVF` is Borland's vector-new-with-constructor and its five arguments -- array, element
 * size, count, flags and the constructor's address -- are exactly what a member array of
 * a class with a constructor compiles to. Nothing in the source says `new`.
 *
 * `GRPort`'s constructor is a FAR call, so it is declared and not defined; the other four are
 * -called and must be defined here for the call form to match (rule 47).
 *
 * RENAMED 2026-09-26 from PanelWin_ctor: the class is ColonyWin: ColonyWin_dtor (1008:e0ef) names the same vtable (DGROUP:0x108) and destroys the same member list, and 0xa14 is the colony window's size -- every colony-screen function reaches its +0xa12.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

ColonyWin::ColonyWin()
{
}
