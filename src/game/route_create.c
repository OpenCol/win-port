/* route_create -- shipped at 1028:cdf4.
 *
 * Ported by tools/port.py from matched/game/route_create.c.
 *
 * STRCAT -> strcat, STRCMP -> strcmp, STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern RouteRec far g_routes[];                      SEG20:0xc804
 *   extern RouteRec far *g_cur_route_rec;                DGROUP:0x4c18
 *   extern int far g_trade_route_count;                  SEG20:0x77a2
 *   extern long far g_text_args;                         SEG31:0x02d4
 *   extern char far g_default_route_name[];              SEG31:0x00a4
 *   extern char far g_text_buf[];                        SEG29:0x003c
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern int far g_random_seed;                        DGROUP:0x4b86
 *
 * The file's own Colony, RouteRec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: sea -> kind
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ATOL_1028_e4e9(char *s);
col_i16 pick_trade_route_stop(col_i16 a, col_i16 b, col_i16 c, col_i16 d);
col_i16 show_text_dialog_1028_e48e(char *msg);
void route_edit(col_i16 r);
void route_entry_init(col_i16 colony);
void set_current_route(col_i16 r);
void set_route(col_i16 stop);
void set_text_arg_i_1028_e3ee(col_i16 a, col_i16 b);

/* 0xca bytes at SEG20:0x7c28 */

/* 0x4a bytes at SEG20:0xc804 */











extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);




extern col_i16 load_text_resource(void *ctx, char *name);
extern void append_space(char *s);
extern char * next_text_line(void);

extern void seed_random(col_i16 seed);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 prompt_text_loop(void *ctx, char *name, col_i16 max, char *buf);
/* same object: 4-byte `push cs; call ` (rule 47) */




void route_create(void)
{
    col_i16 kind;
    col_i16 c;
    col_i16 r;
    col_i16 idx;
    col_i16 n;
    col_i16 k;
    col_i16 dup;
    char name[0x50];
    col_i16 i;
    col_i16 found;

    if (g_trade_route_count >= 12) {
        (*g_text_args) = 12;
        show_text_dialog_1028_e48e("TRADEMANY");
        goto done;
    }
    set_text_arg_i_1028_e3ee(0, 1);
    c = pick_trade_route_stop(-1, -1, 0, 0);
    if (c < 0)
        goto done;
    if (find_adjacent_ocean(g_colonies[c].x, g_colonies[c].y) == 0)
        kind = 0;
    else {
        r = show_text_dialog_1028_e48e("TRADETYPE");
        if (r < 0)
            goto done;
        kind = r == 1;
    }
    strcpy(name, g_colonies[c].name);
    if (load_text_resource(&(*g_text_ctx), "TRADENAMES") == 0) {
        append_space(name);
        n = ATOL_1028_e4e9(next_text_line());
        seed_random(g_random_seed);
        k = rand_range(1, n);
        for (i = 0; i < k; i++)
            next_text_line();
        strcat(name, g_text_buf);
        dup = 0;
        do {
            found = 0;
            for (i = 0; found == 0 && g_trade_route_count > i; i++) {
                if (strcmp(name, ((char *)((unsigned char *)&(g_routes[i]) + 0x0))) == 0) {
                    found = 1;
                    if (dup == 0) {
                        strcat(name, " A");
                        dup = 1;
                    } else
                        name[strlen(name) - 1]++;
                }
            }
        } while (found != 0);
    }
    if (prompt_text_loop(&(*g_text_ctx), "TRADENAME", 0x1f, name) != 0)
        goto done;
    idx = g_trade_route_count;
    set_current_route(idx);
    strcpy(((char *)((unsigned char *)(g_cur_route_rec) + 0x0)), g_default_route_name);
    g_cur_route_rec->kind = kind;
    g_cur_route_rec->stops = 2;
    set_route(0);
    route_entry_init(c);
    set_text_arg_i_1028_e3ee(0, 2);
    c = pick_trade_route_stop(-1, -1, 0, 0);
    if (c < 0)
        goto done;
    set_route(1);
    route_entry_init(c);
    g_trade_route_count++;
    route_edit(idx);
done: ;
}
