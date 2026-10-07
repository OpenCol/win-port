/* CDControlWin_DrawTime: candidate for 1040:9eab (335 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The CD panel's TIME box, next to the track-number box `CDControlWin_DrawTrack` paints: the
 * 0x62 x 0x30 rectangle at (0x64, 0xc), filled with the same colour byte at DGROUP:0x337e and
 * written in colour 12 with the same small font.
 *
 * `MSGetCDInfo` fills two ints and the string is assembled by hand -- minutes, a colon, a
 * leading zero when the seconds are under ten, then the seconds. No CD, or no answer, and it
 * prints "00:00" instead.
 *
 * `buf[0] = 0` between the two `LTOA1` calls does nothing: `LTOA1` writes its own terminator
 * and the buffer is copied out before it is reused. It is in the shipped code.
 *
 * The rectangle is built in WINDOW coordinates and then offset by the window's origin at
 * +0x76a/+0x76c, and the same rectangle is what gets blitted at the end -- so the fill, the
 * text and the blit all agree without any of them recomputing it.
 *
 * RENAMED 2026-09-26 from cd_draw_time: the Mac build's CDControlWin::DrawTime(void): this only, painting the elapsed time; Init calls it and its timer repaints with it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_FillColor(GRPort *w, RECT *r, int color);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);

/* same object: 4-byte `push cs; call ` */
extern void copy_rect_to_screen_1040_af03(CDControlWin *w, RECT *r);

void CDControlWin_DrawTime(CDControlWin *w)
{
    char buf[10];
    int min;
    int sec;
    RECT r;
    char out[24];

    SetRect(&r, 0x64, 0xc, 0xc6, 0x3c);
    OffsetRect(&r, w->rect.left, w->rect.top);
    GRPort_FillColor((GRPort *)w, &r, g_draw_color);
    TextColor(0xc);
    if (MSGetCDInfo(&min, &sec, 0) != 0) {
        LTOA1(min, buf, 10);
        STRCPY(out, buf);
        STRCAT(out, ":");
        buf[0] = 0;
        LTOA1(sec, buf, 10);
        if (sec < 10)
            STRCAT(out, "0");
        STRCAT(out, buf);
    } else {
        STRCPY(out, "00:00");
    }
    GRPort_String((GRPort *)w, (unsigned *)&g_font_small, out, w->rect.left + 0x96, w->rect.top + 0x23, 0);
    copy_rect_to_screen_1040_af03(w, &r);
}
