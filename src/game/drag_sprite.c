/* drag_sprite: candidate for 1050:6782 (471 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: drag_sprite
 * name-source: descriptive
 *
 * Carries a sprite under the mouse until the button comes up, and reports where it was let
 * go. The whole window is saved into a scratch GRPort first and put back at the end, so the
 * drag leaves nothing behind.
 *
 * The sprite is drawn four pixels left and sixteen pixels ABOVE the pointer, which is what
 * puts a 32-pixel icon under a cursor whose hot spot is its top-left.
 *
 * THE DIRTY RECTANGLE IS UNIONED WITH THE WHOLE WINDOW EVERY FRAME, so it is the whole window
 * every frame: `UnionRect(&cur, &cur, &full)` where `full` is (0, 0, width, height). The
 * machinery for tracking just the sprite's own rectangle is all here -- `cur` is where
 * `Sprite_Draw` reports what it drew, `saved` keeps the previous one, and both are pushed to
 * the screen separately -- and the union throws the saving away. In the shipped bytes.
 *
 * `cur` starts as (0, 0, 1, 1) rather than empty, but it is unioned with `full` before the first
 * restore, so that restore copies the whole window like every other.
 *
 * A position outside the window is IGNORED rather than clamped: the frame is skipped whole,
 * so the sprite stays where it last was and `mx`/`my` still carry the outside position into
 * the caller's out-parameters if the button comes up there.
 */

#include "colonization.h"

#include "w16types.h"
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
typedef struct { short style; void *ctable; void *win; short fa; char disabled; } Screen;
extern int Screen_LButtonPressed(Screen *w, char own, char remove, char wait);

typedef struct { char port[0x26]; char dialog[0x9a]; void *relayout; } GameWin;   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void copy_rect_to_screen_1050_8bfa(GameWin *w, RECT *r);

void drag_sprite(char *win, void *sprite, int x0, int y0,
                               int *out_x, int *out_y)
{
    GRPort port;
    RECT saved;
    RECT full;
    RECT cur;
    int mx;
    int my;
    register int w;
    register int h;

    w = port_width_1050_8b8a((GRPort *)win);
    h = port_height_1050_8ba9((GRPort *)win);
    GRPort_Init_wh(&port, w, h);
    SetRect(&full, 0, 0, w, h);
    GRPort_CopyToPort((GRPort *)win, &port, &full, &full);
    mx = x0;
    my = y0;
    SetRect(&cur, 0, 0, 1, 1);
    while (Screen_LButtonPressed((Screen *)(win + 0x26), 0, 1, 0) == 0) {
        get_mouse_pos_for_1050_8f2b(win + 0x26, &mx, &my);
        if (mx <= w && mx >= 0 && my <= h && my >= 0) {
            UnionRect(&cur, &cur, &full);
            GRPort_CopyToPort(&port, (GRPort *)win, &cur, &cur);
            CopyRect(&saved, &cur);
            Sprite_Draw(&cur, (Sprite *)sprite, (GRPort *)win, mx - 4, my - 0x10);
            copy_rect_to_screen_1050_8bfa((GameWin *)win, &saved);
            copy_rect_to_screen_1050_8bfa((GameWin *)win, &cur);
        }
    }
    GRPort_CopyToPort(&port, (GRPort *)win, &full, &full);
    *out_x = mx;
    *out_y = my;
}

