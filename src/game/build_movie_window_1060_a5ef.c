/* PORT: this file's own declarations of MSDisposeWindow, create_MSMovieClass_window, get_win_1060_ab6e, set_hwnd_object removed: the header declares them from the definition (W1 step 3). */
/* build_movie_window_1060_a5ef: candidate for 1060:a5ef (144 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `build_movie_window_1060_a56b`'s twin, 132 bytes below it, and the difference is the last
 * argument: where that one passes a literal `0L` as the owner, this one passes
 * `get_win_1060_ab6e(w)` for a window the caller supplies. Three copies of one builder --
 * 1060:a4d4, a56b and a5ef -- separated by which window class they create and what they own.
 * The style word goes into the object at +0 first; a window already at +6 is torn down by `MSDisposeWindow` and
 * its 0 result written back; `create_MSMovieClass_window` then OVERWRITES +6 with the window it makes;
 * `set_hwnd_object` registers the object against it. The pointer the caller passes last
 * is the owner, handed through `get_win_1060_ab6e` as the new window's parent.
 *
 * +6 holding the old window (disposed, its slot cleared) and then the new window is what the shipped code does, not a reading of
 * it: both stores are to the same pair of words, twenty bytes apart, with the dispose call in
 * between.
 *
 * `get_win_1060_ab6e` is reached by the four-byte `PUSH CS / CALL NEAR` form and is a stub here
 * (rule 47).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */

void build_movie_window_1060_a5ef(Screen *o, char *title, register int style, int x,
                                      int y, int w, int h, void *owner)
{
    o->style = style;
    if (o->win != 0)
        o->win = MSDisposeWindow((MSWin *)o->win);
    o->win = create_MSMovieClass_window(title, style, x, y, w, h, (MSWin *)(get_win_1060_ab6e((Screen *)owner)));
    set_hwnd_object(o, (MSWin *)o->win);
}
