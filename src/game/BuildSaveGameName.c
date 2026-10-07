/* BuildSaveGameName: candidate for 1008:b271 (204 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes "<nation><year>.sav" into dst -- the savegame filename. One of four
 * three-letter nation prefixes is chosen by a switch on the current nation (a jump
 * table), then the year through LTOA1 in base 10, then the ".sav" extension.
 *
 * WHAT THIS FUNCTION IS, corrected 2026-09-20 by tools/check_global_addresses.py. It was
 * read as "<season> <year><suffix>" and the switch subject was declared `g_game.season` at
 * SEG20:0x778e. The bytes say the switch reads SEG20:0x779a, which 538 references
 * elsewhere call `g_game.player_nation`, and the four strings it selects are at DGROUP:0x7fd,
 * 0x801, 0x805 and 0x809 -- 'eng', 'fra', 'spa', 'dut', with '.sav' immediately after
 * them at 0x80d. Nations, not seasons; a filename, not a date. The caller is
 * perform_save.
 *
 * Nothing could have caught this from the bytes alone: the offset in a data reference is
 * a masked FIXUP, so a switch on the wrong global at the wrong address compiles to the
 * same 204 bytes and graded RELOCATABLE with a coherent-sounding story attached.
 *
 * RENAMED 2026-09-27 from format_save_filename: the Mac build's BuildSaveGameName(char*): the save file's name from the nation prefix and the slot number (itoa); perform_save is its caller on both sides.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void BuildSaveGameName(char *dst)
{
    char num[10];
    char buf[0x50];

    dst[0] = 0;
    switch (g_game.player_nation) {
    case 0: STRCPY(buf, "eng"); break;
    case 1: STRCPY(buf, "fra"); break;
    case 2: STRCPY(buf, "spa"); break;
    case 3: STRCPY(buf, "dut"); break;
    }
    LTOA1(g_game.year, num, 10);
    STRCAT(buf, num);
    STRCAT(buf, ".sav");
    STRCPY(dst, buf);
}
