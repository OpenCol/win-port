/* closing_play -- shipped at 1010:032c.
 *
 * Ported by tools/port.py from matched/game/closing_play.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_closing_flag;                           DGROUP:0x4b98
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1010_2a54(void);
void MSDoTask_1010_2b2a(void);
void MSHideWindow_of_1010_2b41(void *port);
void set_handler_574(void *anim, void *fn);
void win_show_focus_at_26_1010_2af0(void *w);

extern void play_sound_effect(col_i16 n);
extern void play_winflc_wav(void);
extern void anim_start_chained(void *anim, void *w);
extern void MSCloseSound(void);
extern void anim_stop(void *anim);
extern void palette_set_range(void *pal, col_i16 first, col_i16 count, void *rgb);







void closing_play(char *w)
{
    win_show_focus_at_26_1010_2af0(w);
    MSDoAllTasks_1010_2a54();
    play_sound_effect(0x3d);
    set_handler_574(w + 0x100, (void *)play_winflc_wav);
    anim_start_chained(w + 0x100, w);
    while (g_closing_flag == 0)
        MSDoTask_1010_2b2a();
    MSCloseSound();
    anim_stop(w + 0x100);
    MSHideWindow_of_1010_2b41(w + 0x26);
    palette_set_range(&(*g_game_palette), 10, 0xec, w + 0x704);
    MSDoAllTasks_1010_2a54();
    MSHideWindow_of_1010_2b41(w + 0x678);
    MSDoAllTasks_1010_2a54();
}
