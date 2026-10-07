/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 127, 129, 130, 131 of the imported file. */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 133, 136, 147, 148 of the imported file. */
/* PORT: this file's own declarations of GameBrains_Init, GameBrains_TurnControlLoop, MSSetFocus_of_1008_d136, MainWin_Init, SetPopupParent, game_font_set_1008_cfe5, get_w_a, show_window_1008_d117, win_hide_1008_d0bc, win_show removed: the header declares them from the definition (W1 step 3). */
/* run_game: candidate for 1008:46ae (863 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: run_game
 * name-source: descriptive
 *
 * THE GAME, from the resource files to the last turn. `1008:cbff` calls it once.
 *
 * Nine `ResourceFile` objects open the resource DLLs by name -- coldata0 through coldata4,
 * coldata6, coldata8, coltext0 and coldata9. Five and seven are missing: there is no coldata5.dll or
 * coldata7.dll in the shipped game. They are STACK objects, so their destructors close all nine again
 * on the way out, in reverse order, and the whole game runs inside their lifetime.
 *
 * Then the nine fonts at DGROUP:0x4b06..0x4b26 are loaded at sizes 16, 14, 12, 10, 12, 14, 18, 24 and
 * 16 -- note the last two are loaded out of address order -- the four text-colour defaults are set,
 * the CD audio starts, and `game_init` runs.
 *
 * THE FOUR COLOUR DEFAULTS ARE TWO PAIRS AND THEY AGREE, which is what settled what the four
 * bytes at DGROUP:0x2adb..0x2ade are: the radio pair is set to 0xc and 7 and the checkbox pair
 * to the same 0xc and 7. `radio_wndproc` and `checkbox_wndproc` each pass the first to
 * `palette_get_entry_rgb` when the item is ENABLED and the second when it is not, so they are
 * palette indexes, not flags, and one startup line gives both controls the same two colours.
 *
 * `begin_game` then puts up the title screen and the new-game choices. If it answers false the
 * function simply closes the DLLs again; if true, the main window is shown, the desktop laid out
 * either from the saved preferences at SEG20:0x10 or by repainting the three map windows, and the
 * game loop runs `GameBrains_TurnControlLoop` and the message pump until DGROUP:0x4b04 is set.
 * `clean_up` then gives the map back.
 *
 * The four NOP-padded calls -- `game_init`, `begin_game`, `CloseBackdrop`
 * and `clean_up` -- are the only externs; every other callee has the four-byte form and
 * is a stub here (rule 47).
 *
 * THE SIX FONTS AND THEIR POINT SIZES, which is the only place the corpus states them:
 *
 *   DGROUP:0x4b0a  g_report_font_small   14
 *   DGROUP:0x4b0e  g_font_tiny           12   the smallest of the six
 *   DGROUP:0x4b1a  g_number_font         14
 *   DGROUP:0x4b1e  g_font_small          18
 *   DGROUP:0x4b22  g_heading_font        16
 *   DGROUP:0x4b26  g_report_font         24
 *
 * The names are not ordered by size and were not chosen together -- `g_font_small` is 18
 * and `g_report_font_small` is 14, both larger than `g_font_tiny`, which was address-named
 * until this table was read. It draws the colony names on the map (`FortDraw`) and the
 * price row in the Europe market.
 *
 * `g_4b2a` IS WRITTEN AND NEVER READ. The two stores here -- 1 before `game_init`, 0 once the main window is shown on the new-game path --
 * are the only instructions in the image that touch 0x4b2a, and nothing loads it
 * (`tools/check_dead_dgroup.py`). It is the shape of a "loading" guard whose reader was cut, so
 * it keeps its address rather than a name like `g_dlls_loading` that would claim a caller which
 * waits on it.
 *
 * RENAMED 2026-09-26 from open_coldata_dlls: The body is the whole game session: it opens the nine resource DLLs as stack objects, loads the fonts, sets the control colours, starts sound, runs game_init, then begin_game, shows the main window and desktop, and loops GameBrains_TurnControlLoop plus the message pump until g_quit_flag is set, then frees the map planes. Its own header calls it 'THE GAME, from the resource files to the last turn'. Opening the DLLs is only the scaffolding whose lifetime wraps the game, so the old name picks a minor step instead of the function's job.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GameWin, CTable, Font, ResourceFile, GamePrefs, DialogWin, MapWin, StatWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
   /* 0x41e bytes: the palette, its handle and the cycling state */



#include "records.h"

   /* DialogWin : EventWin, flattened for C */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;              /* SEG20:0x5ca */
extern "C" void MainWin_Init(MainWin *o);   /* PORT: the header declares it for C only (C++ files view MainWin
                                               opaquely, as here); the definition's signature (W1) */
   /* MapWin : ColChild, the base as its bytes: 0x788 */

extern char g_mini_win;              /* SEG20:0xde0 */


extern StatWin g_stat_win;            /* SEG20:0x1566 */

extern GameBrains g_game_brains;                 /* SEG20:0x40d6 */

extern void MSHideMenuBar(void);
extern void MSShowMenuBar(void);
extern void sound_init(void);
struct logpal;
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);
extern void MainWin_DefaultDesktop(void *o);
extern void clean_up(void);
extern void CloseBackdrop(void);
extern int begin_game(void);

/* same object: 4-byte `push cs; call ` (rule 47) */



extern "C" void run_game(void)
{
    ResourceFile d0("coldata0.dll");
    ResourceFile d1("coldata1.dll");
    ResourceFile d2("coldata2.dll");
    ResourceFile d3("coldata3.dll");
    ResourceFile d4("coldata4.dll");
    ResourceFile d6("coldata6.dll");
    ResourceFile d8("coldata8.dll");
    ResourceFile t0("coltext0.dll");
    ResourceFile d9("coldata9.dll");

    game_font_set_1008_cfe5(&g_title_font, 1, 0x10);
    game_font_set_1008_cfe5(&g_report_font_small, 1, 0xe);
    game_font_set_1008_cfe5(&g_font_tiny, 1, 0xc);
    game_font_set_1008_cfe5(&g_info_font_small, 1, 0xa);
    game_font_set_1008_cfe5(&g_info_font_large, 1, 0xc);
    game_font_set_1008_cfe5(&g_number_font, 1, 0xe);
    game_font_set_1008_cfe5(&g_font_small, 1, 0x12);
    game_font_set_1008_cfe5(&g_report_font, 1, 0x18);
    game_font_set_1008_cfe5(&g_heading_font, 1, 0x10);
    set_button_font_1008_d033(&g_number_font);
    set_radio_color_on(0xc);
    set_radio_color_off(7);
    set_check_color_on(0xc);
    set_check_color_off(7);
    g_panel_shown_mask = 7;
    MSHideMenuBar();
    sound_init();
    g_4b2a = 1;
    game_init();
    CTable_Fill(&g_game_palette, 0xa, 5, (unsigned char*)(g_palette_head_rgb));
    MainWin_Init(&g_main_win);
    win_hide_1008_d0bc((Screen*)(&g_map_window_dlg));
    win_hide_1008_d0bc((Screen*)(&g_mini_win_dlg));
    win_hide_1008_d0bc((Screen*)(&g_info_win_dlg));
    if (begin_game()) {
        MSShowMenuBar();
        SetPopupParent((EventWin*)(&g_main_win));
        CloseBackdrop();
        MSDoAllTasks_1008_ce3f();
        win_show((Screen*)(&g_main_win));
        g_4b2a = 0;
        if (get_w_a(&g_prefs_root))
            MainWin_DefaultDesktop(&g_main_win);
        else {
            show_window_1008_d117(&g_map_window);
            show_window_1008_d117((GameWin *)&g_mini_win);
            show_window_1008_d117(&g_stat_win);
        }
        g_quit_flag = 0;
        GameBrains_Init(&g_game_brains);
        win_show((Screen*)(&g_main_win));
        MSSetFocus_of_1008_d136((Screen*)(&g_main_win));
        MSDoAllTasks_1008_ce3f();
        while (g_quit_flag == 0) {
            GameBrains_TurnControlLoop(&g_game_brains);
            call_message_pump();
        }
        clean_up();
    }
    MSShowMenuBar();
}
