/* window_resized -- shipped at 1048:0a3b.
 *
 * Ported by tools/port.py from matched/game/window_resized.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_prefs_root;                        SEG20:0x0010
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1048_d86c(void *o);
col_i16 port_width_1048_d853(void *o);
col_i16 win_is_iconic_1048_d8ad(void *w);
col_i16 win_is_zoomed_1048_d8d2(void *w);
void MSDoAllTasks_1048_d701(void);
void call_handler_slot_1894(void *o, col_i16 a);
void win_rect_in_parent_1048_d674(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

typedef struct { col_i16 f0; RECT rect; col_i16 code; } PlacementRec;
typedef struct { char pad[6]; void *win; } GameWinBase;
typedef struct { char pad[0x26]; GameWinBase win; char pad2[0x148 - 0x26 - sizeof(GameWinBase)];
                 char flag148; char pad3[0x76a - 0x149]; RECT rc; col_i16 slot; } GameWin_local;

extern char * current_window(void);
extern PlacementRec * find_entry(void *root, col_i16 which);

extern void MSSetFocus_of_1048_e0cd(GameWinBase *w);
extern void focus_marked_ctl(GameWinBase *w);
extern col_i16 win_is_iconic_1078_217b(void *w);
extern col_i16 win_is_zoomed_1078_212b(void *w);
extern void win_rect_in_parent_1078_2644(void *w, RECT *r);

/* 5-byte NOP form, so extern (rule 47) */
extern void draw_window_title(GameWin_local *o);












void window_resized(col_i16 a)
{
    GameWin_local *o;
    PlacementRec *p;
    RECT rc;
    char *w = current_window();

    o = (GameWin_local *)(w != 0 ? w - 0x26 : w);
    p = find_entry(&g_prefs_root, o->slot);
    SetRect(&o->rc, 0xa, 0x20, port_width_1048_d853(o) - 10, port_height_1048_d86c(o) - 10);
    draw_window_title(o);
    call_handler_slot_1894(o, a);
    MSDoAllTasks_1048_d701();
    if (p != 0 && o->flag148 != 0) {
        if (win_is_iconic_1048_d8ad(&o->win))
            p->code = 1;
        else {
            if (win_is_zoomed_1048_d8d2(&o->win))
                p->code = 2;
            else
                p->code = 0;
            win_rect_in_parent_1048_d674(&o->win, &rc);
            p->rect.left = rc.left;
            p->rect.top = rc.top;
            p->rect.right = rc.right;
            p->rect.bottom = rc.bottom;
        }
    }
}
