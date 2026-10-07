/* palette_load_file_entry: candidate for 1068:0263 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `palette_load_file(pal, name)`.
 *
 * A pure pass-through: every argument goes on unchanged. Nothing in the image calls it, so
 * why the program carries a second symbol for the same operation is not shown -- the name
 * says only that this is a separate entry onto `palette_load_file`.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

void palette_load_file_entry(void *pal, char *name)
{
    palette_load_file(pal, name);
}
