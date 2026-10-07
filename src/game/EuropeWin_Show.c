/* EuropeWin_Show: candidate for 1048:8a09 (704 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_Show
 * name-source: sibling-build
 *
 * OPENS THE EUROPE SCREEN for `nation` and runs it modally. After independence (bit 0 of SEG20:0x7784)
 * it only shows EUROPENOTAVAIL and returns.
 *
 * SET-UP: the window is initialised and framed, the nation is made current and stored at +0x77a, and the
 * dock picture is loaded. The picture port at +0x884 is copied whole into the view rectangle at +0x76a,
 * the screen is redrawn and placed, and DGROUP:0xb4 is set.
 *
 * ARRIVAL LINE: with `unit` >= 0, the window's message is rebuilt from four words -- the unit owner's
 * adjective (SEG20:0xd89e), its type's name, the word at SEG20:0xdcd6, and the player's nation name -- and
 * shown with `EuropeWin_EuropeActivateMessage(w, 1, 120)`. The unit index is loaded into the same register
 * variable as `nation`, which is why the source reuses the parameter.
 *
 * TUTORIAL: with tutorial bit 0x80 of SEG20:0x7784 set, TUTORIAL17 is shown once, latched by bit 0x20 of
 * SEG20:0x7782.
 *
 * The modal loop runs with DGROUP:0x4cd2 set, and on the way out the popup is closed, the menu
 * re-attached, and the game record and view at SEG20:0x5ca and 0x1566 restored.
 *
 * `EuropeWin_DrawEurope` is reached by the NOP form and is extern; the other callees are
 * stubs (rule 47). It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from european_status: the Mac build's EuropeWin::Show(short, short): this plus a nation and a unit, opening the Europe screen and running it modally -- the same role PediaWin_Show has for the Colonopedia.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "gameflag.h"
#include "misc.h"
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class EuropeWin : public ColChild { public: short nation; SRegion region; SButton recruit; SButton train; SButton purchase; SImageButton image; GRPort port; EuropeWin(); ~EuropeWin(); };
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;             /* SEG20:0x5ca */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;           /* SEG20:0x1566 */
extern int popm(char *msg, int n);
extern void SetPopupParent(void *parent);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void popup_country(int slot, int flag, int nation);
extern void popup_say(int slot, int v);
extern void EventWin_Modal(EventWin *w);
extern void GameWin_Redraw(GameWin *rec);
extern void EuropeWin_DrawEurope(EuropeWin *w);

/* same object: 4-byte `push cs; call ` */
extern "C" void ColChild_CancelMessage(void *child, int a);
extern "C" void set_europe(int n);
extern "C" void ColChild_Center(void *w);
extern "C" void ColChild_AddToMessage(void *child, int idx);
extern "C" void EuropeWin_Init(EuropeWin *o);
extern "C" void ColChild_DrawNonClient(EuropeWin *w);
extern "C" void EuropeWin_LoadEurope(EuropeWin *w);
extern "C" int port_width_1048_d853(void *port);
extern "C" int port_height_1048_d86c(void *port);

extern "C" void show_window_1048_dd42(void *o);
extern "C" void win_show_and_focus_1048_d8f7(void *w);
extern "C" void EuropeWin_EuropeActivateMessage(EuropeWin *win, int kind, long ms);

extern "C" void set_name_slot_1048_de36(int slot, char *s);

extern "C" void europe_remove_label(void);
extern "C" void win_release_1048_e0a1(EuropeWin *w);

void EuropeWin_Show(EuropeWin *w, int nation, int unit)
{
    RECT r;

    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        popm("EUROPENOTAVAIL", 1);
        return;
    }
    g_modal_active = 0;
    EuropeWin_Init(w);
    SetPopupParent(&(DialogWin &)*w);
    ColChild_DrawNonClient(w);
    ColChild_CancelMessage(w, 0);
    set_europe(nation);
    w->nation = nation;
    EuropeWin_LoadEurope(w);
    SetRect(&r, 0, 0, port_width_1048_d853(&w->port), port_height_1048_d86c(&w->port));
    GRPort_CopyToPort(&w->port, (GRPort *)w, &r, &w->rect);
    EuropeWin_DrawEurope(w);
    copy_all_to_screen_1048_dd24(w);
    g_europe_screen_open = 1;
    ColChild_Center(w);
    show_window_1048_dd42(w);
    win_show_and_focus_1048_d8f7(&(DialogWin &)*w);
    if (unit >= 0) {
        nation = unit;
        ColChild_CancelMessage(w, 0);
        ColChild_AddToMessage(w, g_nation_adjective[(unsigned char)(g_units[nation].flags & UNIT_NATION_MASK)]);
        ColChild_AddToMessage(w, g_unit_types[g_units[nation].type].name);
        ColChild_AddToMessage(w, g_misc_words[MISC_NOW_ARRIVING_IN]);
        ColChild_AddToMessage(w, g_nation_name_word[g_cur_nation]);
        EuropeWin_EuropeActivateMessage(w, 1, 0x78L);
    }
    MSDoAllTasks_1048_d701();
    if ((g_game.game_flags & GAMEFLAG_TUTORIAL) && (g_game.tutorial_flags & 0x20) == 0) {
        popup_say(0, g_nation_name_word[g_cur_nation]);
        popup_country(1, 0, g_cur_nation);
        set_name_slot_1048_de36(2, g_nation_text[g_cur_nation].country);
        ask_text_resource("TUTORIAL17");
        g_game.tutorial_flags |= 0x20;
    }
    g_modal_active = 1;
    EventWin_Modal((EventWin *)&(DialogWin &)*w);
    europe_remove_label();
    win_release_1048_e0a1(w);
    SetPopupParent(&g_main_win);
    GameWin_Redraw((GameWin *)&g_stat_win);
    MSDoAllTasks_1048_d701();
}
