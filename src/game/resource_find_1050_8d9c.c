/* resource_find_1050_8d9c -- shipped at 1050:8d9c.
 *
 * Ported by tools/port.py from matched/game/resource_find_1050_8d9c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * find_and_load_resource(col_i32 *key, char *name);

void * resource_find_1050_8d9c(col_i32 *key, char *name)
{
    return find_and_load_resource(key, name);
}
