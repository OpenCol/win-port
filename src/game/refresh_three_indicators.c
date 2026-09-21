/* refresh_three_indicators -- shipped at 1010:e1c6.
 *
 * Ported by tools/port.py from matched/game/refresh_three_indicators.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char g_st_a;                         DGROUP:0x0a0b
 *   extern unsigned char g_st_b;                         DGROUP:0x0a0d
 *   extern unsigned char g_st_c;                         DGROUP:0x0a0f
 *   extern unsigned char g_val_a;                        DGROUP:0x0a0a
 *   extern unsigned char g_val_b;                        DGROUP:0x0a0c
 *   extern unsigned char g_val_c;                        DGROUP:0x0a0e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1010_bdea(col_i16 n);
void empty_1010_be3e(col_i16 n);
void empty_1010_bf0d(col_i16 n);
void empty_1010_e1aa(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void refresh_three_indicators(void)
{
    unsigned char a, b, c;

    a = g_st_a;
    g_st_a = 0;
    b = g_st_b;
    g_st_b = 0;
    c = g_st_c;
    g_st_c = 0;
    empty_1010_e1aa();
    if (g_st_a == 0 && a != 0)
        empty_1010_be3e(1);
    if (g_st_b == 0 && b != 0)
        empty_1010_bf0d(1);
    if (g_st_c == 0 && c != 0)
        empty_1010_bdea(1);
    if (g_st_a == 0)
        g_val_a = 0xfe;
    if (g_st_b == 0)
        g_val_b = 0xfe;
    if (g_st_c == 0)
        g_val_c = 0xfe;
}
