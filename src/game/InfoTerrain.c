/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* InfoTerrain: candidate for 1058:8c46 (194 bytes).  Verdict: RELOCATABLE.
 * symbol: InfoTerrain
 *
 * Writes a terrain kind's name in parentheses, colour 250, at (*px, *py) in the info window --
 * the small font on a 640-wide desktop, the large one otherwise -- and moves the line down 17.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void open_string(char *s);
extern void close_string(char *s);
extern void say_terrain(char *s, int kind);


/* The module's rectangle-width helper is defined here because the shipped call to it is the
 * compiler's own `push cs / call `. */

void InfoTerrain(GRPort *port, int kind, int *px, int *py)
{
    RECT desk;
    char buf[0x50];

    buf[0] = 0;
    TextColor(0xfa);
    open_string(buf);
    say_terrain(buf, kind);
    close_string(buf);
    MSGetDesktopSize(&desk);
    if (rect_width_1058_bd2d(&desk) <= 640)
        GRPort_String(port, (unsigned *)&g_info_font_small, buf, *px, *py, 5);
    else
        GRPort_String(port, (unsigned *)&g_info_font_large, buf, *px, *py, 5);
    *py += 17;
}
