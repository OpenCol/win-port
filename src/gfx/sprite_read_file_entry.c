/* sprite_read_file_entry: candidate for 1068:6d69 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `sprite_read_file(s, name)`.
 *
 * A pure pass-through: every argument goes on unchanged. Nothing in the image calls it, so
 * why the program carries a second symbol for the same operation is not shown -- the name
 * says only that this is a separate entry onto `sprite_read_file`.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void sprite_read_file(Sprite *sprite, char *name);

void sprite_read_file_entry(Sprite *sprite, char *name)
{
    sprite_read_file(sprite, name);
}
