/* PORT: this file's own declarations of game_font_set_1070_dcc1 removed: the header declares it from the definition (W1 step 3). */
/* write_gui_error_log: candidate for 1070:afce (299 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the log font, then hands the GUI library's error log (the file "SMEDS.LOG" first, then its
 * title "GUI Library Error Log" -- that is the shipped push order, DGROUP:0x3aa2 pushed before
 * DGROUP:0x3a98, per tools/check_data_refs.py) and the version line "Version: 1.3.8 Beta"
 * followed by six spaces to the module's two reporters -- both empty in this build, so nothing
 * is opened or written -- then scale_cache_reset. The two "." literals are separate strings in DGROUP.
 */

#include "colonization.h"

#include "classes.h"

extern void report_error_1070_dd0f(void *ctx, char *file, char *title);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_dc82(void *ctx, char *msg);

void write_gui_error_log(void)
{
    char num[10];
    char buf[0x80];

    game_font_set_1070_dcc1(&g_log_font, 2, 0xe);
    report_error_1070_dd0f(&g_error_log, "SMEDS.LOG", "GUI Library Error Log");
    STRCPY(buf, "Version: ");
    LTOA1(1, num, 10);
    STRCAT(buf, num);
    STRCAT(buf, ".");
    LTOA1(3, num, 10);
    STRCAT(buf, num);
    STRCAT(buf, ".");
    LTOA1(8, num, 10);
    STRCAT(buf, num);
    STRCAT(buf, " ");
    STRCAT(buf, "Beta");
    STRCAT(buf, "      ");
    report_error_1070_dc82(&g_error_log, buf);
    scale_cache_reset();
}
