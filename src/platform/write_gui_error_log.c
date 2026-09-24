/* write_gui_error_log -- shipped at 1070:afce.
 *
 * Ported by tools/port.py from matched/platform/write_gui_error_log.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_log_font;                          DGROUP:0x4e68
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_dc82(void *ctx, char *msg);
void empty_1070_dd0f(void *ctx, char *title, char *file);
void sub_1070_dcc1(void *ctx, col_i16 a, col_i16 b);

extern char * LTOA1(col_i16 v, char *buf, col_i16 radix);




/* same object: 4-byte `push cs; call ` (rule 47) */

extern void scale_cache_reset(void);

void write_gui_error_log(void)
{
    char num[10];
    char buf[0x80];

    sub_1070_dcc1(&(*g_log_font), 2, 0xe);
    empty_1070_dd0f(&(*g_error_ctx), "GUI Library Error Log", "SMEDS.LOG");
    strcpy(buf, "Version: ");
    LTOA1(1, num, 10);
    strcat(buf, num);
    strcat(buf, ".");
    LTOA1(3, num, 10);
    strcat(buf, num);
    strcat(buf, ".");
    LTOA1(8, num, 10);
    strcat(buf, num);
    strcat(buf, " ");
    strcat(buf, "Beta");
    strcat(buf, " ");
    empty_1070_dc82(&(*g_error_ctx), buf);
    scale_cache_reset();
}
