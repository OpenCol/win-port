/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GameWin_Init, ReportWin_DoReport_parent, font_height_1050_9106, win_center_1050_851f, win_show_and_focus_1050_911f removed: the header declares them from the definition (W1 step 3). */
/* Fame: candidate for 1050:7882 (1642 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Fame
 * name-source: sibling-build
 *
 * THE END-OF-GAME SCORE SCREEN. Report 9 is run first for its total; if it comes back at or below
 * zero there is nothing to show. The report's own answer is also published at DGROUP:0x4e00 on the way
 * past, in one chained assignment.
 *
 * HOW THE SCORE BECOMES A RANK. The multiplier is the difficulty plus four, plus one more at
 * difficulty 3 and again at 4 -- so 4, 5, 6, 8, 10 across the five settings. Points are the report
 * total times that over a hundred, and then HALVED, which means the displayed figure is the score
 * times the multiplier over two hundred. The rank is the largest i in 1..24 whose i squared over three
 * is still under the points, minus one, and finally clamped to 23. The squaring makes each rank cost
 * more than the last: rank 1 needs 2 points and rank 23 needs 193.
 *
 * THE SCREEN itself is picture 0x1c3 over a 640 by 480 window, three lines of the EXPLOITS text
 * centred at the top, and then the SCORE list drawn UPWARDS from twenty-four pixels above the bottom,
 * one row per rank achieved, with the row just earned in bright and the rest in grey. Under it the
 * player's title: the leader's name is passed through `strrchr` for a SPACE so only the surname is
 * used, falling back to the whole string when there is no space in it. The rank's own portrait is
 * picture 0x1f4 plus the rank, placed at (0x53, 0x70) and framed by a bevel ten pixels out.
 *
 * THREE SOUNDS, by rank: 0x24 at the very top, 0x25 above rank 6, and 0x21 for everything below.
 *
 * The rank is written through the out parameter TWICE -- once as -1 before anything, and again with
 * the real answer -- so a caller that hits either early return still sees a definite value.
 *
 * WHAT THE BYTES SETTLED: the ranking loop runs `i <= 24`, not `i < 24`, which is what makes rank 23
 * reachable and the clamp that follows a no-op rather than the thing that produces the top rank. The
 * report total is stored by `g_score_total = score = ...`: written as two statements, the second store reloads
 * from the frame. And the halving is `>> 1`, so none of the correction a signed divide needs appears.
 *
 * RENAMED 2026-09-26 from score_screen: the Mac build's Fame(short, short*), bound by layout (between HallOfFame and FinalScore on both sides); the arity agrees and the Mac body is 1.26x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "difficul.h"
#include "gameflag.h"
#include "sounds.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-


class DlgControl;


#include "records.h"

extern void * g_backdrop_win;           /* DGROUP:0xc6, declared */

   /* 0x41e bytes: the palette, its handle and the cycling state */

   /* ReportWin : ColChild, flattened for C (ColChild's kind as colchild_kind): 0x8b8 */

extern GameState g_game;   /* SEG20:0x7782 */

struct logpal;
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);
extern int GRPort_LoadPicture(GRPort *p, int id, int a, int b, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern int text_open(void *a, char *id);
extern char *text_get(void);
extern char *text_item_get(void);

extern void FrameInfoRect(GRPort *w, RECT *r, int depth, int light, int dark);
extern void sound_play(int n);
extern void text_close(void);
extern void EventWin_Modal(EventWin *w);
extern void fame_OnLMouseDown(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void expand_text_template(char *src, char *dst);

extern "C" void set_text_arg(int slot, int v);


extern "C" int port_width_1050_922e(GRPort *port);
extern "C" int port_height_1050_9247(GRPort *port);

void set_on_lbutton_down_1050_8eb9(void *slot, void (*fn)(void)) { }
extern "C" void show_window_1050_8bc8(GameWin *w);
extern "C" void MSHideWindow_of_1050_8cd8(void *w);
extern "C" void win_release_1050_9148(GameWin *w);

extern "C" int Fame(int show, int *rank_out)
{
    register int i;
    register int y;
    int rank = -1;
    int pts = 0;
    GameWin w;
    GRPort port;
    int score;
    int mult;
    int color;
    char *p;
    RECT r3;
    RECT r2;
    RECT r1;
    char buf[0x50];
    char buf2[0x50];
    char pal[0x300];

    if (rank_out != 0)
        *rank_out = -1;
    g_score_total = score = ReportWin_DoReport_parent((ReportWin *)&g_report_window, 9, 1, g_backdrop_win);
    if (score <= 0)
        return pts;
    CTable_CopyRawColors((CTable *)&g_game_palette, pal, 10, 0xec);
    mult = g_game.difficulty + 4;
    if (g_game.difficulty >= DIFFICULTY_GOVERNOR)
        mult++;
    if (g_game.difficulty >= DIFFICULTY_VICEROY)
        mult++;
    if (score != 0) {
        pts = score * mult / 100;
        for (i = 1; i <= 0x18; i++) {
            if (i * i / 3 < pts)
                rank = i - 1;
        }
    }
    pts = pts >> 1;
    rank = min_int_1050_91c4(rank, 0x17);
    if (rank_out != 0)
        *rank_out = rank;
    if (rank < 0 || show == 0 || (g_game.game_flags & GAMEFLAG_SCORED))
        return pts;
    STRCPY(buf, "SCORE");
    if (rank < 9)
        STRCAT(buf, "0");
    say_number(buf, rank + 1);
    GameWin_Init(&w, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette,
                                     (Screen *)g_backdrop_win);
    SetRect(&r1, 0, 0, 0x280, 0x1e0);
    GRPort_LoadPicture(&port, 0x1c3, 0x10, 0x24, (struct logpal *)&g_game_palette);
    GRPort_CopyToPort(&port, &w, &r1, &r1);
    GRPort_Init_wh_1050_91ea(&port);
    win_center_1050_851f((DialogWin *)&w);
    y = 0xf;
    text_open(g_game_txt, "EXPLOITS");
    set_name_slot_1050_8e3a(0, g_nation_text[g_game.player_nation].country);
    set_text_arg(0, pts);
    TextColor(0xc);
    for (i = 0; i < 3; i++) {
        text_get();
        expand_text_template(g_text_buf, buf);
        GRPort_String(&w, (unsigned *)&g_font_small, buf, port_width_1050_8b8a(&w) / 2, y, 0);
        y += font_height_1050_9106(&g_heading_font) + 1;
    }
    text_close();
    text_open(0, "SCORE");
    y = port_height_1050_8ba9(&w) - 0x18;
    for (i = 0; i <= rank; i++) {
        color = 0xfa;
        if (i == rank)
            color = 0xc;
        text_get();
        STRCPY(buf, text_item_get());
        STRCPY(buf2, text_item_get());
        TextColor(color);
        GRPort_String(&w, (unsigned *)&g_number_font, buf,
                             port_width_1050_8b8a(&w) / 2 + port_width_1050_8b8a(&w) / 4, y, 0);
        y -= font_height_1050_9106(&g_number_font) + 1;
    }
    text_close();
    p = strrchr_rtl(g_nation_text[g_game.player_nation].leader, ' ');
    if (p == 0)
        p = g_nation_text[g_game.player_nation].leader;
    set_name_slot_1050_8e3a(0, p);
    expand_text_template(buf2, buf);
    TextColor(0xc);
    y = 0x14f;
    GRPort_String(&w, (unsigned *)&g_heading_font, buf, 0xd1, y, 0);
    GRPort_LoadPicture(&port, rank + 0x1f4, 0x34, 0xc2, (struct logpal *)&g_game_palette);
    SetRect(&r3, 0, 0, port_width_1050_922e(&port), port_height_1050_9247(&port));
    SetRect(&r2, 0x53, 0x70, port_width_1050_922e(&port) + 0x53,
            port_height_1050_9247(&port) + 0x70);
    GRPort_CopyToPort(&port, &w, &r3, &r2);
    InflateRect(&r2, 10, 10);
    FrameInfoRect(&w, &r2, -2, 7, 0);
    if (rank >= 0x17)
        sound_play(SOUND_MUSIC_FIDDLERS_DANCE);
    else if (rank > 6)
        sound_play(SOUND_MUSIC_JINE_THE_CAVALRY);
    else
        sound_play(SOUND_MUSIC_SMOKY_TUNE);
    copy_all_to_screen_1050_843b(&w);
    set_on_lbutton_down_1050_8eb9((EventTable *)&w, fame_OnLMouseDown);
    show_window_1050_8bc8(&w);
    win_show_and_focus_1050_911f((DialogWin *)&w);
    MSDoAllTasks_1050_8be7();
    EventWin_Modal(&w);
    MSHideWindow_of_1050_8cd8((DialogWin *)&w);
    win_release_1050_9148(&w);
    MSDoAllTasks_1050_8be7();
    CTable_Fill(&g_game_palette, 10, 0xec, pal);
    return pts;
}
