/* palette_save_file_all: candidate for 1068:0285 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `palette_save_file(pal, name, 0, 0xff)`: writes a palette to a file from entry 0 with a
 * count of 255 -- entries 0 to 254, one short of the 256-entry table, where palette_save_file itself takes the range. It sits
 * directly after `palette_load_file_entry`, the matching load.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

void palette_save_file_all(void *pal, char *name)
{
    palette_save_file(pal, name, 0, 0xff);
}
