/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 103, 105, 107, 128, 130 of the imported file. */
/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of CTable_Set, GameWin_Init, IsBackdrop, show_window_1040_a79d, win_center_1040_a778, win_show_and_focus_1040_ac70 removed: the header declares them from the definition (W1 step 3). */
/* show_ship: candidate for 1040:7dc4 (1656 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_ship
 * name-source: sibling-build
 * size-source: 1656, not the census 1643. The last thirteen bytes are a jump table of six entries and
 * a byte of padding; `build_check` starts after them.
 *
 * THE GAME'S OPENING SEQUENCE, one page at a time. Scene 0 is the credits screen, and scenes 1 to 6
 * are the pages that introduce the player -- the ones that say who you are, which nation you sail for
 * and which monarch sent you. Every scene loads picture 0x4b0 plus its own number and the text
 * resource BUILD<n+1> for its first line, so the art and the prose are numbered together.
 *
 * WHAT EACH PAGE ADDS to that line is the switch:
 *   1  the difficulty word and then, on a second line twenty-four pixels (0x18) down, the leader's name;
 *   2  the nation's adjective and the next line of the resource;
 *   3  " Statholder " for the Dutch and " King " for everyone else, then the next line, then the
 *      monarch's name, then one more line -- four pieces in one sentence;
 *   6  the nation's name and a full stop;
 *   4, 5 and anything else draw the line exactly as the resource gave it.
 * The Dutch test is on the nation number 3 alone, so it is the one place in the intro where a title
 * is chosen rather than looked up.
 *
 * SCENE 0 also builds the window. It asks `IsBackdrop` for a parent and, when there is one, wires
 * three handlers on the overlay so a click or a key can skip ahead; with no parent it opens over the
 * object at SEG20:0x5ca instead and wires nothing, which is the case during start-up before the
 * desktop exists.
 *
 * The window itself is a STATIC object in the data segment at SEG29:0x11ea, not a local and not a
 * `new` -- so every scene draws into the same one and the sequence is a series of repaints.
 *
 * WHAT THE BYTES SETTLED: the scene number is a REGISTER PARAMETER, which shows as `MOV DI,[BP+6]`
 * BEFORE `__InitExceptBlock_rtl` -- a local copy would be loaded after it. The number buffer the scene
 * index is formatted into is ten bytes, which only the frame size proves. And `LTOA1` takes an INT here
 * despite its name: the call pushes two bytes for the value and eight in total.
 *
 * RENAMED 2026-09-26 from opening_scene: the Mac build's show_ship(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee ColorSync; callee Set; callee TextColor; callee say_string), the runner-up gets 2, the arity agrees (1), and the Mac body is 0.95x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own EventTable, DialogWin, Font, GRPort, CTable, GameWin, Screen removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "nations.h"
   /* DialogWin : EventWin, flattened for C */


#include "records.h"

extern void * g_backdrop_win;           /* DGROUP:0xc6, declared */

   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;                 /* SEG20:0x5ca, by address */
extern GameState g_game;   /* SEG20:0x7782 */

   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */


struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int id, int a, int b, struct logpal *pal);

extern void GRPort_ColorSync(GRPort *port, CTable *pal);
extern void GRPort_FillColor_all(GRPort *w, int colour);
extern void DoFizzle(GRPort *port, GameWin *w, RECT *r, int x, int y,
                              int mode, int step);

extern void opening_dismiss_1040_7d94(void);
extern void opening_dismiss_1040_7d80(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
/* PORT: an in-file stub of set_on_lclick_1040_ab03 removed: src/game/set_on_lclick_1040_ab03.c defines it (W2). */
/* PORT: an in-file stub of set_on_rclick_1040_ab3c removed: src/game/set_on_rclick_1040_ab3c.c defines it (W2). */
/* PORT: an in-file stub of set_on_keydown_after_1040_ab75 removed: src/game/set_on_keydown_after_1040_ab75.c defines it (W2). */
/* PORT: an in-file stub of set_on_keydown_1040_abfe removed: src/game/set_on_keydown_1040_abfe.c defines it (W2). */
/* PORT: an in-file stub of set_on_lbutton_down_1040_ac37 removed: src/game/set_on_lbutton_down_1040_ac37.c defines it (W2). */
extern "C" void text_open(void *a, char *id);
extern "C" char *text_get(void);
extern "C" void text_close(void);
extern "C" int port_width_1040_abae(GRPort *port);
extern "C" int port_height_1040_abc7(GRPort *port);


extern "C" void show_ship(register int n)
{
    register int y;
    GRPort port;
    RECT r;
    void *parent;
    char num[10];
    char line[0x50];
    char buf[0x50];

    if (n == 0) {
        parent = IsBackdrop();
        if (parent != 0) {
            GameWin_Init(&g_opening_win, "", 0x801, 0, 0, 0x280,
                                             0x1e0, &g_game_palette, (Screen *)parent);
            set_on_lclick_1040_ab03((EventTable *)((char *)g_backdrop_win + 0xd),
                                          (void*)(opening_dismiss_1040_7d94));
            set_on_rclick_1040_ab3c((EventTable *)((char *)g_backdrop_win + 0xd),
                                          (void*)(opening_dismiss_1040_7d94));
            set_on_keydown_after_1040_ab75((EventTable *)((char *)g_backdrop_win + 0xd),
                                          (void*)(opening_dismiss_1040_7d80));
        } else
            GameWin_Init(&g_opening_win, "", 0x801, 0, 0, 0x280,
                                             0x1e0, &g_game_palette, (Screen *)&g_main_win);
        GRPort_LoadPicture(&port, 0x4b0, 0xa, 0xea, (struct logpal *)&g_game_palette);
        CTable_Set(&g_game_palette, 0xf5, 0xff, 0xff, 0);
        TextColor(0xf5);
        GRPort_ColorSync(&port, &g_game_palette);
        num[0] = 0;
        STRCPY(buf, "BUILD");
        LTOA1(n + 1, num, 10);
        STRCAT(buf, num);
        text_open(0, buf);
        STRCPY(buf, text_get());
        text_close();
        SetRect(&r, 0, 0, port_width_1040_abae(&port), port_height_1040_abc7(&port));
        GRPort_FillColor_all((GRPort *)&g_opening_win, 0);
        GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, 0x19, 0x10);
        copy_all_to_screen_1040_abe0(&g_opening_win);
        win_center_1040_a778((DialogWin *)((char *)&g_opening_win + 0x26));
        set_on_keydown_1040_abfe((EventTable *)((char *)&g_opening_win + 0x33),
                                      (void*)(opening_dismiss_1040_7d80));
        set_on_lbutton_down_1040_ac37((EventTable *)((char *)&g_opening_win + 0x33),
                                     (void*)(opening_dismiss_1040_7d94));
        win_show_and_focus_1040_ac70((DialogWin *)((char *)&g_opening_win + 0x26));
        show_window_1040_a79d(&g_opening_win);
        MSDoAllTasks_1040_ac99();
        DoFizzle(&port, &g_opening_win, &r, 0, 0, -1, 0x20);
    } else {
        GRPort_LoadPicture(&port, n + 0x4b0, 0xa, 0xea, (struct logpal *)&g_game_palette);
        num[0] = 0;
        CTable_Set(&g_game_palette, 0xf5, 0xff, 0xff, 0);
        TextColor(0xf5);
        GRPort_ColorSync(&port, &g_game_palette);
        STRCPY(buf, "BUILD");
        LTOA1(n + 1, num, 10);
        STRCAT(buf, num);
        text_open(0, buf);
        STRCPY(buf, text_get());
        SetRect(&r, 0, 0, port_width_1040_abae(&port), port_height_1040_abc7(&port));
        y = 0x19;
        switch (n) {
        case 1:
            STRCAT(buf, " ");
            say_string(buf, g_difficulty_words[g_game.difficulty]);
            STRCAT(buf, ",");
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            buf[0] = 0;
            STRCPY(buf, g_nation_text[g_game.player_nation].leader);
            STRCAT(buf, ",");
            y += 0x18;
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            break;
        case 2:
            STRCAT(buf, " ");
            say_string(buf, g_nation_name_word[g_game.player_nation]);
            STRCAT(buf, " ");
            STRCPY(line, text_get());
            STRCAT(buf, line);
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            break;
        case 3:
            if (g_game.player_nation == NATION_NETHERLANDS)
                STRCAT(buf, " Statholder ");
            else
                STRCAT(buf, " King ");
            STRCPY(line, text_get());
            STRCAT(buf, line);
            STRCAT(buf, " ");
            say_string(buf, g_nation_name[g_game.player_nation]);
            STRCAT(buf, text_get());
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            break;
        case 6:
            STRCAT(buf, " ");
            say_string(buf, g_nation_name[g_game.player_nation]);
            STRCAT(buf, ".");
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            break;
        default:
            GRPort_String(&port, (unsigned *)&g_font_small, buf, 0x140, y, 0x10);
            break;
        }
        DoFizzle(&port, &g_opening_win, &r, 0, 0, -1, 0x20);
    }
    text_close();
}

