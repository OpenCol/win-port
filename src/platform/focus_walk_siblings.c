/* focus_walk_siblings -- shipped at 1070:9050.
 *
 * Ported by tools/port.py from matched/platform/focus_walk_siblings.c.
 *
 * The file's own Win, Obj dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Win * get_p_6_1070_95f0(void *o);
col_i16 control_is_current(col_u16 hwnd);
col_i16 get_w_0_1070_9669(void *o);
col_i16 get_w_1a_1070_981e(void *o);
col_i16 get_w_20_1070_997b(void *o);
col_i16 hwnd_is_ms_control(col_u16 h);
col_i16 next_sibling_window(col_u16 h);
col_i16 prev_sibling_window(col_u16 h);
col_i16 same_control_owner(col_u16 a, col_u16 b);
void * list_find_by_id(col_i16 id, void *p);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IsChild(col_u16 parent, col_u16 child);
extern col_i16 pascal IsWindow(col_u16 hwnd);
extern col_i16 pascal IsWindowVisible(col_u16 hwnd);
extern col_u16 pascal GetFocus(void);
extern col_u16 pascal GetWindow(col_u16 hwnd, col_u16 cmd);
extern col_u16 pascal SetFocus(col_u16 hwnd);
#endif

#include "w16const.h"

/* same object: 4-byte `push cs; call ` (rule 47) */











void focus_walk_siblings(Obj *o, col_u16 hwnd, col_i16 dir)
{
    void *ctl;
    col_u16 start;
    register col_u16 h;
    register col_u16 owner;

    h = hwnd;
    if (h == 0) {
        h = GetFocus();
        if (IsChild(get_p_6_1070_95f0(o)->hdc, h) == 0)
            h = 0;
    }
    if (h == 0) {
        if ((*((void * *)((unsigned char *)(o) + 0x8c))) != 0) {
            if (dir == 2)
                h = GetWindow(get_p_6_1070_95f0(o)->hdc, GW_CHILD);
            else
                h = GetWindow(GetWindow(get_p_6_1070_95f0(o)->hdc, GW_CHILD), GW_HWNDLAST);
        } else {
            return;
        }
    } else {
        if (dir == 2)
            h = next_sibling_window(h);
        else
            h = prev_sibling_window(h);
    }
    start = h;
    while (IsWindow(h)) {
        if (IsWindowVisible(h) == 0) {
            if (dir == 2)
                h = next_sibling_window(h);
            else
                h = prev_sibling_window(h);
        } else if (hwnd_is_ms_control(h) == 0) {
            ctl = list_find_by_id(h, (*((void * *)((unsigned char *)(o) + 0x8c))));
            if (ctl != 0) {
                if ((get_w_0_1070_9669(ctl) == 6 && get_w_1a_1070_981e(ctl) != 0)
                 || (get_w_0_1070_9669(ctl) == 2 && get_w_20_1070_997b(ctl) != 0)
                 || get_w_0_1070_9669(ctl) == 4
                 || get_w_0_1070_9669(ctl) == 7) {
                    SetFocus(h);
                    break;
                }
            }
            if (dir == 2)
                h = next_sibling_window(h);
            else
                h = prev_sibling_window(h);
        } else {
            owner = h;
            while (IsWindow(h)) {
                if (same_control_owner(owner, h) != 0) {
                    if (control_is_current(h) != 0) {
                        SetFocus(h);
                        break;
                    } else {
                        owner = h;
                        if (dir == 2)
                            h = next_sibling_window(h);
                        else
                            h = prev_sibling_window(h);
                    }
                } else {
                    focus_walk_siblings(o, owner, dir);
                    break;
                }
            }
            break;
        }
        if (h == start)
            return;
    }
}
