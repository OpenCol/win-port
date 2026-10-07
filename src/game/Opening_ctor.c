/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 90, 91, 92, 93, 94 of the imported file. */
/* PORT: this file's own declarations of Sprite_Init, set_on_keydown_1010_2a1b, set_on_keydown_after_1010_2cdd, set_on_lbutton_down_1010_29e2, set_on_lclick_1010_2c6b, set_on_rclick_1010_2ca4, win_center_1010_299c removed: the header declares them from the definition (W1 step 3). */
/* Opening_ctor: candidate for 1010:0f0e (3099 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening::Opening()
 *
 * Constructs the opening-screen window and all of its ports, colour tables and sprite
 * strips, loads the four sequence resources ('SHIP' path, 'CRED' card times, 'MONS' events,
 * 'CRDS' banner spots) and the animation sprites, assembles the initial
 * title backdrop, and installs the opening's event handlers and low-memory watch.
 *
 * THE EXCEPTION DESCRIPTOR COUNTS SUB-OBJECTS. The word before this function (Borland's
 * per-function unwind table, 14 bytes at 1010:0f00) has an entry for the destructible local, and
 * its third word is the number of destructor slots that PRECEDE that local in the unwind
 * numbering: for a constructor, every base and member of `this` with a destructor, counted
 * transitively through the bases' own bases and members, array elements one each (established
 * with BCC 4.52 probes, 2026-09-28). The shipped word is 0xd0 = 208: GameWin's five (GRPort,
 * DialogWin, EventWin, Screen, EventTable), `port1`, `port2`, `table`, `logo` and the 198 Sprite
 * elements of the eleven strips. With `GameWin` declared as an opaque blob this file produced
 * 0xcb = 203, so the class is declared with its real chain -- GRPort + DialogWin(EventWin(Screen,
 * EventTable)) -- which is what the rest of the corpus declares (ColChild_ctor_1048_e93b,
 * SuperWin_ctor_1060_6068). The code bytes do not change; the grader masks the table, only
 * tools/check_data_refs.py compares it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

/* PORT: an unused `typedef void (*Handler)(void);` removed: the header's Handler is the
   message-handler slot type the other files use (W1). */

extern Opening *g_opening;   /* DGROUP:0x964 */
extern EventWin * g_backdrop_win;   /* DGROUP:0xc6: the backdrop; its EventTable part sits at +0xd */

struct logpal;

extern void CTable_Scale(CTable *, int);
extern void sound_play(int);
extern void Sprite_Extract_key_rect(Sprite *, void *src, unsigned char transparent, RECT *);

extern void GRPort_CopyToPort(GRPort *src, GRPort *, RECT *, RECT *);
extern void CTable_ResetScale(CTable *);
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);


extern "C" void show_window_1010_2af0(void *p);

extern "C" void *MSGetResource_1010_2d16(void *out, int id);
extern "C" void *MSLockResource_1010_2d39(void *h);

extern "C" void MSHideWindow_of_1010_2b41(void *p);

extern "C" void Opening_OpeningKeys(void);
extern "C" void Opening_OpeningMouse(void);

Opening::Opening()
{
    CTable new_table;
    RECT r0;
    RECT r1;
    long end_tick;
    long type0 = g_opening_restypes[0];
    long type1 = g_opening_restypes[1];
    long type2 = g_opening_restypes[2];
    long type3 = g_opening_restypes[3];
    unsigned char rgb[0x300];
    register int i;

    g_opening = this;
    sound_played = 0;
    state = -1;
    sequence_done = 0;

    SetRect(&r0, 0, 0, 0x280, 0x1e0);
    GameWin_Init(this, "Sid Meier's Colonization", 0x801, 0, 0,
                                    r0.right, r0.bottom, &g_game_palette, g_backdrop_win);
    GRPort_LoadPicture(this, 0x270f, 10, 0xec, (struct logpal *)&g_game_palette);
    CTable_SetScale(&g_game_palette, 10, 0xec, 0x32);
    CTable_Scale(&g_game_palette, 0);
    win_center_1010_299c(&(DialogWin &)*this);
    sound_play(0x34);
    copy_all_to_screen_1010_2c4d(this);
    show_window_1010_2af0(this);

    i = 0;
    set_on_keydown_1010_2a1b(&(EventTable &)*this, (void*)(Opening_OpeningKeys));
    set_on_lbutton_down_1010_29e2(&(EventTable &)*this, (void*)(Opening_OpeningMouse));
    set_on_lclick_1010_2c6b(&(EventTable &)*g_backdrop_win, (void*)(Opening_OpeningMouse));
    set_on_rclick_1010_2ca4(&(EventTable &)*g_backdrop_win, (void*)(Opening_OpeningMouse));
    set_on_keydown_after_1010_2cdd(&(EventTable &)*g_backdrop_win, (void*)(Opening_OpeningKeys));
    MSDoAllTasks_1010_2a54();

    end_tick = MSGetTicks_1010_2bbf() + 0xf0L;
    ship_path_res = MSGetResource_1010_2d16(&type0, 0x251c);
    ship_path = (Point *)MSLockResource_1010_2d39(ship_path_res);
    credit_times_res = MSGetResource_1010_2d16(&type1, 0x251d);
    credit_times = (CreditCue *)MSLockResource_1010_2d39(credit_times_res);
    monster_events_res = MSGetResource_1010_2d16(&type2, 0x251e);
    monster_events = (AnimEvent *)MSLockResource_1010_2d39(monster_events_res);
    credit_spots_res = MSGetResource_1010_2d16(&type3, 0x251f);
    credit_spots = MSLockResource_1010_2d39(credit_spots_res);

    Sprite_Init(&logo, 0x237d);
    i = 0;
    while (i <= 0x32) {
        CTable_Scale(&g_game_palette, i++);
        MSWaitTicks_1010_2d59(1);
    }
    for (i = 0; i < 0x12; i++) Sprite_Init(&ship_state1[i], 0x2329 + i);
    for (i = 1; i < 0xd; i++) Sprite_Init(&monster1[i], 0x233a + i);
    for (i = 0; i < 0x36; i++) Sprite_Init(&ship_state2[i], 0x2347 + i);
    for (i = 1; i < 0x10; i++) Sprite_Init(&monster2[i], 0x237d + i);
    for (i = 1; i < 0xe; i++) Sprite_Init(&monster3[i], 0x238c + i);
    for (i = 1; i < 0x16; i++) Sprite_Init(&monster4[i], 0x2399 + i);
    for (i = 0; i < 8; i++) Sprite_Init(&ship_state0[i], 0x23af + i);
    for (i = 1; i < 8; i++) Sprite_Init(&timed_monster[i], 0x23b6 + i);
    for (i = 1; i < 0xb; i++) Sprite_Init(&monster5[i], 0x23bd + i);
    for (i = 1; i < 0xd; i++) Sprite_Init(&monster6[i], 0x23c7 + i);
    for (i = 0; i < 0x15; i++) Sprite_Init(&credit_banners[i], 0x23dc + i);

    GRPort_LoadPicture(&port1, 0x2328, 10, 0xec, (struct logpal *)&new_table);

    SetRect(&r0, 0x2b4, 0xe6,
            0x2b4 + rect_width_1010_2b62(&monster1[1]),
            0xe6 + rect_height_1010_2b81(&monster1[1]));
    Sprite_Extract_key_rect(&monster1[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x499, 0x62,
            0x499 + rect_width_1010_2b62(&monster2[1]),
            0x62 + rect_height_1010_2b81(&monster2[1]));
    Sprite_Extract_key_rect(&monster2[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x499, 0x62,
            0x499 + rect_width_1010_2b62(&monster3[1]),
            0x62 + rect_height_1010_2b81(&monster3[1]));
    Sprite_Extract_key_rect(&monster3[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x2a1, 0x6d,
            0x2a1 + rect_width_1010_2b62(&monster4[1]),
            0x6d + rect_height_1010_2b81(&monster4[1]));
    Sprite_Extract_key_rect(&monster4[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x561, 0x123,
            0x561 + rect_width_1010_2b62(&timed_monster[1]),
            0x123 + rect_height_1010_2b81(&timed_monster[1]));
    Sprite_Extract_key_rect(&timed_monster[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x63d, 0x61,
            0x63d + rect_width_1010_2b62(&monster5[1]),
            0x61 + rect_height_1010_2b81(&monster5[1]));
    Sprite_Extract_key_rect(&monster5[0], &port1, 0xff, &r0);
    SetRect(&r0, 0x4ab, 0x118,
            0x4ab + rect_width_1010_2b62(&monster6[1]),
            0x118 + rect_height_1010_2b81(&monster6[1]));
    Sprite_Extract_key_rect(&monster6[0], &port1, 0xff, &r0);

    monster_slots[0].active = 0;
    monster_slots[1].active = 0;
    monster_slots[2].active = 0;
    monster_slots[3].active = 0;
    monster_slots[4].active = 0;
    monster_slots[5].active = 0;
    monster_slots[6].active = 0;
    ship_sprite = 0;
    ship_x = 0;
    ship_y = 0;
    credit_index = 0;
    credit_shown = 0;

    GRPort_Init_wh(&port2, 0x84, 0x84);
    SetRect(&r0, 0x500, 0x17b, 0x780, 0x1e0);
    SetRect(&r1, 0, 0x17b, 0x280, 0x1e0);
    GRPort_CopyToPort(&port1, &port1, &r0, &r1);
    SetRect(&r0, 0x500, 0, 0x780, 0x38);
    SetRect(&r1, 0, 0, 0x280, 0x38);
    GRPort_CopyToPort(&port1, &port1, &r0, &r1);

    while (MSGetTicks_1010_2bbf() < end_tick && sequence_done == 0)
        MSDoAllTasks_1010_2a54();

    i = 0x32;
    while (i >= 0) {
        CTable_Scale(&g_game_palette, i--);
        MSWaitTicks_1010_2d59(1);
    }
    MSHideWindow_of_1010_2b41(&(DialogWin &)*this);
    CTable_ResetScale(&g_game_palette);
    MSDoAllTasks_1010_2a54();
    CTable_CopyRawColors((CTable *)((struct logpal *)&new_table), rgb, 10, 0xec);
    CTable_Fill(&g_game_palette, 10, 0xec, rgb);

    SetRect(&r0, 0, 0, 0x280, 0x1e0);
    SetRect(&r1, 0x500, 0, 0x780, 0x1e0);
    GRPort_CopyToPort(&port1, this, &r1, &r0);
    copy_all_to_screen_1010_2c4d(this);
    MSDoAllTasks_1010_2a54();

    sequence_done = 0;
    scroll = 0;
    frame = 0;
    state = 0;
    skip = 0;
    show_window_1010_2af0(this);
    EnableLowMemoryCheck(0x5dc, 10, 0x29a, &(EventWin &)*this, 1);
}
