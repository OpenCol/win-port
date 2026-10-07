/* MapWin_RedrawMap: candidate for 1038:a76e (219 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_RedrawMap
 * name-source: sibling-build
 *
 * Rebuilds the map after a zoom change: the current window's owner (the map window) has its
 * off-screen port (data segment 28) resized to the window rectangle and its palette
 * re-selected, the global sprite scale set to (zoom + 8, 8) (SetGlobalSpriteScale), the map redrawn (DrawMap), the
 * mini window at SEG20:0xde0 redrawn (GameWin_Redraw) and the sprite scale reset to (1, 1). The module's rectangle
 * helpers are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 *
 * RENAMED 2026-09-27 from map_zoom_redraw: the Mac build's MapWin_RedrawMap, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: current_window's MapWin, then GRPort_ColorSync, SetGlobalSpriteScale, DrawMap(1), GameWin_Redraw, SetGlobalSpriteScale(1,1): the Mac RedrawMap sequence (Windows adds a ResizePort); a handler with no this; ratio 2.29.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x41e bytes: the palette, its handle and the cycling state */

extern char g_mini_win;  /* SEG20:0xde0 */
extern void *GetEventWindow(void);

extern int GRPort_ResizePort(GRPort *port, RECT *r);
extern void GRPort_ColorSync(GRPort *port, CTable *pal);
extern void SetGlobalSpriteScale(int num, int den);
extern void DrawMap(int flag);
extern void GameWin_Redraw(GameWin *rec);

void MapWin_RedrawMap(void)
{
    MapWin *o;
    RECT rc;
    int zoom;
    int eight;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (MapWin *)w;
    zoom = o->zoom + 8;
    eight = 8;
    SetRect(&rc, 0, 0, rect_width_1038_dfbd(&o->rect), rect_height_1038_dfdc(&o->rect));
    GRPort_ResizePort(&g_map_port, &rc);
    GRPort_ColorSync(&g_map_port, &g_game_palette);
    SetGlobalSpriteScale(zoom, eight);
    DrawMap(1);
    GameWin_Redraw((GameWin *)&g_mini_win);
    SetGlobalSpriteScale(1, 1);
}
