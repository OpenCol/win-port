/* MainWin_Init -- shipped at 1048:a4bc.
 *
 * Ported by tools/port.py from matched/game/MainWin_Init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_prefs_root;                        SEG20:0x0010
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern int g_4cf4;                                   DGROUP:0x4cf4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * set_handler_slot_16_1048_df05(void *o, void *h);
void * set_handler_slot_32_1048_dceb(void *o, void *h);
void * set_handler_slot_48_1048_df3e(void *o, void *h);
void * set_handler_slot_64(void *o, void *h);
void * set_handler_slot_68(void *o, void *h);
void MSHideWindow_of_1048_d4d5(void *o);
void MSMaximizeWindow_of(void *o);
void menu_init(void *slot, col_i16 id);
void set_p_80_1048_e176(void *o, void *h);
void set_pair_6e_1048_e130(void *o, col_i16 w, col_i16 h);
void win_move_to_rect_1048_d6da(void *o, RECT *r);
void win_screen_rect(void *o, RECT *r);
void win_set_back_color_1048_e152(void *o, col_i16 cursor);
void win_set_icon_1048_e191(void *o, col_i16 n);

typedef struct { char pad[6]; void *win; char pad2[0x6e - 10]; col_i16 min_w, min_h; char pad3[0x80 - 0x72]; void *command_handler; char pad4[0x8c - 0x84]; col_u16 hmenu; char mapwin[0x816 - 0x8e]; char statwin[0xf9c - 0x816]; char infowin[1]; } MainWin;
typedef struct { char pad[16]; void *slot; } Slot16;
typedef struct { char pad[32]; void *slot; } Slot32;
typedef struct { char pad[48]; void *slot; } Slot48;
typedef struct { char pad[64]; void *slot; } Slot64;
typedef struct { char pad[68]; void *slot; } Slot68;
typedef struct { col_i16 f0; RECT rect; col_i16 code; } PlacementRec;



extern col_u16 MSInitMenu(col_i16 id);
extern void win_set_back_color_1078_1ed6(void *w, col_i16 cursor);
extern void win_set_icon_1078_2832(void *w, col_i16 n);
extern void win_move_to_rect_1078_221b(void *w, RECT *r);
extern void MSMaximizeWindow(void *w);
extern void MSHideWindow(void *w);
extern void MSGetWinRect(void *w, RECT *r);
extern void init_and_set_1060_9e4d(MainWin *o, char *title, col_i16 style, col_i16 x, col_i16 y, col_i16 w, col_i16 h, char *palette, col_u16 *menu);
extern void empty_1070_7e42(MainWin *o);
extern PlacementRec * find_entry(void *root, col_i16 which);
extern void get_entry_rect(void *root, col_i16 which, RECT *r);
extern col_i16 find_entry_value(void *root, col_i16 which);
extern void rect_list_add(void *root, col_i16 which, RECT *r, col_i16 code);
extern void MapWin_Init(void *w);
extern void info_window_init(void *w);
extern void map_window_init(void *w);
extern void low_memory_watch_start(col_i16 kb, col_i16 secs, col_i16 res, MainWin *win, col_i16 flag);
extern col_i16 MainWin_CanClose(void);
extern void wm_command_dispatcher(col_i32 cmd);
extern void keyboard_accelerator_handler(col_i16 key);
extern void MainWin_RightMouseClick(void *p);
extern void mainwin_geometry_handler_68(void);
extern void mainwin_geometry_handler_64(void);





























void MainWin_Init(MainWin *o)
{
    RECT rc;

    menu_init(&o->hmenu, 0x80);
    init_and_set_1060_9e4d(o, "Colonization for Windows", 0x7c, 0, 0, 640, 440, &(*g_game_palette), &o->hmenu);
    set_pair_6e_1048_e130(o, 500, 400);
    g_4cf4 = 0;
    empty_1070_7e42(o);
    win_set_back_color_1048_e152(o, 7);
    set_handler_slot_48_1048_df3e((Slot48 *)((char *)o + 0xd), (void *)MainWin_CanClose);
    set_p_80_1048_e176(o, (void *)wm_command_dispatcher);
    set_handler_slot_32_1048_dceb((Slot32 *)((char *)o + 0xd), (void *)keyboard_accelerator_handler);
    set_handler_slot_16_1048_df05((Slot16 *)((char *)o + 0xd), (void *)MainWin_RightMouseClick);
    win_set_icon_1048_e191(o, 0x80);
    if (find_entry(&g_prefs_root, 1) != 0) {
        get_entry_rect(&g_prefs_root, 1, &rc);
        win_move_to_rect_1048_d6da(o, &rc);
        if (find_entry_value(&g_prefs_root, 1) == 2) {
            MSMaximizeWindow_of(o);
            MSHideWindow_of_1048_d4d5(o);
        }
    } else {
        MSMaximizeWindow_of(o);
        MSHideWindow_of_1048_d4d5(o);
        win_screen_rect(o, &rc);
        rect_list_add(&g_prefs_root, 1, &rc, 0);
    }
    set_handler_slot_68((Slot68 *)((char *)o + 0xd), (void *)mainwin_geometry_handler_68);
    set_handler_slot_64((Slot64 *)((char *)o + 0xd), (void *)mainwin_geometry_handler_64);
    MapWin_Init(o->mapwin);
    info_window_init(o->infowin);
    map_window_init(o->statwin);
    low_memory_watch_start(1500, 10, 666, o, 1);
}
