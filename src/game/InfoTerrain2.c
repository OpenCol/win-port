/* InfoTerrain2: candidate for 1058:8d08 (206 bytes).  Verdict: RELOCATABLE.
 * symbol: InfoTerrain2
 *
 * InfoTerrain's sibling for the word table at SEG20:0xd8d8 (a terrain feature): the name in
 * parentheses, colour 250, small or large font by desktop width, line advanced by 17.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void open_string(char *s);
extern void close_string(char *s);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

/* The module's rectangle-width helper is defined here because the shipped call to it is the
 * compiler's own `push cs / call `. */

void InfoTerrain2(GRPort *port, int kind, int *px, int *py)
{
    RECT desk;
    char buf[0x50];

    buf[0] = 0;
    TextColor(0xfa);
    open_string(buf);
    say_string(buf, g_feature_words[kind]);
    close_string(buf);
    MSGetDesktopSize(&desk);
    if (rect_width_1058_bd2d(&desk) <= 640)
        GRPort_String(port, (unsigned *)&g_info_font_small, buf, *px, *py, 5);
    else
        GRPort_String(port, (unsigned *)&g_info_font_large, buf, *px, *py, 5);
    *py += 17;
}
