/* PORT: this file's own declarations of file_MSOpenFile_1040_a922, read_text_line_1040_a89b, read_text_paragraph removed: the header declares them from the definition (W1 step 3). */
/* DoFancyText: candidate for 1040:6592 (334 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DoFancyText
 *
 * Shows a formatted text page: the file `name` is opened as a stack DiskFile (a failure is
 * reported with "Could not open file in DoFancyText -"), read line by line until the line "@tag", whose following
 * words are read and rendered into the window with `arg` by draw_wrapped_text.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1040_a948(void *ctx, char *msg, char *name);

extern "C" void draw_wrapped_text(void *port, void *rect, char *line);

extern "C" void DoFancyText(void *w, char *name, char *tag, void *rect)
{
    DiskFile f;
    char *p;
    char line[0x226];
    int n;

    if (file_MSOpenFile_1040_a922((MSFile *)&f, name) == 0) {
        report_error_1040_a948(&g_error_log, "Could not open file in DoFancyText -", name);
        return;
    }
    MSSeekFile_1040_a877((MSFile *)&f, 0L);
    n = read_text_line_1040_a89b((MSFile *)&f, line, 0xff);
    while (n != 0) {
        if ((int)line[0] == '@') {
            p = line + 1;
            if (STRCMP(p, tag) == 0) {
                read_text_paragraph(line, &f);
                draw_wrapped_text(w, rect, line);
                break;
            }
        }
        n = read_text_line_1040_a89b((MSFile *)&f, line, 0xff);
    }
    MSCloseFile_1040_a956((MSFile *)&f);
}
