/* ColonyWin_Init: candidate for 1010:a7c7 (1025 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_Init
 * name-source: descriptive
 *
 * BUILDS THE COLONY WINDOW: `ColonyWin::Init`, the counterpart of `EuropeWin_Init`.
 *
 *   - the "Colony Window" (style 6, 640x440), placed by `ColChild_Center`, with no
 *     colony yet (+0xa12 = -1). It is remembered at DGROUP:0xa00;
 *   - the mouse handlers (left click, move, right click, slot 28) unless DGROUP:0xa08 is set (the
 *     window then only takes keys), and the key and close handlers always;
 *   - the port filled with the background colour, and a region (id 0x80) in the bottom
 *     right corner with `ColonyWin_OnOKClick`, the screen's exit, as its handler;
 *   - three image buttons down the right edge (ids 0x64..0x66, pictures 0xcf..0xd1, 37 pixels apart),
 *     each routed to `ColonyWin_MiscButtonClick`;
 *   - "Buy" (id 0xc8) and "Change" (id 0xc9) buttons in the font at DGROUP:0x4b16, routed to the build
 *     menu's handlers, after which the font at 0x4b1a is restored. The window's +0xa08 and +0xa09
 *     flags are set;
 *   - the close image button (id 0x65, picture 0xce) routed to `ColonyWin_OnOKClick`, the exit.
 * The same 0xa08 switch that withholds the mouse handlers also withholds the button routings.
 *
 * It compiled RELOCATABLE on the first build.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










extern ColonyWin *g_colony_window_ptr;       /* DGROUP:0xa00, in this module */

extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y, int w, int h, int style, char save, char noclose);
extern void ColChild_Center(ColChild *w);
extern void GRPort_FillColor_all(GRPort *p, int colour);

extern void ColonyWin_DblMouseClick(void);

extern void ColonyWin_OnOKClick(int unused1);
extern void ColonyHammerBuy(void);
extern void ColonyHammerChange(void);
extern void ColonyWin_MiscButtonClick(void);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_on_mousemove_1010_e7a1(void *o, void *fn);
extern "C" void set_on_lbutton_down_1010_e7d8(void *o, void *fn);
extern "C" void set_on_rbutton_down_1010_e811(void *o, void *fn);
extern "C" void set_on_ldblclk(void *o, void *fn);
extern "C" void set_on_keydown_1010_e36f(void *o, void *fn);
extern "C" void set_on_close_1010_e883(void *o, void *fn);
extern "C" void sregion_init_1010_e8bc(void *b, void *win, int id, RECT *r);
extern "C" void set_region_click_1010_e93f(void *region, void *fn);
extern "C" void image_button_init_1010_e976(void *b, void *win, int id, int x, int y, int res, int res2, int w, int h);

extern "C" void sbutton_init_f20(void *button, void *win, int id, RECT *r, int w20, char *title);
extern "C" void set_button_action_1010_eaf5(void *b, void *fn);
extern "C" void set_control_action_1010_eb0f(void *b, void *fn);
extern "C" void set_keyboard_nav_1010_eb29(void *win, int keyboard_nav);

void ColonyWin_Init(ColonyWin *w)
{
    RECT r;

    w->colony = -1;
    ColChild_InitCWin_style_nc(w, "Colony Window", 6, 0, 0, 0x280, 0x1b8, 0, 0, 1);
    g_colony_window_ptr = w;
    ColChild_Center((ColChild *)w);
    if (g_scout_visit == 0) {
        set_on_mousemove_1010_e7a1(&(EventTable &)*w, ColonyWin_MouseMove);
        set_on_lbutton_down_1010_e7d8(&(EventTable &)*w, ColonyWin_LeftMouseClick);
        set_on_rbutton_down_1010_e811(&(EventTable &)*w, ColonyWin_RightMouseClick);
        set_on_ldblclk(&(EventTable &)*w, ColonyWin_DblMouseClick);
    }
    set_on_keydown_1010_e36f(&(EventTable &)*w, ColonyWin_KeyClick);
    set_on_close_1010_e883(&(EventTable &)*w, ColonyWin_Close);
    GRPort_FillColor_all((GRPort *)w, g_draw_color);
    SetRect(&r, w->rect.left + 0x262, w->rect.top + 0x186, w->rect.left + 0x280, w->rect.top + 0x1b8);
    sregion_init_1010_e8bc(&w->region, &(DialogWin &)*w, 0x80, &r);
    set_region_click_1010_e93f(&w->region, ColonyWin_OnOKClick);
    image_button_init_1010_e976(&w->ib[0], &(DialogWin &)*w, 0x64, w->rect.left + 0x25d, w->rect.top + 0x113, 0xcf, 0xcf, 0x28, 0x60);
    image_button_init_1010_e976(&w->ib[1], &(DialogWin &)*w, 0x65, w->rect.left + 0x25d, w->rect.top + 0x138, 0xd0, 0xd0, 0x28, 0x60);
    image_button_init_1010_e976(&w->ib[2], &(DialogWin &)*w, 0x66, w->rect.left + 0x25d, w->rect.top + 0x15d, 0xd1, 0xd1, 0x28, 0x60);
    set_button_font_1010_ea9e(&g_info_font_large);
    SetRect(&r, w->rect.left + 0x1ae, w->rect.top + 0x122, w->rect.left + 0x1fe, w->rect.top + 0x136);
    sbutton_init_f20(&w->b[0], &(DialogWin &)*w, 0xc8, &r, 0x7c, "Buy");
    SetRect(&r, w->rect.left + 0x208, w->rect.top + 0x122, w->rect.left + 0x24e, w->rect.top + 0x136);
    sbutton_init_f20(&w->b[1], &(DialogWin &)*w, 0xc9, &r, 0x7c, "Change");
    set_button_font_1010_ea9e(&g_number_font);
    w->buy_shown = 1;
    w->change_shown = 1;
    if (g_scout_visit == 0) {
        set_button_action_1010_eaf5(&w->b[0], ColonyHammerBuy);
        set_button_action_1010_eaf5(&w->b[1], ColonyHammerChange);
        set_control_action_1010_eb0f(&w->ib[0], ColonyWin_MiscButtonClick);
        set_control_action_1010_eb0f(&w->ib[1], ColonyWin_MiscButtonClick);
        set_control_action_1010_eb0f(&w->ib[2], ColonyWin_MiscButtonClick);
    }
    set_keyboard_nav_1010_eb29(&(DialogWin &)*w, 0);
    image_button_init_1010_e976(&w->ib2, &(DialogWin &)*w, 0x65, 0, 0, 0xce, 0xce, 0x28, 0x60);
    set_control_action_1010_eb0f(&w->ib2, ColonyWin_OnOKClick);
}
