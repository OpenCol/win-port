/* font_handle_1070_9806: candidate for 1070:9806 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The font reference's HANDLE: the word at +0.
 *
 * Every call site hands the result straight to `MSHLock`, `MSHUnlock`, `MSGetTextLength` or
 * `measure_wrapped_text`, all of which take a font handle, and the argument is always one
 * of the `g_font_*` references. `GameFont` is `{ short handle; short height; }`, so this is
 * its first field and `font_height_*` is its second.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

#include "classes.h"

int font_handle_1070_9806(Font *ref)
{
    return ref->hfont;
}
