/* expand_text_template: candidate for 1050:5c00 (641 bytes, 11 callers).  Verdict: RELOCATABLE.
 *
 * Expands a message template into dst. Text up to each '%' is copied; the directive after
 * it is one of STRINGn (a name slot), NUMBERn (a long, base 10), HEXn (a long, base 16,
 * zero-padded to four), COUNTRY (the current nation's adjective through say_string), YEAR,
 * or %% for a literal percent. The template pointer is advanced past each directive.
 *
 * RENAMED 2026-09-27 from say_country: the body is the general message-template expander (%STRINGn, %NUMBERn, %HEXn, %COUNTRY, %YEAR, %%) over (src, dst) for 11 popup callers; the Mac say_country(short, short, char*) has three arguments and is not this function.
 *
 * THE MAC NAME IS popup_string(char*, char*) -- tools/mac_bind.py binds it by call-sequence
 * alignment (2.5 votes to 0, arity 2 = (src, dst)) -- and it is HELD there rather than spelled:
 * a popup_string.c would be the same file as Popup_string.c (Popup::string) on this
 * case-insensitive filesystem.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int ATOL_1050_8de2(char *digits);

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void expand_text_template(char *src, char *dst)
{
    char *p;
    char buf[0x28];
    int i;
    int n;

    dst[0] = 0;
    do {
        p = STRCHR(src, '%');
        if (p != 0)
            *p = 0;
        if (*src != 0)
            STRCAT(dst, src);
        if (p != 0) {
            p++;
            src = p;
            if (STRNICMP(p, "STRING", 6) == 0) {
                n = ATOL_1050_8de2(p + 6);
                STRCAT(dst, g_name_slots[n]);
                src += 7;
            } else if (STRNCMP(p, "NUMBER", 6) == 0) {
                n = ATOL_1050_8de2(p + 6);
                ltoa_rtl(g_text_args[n], buf, 10);
                STRCAT(dst, buf);
                src += 7;
            } else if (STRNCMP(p, "HEX", 3) == 0) {
                n = ATOL_1050_8de2(p + 3);
                ltoa_rtl(g_text_args[n], buf, 16);
                for (i = 0; 4 - STRLEN(buf) > i; i++)
                    STRCAT(dst, "0");
                STRCAT(dst, buf);
                src += 4;
            } else if (STRNCMP(p, "COUNTRY", 7) == 0) {
                buf[0] = 0;
                say_string(buf, g_nation_name[g_game.player_nation]);
                STRCAT(dst, buf);
                src += 7;
            } else if (STRNCMP(p, "YEAR", 4) == 0) {
                LTOA1(g_game.year, buf, 10);
                STRCAT(dst, buf);
                src += 4;
            } else if ((int)*p == '%') {
                STRCAT(dst, "%");
                src++;
            }
        }
    } while (p != 0);
}
