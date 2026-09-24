/* track_nation_choice -- shipped at 1030:063b.
 *
 * Ported by tools/port.py from matched/game/track_nation_choice.c.
 *
 * The file's own Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 panel_at(Screen *s, col_i16 x, col_i16 y);
void SetNations_DrawIt(Screen *s);
void copy_rect_to_screen_1030_e699(Screen *s, RECT *r);
void get_mouse_pos_for(void *w, col_i16 *px, col_i16 *py);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
#endif

extern void FillColor(Screen *s, RECT *r, col_i16 colour);
extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern col_i16 msg_box_1060_a722(void *w, col_i16 a, col_i16 b, col_i16 c);

/* same object: 4-byte `push cs; call ` */





void track_nation_choice(Screen *s, register col_i16 n)
{
    col_i16 x;
    col_i16 y;
    RECT inner;
    RECT r;
    RECT outer;
    col_i16 prev;

    if (n == 0)
        goto out;
    switch (n) {
    case 1:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[0]);
        break;
    case 2:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[1]);
        break;
    case 3:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[2]);
        break;
    case 4:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[3]);
        break;
    }
    prev = 0;
    while (msg_box_1060_a722((char *)s + 0x26, 0, 1, 0) == 0) {
        get_mouse_pos_for((char *)s + 0x26, &x, &y);
        (*((col_i16 *)((unsigned char *)(s) + 0xe4))) = panel_at(s, x, y);
        if ((*((col_i16 *)((unsigned char *)(s) + 0xe4))) != prev && (*((col_i16 *)((unsigned char *)(s) + 0xe4))) == n) {
            CopyRect(&inner, &r);
            CopyRect(&outer, &r);
            inner.left += 3;
            inner.top += 3;
            inner.bottom++;
            inner.right++;
            outer.right -= 3;
            outer.bottom -= 3;
            FillColor(s, &r, 0);
            CopyToPort((char *)s + 0x12c, s, &outer, &inner);
            copy_rect_to_screen_1030_e699(s, &r);
        } else if ((*((col_i16 *)((unsigned char *)(s) + 0xe4))) != prev && (*((col_i16 *)((unsigned char *)(s) + 0xe4))) != n) {
            CopyToPort((char *)s + 0x12c, s, &r, &r);
            copy_rect_to_screen_1030_e699(s, &r);
        }
        prev = (*((col_i16 *)((unsigned char *)(s) + 0xe4)));
    }
    if (prev == n)
        (*((col_i16 *)((unsigned char *)(s) + 0xe6))) = n;
    SetNations_DrawIt(s);
out:
    ;
}
