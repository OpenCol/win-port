/* ReportWin_TradeRoutes: candidate for 1058:7fca (1643 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_TradeRoutes
 * name-source: curated, matched to `ReportWin::TradeRoutes` in the Macintosh sibling build
 *
 * THE TRADE ROUTES PAGE, case 99 of the report dispatcher and the only page called without a
 * page index. It lays out the header and the five empty stop panels; `ReportWin_TradeRoute1`
 * then fills them with the route's actual stops.
 *
 * THE HEADER is two label/value pairs stacked at x 10, each label in colour 255 and each value
 * in colour 12 at a second column whose x is the width of the FIRST label plus ten -- measured
 * once, so the second value sits in that column whatever its own label's width. The first pair is
 * the route's name (`g_cur_route_rec->name`, the first 32 bytes of the record); the second is the
 * route's kind, which picks label 3 when `mode` is set and label 4 when it is clear. All eight
 * labels come from one runtime-filled table of text ids at SEG20:0xd9bc.
 *
 * THE COLUMN HEADS sit on their own baseline, `0x69 - (line height + 1)`, i.e. just above the
 * panels rather than a fixed distance below the header -- so a taller font pushes them up, not
 * down. Their first x is measured from the literal "0.  ", the widest stop number the list can
 * print, and the other two are hard-coded at 225 and 425. A raised rect from y-12 to 104 boxes
 * the whole header in.
 *
 * THE FIVE PANELS are drawn from an off-screen 300x75 port that is tiled with the sprite at
 * SEG20:0x7526, then blitted three times per row -- 1..215, 215..415, 415..width-1 -- each blit
 * preceded by a two-pixel inset bevel. Rows are 75 apart from y 105.
 *
 * TWO THINGS THE BYTES SHOW. First, the tiling loop counts with the WRONG sprite: the number of
 * columns and rows is the window divided by the dimensions of SEG20:0x7506 (the background sprite
 * `ReportWin_TradeRoute2` tiles with), while the sprite actually drawn and the step between
 * copies come from 0x7526. The two happen to be close enough in size that nothing shows.
 * Second, the function opens by composing "<label 0> <route number>" into `buf` -- a STRCPY, an
 * space_string and a say_number over the record's index in `g_routes` -- and then immediately
 * writes `buf[0] = 0` without ever drawing it. Three calls and a pointer division for a string
 * that is thrown away; the route number never reaches the screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "labels.h"
typedef struct { short hfont, height; } Font;
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SButton; class ReportWin : public ColChild { public: short kind; SImageButton image; short nation; short page; short view; short score; unsigned char modal; char pad_7f9; void *rec; int cell_x[29]; int cell_y[29]; int counts[29]; SButton *prev; SButton *next; SButton *extra_btn; ReportWin(); ~ReportWin(); };
#include "records.h"

#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-

extern char *strings(int n);
extern void space_string(char *buf);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);
extern int GRPort_Init(GRPort *p, void *r);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void FrameInfoRect(GRPort *w, RECT *r, int depth, int light, int dark);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void ReportWin_OnTradeRouteClick(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int sprite_width_1058_c203(void *sprite);
extern "C" int sprite_height_1058_c24c(void *sprite);
extern "C" int MSGetTextLength_1058_be29(void *font, const char *s);
extern "C" int font_height_1058_be4f(void *font);

extern "C" void ReportWin_TradeRoute2(ReportWin *o);
extern "C" void ReportWin_TradeRoute1(ReportWin *o, int y0);
extern "C" void *set_on_lclick_1058_bdb7(EventTable *table, void *h);

void ReportWin_TradeRoutes(ReportWin *o)
{
    register int i;
    register int top;
    GRPort port;
    int x, y, x2, col, row;
    RECT r;
    RECT src;
    RECT tile;
    char buf[0x50];

    ReportWin_TradeRoute2(o);
    STRCPY(buf, strings(g_route_label[ROUTE_LABEL_EDIT_TRADE_ROUTE]));
    space_string(buf);
    say_number(buf, (int)(g_cur_route_rec - g_routes) + 1);
    x = 10;
    y = 0x19;

    buf[0] = 0;
    say_string(buf, g_route_label[ROUTE_LABEL_ROUTE_NAME]);
    space_string(buf);
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
    x2 = MSGetTextLength_1058_be29(&g_font_small, buf) + 10;
    TextColor(0xc);
    GRPort_String(o, (unsigned *)&g_font_small, g_cur_route_rec->name, o->rect.left + x2, o->rect.top + y, 1);
    y += font_height_1058_be4f(&g_font_small) + 2;

    buf[0] = 0;
    say_string(buf, g_route_label[ROUTE_LABEL_ROUTE_TYPE]);
    space_string(buf);
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);

    buf[0] = 0;
    say_string(buf, g_route_label[g_cur_route_rec->sea ? 3 : 4]);
    TextColor(0xc);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x2, o->rect.top + y, 1);
    y += font_height_1058_be4f(&g_font_small) + 2;

    y = 0x69 - (font_height_1058_be4f(&g_font_small) + 1);
    x2 = MSGetTextLength_1058_be29(&g_font_small, "0.  ") + 10;
    TextColor(0xfa);
    GRPort_String(o, (unsigned *)&g_font_small, strings(g_route_label[ROUTE_LABEL_DESTINATION]), o->rect.left + x2, o->rect.top + y + 3, 1);
    GRPort_String(o, (unsigned *)&g_font_small, strings(g_route_label[ROUTE_LABEL_UNLOAD_CARGO]), o->rect.left + 0xe1, o->rect.top + y + 3, 1);
    GRPort_String(o, (unsigned *)&g_font_small, strings(g_route_label[ROUTE_LABEL_LOAD_CARGO]), o->rect.left + 0x1a9, o->rect.top + y + 3, 1);

    SetRect(&r, 1, y - 12, rect_width_1058_bd2d(&o->rect) - 2, 0x68);
    OffsetRect(&r, o->rect.left, o->rect.top);
    GRPort_DrawRaisedRect(o, &r);

    y = 0x69;
    SetRect(&src, 0, 0, 0x12c, 0x4b);
    GRPort_Init(&port, &src);
    for (col = 0; col < rect_width_1058_bd2d(&o->rect) / sprite_width_1058_c203(&g_background_tile); col++)
        for (i = 0; i < rect_height_1058_bd4c(&o->rect) / sprite_height_1058_c24c(&g_background_tile); i++)
            Sprite_Draw(&tile, (Sprite *)&g_route_panel_sprite, &port,
                        sprite_width_1058_c203(&g_route_panel_sprite) * col,
                        sprite_height_1058_c24c(&g_route_panel_sprite) * i);
    TextColor(g_draw_color);
    for (row = 0; row <= 4; row++) {
        top = row * 0x4b + y;
        SetRect(&r, 1, top, 0xd7, top + 0x4b);
        OffsetRect(&r, o->rect.left, o->rect.top);
        FrameInfoRect(o, &r, -2, 0xff, 7);
        GRPort_CopyToPort(&port, o, &src, &r);
        SetRect(&r, 0xd7, top, 0x19f, top + 0x4b);
        OffsetRect(&r, o->rect.left, o->rect.top);
        FrameInfoRect(o, &r, -2, 0xff, 7);
        GRPort_CopyToPort(&port, o, &src, &r);
        SetRect(&r, 0x19f, top, rect_width_1058_bd2d(&o->rect) - 1, top + 0x4b);
        OffsetRect(&r, o->rect.left, o->rect.top);
        FrameInfoRect(o, &r, -2, 0xff, 7);
        GRPort_CopyToPort(&port, o, &src, &r);
    }
    GRPort_Init_wh_1058_c064(&port);
    MSDoAllTasks_1058_bc15();
    ReportWin_TradeRoute1(o, y);
    set_on_lclick_1058_bdb7(&(EventTable &)*o, (void *)ReportWin_OnTradeRouteClick);
    MSDoAllTasks_1058_bc15();
}

