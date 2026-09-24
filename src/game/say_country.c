/* say_country -- shipped at 1050:5c00.
 *
 * Ported by tools/port.py from matched/game/say_country.c.
 *
 * STRCAT -> strcat, STRCHR -> strchr, STRLEN -> strlen, STRNCMP -> strncmp: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *   extern int far g_year;                               SEG20:0x778c
 *   extern char g_kw_string[];                           DGROUP:0x25cc
 *   extern char g_kw_number[];                           DGROUP:0x25d3
 *   extern char g_kw_hex[];                              DGROUP:0x25da
 *   extern char g_zero[];                                DGROUP:0x25de
 *   extern char g_kw_country[];                          DGROUP:0x25e0
 *   extern char g_kw_year[];                             DGROUP:0x25e8
 *   extern char g_percent[];                             DGROUP:0x25ed
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 atol(char *digits);

extern col_i16 STRNICMP(const char *a, const char *b, col_u16 n);
extern void ltoa_rtl(col_i32 v, char *s, col_i16 radix);
extern void LTOA1(col_i16 v, char *s, col_i16 radix);
extern void say_string(char *buf, col_i16 word);
/* same object: 4-byte `push cs; call ` (rule 47) */














void say_country(char *src, char *dst)
{
    char *p;
    char buf[0x28];
    col_i16 i;
    col_i16 n;

    dst[0] = 0;
    do {
        p = strchr(src, '%');
        if (p != 0)
            *p = 0;
        if (*src != 0)
            strcat(dst, src);
        if (p != 0) {
            p++;
            src = p;
            if (STRNICMP(p, g_kw_string, 6) == 0) {
                n = atol(p + 6);
                strcat(dst, g_name_slots[n]);
                src += 7;
            } else if (strncmp(p, g_kw_number, 6) == 0) {
                n = atol(p + 6);
                ltoa_rtl(g_text_args[n], buf, 10);
                strcat(dst, buf);
                src += 7;
            } else if (strncmp(p, g_kw_hex, 3) == 0) {
                n = atol(p + 3);
                ltoa_rtl(g_text_args[n], buf, 16);
                for (i = 0; 4 - strlen(buf) > i; i++)
                    strcat(dst, g_zero);
                strcat(dst, buf);
                src += 4;
            } else if (strncmp(p, g_kw_country, 7) == 0) {
                buf[0] = 0;
                say_string(buf, g_nation_name[g_player_nation]);
                strcat(dst, buf);
                src += 7;
            } else if (strncmp(p, g_kw_year, 4) == 0) {
                LTOA1(g_year, buf, 10);
                strcat(dst, buf);
                src += 4;
            } else if ((col_i16)*p == '%') {
                strcat(dst, g_percent);
                src++;
            }
        }
    } while (p != 0);
}
