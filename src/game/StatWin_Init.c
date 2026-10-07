/* PORT: this file's own declarations of ColChild_IsSmallDesktop removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of set_repaint_handler_1058_ba08, win_set_icon_1078_2832 removed: the header declares them from the definition (W1 step 3). */
/* StatWin_Init: candidate for 1058:8a34 (307 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: StatWin_Init
 * name-source: sibling-build
 *
 * Builds the info window (a ColChild at +0 with its window at +0x26 and a picture port at
 * +0x77a): flag +0x98 cleared, picture resource 200 (16x24 cells) loaded with the game
 * palette, the client rectangle 135x260 on a 640-wide desktop or 200x300 otherwise, the
 * child window "Info Window" created (style 5, at 100/250), the pointer and keyboard
 * handlers installed, icon 0x81, the picture copied into the port, and the repaint
 * handler stored at +0x762. The module's one-line wrappers are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from info_window_init: the Mac build's StatWin::Init(void). The "Info Window" is StatWin: MainWin's child at +0xf9c (MainWin_ctor), 0x7a0 bytes with its GRPort at +0x77a (StatWin_ctor) -- the picture port this function loads -- and this is the method that creates it and installs its handlers.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, StatWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */


#pragma option -a

#pragma option -a-









struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);

extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void StatWin_LeftMouseClick(int x, int y);
extern void KeyEvents(int key);
extern void UpdateStats(void);

extern "C" void set_keyboard_nav_1058_b992(DialogWin *w, char keyboard_nav);

extern "C" void win_set_icon_1058_b9ab(DialogWin *w, int icon);


extern "C" void *set_on_lbutton_down_1058_c2ce(EventTable *table, void *h);

extern "C" void *set_on_keydown_1058_c307(EventTable *table, void *h);

void StatWin_Init(StatWin *o)
{
    RECT rc;

    set_keyboard_nav_1058_b992(&(DialogWin &)*o, 0);
    GRPort_LoadPicture(&o->picture, 200, 16, 24, (struct logpal *)&g_game_palette);
    if (ColChild_IsSmallDesktop(o))
        SetRect(&rc, 0, 0, 135, 260);
    else
        SetRect(&rc, 0, 0, 200, 300);
    ColChild_InitCWin((ColChild *)o, "Info Window", 5, 100, 250, rect_width_1058_bd2d(&rc), rect_height_1058_bd4c(&rc), 1);
    set_on_lbutton_down_1058_c2ce(&(EventTable &)*o, (void *)StatWin_LeftMouseClick);
    set_on_keydown_1058_c307(&(EventTable &)*o, (void *)KeyEvents);
    win_set_icon_1058_b9ab(&(DialogWin &)*o, 0x81);
    GRPort_CopyToPort(&o->picture, (GRPort *)o, &rc, &o->rect);
    set_repaint_handler_1058_ba08(o, (void *)UpdateStats);
}
