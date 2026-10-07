/* woodcut_show: candidate for 1060:003a (1077 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: woodcut_show
 *
 * THE FULL-SCREEN WOODCUT, for `1060:0508`. `index` picks both the caption -- line `index` of the
 * WOODCUT text resource -- and the picture (resource 0x18f + index).
 *
 * The caption is drawn in the game's letter sprites, not in a font: 26 of them (resources 500..525)
 * and the three pieces of the title bar (530..532). Its width is measured first, a space counting 14
 * and every letter its sprite's width plus 1, and the bar's middle piece is repeated until it covers
 * that width. The bar is centred on x = 320, then the caption is centred under it at y = 397, where a
 * space steps 20 and a letter its own width.
 *
 * The window is a 0x414-byte SuperWin (its constructor is module 1060's own copy, -called) with its own picture (0x1c2, 10 by 0x27), the picture port holds
 * the frame (0x519) and the woodcut itself, and everything uses the palette at SEG20:0x1ac. The
 * picture is blitted into the rectangle (131, 99)-(507, 364) and the dialog runs modally until closed.
 *
 * The caption is measured over STRLEN - 1 characters but drawn over all of them, so a caption ending
 * in a letter is centred slightly to the right of where it was measured.
 *
 * `Sprite_Draw`'s SPRITE parameter is a TYPED pointer and its port parameters are `void *`:
 * that is what puts `PUSH SS` after the offset computation for the letter and before it for the
 * window (rule 56). The two sprite arrays are built and destroyed by `VNEWVF`/`VDELF`.
 *
 * RENAMED 2026-09-27 from woodcut: String witness: WOODCUT is in Mac woodcut_show(short), and the Mac woodcut has no strings at all. Call graph: Mac woodcut_show's only caller is woodcut, and 003a's only caller is 0508. Mac woodcut_show builds a GRPort, Screen and Sprites, then does text_open/get/close, SuperWin::Init, Sprite::Draw x4, MSCenterWindow, SuperWin::Execute and GoAway. 003a calls GRPort_ctor, Sprite_Init x2, text_open/get/close, SuperWin_Init, Sprite_Draw x4, win_center, SuperWin_Execute and SuperWin_dtor. Arity: (short) against woodcut(int index). Size is 2794 Mac against 1077 Windows, the large one on both sides. Layout: Mac CODE 13 runs woodcut_show, woodcut_set, woodcut_get, woodcut, and Windows 1060 runs 003a, 046f set, 04c2 get, 0508, the same order. Why it went wrong: 003a was bound first as the only function that uses the WOODCUT TEXT identifier, on the assumption that the resource's user carries the family's base name. 0508 then took woodcut_show by elimination (see the woodcut_get.c header). This is a swap and needs a temporary name.
 *
 * THE DIALOG CLASS IS `SuperWin`, renamed from `EventDialog` 2026-09-28: its destructor call lands on
 * SuperWin::~SuperWin (1050:6b7d), and the image's RTTI names `SuperWin` (never `EventDialog`)
 * in this module's segment. Its constructor is the module's inline copy (EventDialog_ctor_*).
 * SuperWin::SuperWin is defined before the function, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, Sprite, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SuperWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a

#pragma option -a-
#pragma option -a

#pragma option -a-


class DlgControl;




extern void Sprite_Init(Sprite *s, int res);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern int text_open(void *ctx, char *name);
extern char *text_get(void);
extern void text_close(void);

extern void SuperWin_Init(SuperWin *w);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void DoFizzle(GRPort *port, GameWin *o, RECT *r, int x, int y, int mode, int step);
extern void SuperWin_Execute(SuperWin *w);
extern void SuperWin_GoAway(SuperWin *w);

extern "C" int sprite_width_1060_60c4(Sprite *sprite);
extern "C" void win_center_1060_60e7(void *w);

extern "C" void show_window_1060_612a(SuperWin *w);

int woodcut_show(int index)
{
    SuperWin dlg;
    GRPort port;
    Sprite letters[26];
    Sprite frame[3];
    RECT rc;
    char *line;
    int w_left;
    int w_mid;
    int w_right;
    int total;
    int count;
    int text_w;
    int x;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    char buf[0x50];
    register int k;
    register int i;

    text_w = 0;
    for (k = 0; k < 26; k++)
        Sprite_Init(&letters[k], k + 500);
    for (k = 0; k < 3; k++)
        Sprite_Init(&frame[k], k + 530);
    w_left = sprite_width_1060_60c4(&frame[0]);
    w_mid = sprite_width_1060_60c4(&frame[1]);
    w_right = sprite_width_1060_60c4(&frame[2]);
    text_open("WOODCUT", "WOODCUT");
    for (i = 0; i <= index; i++)
        line = text_get();
    text_close();
    STRCPY(buf, line);
    total = 0;
    count = 0;
    for (line = buf, i = 0; STRLEN(buf) - 1 > i; line++, i++) {
        if ((int)*line == ' ') {
            text_w += 14;
        } else {
            k = *line - 'A';
            text_w += sprite_width_1060_60c4(&letters[k]) + 1;
        }
    }
    while (total < text_w) {
        total += w_mid;
        count++;
    }
    GRPort_LoadPicture(&port, 0x519, 0xbb, 0x21, (struct logpal *)&g_game_palette);
    SuperWin_Init(&dlg);
    GRPort_LoadPicture(&dlg, 0x1c2, 10, 0x27, (struct logpal *)&g_game_palette);
    x = 0x140;
    x -= (total + w_left + w_right) >> 1;
    Sprite_Draw(&r1, &frame[0], &dlg, x, 0x186);
    x += w_left;
    for (i = 0; i < count; i++) {
        Sprite_Draw(&r2, &frame[1], &dlg, x, 0x186);
        x += w_mid;
    }
    Sprite_Draw(&r3, &frame[2], &dlg, x, 0x186);
    x = 0x140;
    x -= text_w >> 1;
    for (line = buf, i = 0; STRLEN(buf) > i; line++, i++) {
        if ((int)*line == ' ') {
            x += 20;
        } else {
            k = *line - 'A';
            Sprite_Draw(&r4, &letters[k], &dlg, x, 0x18d);
            x += sprite_width_1060_60c4(&letters[k]);
        }
    }
    GRPort_LoadPicture(&port, index + 0x18f, 0x31, 0x8a, (struct logpal *)&g_game_palette);
    SetRect(&rc, 0x83, 0x63, 0x1fb, 0x16c);
    win_center_1060_60e7((DialogWin *)&dlg);
    copy_all_to_screen_1060_610c(&dlg);
    show_window_1060_612a(&dlg);
    MSDoAllTasks_1060_6149();
    DoFizzle(&port, &dlg, &rc, 0x83, 0x63, -1, 0x10);
    SuperWin_Execute(&dlg);
    SuperWin_GoAway(&dlg);
}

