/* build_movie_window_1060_a56b -- shipped at 1060:a56b.
 *
 * Ported by tools/port.py from matched/game/build_movie_window_1060_a56b.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * menu_attach_to_window(void *menu);
extern void * create_MSMovieClass_window(char *title, col_i16 style, col_i16 d, col_i16 e,
                                                 col_i16 f, col_i16 g, void *p);
extern void set_hwnd_object(Win *o, void *h);

void build_movie_window_1060_a56b(Win *o, char *title, register col_i16 style, col_i16 d,
                                      col_i16 e, col_i16 f, col_i16 g)
{
    (*((col_i16 *)((unsigned char *)(o) + 0x0))) = style;
    if ((*((void * *)((unsigned char *)(o) + 0x6))) != 0)
        (*((void * *)((unsigned char *)(o) + 0x6))) = menu_attach_to_window((*((void * *)((unsigned char *)(o) + 0x6))));
    (*((void * *)((unsigned char *)(o) + 0x6))) = create_MSMovieClass_window(title, style, d, e, f, g, 0L);
    set_hwnd_object(o, (*((void * *)((unsigned char *)(o) + 0x6))));
}
