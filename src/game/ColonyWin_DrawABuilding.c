/* ColonyWin_DrawABuilding: candidate for 1010:9e22 (1090 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawABuilding
 * name-source: sibling-build
 *
 * Draws one building site of the colony window's buildings panel, for `ColonyWin_DrawBuildings`:
 * the building's picture, what it produces, who works in it, and a number for the two buildings that
 * count something. The layout comes from the per-kind tables in SEG21 the click handler
 * (`ColonyWin_ColonyMouseBuildings`) also reads.
 *
 * The picture is resource 0x15e + building + 1, loaded into a local `Sprite`, with three substitutions:
 * building 0 shows picture 0 when the colony lacks it, and sites 0xf and 0x11 show picture 0x12 when
 * the colony lacks building 0xf, or picture 0x11 when it has both 0xf and 0x11. The building
 * `ColonyWin_DoBuildingFizzle` is
 * animating (DGROUP:0xc20) is not drawn at all.
 *
 * For a building with a job (or 0x11, 0x13, 0x14): the production row from `output_of_building`
 * at the kind's output position, then either the workers' row (`ColonyWin_DrawIndividual`, called
 * from two identical arms of a test for top level 0xc) or, for a negative worker count on a top-level
 * building, the people outside (`ColonyWin_DrawFort`). Sites 0xf and 0x1e then print the
 * colony's bytes at +0x95 and +0x96 when above one, twice, one pixel apart, in colour 0xff.
 *
 * FRAME LAYOUT AND INITIALISATION ORDER ARE DIFFERENT THINGS (rule 72). The shipped stores `yoff = 0`
 * before constructing the Sprite, yet `yoff` sits below all the other scalars. Declared first, it
 * lands at the top of the scalars; declared last but after the Sprite, its store follows the
 * constructor. Every scalar declared, `yoff = 0` last of them, and THEN `Sprite spr;` gives both.
 * The building and kind are `register` parameters (rule 69).
 *
 * RENAMED 2026-09-26 from colony_draw_site: the Mac build's ColonyWin::DrawABuilding(short, short, short, short): this plus four shorts, painting one built site for ColonyWin_DrawBuildings.
 */

#include "colonization.h"

#include "building.h"
#include "w16types.h"
typedef struct { short hfont, height; } Font;
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
class ColonyWin : public ColChild { public: SRegion region; GRPort back; GRPort terrain; SImageButton ib[3]; SButton b[2]; SImageButton ib2; char buy_shown; char change_shown; short panel; short cx; short cy; int timer; int colony; ColonyWin(); ~ColonyWin(); };
#include "records.h"

extern void set_colony(int colony);
extern int has_building(int bit);
extern void Sprite_Init(Sprite *s, int res);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int job_of_building(int building);
extern int guys_at_building(int building);
extern int output_of_building(int building, int *kind_out, int *pic_out, int *extra_out);
extern void DrawIconRow(GRPort *w, int pic, int n, int m, int px, int y,
                                        int span, int span2, int marked, int style);
extern int base_building(int building);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColonyWin_DrawIndividual(ColonyWin *w, int job, int x, int y, int slot);
extern "C" void ColonyWin_DrawFort(ColonyWin *w, int x, int y, int count);

extern "C" void ColonyWin_DrawABuilding(ColonyWin *w, register int building, int x, int y, register int kind)
{
    int job;
    int workers;
    int out_kind;
    int amount;
    int pic;
    int marked;
    int count;
    int pic_id;
    int yoff = 0;
    Sprite spr;
    struct { int l, t, r, b; } out;
    char buf[0x50];

    set_colony(w->colony);
    g_draw_labels = g_show_icon_counts;
    pic_id = building + 1;
    if (building == 0 && has_building(BUILDING_STOCKADE) == 0)
        pic_id = 0;
    if (building == BUILDING_WAREHOUSE || building == BUILDING_STABLES) {
        if (has_building(BUILDING_WAREHOUSE) != 0) {
            if (has_building(BUILDING_STABLES) != 0)
                pic_id = 0x11;
        } else
            pic_id = 0x12;
    }
    Sprite_Init(&spr, pic_id + 0x15e);
    if (g_new_building == building)
        return;
    Sprite_Draw((RECT *)&out, &spr, w, w->rect.left + x, w->rect.top + y);
    job = job_of_building(building);
    if (building == BUILDING_WAREHOUSE && has_building(BUILDING_STABLES) != 0)
        building = BUILDING_STABLES;
    if (job >= 0 || building == BUILDING_PRINTING_PRESS || building == BUILDING_NEWSPAPER || building == BUILDING_STABLES) {
        workers = guys_at_building(building);
        amount = output_of_building(building, &out_kind, &pic, &marked);
        if (building == BUILDING_STABLES)
            yoff = 0x14;
        if (amount != 0)
            DrawIconRow(w, pic, amount, amount, g_kind_out_x[kind] + x + w->rect.left + 5,
                                    g_kind_out_y[kind] + y + yoff + w->rect.top,
                                    g_kind_out_w[kind] - 10, g_kind_out_w[kind] - 10, marked, 0);
        if (workers != 0) {
            if (workers > 0) {
                if (base_building(building) == BUILDING_SCHOOLHOUSE)
                    ColonyWin_DrawIndividual(w, job, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, kind);
                else
                    ColonyWin_DrawIndividual(w, job, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, kind);
            } else if (base_building(building) == 0)
                ColonyWin_DrawFort(w, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, 0 - workers);
        }
    }
    if (building == BUILDING_STABLES) {
        if (has_building(BUILDING_WAREHOUSE) != 0)
            building = BUILDING_WAREHOUSE;
        yoff = -11;
    } else
        yoff = 0;
    count = 0;
    if (building == BUILDING_WAREHOUSE)
        count = g_colonies[w->colony].warehouses;
    if (building == BUILDING_CAPITOL)
        count = g_colonies[w->colony].capitols;
    if (count > 1) {
        LTOA1(count, buf, 10);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_heading_font, buf, (g_kind_w[kind] >> 1) + (w->rect.left + x),
                             (g_kind_h[kind] >> 1) + (w->rect.top + y) + yoff, 0);
        GRPort_String(w, (unsigned *)&g_heading_font, buf, (g_kind_w[kind] >> 1) + (w->rect.left + x) + 1,
                             (g_kind_h[kind] >> 1) + (w->rect.top + y) + yoff, 0);
    }
    g_draw_labels = 0;
}
