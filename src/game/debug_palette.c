/* debug_palette: candidate for 1050:65e0 (389 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: debug_palette
 *
 * The "Debug Info" window: a 0x200 by 0x100 window over the palette at SEG20:0x1ac, filled with
 * the colour at DGROUP:0x337f, whose click handler is `dispatch_click` and whose quit
 * handler is `set_quit_flag`. Its left 256 by 256 pixels are painted pixel by pixel
 * with the palette laid out as a 16 by 16 grid -- entry `(y / 16 << 4) + x / 16` -- and copied to
 * the screen twice before the dialog at +0x26 is closed.
 *
 * The GRPort and both Sprites are locals that nothing uses: they are constructed and destroyed,
 * and cost the function its exception block.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, GRPort, Sprite, GameWin, EventTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */

#pragma option -a

#pragma option -a-

   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
extern void build_window_with_port(GameWin *o, char *name, int style, int x, int y,
                                       int w, int h, void *pal);
extern void GRPort_FillColor_all(GRPort *p, int color);

extern void MSLockGraf(GRPort *port);

extern void dispatch_click(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void *set_on_lbutton_down_1050_8eb9(EventTable *table, void *h);
extern "C" void *set_on_close_1050_8ef2(EventTable *table, void *h);
extern "C" void copy_rect_to_screen_1050_8bfa(GameWin *w, RECT *r);
extern "C" void show_window_1050_8bc8(GameWin *o);

void debug_palette(char *w)
{
    GRPort port;
    Sprite s1;
    Sprite s2;
    RECT r;
    register int y;
    register int x;

    build_window_with_port((GameWin *)w, "Debug Info", 0x69, 0, 0, 0x200, 0x100, &g_game_palette);
    set_on_lbutton_down_1050_8eb9((EventTable *)(w + 0x33), (void *)dispatch_click);
    GRPort_FillColor_all((GRPort *)w, g_text_colour);
    set_on_close_1050_8ef2((EventTable *)(w + 0x33), (void *)set_quit_flag);
    SetRect(&r, 0, 0, 0x100, 0x100);
    MSLockGraf((GRPort *)w);
    for (x = 0; x < 0x100; x++)
        for (y = 0; y < 0x100; y++)
            SetPixel(w, x, y, (y / 0x10 << 4) + x / 0x10);
    copy_rect_to_screen_1050_8bfa((GameWin *)w, &r);
    copy_rect_to_screen_1050_8bfa((GameWin *)w, &r);
    show_window_1050_8bc8((GameWin *)w);
}

