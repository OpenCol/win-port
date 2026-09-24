/* prompt_text_number -- shipped at 1050:6350.
 *
 * Ported by tools/port.py from matched/game/prompt_text_number.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 atol(char *digits);
col_i16 prompt_text_loop(void *ctx, char *name, col_i16 max, char *buf);

extern char * LTOA1(col_i16 v, char *s, col_i16 radix);




col_i16 prompt_text_number(char *ctx, char *name, col_i16 n)
{
    col_i16 result;
    char buf[20];

    LTOA1(n, buf, 10);
    result = prompt_text_loop(ctx, name, 5, buf);
    g_text_args[0] = atol(g_name_slots[0]);
    return result;
}
