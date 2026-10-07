/* PORT: this file's own declarations of GRPort_String, font_height_1058_be4f removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of GRPort_DrawInsetRect, GameWin_Init, show_window_1058_bc28 removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_DoExtendedLaborReport: candidate for 1058:3236 (2270 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_DoExtendedLaborReport
 * name-source: sibling-build
 *
 * THE DETAIL WINDOW the labour report opens when one of its twenty-nine rows is clicked. It
 * answers one question -- where is every colonist of this trade? -- by walking the whole game
 * state twice and listing the colonies that hold one.
 *
 * FINDING THE ROW is a linear hit test over the twenty-nine rectangles the report keeps in three
 * parallel arrays in its own object (x at +0x7fe, y at +0x838, the row's count at +0x872), each
 * 0xce wide and 0x25 tall. The loop does NOT stop at the first hit, so a click in overlapping
 * rectangles takes the last. Nothing found and the window is never built.
 *
 * THE TALLY is two passes. Every colony of the player's is asked for each colonist's specialty,
 * and every unit of the player's that is a colonist and is standing somewhere is asked where: off
 * the map it counts as at sea, on the map but outside a colony it counts as outside, inside one it
 * is added to that colony's bucket. Specialty 0x13 is counted as 0x1c -- the one place the row
 * index and the specialty number disagree.
 *
 * THE WINDOW is tiled with the grid sprite, has picture 0xca blitted through an offscreen port and
 * three nested frames (a bevel, an inset rect and a raised rect) inflated out of one rectangle.
 * The title is drawn TWICE, one pixel apart, which is how this module makes text bold. Then the
 * three tallies at sea / outside / total, the trade's name and the clicked row's count, and last
 * the colonies, three across and TEN ROWS DEEP.
 *
 * TWO THINGS SHIPPED IMPERFECT. The colony list stops after thirty entries while the bucket array
 * holds forty-eight, so a player with more than thirty colonies holding this trade sees the tally
 * count colonists the list never shows. And two locals are initialised to 0x3e and 0x2f -- the
 * tile width and height -- and never read; the tiling loop uses the same two literals directly.
 *
 * WHAT THE BYTES SETTLED: the locals WITH initialisers are declared BEFORE the two objects, which
 * is why all five stores land ahead of `__InitExceptBlock_rtl`'s count and the constructors. The
 * nation field is `unsigned char` so that both owner tests stay byte-wide, and the unit test needs
 * `(unsigned char)(flags & 0xf)` to keep the mask in AL (rule 83). The modal wait at the end is a
 * `while` with an EMPTY body, which puts its entry jump on the instruction after it -- the `EB 00`
 * of rule 54.
 *
 * RENAMED 2026-09-27 from labor_report_colonist_detail: CONST PROPOSAL. Mac ReportWin::DoExtendedLaborReport(short, short), CODE 18 0x2f9a, 2590 bytes, called only by OnLaborClick. 3236 (ReportWin *o, int mx, int my), 2270 bytes, is called only by ReportWin_OnLaborClick. Callees: GRPort/Screen ctors in an EH frame, SetGlobalSpriteScale, GameWin::Init, center window, Sprite::Draw, LoadPicture, GRPort_CopyToPort, GRPort::Init, bevel/inset/raised rects, set_colony, get_specialty, is_colonist, on_map, find_colony, strings, String, say_string/say_number, ColonistIcon, show window, MSDoAllTasks, LButtonPressed. 3236's list is the same. Layout: Mac ReportsLabor, OnLaborMouseMove, DoExtendedLaborReport, OnLaborClick; Windows ReportWin_ReportsLabor, ReportWin_OnLaborMouseMove, 3236, ReportWin_OnLaborClick. C++ file with an extern "C" definition.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "misc.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SButton, SImageButton, ColChild, ReportWin, Sprite, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-


class DlgControl;






class SButton;

#include "records.h"
#pragma option -a

#pragma option -a-

   /* 0x41e bytes: the palette, its handle and the cycling state */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int id, int a, int b, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void FrameInfoRect(GRPort *port, RECT *r, int depth, int light, int dark);
extern void GRPort_DrawRaisedRect(GRPort *p, RECT *r);

extern int get_specialty(int u);
extern int is_colonist(int u);

extern char *strings(int n);

extern int ColonistIcon(int spec);
extern int Screen_LButtonPressed(Screen *o, char own, char remove, char wait);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_center_1058_c020(DialogWin *w);


extern "C" void ReportWin_DoExtendedLaborReport(ReportWin *o, int mx, int my)
{
    register int i;
    register int u;
    int tw = 0x3e;
    int th = 0x2f;
    int spec;
    int count;
    RECT r;
    RECT r2;
    int row;
    int col;
    int x;
    int y;
    int x2;
    int y2;
    int key;
    int sp;
    int colony;
    int j;
    int ncol;
    int nrow;
    int atsea = 0;
    int outside = 0;
    int total = 0;
    GameWin win;
    GRPort port;
    RECT r3;
    RECT r4;
    int counts[0x30];

    u = -1;
    for (i = 0; i < 0x1d; i++) {
        SetRect(&r, o->cell_x[i], o->cell_y[i] - 1, o->cell_x[i] + 0xce, o->cell_y[i] + 0x24);
        if (mx > r.left && mx < r.right && my > r.top && my < r.bottom)
            u = i;
    }
    if (u < 0)
        return;
    count = o->counts[u];
    spec = u;
    SetGlobalSpriteScale(1, 1);
    GameWin_Init(&win, "", 0x202, 0, 0, 0x1c2, 0x180, &g_game_palette,
                                     (Screen *)((char *)o + 0x26));
    win_center_1058_c020(&win);
    for (row = 0; port_height_1058_c045(&win) / 0x2f + 1 > row; row++)
        for (col = 0; port_width_1058_be8e(&win) / 0x3e + 1 > col; col++)
            Sprite_Draw(&r3, &g_background_tile, &win, col * 0x3e, row * 0x2f);
    GRPort_LoadPicture(&port, 0xca, 0x10, 0x18, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_be8e(&win), port_height_1058_c045(&win));
    SetRect(&r2, 0, 0, port_width_1058_be8e(&win), port_height_1058_c045(&win));
    InflateRect(&r2, -0x10, -0x10);
    GRPort_CopyToPort(&port, &win, &r, &r2);
    GRPort_Init_wh_1058_c064(&port);
    SetRect(&r, 0, 0, port_width_1058_be8e(&win), port_height_1058_c045(&win));
    InflateRect(&r, -0x10, -0x10);
    r.top += 5;
    FrameInfoRect(&win, &r, -3, 0x16, 0x12);
    InflateRect(&r, -3, -3);
    GRPort_DrawInsetRect(&win, &r);
    InflateRect(&r, 6, 6);
    GRPort_DrawRaisedRect(&win, &r);
    memset_rtl(counts, 0, 0x60);
    key = u;
    if (key == 0x13)
        key = 0x1c;
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if (g_cur_colony_rec->nation != (unsigned char)o->nation)
            continue;
        for (j = 0; g_cur_colony_rec->pop > j; j++) {
            sp = get_specialty(j);
            if (sp == key) {
                total++;
                counts[i]++;
            }
        }
    }
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != (unsigned char)o->nation)
            continue;
        if (is_colonist(u) == 0)
            continue;
        sp = g_units[u].spec;
        if (sp != key)
            continue;
        if (on_map(g_units[u].x, g_units[u].y) == 0) {
            atsea++;
            continue;
        }
        colony = find_colony(g_units[u].x, g_units[u].y);
        if (colony >= 0) {
            total++;
            counts[colony]++;
        } else
            outside++;
    }
    y = 5;
    TextColor(0xc);
    GRPort_String(&win, (unsigned *)&g_font_small, strings(g_misc_words[MISC_LABOR_ADVISOR_REPORT]),
                         port_width_1058_be8e(&win) / 2, 0xa, 0);
    GRPort_String(&win, (unsigned *)&g_font_small, strings(g_misc_words[MISC_LABOR_ADVISOR_REPORT]),
                         port_width_1058_be8e(&win) / 2 + 1, 0xa, 0);
    y += font_height_1058_be4f(&g_font_small) + 0xe;
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_spec_recs[spec].expert);
    TextColor(0xff);
    GRPort_String(&win, (unsigned *)&g_font_small, message_buf_1058_bec6(),
                         port_width_1058_be8e(&win) / 2, y, 0);
    SetGlobalSpriteScale(2, 1);
    Sprite_Draw(&r4, &g_icon_sprites[ColonistIcon(spec)], &win, 0x5a, 0x3c);
    SetGlobalSpriteScale(1, 1);
    x = 0xb4;
    y = 0x50;
    y2 = y;
    for (i = 0; i < 3; i++) {
        message_buf_clear();
        say_string(message_buf_1058_bec6(), g_misc_words[MISC_OFF_MAPBOARD_EUROPE + i]);
        message_buf_cat_1058_c084(":");
        TextColor(0xff);
        GRPort_String(&win, (unsigned *)&g_number_font, message_buf_1058_bec6(), x, y2, 1);
        x2 = x + 0xa0;
        message_buf_clear();
        switch (i) {
        case 0:
            say_number(message_buf_1058_bec6(), atsea);
            break;
        case 1:
            say_number(message_buf_1058_bec6(), outside);
            break;
        case 2:
            say_number(message_buf_1058_bec6(), total);
            break;
        }
        TextColor(0xff);
        GRPort_String(&win, (unsigned *)&g_number_font, message_buf_1058_bec6(), x2, y2, 1);
        y2 += font_height_1058_be4f(&g_number_font) + 3;
    }
    x = 0x23;
    y = 0xb4;
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_spec_recs[spec].expert);
    message_buf_cat_1058_c084(":");
    say_number_1058_bede(count);
    TextColor(0xff);
    GRPort_String(&win, (unsigned *)&g_number_font, message_buf_1058_bec6(), x, y, 1);
    x = 0x2d;
    y = 0xc8;
    ncol = 0;
    nrow = 0;
    x2 = x;
    y2 = y;
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if (counts[i] == 0)
            continue;
        message_buf_set_1058_c0a6(g_cur_colony_rec->name);
        message_buf_cat_1058_c084(":");
        say_number_1058_bede(counts[i]);
        TextColor(7);
        GRPort_String(&win, (unsigned *)&g_number_font, message_buf_1058_bec6(), x2, y2, 1);
        x2 += 0x87;
        ncol++;
        if (ncol >= 3) {
            ncol = 0;
            y2 += 0xf;
            x2 = x;
            nrow++;
            if (nrow == 0xa)
                break;
        }
    }
    show_window_1058_bc28(&win);
    MSDoAllTasks_1058_bc15();
    while (Screen_LButtonPressed(&win, 0, 1, 0) == 0)
        ;
}
