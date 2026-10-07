/* PORT: this file's own declarations of MSGetTextLength, font_handle_1068_dd1e removed: the header declares them from the definition (W1 step 3). */
/* text_line_count: candidate for 1068:cc91 (66 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 66, not the census 59. The trailing 7 bytes are the dead second epilogue.
 *
 * How many lines a string needs at a given width: measure it once and divide, plus one.
 * `text_widest_line` is the next function up and measures the other axis -- these
 * two are the layout pair, one for the width of the widest line and one for the count.
 *
 * The divide is SIGNED -- `CWD` then `IDIV` -- so a negative width would not trap the way an
 * unsigned one would; the width comes from the caller and nothing here checks it.
 *
 * The measured width goes through a frame slot and is read straight back out, which is `-Od`
 * keeping an assignment the source wrote rather than a temporary the compiler needed.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */

int text_line_count(char *s, int width, void *font)
{
    int w;

    w = MSGetTextLength(font_handle_1068_dd1e((Font *)font), s);
    return w / width + 1;
}
