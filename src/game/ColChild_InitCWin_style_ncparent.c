/* PORT: this file's own declarations of ColChild_OnSize, GamePrefs_GetWindow, GamePrefs_RegisterWindow, GamePrefs_WindowArea, GameWin_Init, image_button_init_1048_d532, set_control_action_1048_d65a, set_on_move, set_on_size, set_relayout_handler_1048_d738, win_set_drag_height_1048_d714 removed: the header declares them from the definition (W1 step 3). */
/* ColChild_InitCWin_style_ncparent: candidate for 1048:01ff (801 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_InitCWin_style_ncparent
 * name-source: sibling-build
 *
 * Creates one of the game's framed windows and restores where it was. The client area is
 * w by h; the window is built 0x14 wider and 0x2a taller for the frame and title bar, over the game
 * palette, with style bits `0x601` added to the caller's -- or `0xc01` for a window without the
 * close box, which is then parented to `parent` when one is given. Either way it starts hidden.
 *
 * A window with the close box gets its image button (picture 0xcd) 0x20 in from the
 * right of the title bar, wired to `ColChild_MinButton`.
 *
 * With `save` set the placement is remembered under `which` in the preferences at SEG20:0x10: a
 * saved rectangle moves the window there, and without one the window's current rectangle is
 * recorded. Then the title is copied to +0xc4, the client rectangle set at (0xa, 0x20), and the
 * three placement handlers installed -- `ColChild_DrawAll` as the geometry callback,
 * `ColChild_OnMove` and `ColChild_OnSize` in slots 0x68 and 0x64.
 *
 * Several stores go nowhere. The rectangle built for the close box is passed to nothing; the
 * saved rectangle's corner and the caller's position and size are copied into four locals that
 * are never read; and the saved and current rectangles are compared, width and height, by an `if`
 * with an empty body.
 *
 * RENAMED 2026-09-27 from game_window_init: cleans 28 = InitCWin(char*, short x6, char, char, EventWin*); body matches the Mac one (GamePrefs GetWindow/WindowArea/RegisterWindow, image button)
 *
 * RENAMED 2026-09-27 from ColChild_InitCWin_style_nc_parent: Borland C++ keeps 32 significant characters of an identifier, and ColChild_InitCWin_style_nc_parent is 33: the compiled PUBDEF came out truncated and no longer matched its own name. The Mac name is otherwise kept.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, GamePrefs, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







#include "records.h"


   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;             /* SEG20:0x5ca */


extern void ColChild_MinButton(void);
extern void ColChild_DrawAll(void);
extern void ColChild_OnMove(void);

/* 5-byte NOP form, so extern (rule 47) */
extern void ColChild_ExtractSprite(ColChild *o);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSHideWindow_of_1048_d4d5(Screen *o);

extern "C" void MSGetSizeParent_of(Screen *o, RECT *r);

extern "C" void win_move_to_rect(Screen *o, RECT *r);


void ColChild_InitCWin_style_ncparent(ColChild *o, char *title, int which, int x, int y,
                                    register int w, register int h, int style, char save,
                                    char noclose, void *parent)
{
    RECT r;
    RECT cur;
    int px;
    int py;
    int pw;
    int ph;

    o->which = which;
    o->save_flag = save;
    o->noclose = noclose;
    o->msg_timer = -1;
    o->queued = 0;
    if (noclose == 0)
        GameWin_Init((GameWin *)o, title, 0x601 | style, x, y, w + 0x14, h + 0x2a,
                                         &g_game_palette, (Screen *)&g_main_win);
    else if (parent == 0)
        GameWin_Init((GameWin *)o, title, 0xc01 | style, x, y, w + 0x14, h + 0x2a,
                                         &g_game_palette, (Screen *)&g_main_win);
    else
        GameWin_Init((GameWin *)o, title, 0xc01 | style, x, y, w + 0x14, h + 0x2a,
                                         &g_game_palette, (Screen *)parent);
    MSHideWindow_of_1048_d4d5(&(DialogWin &)*o);
    if (noclose == 0) {
        SetRect(&r, port_width_1048_d4f6(o) - 0x1c, 0, port_width_1048_d4f6(o) - 0xe, 0x1b);
        image_button_init_1048_d532(&o->btn, &(DialogWin &)*o, 5,
                                    port_width_1048_d4f6(o) - 0x20, 0, 0xcd, 0xcd, 0x10, 0x18);
        set_control_action_1048_d65a(&o->btn, (void *)ColChild_MinButton);
    }
    if (save != 0) {
        if (GamePrefs_GetWindow(&g_prefs_root, which) != 0) {
            GamePrefs_WindowArea(&g_prefs_root, which, &r);
            px = r.left;
            py = r.top;
            MSGetSizeParent_of(&(DialogWin &)*o, &cur);
            if (rect_width_1048_d69b(&cur) == rect_width_1048_d69b(&r)
                && rect_height_1048_d6ba(&cur) != rect_height_1048_d6ba(&r)) {
            }
            win_move_to_rect(&(DialogWin &)*o, &r);
        } else {
            px = x;
            py = y;
            ph = h;
            pw = w;
            MSGetSizeParent_of(&(DialogWin &)*o, &r);
            GamePrefs_RegisterWindow(&g_prefs_root, which, &r, 0);
        }
    }
    MSDoAllTasks_1048_d701();
    STRCPY(o->title, title);
    win_set_drag_height_1048_d714(&(DialogWin &)*o, 0x20);
    SetRect(&o->rect, 0xa, 0x20, w + 0xa, h + 0x20);
    ColChild_ExtractSprite(o);
    set_relayout_handler_1048_d738(o, (void *)ColChild_DrawAll);
    set_on_move(&(EventTable &)*o, (void *)ColChild_OnMove);
    set_on_size(&(EventTable &)*o, (void *)ColChild_OnSize);
}
