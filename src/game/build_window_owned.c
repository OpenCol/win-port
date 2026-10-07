/* build_window_owned: candidate for 1060:a67f (163 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The fourth and widest of the window builders in this segment -- 1060:a4d4, a56b, a5ef and
 * this one. It is `Screen_Init` with one more parameter: where that one passes a
 * literal `0L` as the window's owner, this passes `get_win_1060_ab6e(w)` for a window the caller
 * supplies. The style word goes into the
 * object at +0 first; a window already at +6 is torn down by `MSDisposeWindow` and
 * its 0 result written back; `create_MSWindowClass_window` then OVERWRITES +6 with the window it makes;
 * `set_hwnd_object` registers the object against it, and `screen_set_ctable` hands
 * over the pointer the caller passed last.
 *
 * +6 holding the old window (disposed, its slot cleared) and then the new window is what the shipped code does, not a reading of
 * it: both stores are to the same pair of words, twenty bytes apart, with the dispose call in
 * between.
 *
 * `screen_set_ctable` and `get_win_1060_ab6e` are reached by the four-byte `PUSH CS /
 * CALL NEAR` form and are stubs here (rule 47).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void *MSDisposeWindow(void *win);
extern void *create_MSWindowClass_window(char *title, int style, int x, int y,
                                                 int f, int g, void *parent);
extern void set_hwnd_object(Screen *obj, void *win);

/* same object: 4-byte `push cs; call ` */
extern void screen_set_ctable(Screen *o, void *t);
extern void *get_win_1060_ab6e(void *o);

void build_window_owned(Screen *o, char *title, register int style, int x,
                                      int y, int w, int h, void *target, void *owner)
{
    o->style = style;
    if (o->win != 0)
        o->win = MSDisposeWindow((MSWin *)o->win);
    o->win = create_MSWindowClass_window(title, style, x, y, w, h, (MSWin *)(get_win_1060_ab6e((Screen *)owner)));
    set_hwnd_object(o, (MSWin *)o->win);
    screen_set_ctable(o, target);
}
