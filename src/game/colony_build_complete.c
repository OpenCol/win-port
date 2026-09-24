/* colony_build_complete -- shipped at 1018:8e3d.
 *
 * Ported by tools/port.py from matched/game/colony_build_complete.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern char g_cur_colony_is_players;                 DGROUP:0x4baa
 *   extern char g_msg_seen;                              DGROUP:0x4bab
 *   extern int g_new_building;                           DGROUP:0x0c20
 *
 * The file's own Colony, Nation, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * build_item_name(col_i16 b);
char colony_event_message(char *msg, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);
col_i16 build_item_kind(col_i16 b, col_i16 *out);
col_i16 current_colony_flag_get(col_i16 bit);
void colony_flag_set(col_i16 bit, col_i16 v);
void colony_site_layout(void);
void set_name_slot_1018_b3f6(col_i16 slot, char *s);

#include "nations.h"
#include "control.h"
#include "colflag.h"
/* +1 is CONTROL: 0 human, 1 computer, 2 defeated */






extern col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);
extern void music_mood_if_playing(col_i16 n);









void colony_build_complete(void)
{
    col_i16 item;
    col_i16 type;
    register col_i16 kind;
    col_i16 unit;

    kind = build_item_kind(g_cur_colony_rec->item, &item);
    if (kind == 0)
        goto out;
    if (kind == 1) {
        if (current_colony_flag_get(item))
            goto out;
        if (item == 0x10 && g_cur_colony_rec->warehouses >= 2) {
            g_msg_seen |= colony_event_message("NOMOREWAREHOUSE", !g_msg_seen, 2, 0, 0, 5, 0);
            g_cur_colony_rec->status |= COLONY_BUILD_DONE;
            goto out;
        }
        if (item == 0xf || item == 0x10) {
            g_cur_colony_rec->warehouses++;
            if (item == 0x10) {
                g_cur_colony_rec->status |= COLONY_BUILD_DONE;
                goto done;
            }
        } else if (item == 0x1e || item == 0x1f) {
            g_cur_colony_rec->fort++;
            if (item == 0x1f)
                goto done;
        }
        colony_flag_set(item, 1);
        g_cur_colony_rec->status |= COLONY_BUILD_DONE;
    }
    if (kind == 2) {
        type = item;
        unit = create_unit_1038_2246(type, g_cur_colony_rec->nation,
                                     g_cur_colony_rec->x, g_cur_colony_rec->y);
        if (type == 0xb
            && (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_colony_rec->nation].control != CONTROL_HUMAN))
            g_nations[g_cur_colony_rec->nation].muskets_stock++;
    }
done:
    g_cur_colony_rec->hammers = 0;
    set_name_slot_1018_b3f6(1, build_item_name(g_cur_colony_rec->item));
    if (g_cur_colony_is_players) {
        if (kind == 1 && (item == 0x13 || item == 0x14 || item == 3 || item == 4))
            music_mood_if_playing(3);
        else
            music_mood_if_playing(2);
    }
    g_msg_seen |= colony_event_message("BUILT", !g_msg_seen, 2, 0, 0, 5, 0x54);
    if (kind == 1 && item != 0x1f)
        g_new_building = item;
    colony_site_layout();
out:
    ;
}
