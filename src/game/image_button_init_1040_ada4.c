/* PORT: this file's own declarations of MSInitImageButton, dlg_control_init_1040_b207, get_ctable_1040_b1ea removed: the header declares them from the definition (W1 step 3). */
/* image_button_init_1040_ada4: candidate for 1040:ada4 (296 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: image_button_init_1040_ada4
 * name-source: descriptive
 *
 * Builds an image button into a dialog-control record: load its picture into a scratch
 * `GRPort`, size the control from what the picture turned out to be, register the control,
 * and hand the whole thing to `MSInitImageButton`.
 *
 * The ORDER is what makes it readable. The port is constructed first and the picture loaded
 * into it, and only THEN is the rectangle built -- from `port_width_1040_abae` and
 * `port_height_1040_abc7` on the port, plus the caller's origin and two pixels more in each
 * dimension, added at the right and bottom. So the button's size is the picture's size and not the caller's; the caller
 * gives a corner and a resource.
 *
 * `MSInitImageButton` cleans EIGHTEEN bytes here, where the Macintosh declares
 * `MSInitImageButton(short, short, void*, short, short, short, short)` at sixteen. The
 * Windows entry takes one short more.
 *
 * The two `long` zeroes at +0x1a and +0x1e are written as 32-bit stores, which is what says
 * they are `long` and not two words each.
 *
 * The `GRPort` is a stack object with a destructor, so the function carries an exception
 * frame and the destructor runs on both paths. One of FIVE copies, one per module; every
 * accessor it calls is this module's own.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, SImageButton removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
class DialogWin;
   /* SImageButton : DlgControl, flattened for C */

struct logpal;
extern int GRPort_LoadPicture(GRPort *port, int a, int b, int c, struct logpal *pic);

/* same object: 4-byte `push cs; call ` */
extern "C" int port_width_1040_abae(GRPort *port);
extern "C" int port_height_1040_abc7(GRPort *port);


void image_button_init_1040_ada4(SImageButton *button, DialogWin *win, int id, register int x, register int y,
                                  int res_down, int res_up, int pal_first, int pal_count)
{
    GRPort port;
    RECT r;

    button->reinit_faces = 0;
    button->ctable = get_ctable_1040_b1ea(win);
    GRPort_LoadPicture(&port, res_down, pal_first, pal_count, (struct logpal *)button->ctable);
    SetRect(&r, x, y, port_width_1040_abae(&port) + x + 2, port_height_1040_abc7(&port) + y + 2);
    GRPort_Init_wh_1040_b0c8(&port);
    dlg_control_init_1040_b207(button, win, 6, id, &r);
    button->on_click = 0L;
    button->on_rclick = 0L;
    button->hwnd = MSInitImageButton(x, y, button, res_down, res_up, pal_first, pal_count, 1);
}

