/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 61, 62 of the imported file. */
/* ColChild_DragIcon: candidate for 1048:14ca (645 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DragIcon
 *
 * DRAGS AN ICON around a colony window and answers where it was dropped, relative to the window's
 * rectangle. Nine callers -- everything that can be picked up in the colony and Europe screens.
 *
 * It clips the port, saves the scale and works at 1:1, and installs its own mouse handlers on the
 * object's slots at +0x33: `ColChild_DragIconMove` on slot 0, `ColChild_DragIconMUp` on slot 8, and
 * nothing on slot 12 -- keeping the three it replaced.
 *
 * The sprite goes into DGROUP:0x4cb8 with its hot spot saved and zeroed, a GRPort is allocated with
 * `new` into DGROUP:0x4cbc and sized to the sprite, and the two rectangles at SEG30:0x0 and 0x8 are set
 * to the sprite's size and to 1x1 -- the source and step of the drag image.
 *
 * Then it PUMPS THE MESSAGE QUEUE until the handler sets +0x760, restores everything it changed, and
 * reports the drop point from DGROUP:0x4cc0/0x4cc2 less the window's origin. The port is deleted last.
 *
 * The locals' declaration order is what the frame says: the scale pair, then the hot spot, then the
 * three saved handlers in slot order 0, 8, 12 -- which is not the order they are installed in.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class DlgControl;

extern GRPort *g_drag_port;             /* DGROUP:0x4cbc */

extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
extern void Sprite_GetHotSpot(void *s, int *x, int *y);

extern void Sprite_SetHotSpot(Sprite *s, int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *set_on_lbutton_up_1048_d9fc(EventTable *table, void *fn);
extern "C" void *set_on_mousemove_1048_da35(EventTable *table, void *fn);
extern "C" void *set_on_lclick_1048_da6c(EventTable *table, void *fn);
extern "C" void port_set_capture(Screen *p);

extern "C" void port_release_capture(Screen *p);

void ColChild_DragIcon(ColChild *o, Sprite *sprite, int unused1, int unused2,
                           int *out_x, int *out_y)
{
    int scale_a;
    int scale_b;
    int hot_x;
    int hot_y;
    void *old0;
    void *old8;
    void *old12;

    GRPort_SetClip(o, &o->rect);
    GetGlobalSpriteScale(&scale_a, &scale_b);
    SetGlobalSpriteScale(1, 1);
    old8 = set_on_lbutton_up_1048_d9fc(&(EventTable &)*o, (void*)(ColChild_DragIconMUp));
    old0 = set_on_mousemove_1048_da35(&(EventTable &)*o, (void*)(ColChild_DragIconMove));
    old12 = set_on_lclick_1048_da6c(&(EventTable &)*o, 0);
    g_drag_sprite = sprite;
    Sprite_GetHotSpot(g_drag_sprite, &hot_x, &hot_y);
    Sprite_SetHotSpot(g_drag_sprite, 0, 0);
    port_set_capture(&(Screen &)*o);
    o->dropped = 0;
    g_drag_port = new GRPort;
    GRPort_Init_wh(g_drag_port, rect_width_1048_daca(&sprite->bounds), rect_height_1048_dae9(&sprite->bounds));
    SetRect(&g_drag_rect_src, 0, 0, rect_width_1048_daca(&sprite->bounds),
            rect_height_1048_dae9(&sprite->bounds));
    SetRect((RECT *)g_drag_rect_dst, 0, 0, 1, 1);
    while (o->dropped == 0)
        MSDoTask_1048_db08();
    port_release_capture(&(Screen &)*o);
    set_on_lbutton_up_1048_d9fc(&(EventTable &)*o, old8);
    set_on_lclick_1048_da6c(&(EventTable &)*o, old12);
    set_on_mousemove_1048_da35(&(EventTable &)*o, old0);
    GRPort_ResetClip(o);
    Sprite_SetHotSpot(g_drag_sprite, hot_x, hot_y);
    *out_x = g_drag_x - o->rect.left;
    *out_y = g_drag_y - o->rect.top;
    SetGlobalSpriteScale(scale_a, scale_b);
    delete g_drag_port;
}

