/* show_opening: candidate for 1010:043e (120 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_opening
 * name-source: descriptive
 *
 * Opens `coldata5.dll` as a ResourceFile, builds an Opening on the stack, runs it, and lets
 * both destructors fire on the way out. The exception block is theirs.
 *
 * PARKED FOR A WHILE ON TWO BYTES OF FRAME, and the answer was a class size the corpus
 * already knew. The earlier attempt declared `ResourceFile` as four bytes, which put the
 * first object at `bp-6` where the shipped has it at `bp-4` and made the frame
 * `SUB SP,0x1eaa` against `SUB SP,0x1ea8`. `ResourceFile` is TWO bytes -- one handle --
 * which `ResourceFile_ctor_1010_281e` has said since it was filed: its `operator new` asks
 * for 2.
 *
 * The arithmetic is worth keeping because it is checkable by hand. Under `-WS` the saved
 * `DS` sits at `bp-2`, so a four-byte object at `bp-4` would overlap it; the shipped
 * placement is only possible for a two-byte one. `Opening` is 0x1e88 and ends exactly where
 * `ResourceFile` begins.
 *
 * **When a frame is off by N bytes, the first thing to re-check is every class size in the
 * file against the corpus's own reconstruction of that class's constructor.** The frame is
 * the sum of them, so any one being wrong shows up as one number at the top of the function
 * and nowhere else.
 * ResourceFile's constructor and destructor are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "classes.h"
#include "w16types.h"
class DlgControl;
#include "records.h"
extern "C" void Opening_Play(Opening *o);

extern "C" void show_opening(void)
{
    ResourceFile rf("coldata5.dll");
    Opening op;

    Opening_Play(&op);
}
