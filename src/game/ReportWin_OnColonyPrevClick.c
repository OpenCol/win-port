/* ReportWin_OnColonyPrevClick: candidate for 1058:4d02 (361 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnColonyPrevClick
 *
 * The "previous" button of the colony report: the index steps back (not below 0) and the
 * report is redrawn -- the colony report or the Sons of Liberty page by the view flag. The arrow picture (resource 0x131, 142x64, game palette) is drawn into the
 * report's client rectangle through a stack GRPort first.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SButton; class ReportWin : public ColChild { public: short kind; SImageButton image; short nation; short page; short view; short score; unsigned char modal; char pad_7f9; void *rec; int cell_x[29]; int cell_y[29]; int counts[29]; SButton *prev; SButton *next; SButton *extra_btn; ReportWin(); ~ReportWin(); };

extern void *GetEventWindow(void);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1058_bcfb(GRPort *port);
extern "C" int port_height_1058_bd14(GRPort *port);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void ReportWin_ReportsColony(ReportWin *o, int item);
extern void ReportWin_ColonyLibertyReport(ReportWin *o, int item);

extern "C" void ReportWin_OnColonyPrevClick(void)
{
    GRPort port;
    ReportWin *o;
    RECT r;
    char *w = (char *)GetEventWindow();

    o = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (o == 0)
        return;
    GRPort_LoadPicture(&port, 0x131, 0x8e, 0x40, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
    GRPort_CopyToPort(&port, o, &r, &o->rect);
    GRPort_Init_wh_1058_c064(&port);
    o->page--;
    if (o->page < 0)
        o->page = 0;
    if (o->view == 0)
        ReportWin_ReportsColony(o, o->nation);
    else
        ReportWin_ColonyLibertyReport(o, o->nation);
    copy_all_to_screen_1058_c1bf(o);
}
