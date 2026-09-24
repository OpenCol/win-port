/* cd_control_command -- shipped at 1040:9ffa.
 *
 * Ported by tools/port.py from matched/game/cd_control_command.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void draw_gold_box(Win *w);

extern char * current_window(void);
extern col_i16 MSGetCDTracks(void);
extern col_i32 MSPlayCDTrack(unsigned char track);
extern void MSCloseCD(void);
extern void MSPauseCD(void);
extern void MSUnPauseCD(void);
extern void MSOpenCDTray(void);
extern void MSCloseCDTray(void);

/* same object: 4-byte `push cs; call ` */


void cd_control_command(col_i16 cmd)
{
    col_i16 ntracks;
    Win *w;
    Win *cur;

    ntracks = MSGetCDTracks();
    cur = (Win *)current_window();
    w = (cur != 0) ? (Win *)((char *)cur - 0x26) : cur;
    switch (cmd) {
    case 0x64:
        MSPlayCDTrack(w->track);
        draw_gold_box(w);
        w->stopped = 0;
        break;
    case 0x65:
        MSCloseCD();
        w->stopped = 1;
        break;
    case 0x66:
        if (w->paused != 0) {
            MSUnPauseCD();
            w->paused = 0;
        } else {
            MSPauseCD();
            w->paused = 1;
        }
        break;
    case 0x67:
        if (w->track != ntracks) {
            w->track++;
            draw_gold_box(w);
            if (w->stopped == 0)
                MSPlayCDTrack(w->track);
        }
        break;
    case 0x68:
        if (w->track != 1) {
            w->track--;
            draw_gold_box(w);
            if (w->stopped == 0)
                MSPlayCDTrack(w->track);
        }
        break;
    case 0x69:
        if (w->tray_open != 0) {
            MSCloseCD();
            MSOpenCDTray();
            w->stopped = 1;
            w->tray_open = 0;
        } else {
            MSCloseCDTray();
            w->tray_open = 1;
        }
        break;
    }
}
