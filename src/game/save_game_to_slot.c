/* save_game_to_slot -- shipped at 1028:d1ec.
 *
 * Ported by tools/port.py from matched/game/save_game_to_slot.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void format_save_filename(char *dst);
extern void save_game_to_file(char *path);

void save_game_to_slot(col_i16 slot)
{
    char name[0x50];
    char path[0x100];

    if (slot == 10)
        strcpy(name, "youwin.sav");
    else if (slot == 9)
        strcpy(name, "auto01.sav");
    else if (slot == 8)
        strcpy(name, "auto02.sav");
    else
        format_save_filename(name);
    strcpy(path, name);
    save_game_to_file(path);
}
