/* build_movie_window_1060_a56b: candidate for 1060:a56b (132 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `Screen_Init`'s twin, 151 bytes below it: the same four steps with one fewer --
 * there is no `screen_set_ctable` at the end and so no trailing pointer -- and
 * `create_MSMovieClass_window` in place of `create_MSWindowClass_window`. The style word goes
 * into the object at +0 first; a window already at +6 is torn down by `MSDisposeWindow` and
 * its 0 result written back; `create_MSMovieClass_window` then OVERWRITES +6 with the window it makes;
 * `set_hwnd_object` registers the object against it, and there it stops.
 *
 * +6 holding the old window (disposed, its slot cleared) and then the new window is what the shipped code does, not a reading of
 * it: both stores are to the same pair of words, twenty bytes apart, with the dispose call in
 * between.
 *
 * The final `0L` is one 32-bit push under `-3`, which is what says it is a pointer or a
 * long and not two zero words.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void *MSDisposeWindow(void *win);
extern void *create_MSMovieClass_window(char *title, int style, int x, int y,
                                                 int f, int g, void *parent);
extern void set_hwnd_object(Screen *obj, void *win);

void build_movie_window_1060_a56b(Screen *o, char *title, register int style, int x,
                                      int y, int w, int h)
{
    o->style = style;
    if (o->win != 0)
        o->win = MSDisposeWindow((MSWin *)o->win);
    o->win = create_MSMovieClass_window(title, style, x, y, w, h, 0L);
    set_hwnd_object(o, (MSWin *)o->win);
}
