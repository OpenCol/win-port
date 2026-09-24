/* store_entry_ptr -- shipped at 1050:0fad.
 *
 * Ported by tools/port.py from matched/game/store_entry_ptr.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * list_find_by_key(Rec *r, col_i16 a);

void store_entry_ptr(Rec *r, col_i16 a)
{
    (*((void * *)((unsigned char *)(r) + 0x366))) = list_find_by_key(r, a);
}
