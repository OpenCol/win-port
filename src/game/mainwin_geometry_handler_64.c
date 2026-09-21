/* mainwin_geometry_handler_64 -- shipped at 1048:cd46.
 *
 * Ported by tools/port.py from matched/game/mainwin_geometry_handler_64.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_prefs_root;                        SEG20:0x0010
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_screen_rect(char *w, RECT *r);

typedef struct { col_i16 f0; RECT rect; col_i16 code; } PlacementRec;
extern char * current_window(void);

extern PlacementRec * find_entry(void *root, col_i16 which);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSGetDesktopSize(RECT *r);
extern col_i16 rect_width_1048_d69b(RECT *r);
extern col_i16 rect_height_1048_d6ba(RECT *r);
extern col_i16 win_is_iconic_1048_d8ad(char *w);

void mainwin_geometry_handler_64(void)
{
    char *w;
    PlacementRec *rec;
    RECT desk;
    RECT wr;

    w = current_window();
    rec = find_entry(&g_prefs_root, 1);
    if (rec != 0) {
        win_screen_rect(w, &wr);
        MSGetDesktopSize(&desk);
        if (rect_width_1048_d69b(&desk) <= rect_width_1048_d69b(&wr) && rect_height_1048_d6ba(&desk) <= rect_height_1048_d6ba(&wr)) {
            rec->code = 2;
        } else if (win_is_iconic_1048_d8ad(w)) {
            rec->code = 1;
        } else {
            rec->rect.left = wr.left;
            rec->rect.top = wr.top;
            rec->rect.right = wr.right;
            rec->rect.bottom = wr.bottom;
            rec->code = 0;
        }
    }
}
