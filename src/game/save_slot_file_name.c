/* save_slot_file_name: candidate for 1028:d1ba (50 bytes).  Verdict: RELOCATABLE.
 *
 * Writes "COLONY" + ".SAV" -- the save-file name -- into out. (The census name predates
 * reading the two strings.)
 *
 * RENAMED 2026-09-27 from get_colony_name: The body is 50 bytes (disasm/SEG6.asm): STRCPY(out, DS:0x12a9 'COLONY') and then STRCAT(out, DS:0x12b0 '.SAV'). The int parameter is never read. The callers are build_slot_menu (1028:d2c4), which calls it per slot and then opens the save header, and save_game_dialog (1028:dca2). So this is the save-slot file name, not a colony name. There is no Mac counterpart: Mac builds save names in BuildSaveGameName (already bound on Windows to 1008:b271) and perform_save opens its segment, so a descriptive name is used. Why the first binding went wrong: tools/mac_rename.py matched the (short, char*) shape and the one-to-one COLONY identifier. But 'COLONY' here is a filename stem that shares its spelling with the COLONY TEXT section ('What shall we name this colony?'). The same coincidence gives this pair a false AGREE in analysis/mac-string-witness.json (shared 'COLONY', df 3/2, not promoted, still medium). The binary evidence entry recorded on 1028_d1ba.json by commit 70e62e697 should be dropped or overridden with the rename. The new name is free under a case-insensitive check. This rename must come first in its chain.
 */

#include "colonization.h"

void save_slot_file_name(char *out, int unused1)
{
    STRCPY(out, "COLONY");
    STRCAT(out, ".SAV");
}
