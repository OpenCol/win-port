/* PORT: this file's own declarations of GameWin_Init removed: the header declares it from the definition (W1 step 3). */
/* SuperWin_Init: candidate for 1050:6a16 (359 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets a SuperWin up: a desktop-sized EventWin at +0xc4 built first with a black background,
 * then the SuperWin's own 640x480 port built OVER it as a child, then the same three handlers
 * installed on BOTH handler blocks -- the SuperWin's at +0x33 and the EventWin's at +0xd1 --
 * so a click anywhere, on the frame or on the picture, reaches the same three routines.
 *
 * IT IS NOT THE CONSTRUCTOR, although `SuperWin_dtor` is the destructor of the same class. A
 * C++ constructor of a class with a virtual function opens with `if (this) this->vptr = ...`,
 * and this function opens with a call: no null guard, no vtable store. So the object is
 * constructed elsewhere and this is a separate initialiser that takes the object as an
 * ordinary pointer.
 *
 * The palette buffer at +0x150 is 0xec entries of three bytes -- 0x2c4 -- packed out of the
 * game palette before either window exists -- but both windows are created with the game palette
 * itself (SEG20:0x1ac), not with this copy.
 *
 * Two empty-string window names, at DGROUP:0x25fc and 0x25fd: two `""` literals the compiler
 * did not pool, the same pair of neighbours `popup_ask` and its twin use.
 *
 * RENAMED 2026-09-26 from super_win_init: the Mac build's SuperWin::Init(void): this only, and it builds the window and installs the three dismiss handlers.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void MSHideMenuBar(void);
struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);

extern void EventWin_Init(char *w, char *name, int style,
                                       int x, int y, int cx, int cy, CTable *pal);
extern void SuperWin_LeftMouseDown(void);
extern void SuperWin_RightMouseDown(void);
extern void SuperWin_KeyDown(void);

/* same object: 4-byte `push cs; call ` */
/* PORT: the in-file stubs of set_on_lclick_1050_8f80, set_on_rclick_1050_8fb9, set_on_keydown_after_1050_8544 removed: they shaped the call for the byte match, and
   src/ defines each function (W1). */

extern void win_set_back_color_1050_8f5c(DialogWin *w, int index);
extern void MSShowWindow_of_1050_8ff2(DialogWin *w);

void SuperWin_Init(SuperWin *o)
{
    RECT r;

    MSHideMenuBar();
    g_modal_done = 0;
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), o->palette, 10, 0xec);
    MSGetDesktopSize(&r);
    EventWin_Init((char *)&o->win, "", 0x801, 0, 0, rect_width_1050_830c(&r),
                           rect_height_1050_832b(&r), &g_game_palette);
    win_set_back_color_1050_8f5c((DialogWin *)&o->win, 0);
    GameWin_Init((GameWin *)o, "", 0x801, 0, 0, 0x280, 0x1e0,
                                     &g_game_palette, (Screen *)&o->win);
    set_on_lclick_1050_8f80((EventTable *)((char *)o + 0x33), SuperWin_LeftMouseDown);
    set_on_rclick_1050_8fb9((EventTable *)((char *)o + 0x33), SuperWin_RightMouseDown);
    set_on_keydown_after_1050_8544((EventTable *)((char *)o + 0x33), SuperWin_KeyDown);
    set_on_lclick_1050_8f80((EventTable *)((char *)o + 0xd1), SuperWin_LeftMouseDown);
    set_on_rclick_1050_8fb9((EventTable *)((char *)o + 0xd1), SuperWin_RightMouseDown);
    set_on_keydown_after_1050_8544((EventTable *)((char *)o + 0xd1), SuperWin_KeyDown);
    MSShowWindow_of_1050_8ff2((DialogWin *)&o->win);
    MSDoAllTasks_1050_8be7();
}
