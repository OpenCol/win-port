/* PORT: the version word is 16-bit: `ver` i16, so the 2-byte read fills it (W7: AMER2.MP was refused, error 3). */
/* PORT: this file's own declarations of file_MSOpenFile_1038_e492 removed: the header declares it from the definition (W1 step 3). */
/* load_map_file: candidate for 1038:cfae (455 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_map_file
 * name-source: sibling-build
 *
 * Reads a map file into the three planes `map_startup` has already allocated: a
 * width and height (through the identity `swap_word_identity_1038_e4e2`, so read as stored), a version word, then three blocks of `width * height` bytes
 * straight into the locked plane pointers at DGROUP:0x1c72, 0x1c76 and 0x1c7a.
 *
 * SIX ERROR CODES, all left at DGROUP:0x1c68 and all distinct: 1 the file would not open, 2
 * a short header, 3 a version the loader will not take, and 4, 5, 6 for the three planes in
 * order -- so the code says which plane ran out. Success writes 0 there and returns 0, and
 * every other path returns 1.
 *
 * THE VERSION CHECK IS TWO COMPARISONS AND NOT ONE. `ver > 4 || ver < 4` is what the shipped
 * does -- `JG` then `JNL`, two `CMP`s against the same 4 -- where `ver != 4` is a single
 * compare and a `JZ`. And it only rejects when DGROUP:0x1c62 is already non-negative, so the
 * FIRST map read in a session takes any version and sets the word; every later one must be
 * version 4.
 *
 * The file name is copied into a local before it is opened, through `STRCAT` onto an empty
 * buffer rather than `STRCPY`. It is the same SEG27:0x3c name `map_startup` uses, and
 * the shipped data leaves it empty.
 *
 * `err` is initialised in its declaration -- the `MOV SI,1` lands before the exception
 * frame's 6 and before the DiskFile is constructed -- the same shape as the sibling loader.
 *
 * RENAMED 2026-09-27 from map_read_planes: Mac load_map_file(void), 526 bytes, against 455. Caller: start_new_game on both. Mac seq DiskFile ctor, MSOpenFile, MSReadFile x2, map_check, MSReadFile x3, MSCloseFile is the Windows seq DiskFile_ctor, file_MSOpenFile, read_file x2 (+swap words), return_zero (map_check), read_file x3, MSCloseFile. Arity: void. Layout: Mac CODE 20 allocate_map_memory, map_check, load_map_file, map_startup; Windows 1038 map_planes_alloc, return_one_1038_cf4b (0 callers, Windows-only), return_zero_1038_cf6e, map_read_planes, [close_log_file, map_alloc: Windows-only, 0 callers], map_load.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* DEFINED in this module: load_map_file and map_startup take &g_map_size.w as `push ds`,
 * which Borland emits only after a definition, while earlier functions of the module
 * reach it . */
MapSize g_map_size;   /* DGROUP:0x4c40 */

/* same object: 4-byte `push cs; call ` */
extern "C" int read_file_1038_e4b8(MSFile *file, void *buf, long n);

extern "C" int map_check(void);

int load_map_file(void)
{
    register int err = 1;
    DiskFile f;
    i16 ver;
    char name[0x14];

    name[0] = 0;
    STRCAT(name, g_map_file_name);
    if (file_MSOpenFile_1038_e492((MSFile *)&f, name) == 0) {
        g_map_load_error = 1;
        goto out;
    }
    if (read_file_1038_e4b8((MSFile *)&f, &g_map_size, (long)sizeof(MapSize)) == 0) {
        g_map_load_error = 2;
        goto out;
    }
    g_map_size.w = swap_word_identity_1038_e4e2(g_map_size.w);
    g_map_size.h = swap_word_identity_1038_e4e2(g_map_size.h);
    if (read_file_1038_e4b8((MSFile *)&f, &ver, 2L) == 0) {
        g_map_load_error = 2;
        goto out;
    }
    ver = swap_word_identity_1038_e4e2(ver);
    if ((ver > 4 || ver < 4) && g_map_version >= 0) {
        g_map_load_error = 3;
        goto out;
    }
    g_map_version = ver;
    g_map_cells = (long)g_map_size.w * g_map_size.h;
    if (map_check() != 0)
        goto out;
    if (read_file_1038_e4b8((MSFile *)&f, g_plane0, g_map_cells) == 0) {
        g_map_load_error = 4;
        goto out;
    }
    if (read_file_1038_e4b8((MSFile *)&f, g_plane1, g_map_cells) == 0) {
        g_map_load_error = 5;
        goto out;
    }
    if (read_file_1038_e4b8((MSFile *)&f, g_plane2, g_map_cells) == 0) {
        g_map_load_error = 6;
        goto out;
    }
    err = 0;
    g_map_load_error = 0;
out:
    MSCloseFile_1038_e4f7(&f);
    return err;
}

