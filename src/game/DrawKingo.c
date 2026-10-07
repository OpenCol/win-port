/* DrawKingo: candidate for 1050:5028 (156 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DrawKingo
 *
 * One frame of an animation: sprite resource 0x7d + n is loaded into a stack Sprite, drawn at
 * (10, 10), and the 0x50 by 0x92 rectangle from (10, 0x40) copied to the screen. The caller,
 * 1050:50c4, passes a post-incremented counter it tests against 5.
 *
 * RENAMED 2026-09-26 from draw_anim_frame: the Mac build's DrawKingo(GameWin&, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between Popup_draw and Popup_exec, which are already bound; the arity agrees and the Mac body is 1.53x this one.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char port[0x26]; char dialog[0x9a]; void *relayout; } GameWin;   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-

extern void Sprite_Init(Sprite *s, int res);
typedef struct { short vptr; int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; } GRPort;
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void copy_rect_to_screen_1050_8bfa(void *w, RECT *r);

void DrawKingo(GameWin *win, int n)
{
    Sprite s;
    RECT r;
    RECT out;

    Sprite_Init(&s, n + 0x7d);
    Sprite_Draw(&out, &s, (GRPort *)win, 10, 10);
    SetRect(&r, 10, 0x40, 0x5a, 0xd2);
    copy_rect_to_screen_1050_8bfa(win, &r);
}

