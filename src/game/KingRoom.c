/* PORT: this file's own declarations of GameWin_Init, IsBackdrop, set_on_keydown_after_1008_da94, set_on_lclick_1008_da22, set_on_rclick_1008_da5b, show_window_1008_d117, win_center_1008_dacd, win_hide_1008_d0bc removed: the header declares them from the definition (W1 step 3). */
/* KingRoom: candidate for 1008:b3d0 (2347 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: KingRoom
 * name-source: sibling-build
 * size-source: 2347, not the census 2339. The last eight bytes are the four-entry jump table of
 * the nation switch; `KingHowdy` starts after them.
 *
 * THE THREE END-OF-GAME SPEECHES, one function with two flags. (1, 1) is the viceroy's promotion,
 * (1, anything else) is KINGLOSE and anything else is KINGWIN. The name it carries is only the
 * first of the three resources it can load.
 *
 * EACH SCREEN IS THE SAME SHAPE: save the palette, build a full-screen 0x280 x 0x1e0 window over
 * whatever `IsBackdrop` gives as a parent (and wire three skip handlers on the overlay when
 * there is one), load a picture into an offscreen port, cut a sprite out of it, load the nation's
 * own picture into the window, draw the sprite over it, then pour the text resource down the
 * screen a line every 0x12 pixels. It ends by waiting on DGROUP:0x4b8c, which those handlers set,
 * hiding the window and putting the palette back.
 *
 * THE DUTCH GET DIFFERENT PROSE. In the viceroy speech nation 3 reads its own resource VICEROY2
 * where everyone else reads VICEROY, gets a fourth line straight from the resource where the
 * others get "The King of <nation>" composed here, and gets "the " before its name in the "glory
 * of" line. The others are given three pixels of indent instead, put on before that line and
 * TAKEN OFF AGAIN after it -- a three-pixel hanging indent spelled as two adjustments to x.
 *
 * ONE MORE INDENT is the "we " paragraph: the y it starts at is remembered and every line
 * compares against it, so the FIRST line of the paragraph is indented three pixels and the rest
 * are not. That is the only use the remembered y has.
 *
 * TWO DEAD THINGS. A twenty-byte buffer is filled with the nation's own name -- ENGLND, FRANCE,
 * SPAIN or DUTCH, through a four-way jump table -- and then the scene number is appended to it,
 * and it is NEVER READ; the two later branches overwrite it with "KINGLOSE" and "KINGWIN" and
 * never read it either, while the calls that actually load those resources use their own separate
 * literals. It is the remains of a per-nation resource name, kept alive by the jump table it
 * still needs. And a rectangle is set twice, in the two `scene == 1` branches, and never passed
 * anywhere.
 *
 * RENAMED 2026-09-26 from show_endgame_speech: the Mac build's KingRoom(short, short, char*), bound by call-sequence alignment (1.0 aligned gaps in end_of_game_check leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.20x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "sounds.h"
#include "nations.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, Sprite, Font, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-
#pragma option -a-


class DlgControl;


#pragma option -a

#pragma option -a-


extern EventWin * g_backdrop_win;           /* DGROUP:0xc6, declared */

   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;                 /* SEG20:0x5ca */
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

struct logpal;
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern int GRPort_LoadPicture(GRPort *p, int id, int a, int b, struct logpal *pal);
extern void Sprite_Extract(Sprite *spr, GRPort *src, int x, int y, int w, int h);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int text_open(void *a, char *id);
extern char *text_get(void);
extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int size);
extern void sound_play(int id);
extern void text_close(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void MSFlushInput_1008_cefc(void);

extern "C" void Font_ctor_1008_d9c4(Font *o, int a, int b);

extern "C" void KingRoom(int scene, int mode, void *unused1)
{
    register int y;
    register int x;
    GameWin win;
    GRPort port;
    Sprite spr;
    Font font(0, 0x10);
    RECT box;
    int y0;
    void *parent;
    RECT r1;
    RECT r2;
    RECT r3;
    char pal[0x300];
    char name[0x14];
    char line[0x50];

    TextColor(g_draw_color);
    g_done_flag = 0;
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 0xa, 0xec);
    switch (g_game.player_nation) {
    case 0:
        STRCPY(name, "ENGLND");
        break;
    case 1:
        STRCPY(name, "FRANCE");
        break;
    case 2:
        STRCPY(name, "SPAIN");
        break;
    case 3:
        STRCPY(name, "DUTCH");
        break;
    }
    say_number(name, scene);
    parent = IsBackdrop();
    if (parent != 0) {
        GameWin_Init(&win, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette, (Screen *)parent);
        set_on_lclick_1008_da22(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b362);
        set_on_rclick_1008_da5b(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b362);
        set_on_keydown_after_1008_da94(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b37c);
    } else
        GameWin_Init(&win, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette,
                                         (Screen *)&g_main_win);
    win_center_1008_dacd(&(DialogWin &)win);
    TextColor(g_draw_color);
    if (scene == 1) {
        SetRect(&box, 0x1ce, 0x4b, 0x24f, 0x170);
        if (mode == 1) {
            y = 0x4c;
            x = 0x218;
            GRPort_LoadPicture(&port, 0x518, 0xa, 0xe3, (struct logpal *)&g_game_palette);
            Sprite_Extract(&spr, &port, 0, 0, 0x17c, 0x1e0);
            GRPort_LoadPicture(&win, g_game.player_nation + 0x514, 0xa, 0xe3, (struct logpal *)&g_game_palette);
            Sprite_Draw(&r1, &spr, &win, 0, 0);
            if (g_game.player_nation == NATION_NETHERLANDS)
                text_open(0, "VICEROY2");
            else
                text_open(0, "VICEROY");
            GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
            y += 0x12;
            GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
            y += 0x24;
            text_get();
            GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
            y += 0x12;
            if (g_game.player_nation == NATION_NETHERLANDS)
                GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
            else {
                STRCPY(line, "The King of ");
                say_string(line, g_nation_name[g_game.player_nation]);
                GRPort_String(&win, (unsigned *)&font, line, x, y, 0);
                text_get();
            }
            y += 0x24;
            text_get();
            GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
            y += 0x12;
            STRCPY(line, "glory of ");
            if (g_game.player_nation == NATION_NETHERLANDS)
                STRCAT(line, "the ");
            else
                x += 3;
            say_string(line, g_nation_name[g_game.player_nation]);
            STRCAT(line, ",");
            GRPort_String(&win, (unsigned *)&font, line, x, y, 0);
            if (g_game.player_nation != NATION_NETHERLANDS)
                x -= 3;
            y += 0x12;
            text_get();
            STRCPY(line, "we ");
            STRCAT(line, text_get());
            y0 = y;
            while (line[0] != 0) {
                GRPort_String(&win, (unsigned *)&font, line, y == y0 ? x + 3 : x, y, 0);
                y += 0x12;
                STRCPY(line, text_get());
            }
            MSDoAllTasks_1008_ce3f();
            sound_play(SOUND_MEETKING);
            text_close();
        } else {
            y = 0x4c;
            x = 0x218;
            STRCPY(name, "KINGLOSE");
            GRPort_LoadPicture(&port, 0x51b, 0xa, 0xe3, (struct logpal *)&g_game_palette);
            Sprite_Extract(&spr, &port, 0, 0, 0x190, 0x1e0);
            GRPort_LoadPicture(&win, g_game.player_nation + 0x514, 0xa, 0xe3, (struct logpal *)&g_game_palette);
            Sprite_Draw(&r2, &spr, &win, 0, 0);
            SetRect(&box, 0x1d1, 0x46, 0x253, 0x168);
            text_open(0, "KINGLOSE");
            STRCPY(line, text_get());
            while (line[0] != 0) {
                GRPort_String(&win, (unsigned *)&font, line, x, y, 0);
                y += 0x12;
                STRCPY(line, text_get());
            }
            MSDoAllTasks_1008_ce3f();
            sound_play(SOUND_MEETKING);
            text_close();
        }
    } else {
        x = 0x1ea;
        y = 0x136;
        STRCPY(name, "KINGWIN");
        text_open(0, "KINGWIN");
        GRPort_LoadPicture(&port, 0x51a, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        Sprite_Extract(&spr, &port, 0, 0, 0x1f4, 0x1e0);
        GRPort_LoadPicture(&win, g_game.player_nation + 0x51c, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        Sprite_Draw(&r3, &spr, &win, 0, 0);
        GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
        y += 0x12;
        GRPort_String(&win, (unsigned *)&font, text_get(), x, y, 0);
        y += 0x12;
        STRCPY(line, "from mother ");
        say_string(line, g_nation_name[g_game.player_nation]);
        GRPort_String(&win, (unsigned *)&font, line, x, y, 0);
        y += 0x12;
        text_get();
        STRCPY(line, text_get());
        while (line[0] != 0) {
            GRPort_String(&win, (unsigned *)&font, line, x, y, 0);
            y += 0x12;
            STRCPY(line, text_get());
        }
        MSDoAllTasks_1008_ce3f();
        sound_play(SOUND_MEETKING);
        text_close();
    }
    MSFlushInput_1008_cefc();
    if (parent != 0) {
        set_on_lclick_1008_da22(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b362);
        set_on_rclick_1008_da5b(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b362);
        set_on_keydown_after_1008_da94(&(EventTable &)*g_backdrop_win,
                                      (void *)set_done_flag_1008_b37c);
    }
    set_on_lclick_1008_da22(&(EventTable &)win, (void *)set_done_flag_1008_b362);
    set_on_rclick_1008_da5b(&(EventTable &)win, (void *)set_done_flag_1008_b362);
    set_on_keydown_after_1008_da94(&(EventTable &)win, (void *)set_done_flag_1008_b37c);
    show_window_1008_d117(&win);
    while (g_done_flag == 0)
        call_message_pump();
    win_hide_1008_d0bc(&(DialogWin &)win);
    MSDoAllTasks_1008_ce3f();
    CTable_Fill(&g_game_palette, 0xa, 0xec, pal);
}
