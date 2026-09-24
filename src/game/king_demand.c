/* king_demand -- shipped at 1048:7c13.
 *
 * Ported by tools/port.py from matched/game/king_demand.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation34 far g_nation_text[];                 SEG20:0x7810
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern unsigned char far g_nation_colony_count[];    DGROUP:0x4b48
 *   extern unsigned char far g_nation_population[];      DGROUP:0x4b54
 *   extern int far g_year;                               SEG20:0x778c
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern signed char far g_year_hi;                    SEG20:0x77a9
 *   extern signed char far g_year_lo;                    SEG20:0x77aa
 *   extern int far g_rebel_sentiment;                    SEG20:0x77d2
 *   extern int far g_difficulty_words[];                 SEG20:0xd8fe
 *   extern char far g_text_ctx[];                        DGROUP:0x009e
 *   extern char far g_text_buf[];                        SEG29:0x003c
 *   extern char g_str_kingvictory[];                     DGROUP:0x21be
 *   extern char g_str_countries[];                       DGROUP:0x21ca
 *   extern char g_str_kingwife[];                        DGROUP:0x21d4
 *   extern char g_str_ordinal[];                         DGROUP:0x21dd
 *   extern char g_str_kingwar[];                         DGROUP:0x21e5
 *   extern char g_str_countries2[];                      DGROUP:0x21ed
 *   extern char g_str_kingnavact[];                      DGROUP:0x21f7
 *   extern char g_str_kingstampact[];                    DGROUP:0x2202
 *
 * The file's own Nation, Nation34 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1048_db44(col_i16 a, col_i16 b);
void apply_tax_change(char *s, col_i16 n);
void dialog_set_option_text(col_i16 slot, void *s);

/* 0x34 bytes at SEG20:0x7810 */






















extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);

extern void skip_text_lines(void *rec, char *key, col_i16 n);
extern col_i16 * max_ptr_1048_efef(col_i16 *a, col_i16 *b);

/* same object: 4-byte `push cs; call ` */




col_i16 king_demand(void)
{
    col_i16 r;
    col_i16 div;
    col_i16 t;
    col_i16 period;
    col_i16 a;
    col_i16 b;
    char buf[0x50];
    register col_i16 score;
    register col_i16 n;

    r = 0;
    if (g_nation_colony_count[g_cur_nation] == 0)
        goto out;
    if (g_turn_counter < 0x1e)
        goto out;
    period = 0x12;
    if (g_year > 0x640)
        period -= 3;
    if (g_year > 0x6a4)
        period -= 3;
    if (g_year > 0x6d6)
        period -= 3;
    div = period - (g_cur_nation < 4 && g_nation_text[g_cur_nation].met == 0
                    ? g_difficulty - 2 : 0) * 2;
    if (g_turn_counter % div != 0)
        goto out;
    if ((col_i16)g_cur_nation_rec->tax > 0x55)
        goto out;
    score = rand_range(1, 0x3e8);
    score -= (col_i16)g_cur_nation_rec->tax * 5;
    score += g_nation_population[g_cur_nation];
    score += (col_i16)(g_cur_nation_rec->gold / 100);
    score += g_turn_counter / 0x1e;
    score += g_rebel_sentiment * 10;
    set_name_slot_from_text(0, g_difficulty_words[g_difficulty]);
    dialog_set_option_text(1, ((char *)((unsigned char *)&(g_nation_text[g_cur_nation]) + 0x0)));
    if (score < 0x64) {
        n = rand_range(2, 5);
        n = min_int_1048_db44(n, (col_i16)g_cur_nation_rec->tax);
        if (n <= 0)
            goto out;
        n = -n;
        strcpy(buf, g_str_kingvictory);
        skip_text_lines(g_text_ctx, g_str_countries, (col_i16)g_year_lo - 1);
        dialog_set_option_text(2, g_text_buf);
    } else if (score < 0x28a && (col_i16)g_year_hi < 0x1e) {
        n = 1;
        strcpy(buf, g_str_kingwife);
        g_year_hi = *max_ptr_1048_efef((b = 1, &b), (a = (col_i16)g_year_hi + 1, &a));
        skip_text_lines(g_text_ctx, g_str_ordinal, (col_i16)g_year_hi - 1);
        dialog_set_option_text(2, g_text_buf);
    } else if (score < 0x3b6) {
        n = 2;
        strcpy(buf, g_str_kingwar);
        do {
            t = rand_range(1, 8);
        } while ((col_i16)g_year_lo == t);
        g_year_lo = (char)t;
        skip_text_lines(g_text_ctx, g_str_countries2, t - 1);
        dialog_set_option_text(2, g_text_buf);
    } else if (score < 0x44c) {
        n = rand_range(3, 4);
        strcpy(buf, g_str_kingnavact);
    } else {
        n = rand_range(5, 8);
        dialog_set_option_text(2, ((char *)((unsigned char *)&(g_nation_text[g_cur_nation]) + 0x18)));
        strcpy(buf, g_str_kingstampact);
    }
    g_cur_nation_rec->f10 = n;
    apply_tax_change(buf, n);
    r = 1;
out:
    return r;
}
