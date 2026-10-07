/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of CTable_Set, GameWin_Init, IsBackdrop, set_on_keydown_after_1008_da94, set_on_lclick_1008_da22, set_on_rclick_1008_da5b, show_window_1008_d117, win_center_1008_dacd, win_hide_1008_d0bc removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 127, 128, 129, 133, 134, 135, 137 of the imported file. */
/* get_game_options: candidate for 1008:bd72 (1752 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_game_options
 *
 * THE PLAYER'S CHOICES FOR A NEW GAME, called from `start_new_game`: difficulty, nation,
 * the leader's name, and the two illustrated introduction pages for that nation. It answers 1 when
 * the player cancels any of the three dialogs and 0 once everything is chosen.
 *
 * With DGROUP:0x8a set (the non-interactive start) there are no dialogs: the difficulty is 2 and
 * the nation is a random 0..3. Otherwise `set_difficulty` and `set_nation` ask. Difficulty 0
 * sets bit 7 of SEG20:0x7784. A nation choice above 3 is remembered, then played as England.
 *
 * THE INTRODUCTION, interactive only. Palette entries 15 (yellow) and 14 (green) are set, and a
 * 640x480 window is built over the object `IsBackdrop` returns (whose left-click, right-click and key
 * handlers at +0xd are pointed at the two `flag_4b8c_set` routines) or over the game record when it
 * returns none; the window's own handlers get the same. Picture 0xca is loaded, and LEADERNAME asks
 * for the leader's name with the nation's default at SEG20:0x7810, which is then copied back from
 * the edit buffer. Then, twice: a double bevel, the nation's name in capitals centred at the top in
 * the font at DGROUP:0x4b26, and the text resource NATIONnA -- the second time NATIONnB, n being the
 * nation -- set into the frame; the window is shown and the message pump runs until one of the
 * handlers sets DGROUP:0x4b8c.
 *
 * FINALLY every European nation is made a computer player with no gold and a clear +0 byte, and the
 * chosen one is made human again. The "4" case -- a choice past the four nations -- falls back to
 * England through a loop with an empty body, a leftover of whatever it once searched for.
 *
 * `names` is an initialised local, so its four pointers are copied from DGROUP:0xcc with
 * `REP MOVSW` (rule 74); the strings they point to -- ENGLAND, FRANCE, SPAIN, NETHERLANDS -- come
 * first in the string pool, ahead of the two empty window names and LEADERNAME. `GameWin` and
 * `GRPort` are destructible locals and take the top of the frame (rule 72); `GameWin`'s constructor
 * and destructor are -called and so are defined here, `GRPort`'s are . It graded RELOCATABLE
 * on the first build.
 *
 * RENAMED 2026-09-27 from new_game_player_setup: String witness: Mac get_game_options(void) has GAME, LEADERNAME and NATION0A, and bd72 has exactly those three. Call graph: Mac get_game_options is called only by start_new_game, and bd72 only by c44a, which becomes start_new_game. Mac calls set_difficulty, set_nation, random, CTable::Set x2, IsBackdrop, popup_ask, TextColor and GRPort::String x2. bd72 calls set_difficulty, set_nation, rand_range, CTable_Set x2, IsBackdrop, popup_ask, TextColor x2 and GRPort_String x2. Arity: (void) on both sides; the reconstruction is extern "C" int new_game_player_setup(void), in a C++ file. Size is 2232 Mac against 1752 Windows. The old name was a descriptive paraphrase. Apply this only after 1048:b6ae has moved to game_menu_options.
 */

#include "colonization.h"

#include "w16types.h"
#include "difficul.h"
#include "control.h"
#include "nations.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-


class DlgControl;


#include "records.h"

extern EventWin * g_backdrop_win;             /* DGROUP:0xc6, declared */

   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;              /* SEG20:0x5ca */
extern GameState g_game;   /* SEG20:0x7782 */
extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern int set_difficulty(void);
extern int set_nation(void);

struct logpal;
extern int GRPort_LoadPicture(GRPort *port, int res, int a, int b, struct logpal *pal);
extern "C" int popup_ask(void *ctx, void *name, int max_len, void *dflt);   /* PORT: C linkage, as defined (W2) */

extern void FrameInfoRect(GRPort *p, RECT *r, int depth, int light, int dark);

extern void draw_text_resource(void *target, char *name, void *rect);

/* same object: 4-byte `push cs; call ` (rule 47) */



extern "C" int get_game_options(void)
{
    int result = 1;
    GameWin w;
    GRPort port;
    int saved;
    int i;
    RECT r;
    void *p;
    char name[80];
    char *names[4] = { "ENGLAND", "FRANCE", "SPAIN", "NETHERLANDS" };

    if (g_debug_mode == 0) {
        if (set_difficulty())
            goto out;
    } else
        g_game.difficulty = 2;
    if (g_game.difficulty == DIFFICULTY_DISCOVERER)
        g_game.game_flags |= 0x80;
    if (g_debug_mode == 0) {
        if (set_nation())
            goto out;
    } else
        g_game.player_nation = rand_range(0, 3);
    saved = g_game.player_nation;
    if (g_game.player_nation > NATION_NETHERLANDS)
        g_game.player_nation = NATION_ENGLAND;
    if (g_debug_mode == 0) {
        CTable_Set(&g_game_palette, 0xf, 0xff, 0xff, 0);
        CTable_Set(&g_game_palette, 0xe, 0, 0xff, 0);
        g_done_flag = 0;
        p = IsBackdrop();
        if (p != 0) {
            GameWin_Init(&w, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette, (Screen *)p);
            set_on_lclick_1008_da22(&(EventTable &)*g_backdrop_win, (void*)(set_done_flag_1008_b362));
            set_on_rclick_1008_da5b(&(EventTable &)*g_backdrop_win, (void*)(set_done_flag_1008_b362));
            set_on_keydown_after_1008_da94(&(EventTable &)*g_backdrop_win, (void*)(set_done_flag_1008_b37c));
        } else
            GameWin_Init(&w, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette,
                                             (Screen *)&g_main_win);
        set_on_lclick_1008_da22((EventTable *)&w, (void*)(set_done_flag_1008_b362));
        set_on_rclick_1008_da5b((EventTable *)&w, (void*)(set_done_flag_1008_b362));
        set_on_keydown_after_1008_da94((EventTable *)&w, (void*)(set_done_flag_1008_b37c));
        GRPort_LoadPicture(&w, 0xca, 0x10, 0x18, (struct logpal *)&g_game_palette);
        if (popup_ask(g_game_txt, (void*)("LEADERNAME"), 0x17,
                                       g_nation_text[g_game.player_nation].leader))
            goto out;
        STRCPY(name, g_name_slots[0]);
        STRCPY(g_nation_text[g_game.player_nation].leader, name);
        STRCPY(name, "NATION0A");
        name[6] += (char)g_game.player_nation;
        set_wrap_font(&g_font_small);
        SetRect(&r, 10, 10, port_width_1008_daf2(&w) - 10, port_height_1008_db11(&w) - 10);
        FrameInfoRect(&w, &r, 2, 7, g_draw_color);
        InflateRect(&r, -10, -0x14);
        r.top += 10;
        FrameInfoRect(&w, &r, -2, 7, g_draw_color);
        TextColor(0xc);
        GRPort_String(&w, (unsigned *)&g_report_font, names[g_game.player_nation], port_width_1008_daf2(&w) / 2,
                             0x1c, 0);
        SetRect(&r, 0x3c, 0x50, port_width_1008_daf2(&w) - 0x32, port_height_1008_db11(&w) - 0x3c);
        draw_text_resource(&w, name, &r);
        copy_all_to_screen_1008_db30(&w);
        win_center_1008_dacd((DialogWin *)&w);
        show_window_1008_d117(&w);
        MSDoAllTasks_1008_ce3f();
        while (g_done_flag == 0)
            call_message_pump();
        win_hide_1008_d0bc((DialogWin *)&w);
        MSDoAllTasks_1008_ce3f();
        GRPort_LoadPicture(&w, 0xca, 0x10, 0x18, (struct logpal *)&g_game_palette);
        g_done_flag = 0;
        STRCPY(name, "NATION0A");
        name[6] += (char)g_game.player_nation;
        name[7]++;
        set_wrap_font(&g_font_small);
        SetRect(&r, 10, 10, port_width_1008_daf2(&w) - 10, port_height_1008_db11(&w) - 10);
        FrameInfoRect(&w, &r, 2, 7, g_draw_color);
        InflateRect(&r, -10, -0x14);
        r.top += 10;
        FrameInfoRect(&w, &r, -2, 7, g_draw_color);
        TextColor(0xc);
        GRPort_String(&w, (unsigned *)&g_report_font, names[g_game.player_nation], port_width_1008_daf2(&w) / 2,
                             0x1c, 0);
        SetRect(&r, 0x5a, 0x50, port_width_1008_daf2(&w) - 0x32, port_height_1008_db11(&w) - 0x3c);
        draw_text_resource(&w, name, &r);
        copy_all_to_screen_1008_db30(&w);
        win_center_1008_dacd((DialogWin *)&w);
        show_window_1008_d117(&w);
        MSDoAllTasks_1008_ce3f();
        while (g_done_flag == 0)
            call_message_pump();
        MSDoAllTasks_1008_ce3f();
    }
    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        g_nation_text[i].control = CONTROL_AI;
        g_nations[i].gold = 0;
        g_nation_text[i].once_flags = 0;
    }
    if (saved == 4) {
        saved = 0;
        for (i = 0; i < NATION_EUROPEAN_COUNT; i++)
            ;
        if (saved == 0) {
            g_game.player_nation = NATION_ENGLAND;
            g_nation_text[0].control = CONTROL_HUMAN;
        }
    } else
        g_nation_text[saved].control = CONTROL_HUMAN;
    result = 0;
out:
    return result;
}
