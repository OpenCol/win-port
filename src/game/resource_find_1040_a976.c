/* resource_find_1040_a976 -- shipped at 1040:a976.
 *
 * Ported by tools/port.py from matched/game/resource_find_1040_a976.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * find_and_load_resource(col_i32 *key, char *name);

void * resource_find_1040_a976(col_i32 *key, char *name)
{
    return find_and_load_resource(key, name);
}
