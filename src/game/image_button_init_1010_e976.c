/* image_button_init_1010_e976: candidate for 1010:e976 (296 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: image_button_init_1010_e976
 * name-source: descriptive
 *
 * Builds an image button into a dialog-control record: load its picture into a scratch
 * `GRPort`, size the control from what the picture turned out to be, register the control,
 * and hand the whole thing to `MSInitImageButton`.
 *
 * The ORDER is what makes it readable. The port is constructed first and the picture loaded
 * into it, and only THEN is the rectangle built -- from `port_width_1010_ebd4` and
 * `port_height_1010_ebed` on the port, plus the caller's origin and two pixels added to the
 * width and height (right and bottom only). So the button's size is the picture's size and not the caller's; the caller
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
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class DialogWin;
typedef struct { short kind; short id; void *win; RECT rect; short hwnd; struct DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; long on_click; long on_rclick; char up[0x26]; char down[0x26]; void *ctable; char reinit_faces; char pad_73; } SImageButton;   /* SImageButton : DlgControl, flattened for C */

struct logpal;
extern int GRPort_LoadPicture(GRPort *port, int a, int b, int c, struct logpal *pic);
extern unsigned MSInitImageButton(int x, int y, SImageButton *c, int a, int b, int d, int e, int f);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_ctable_1010_ee6f(void *p);
extern "C" int port_width_1010_ebd4(GRPort *port);
extern "C" int port_height_1010_ebed(GRPort *port);

extern "C" void dlg_control_init_1010_edf9(SImageButton *ctl, void *win, int kind, int id, RECT *r);

void image_button_init_1010_e976(SImageButton *button, DialogWin *win, int id, register int x, register int y,
                                  int res_down, int res_up, int pal_first, int pal_count)
{
    GRPort port;
    RECT r;

    button->reinit_faces = 0;
    button->ctable = get_ctable_1010_ee6f(win);
    GRPort_LoadPicture(&port, res_down, pal_first, pal_count, (struct logpal *)button->ctable);
    SetRect(&r, x, y, port_width_1010_ebd4(&port) + x + 2, port_height_1010_ebed(&port) + y + 2);
    GRPort_Init_wh_1010_ee8c(&port);
    dlg_control_init_1010_edf9(button, win, 6, id, &r);
    button->on_click = 0L;
    button->on_rclick = 0L;
    button->hwnd = MSInitImageButton(x, y, button, res_down, res_up, pal_first, pal_count, 1);
}

