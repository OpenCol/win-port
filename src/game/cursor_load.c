/* cursor_load: candidate for 1008:d689 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Loads cursor resource `id` and fills a CursorClass with it: the handle `MSLoadCursor` returns
 * at +0, the id it was asked for at +2. SI holds the id across the call, which is why the
 * function saves and restores it.
 *
 * `game_init` calls it twelve times over `g_cursors` -- the fifteen four-byte objects
 * `create_game_globals` defines at SEG20:0x7746 -- with the ids 0x80..0x8b.
 */

#include "colonization.h"

#include "classes.h"

void cursor_load(CursorClass *cursor, int id)
{
    cursor->hcursor = MSLoadCursor(id);
    cursor->id = id;
}
