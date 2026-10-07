/* mark_placement -- shipped at 1048:0c8f.
 *
 * Ported by tools/port.py from matched/game/mark_placement.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_prefs_root;                        SEG20:0x0010
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_minimize_of(void *w);

typedef struct { col_i16 f0; RECT rect; col_i16 code; } PlacementRec;
typedef struct { char pad[6]; void *win; } GameWinBase;
typedef struct { char pad[0x26]; GameWinBase win; char pad2[0x148 - 0x26 - sizeof(GameWinBase)]; char flag148; char pad3[0x772 - 0x149]; col_i16 slot; } GameWin_local;

extern char * current_window(void);
extern PlacementRec * find_entry(void *root, col_i16 which);
extern void win_minimize(void *w);



void mark_placement(void)
{
    GameWin_local *o;
    PlacementRec *p;
    char *w = current_window();

    o = (GameWin_local *)(w != 0 ? w - 0x26 : w);
    p = find_entry(&g_prefs_root, o->slot);
    if (p != 0 && o->flag148 != 0)
        p->code = 1;
    win_minimize_of(&o->win);
}
