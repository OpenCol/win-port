/* Font_ctor_1060_a8b0: candidate for 1060:a8b0 (61 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::Font()
 * name-source: sibling-build
 *
 * The constructor beside `Font_dtor_1060_a8ed`: a 4-byte object whose font handle is
 * zeroed. Module 1060's copy of `Font_ctor_1008_d478`, byte for byte the same function at
 * the same 61 bytes, which is what a header-defined constructor emitted into a second
 * translation unit looks like.
 *
 * The second member is what makes `sizeof` 4 rather than 2. Nothing in either constructor
 * or either destructor touches it; it is the allocation size that proves it is there.
 */

#include "colonization.h"

#include "classes.h"

/* PORT: this copy of Font::Font removed: src/game/Font_ctor_1008_d478.c defines it once (W2). */
