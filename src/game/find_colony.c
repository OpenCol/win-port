/* find_colony -- shipped at 1048:a253.
 *
 * Ported by tools/port.py from matched/game/find_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *id);
void dialog_set_option_text(col_i16 slot, char *s);

#include "nations.h"
extern col_i16 prompt_text_loop(char *a, char *prompt, col_i16 n, char *dflt);
extern col_i16 STRICMP(char *a, char *b);
extern void set_current_colony(col_i16 c);
extern unsigned char map_plane3_at(col_i16 x, col_i16 y);
extern void view_pieces(void *ctx);
extern void map_goto(col_i16 x, col_i16 y, col_i16 mode);





void find_colony(void)
{
    col_i16 found;
    register col_i16 c;

    found = -1;
    if (prompt_text_loop(&(*g_text_ctx), "FINDCITY", 0x17, "") == 0) {
        for (c = 0; found < 0 && c < g_colony_count; c++) {
            set_current_colony(c);
            if (STRICMP(g_name_slots[0], ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2))) == 0) {
                if (g_view_nation >= NATION_EUROPEAN_COUNT
                    || (map_plane3_at(g_cur_colony_rec->x, g_cur_colony_rec->y)
                        & (0x10 << g_view_nation))
                    || g_reveal_all != 0)
                    found = c;
            }
        }
        if (found >= 0) {
            view_pieces(&(*g_game_record2));
            map_goto(g_cur_colony_rec->x, g_cur_colony_rec->y, 0);
        } else {
            dialog_set_option_text(0, g_name_slots[0]);
            ask_text_resource("NOCITY");
        }
    }
}
