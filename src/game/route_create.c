/* route_create: candidate for 1028:cdf4 (660 bytes).  Verdict: RELOCATABLE.
 *
 * Creates a trade route: refuses a 13th (TRADEMANY, count 12 in the message number), asks
 * for the first stop, decides land or sea (TRADETYPE when the colony is coastal), proposes a
 * name -- the colony's name plus a random line of TRADENAMES and the suffix at data segment
 * 29 +0x3c, with " A" (then B, C...) appended while it collides with an existing route --
 * lets the player edit it (TRADENAME, 31 chars), fills the new record (name, kind at +0x20,
 * two stops), asks for the second stop and opens the route editor on it.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int ask_port(int a, int b, int c, int d);
extern int water_access(int x, int y);

extern int text_open(void *ctx, char *name);
extern void space_string(char *buf);
extern char *text_get(void);

extern void random_activate(int seed);

extern int popup_ask(void *ctx, void *name, int max, void *dflt);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_dest(int r);

extern void new_dest(int colony);

void route_create(void)
{
    int kind;
    int colony;
    int r;
    int idx;
    int n;
    int k;
    int dup;
    char name[0x50];
    int i;
    int found;

    if (g_game.trade_route_count >= 12) {
        g_text_args[0] = 12;
        show_text_dialog_1028_e48e("TRADEMANY");
        goto done;
    }
    set_text_arg_i_1028_e3ee(0, 1);
    colony = ask_port(-1, -1, 0, 0);
    if (colony < 0)
        goto done;
    if (water_access(g_colonies[colony].x, g_colonies[colony].y) == 0)
        kind = 0;
    else {
        r = show_text_dialog_1028_e48e("TRADETYPE");
        if (r < 0)
            goto done;
        kind = r == 1;
    }
    STRCPY(name, g_colonies[colony].name);
    if (text_open(g_game_txt, "TRADENAMES") == 0) {
        space_string(name);
        n = ATOL_1028_e4e9(text_get());
        random_activate(g_random_seed);
        k = rand_range(1, n);
        for (i = 0; i < k; i++)
            text_get();
        STRCAT(name, g_text_buf);
        dup = 0;
        do {
            found = 0;
            for (i = 0; found == 0 && g_game.trade_route_count > i; i++) {
                if (STRCMP(name, g_routes[i].name) == 0) {
                    found = 1;
                    if (dup == 0) {
                        STRCAT(name, " A");
                        dup = 1;
                    } else
                        name[STRLEN(name) - 1]++;
                }
            }
        } while (found != 0);
    }
    if (popup_ask(g_game_txt, "TRADENAME", 0x1f, name) != 0)
        goto done;
    idx = g_game.trade_route_count;
    set_dest(idx);
    STRCPY(g_cur_route_rec->name, g_name_slots[0]);
    g_cur_route_rec->sea = kind;
    g_cur_route_rec->count = 2;
    set_route(0);
    new_dest(colony);
    set_text_arg_i_1028_e3ee(0, 2);
    colony = ask_port(-1, -1, 0, 0);
    if (colony < 0)
        goto done;
    set_route(1);
    new_dest(colony);
    g_game.trade_route_count++;
    route_edit(idx);
done: ;
}
