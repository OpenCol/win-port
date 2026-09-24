/* win_set_long -- shipped at 1060:ab8b.
 *
 * Ported by tools/port.py from matched/game/win_set_long.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *   extern int g_stub_a;                                 
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_0(void *p);
void * window_of(void *o);

extern void win_set_menu(void *w, col_i16 a);




void win_set_long(Rec *o, void *p)
{
    win_set_menu(window_of(o), get_w_0(p));
    (*((void * *)((unsigned char *)(o) + 0x6a))) = p;
}
