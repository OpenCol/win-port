/* PORT: this file's own declarations of GamePrefs_GetWindow, GamePrefs_RegisterWindow, GamePrefs_WindowArea, GamePrefs_WindowState, MSHideWindow, MSMaximizeWindow, MSSetBackground, MapWin_Init, MiniWin_Init, StatWin_Init, win_set_icon_1078_2832 removed: the header declares them from the definition (W1 step 3). */
/* MainWin_Init: candidate for 1048:a4bc (543 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin_Init
 *
 * Builds the main window: menu 0x80 into the menu slot (+0x8c), the frame window
 * "Colonization for Windows" (style 0x7c, 640x440, the game palette at SEG20:0x1ac), a
 * 500x400 minimum size, a call to the empty function at 1070:7e42, background colour 7, and the handlers: close
 * query, WM_COMMAND dispatcher, keyboard accelerators, right click, the two geometry
 * handlers. Placement 1 of the preferences root (SEG20:0x10) restores the saved rectangle
 * (maximised when its code is 2); without one the window is maximised, hidden and its
 * rectangle recorded. Then the map, info and status children are initialised and the
 * low-memory watchdog armed (warn below 1500 KB, checked every 10 seconds, message resource 666).
 *
 * The module's one-line wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call `.
 *
 * `g_4cf4` IS WRITTEN ONCE AND NEVER READ, and the store is `= 0` over a cell DGROUP already
 * ships as 0. One instruction in the image touches 0x4cf4 and it is this one
 * (`tools/check_dead_dgroup.py`), so the statement cannot change anything the game observes. It
 * is in the shipped bytes and stays; it keeps its address because there is nothing to name.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, MenuClass, MapWin, MiniWin, StatWin, ReportWin, ColonyWin, EuropeWin, CTable, GamePrefs removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-









class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin { public: MenuClass menu; MapWin map; MiniWin mini; StatWin stat; ReportWin report; ColonyWin colony; EuropeWin europe; PediaWin pedia; MainWin(); ~MainWin(); };
   /* 0x41e bytes: the palette, its handle and the cycling state */
#include "records.h"


extern void MSFitWindow(MSWin *win, RECT *r);
extern void MSGetWinSize(MSWin *win, RECT *r);

extern void EnableLowMemoryCheck(int kb, int secs, int res, EventWin *win, int flag);

extern void MainWin_MainMenuProc(long cmd);
extern void KeyEvents(int key);

extern void MainWin_OnMove(void);
extern void MainWin_OnSize(void);

extern "C" void set_min_track_size_1048_e130(EventWin *o, int min_track_w, int min_track_h);

extern "C" void win_set_back_color_1048_e152(Screen *o, int cursor);

extern "C" void *set_on_close_1048_df3e(EventTable *table, void *h);

extern "C" void set_command_handler_1048_e176(EventWin *o, void *h);

extern "C" void *set_on_keydown_1048_dceb(EventTable *table, void *h);

extern "C" void *set_on_rbutton_down_1048_df05(EventTable *table, void *h);

extern "C" void win_set_icon_1048_e191(Screen *o, int icon);

extern "C" void win_move_to_rect(Screen *o, RECT *r);

extern "C" void MSMaximizeWindow_of(Screen *o);

extern "C" void MSHideWindow_of_1048_d4d5(Screen *o);

extern "C" void MSGetWinSize_of_1048_e1d6(Screen *o, RECT *r);

extern "C" void *set_on_move(EventTable *table, void *h);

extern "C" void *set_on_size(EventTable *table, void *h);

void MainWin_Init(MainWin *o)
{
    RECT rc;

    menu_init(&o->menu, 0x80);
    init_and_set_1060_9e4d(o, "Colonization for Windows", 0x7c, 0, 0, 640, 440, &g_game_palette, &o->menu);
    set_min_track_size_1048_e130(o, 500, 400);
    g_4cf4 = 0;
    empty_1070_7e42(o);
    win_set_back_color_1048_e152(o, 7);
    set_on_close_1048_df3e(&(EventTable &)*o, (void *)MainWin_CanClose);
    set_command_handler_1048_e176(o, (void *)MainWin_MainMenuProc);
    set_on_keydown_1048_dceb(&(EventTable &)*o, (void *)KeyEvents);
    set_on_rbutton_down_1048_df05(&(EventTable &)*o, (void *)MainWin_RightMouseClick);
    win_set_icon_1048_e191(o, 0x80);
    if (GamePrefs_GetWindow(&g_prefs_root, 1) != 0) {
        GamePrefs_WindowArea(&g_prefs_root, 1, &rc);
        win_move_to_rect(o, &rc);
        if (GamePrefs_WindowState(&g_prefs_root, 1) == 2) {
            MSMaximizeWindow_of(o);
            MSHideWindow_of_1048_d4d5(o);
        }
    } else {
        MSMaximizeWindow_of(o);
        MSHideWindow_of_1048_d4d5(o);
        MSGetWinSize_of_1048_e1d6(o, &rc);
        GamePrefs_RegisterWindow(&g_prefs_root, 1, &rc, 0);
    }
    set_on_move(&(EventTable &)*o, (void *)MainWin_OnMove);
    set_on_size(&(EventTable &)*o, (void *)MainWin_OnSize);
    MapWin_Init(&o->map);
    StatWin_Init(&o->stat);
    MiniWin_Init(&o->mini);
    EnableLowMemoryCheck(1500, 10, 666, (EventWin *)o, 1);
}
