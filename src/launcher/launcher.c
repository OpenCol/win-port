/* The release kit's Windows launcher (roadmap W10): Colonization.exe beside bin\colonize.exe.
 *
 * The port reads the game's own files -- COLONIZE.EXE (its initial data), the COLDATA DLLs,
 * the pictures and sounds -- from its working directory, and saves there, as the original
 * did. No game file is in the kit. This finds the player's copy, checks it is the build the
 * port was made from, and starts the port in it:
 *   1. the folder given on the command line;
 *   2. the current folder;
 *   3. this launcher's own folder, or the folder it was unzipped into (the kit put in the game
 *      folder, or in Steam's or GOG's Colonization folder);
 *   4. the folder used last time (%APPDATA%\colonization-port\gamedir.txt);
 *   5. otherwise it asks, with the folder picker, until a folder holds the game.
 * In each folder: the installed Windows edition (COLONIZE.EXE and its data DLLs); else Steam's
 * COLONIZE.ISO or GOG's COLONIZE.GOG -- the DOS release's disc image, which carries the Windows
 * edition too -- in it or one folder down, unpacked once into a game folder of the port's own,
 * %LOCALAPPDATA%\colonization-port\game (isoimport.c), which is then used.
 * COLONIZE.EXE's SHA-256 must be the patched 1995 release's or the original's (before the patch,
 * as on the disc): the port reads only its pictures, which the two share. The original lacks
 * the patch's COLDATA9.DLL, so the Jukebox's Control Panel and the Cheat menu's dialogs are
 * missing (README.md); its COCKGUN.WAV is GUNCOCK.WAV renamed, which the unpacking writes.
 *
 * `Colonization.exe --import ISO DEST` only unpacks (the Linux and macOS kits' script runs it
 * under Wine): exit code 0 when done, the outcome on stdout and in the log.
 *
 * Every step goes to colonization-port.log beside this launcher (else in %TEMP%), and the
 * port is given the same file ($PORT_LOG): its start, and a crash's report, follow there, a
 * crash's minidump beside it (src/host/w16_crash.c). The launcher waits for the game and, if
 * it did not end normally, says what the exit code means and which files to send. A player
 * with one try at a machine then has everything from that try.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <wincrypt.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "isoimport.h"

#define PATCHED  "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
#define PREPATCH "ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650"
#define TITLE "Colonization"
#ifndef KIT_VERSION
#define KIT_VERSION "dev"
#endif

static char log_path[MAX_PATH + 32];

/* a line in the log: opened and closed each time, the port appends to the same file */
static void say(const char *fmt, ...)
{
    FILE *f = log_path[0] ? fopen(log_path, "a") : 0;
    va_list ap;
    if (!f)
        return;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f);
    fclose(f);
}

/* the log beside this launcher, else in %TEMP%; one over 4 MB is set aside as .old */
static void open_log(const char *self)
{
    char tmp[MAX_PATH], old[MAX_PATH + 40];
    FILE *f;
    int k;
    for (k = 0; k < 2; k++) {
        if (k == 0)
            snprintf(log_path, sizeof log_path, "%s\\colonization-port.log", self);
        else if (GetTempPathA(sizeof tmp, tmp))
            snprintf(log_path, sizeof log_path, "%scolonization-port.log", tmp);
        else
            break;
        if ((f = fopen(log_path, "a"))) {
            long n = (fseek(f, 0, SEEK_END), ftell(f));
            fclose(f);
            if (n > 4L << 20) {
                snprintf(old, sizeof old, "%s.old", log_path);
                MoveFileExA(log_path, old, MOVEFILE_REPLACE_EXISTING);
            }
            return;
        }
    }
    log_path[0] = 0;
}

static void windows_version(void)
{
    typedef LONG (WINAPI *rtl_version)(OSVERSIONINFOEXW *);
    typedef const char *(CDECL *wine_version)(void);
    typedef BOOL (WINAPI *wow64)(HANDLE, BOOL *);
    HMODULE ntdll = GetModuleHandleA("ntdll.dll"), k32 = GetModuleHandleA("kernel32.dll");
    rtl_version rv = ntdll ? (rtl_version)(void (*)(void))GetProcAddress(ntdll, "RtlGetVersion") : 0;
    wine_version wv = ntdll ? (wine_version)(void (*)(void))GetProcAddress(ntdll, "wine_get_version") : 0;
    wow64 w64 = k32 ? (wow64)(void (*)(void))GetProcAddress(k32, "IsWow64Process") : 0;
    OSVERSIONINFOEXW v;
    BOOL is_wow = FALSE;
    memset(&v, 0, sizeof v);
    v.dwOSVersionInfoSize = sizeof v;
    if (w64)
        w64(GetCurrentProcess(), &is_wow);
    if (rv && !rv(&v))
        say("windows %lu.%lu build %lu, service pack %u, %s%s%s", v.dwMajorVersion, v.dwMinorVersion,
            v.dwBuildNumber, v.wServicePackMajor, is_wow ? "64-bit" : sizeof(void *) == 8 ? "64-bit" : "32-bit",
            wv ? ", wine " : "", wv ? wv() : "");
}

/* what a process exit code says, for the player and the log */
static const char *exit_meaning(DWORD c)
{
    switch (c) {
    case 0: return "the game ended normally";
    case 3: return "the game stopped itself: a Windows function the port does not have yet, or an assertion";
    case 6: return "the game stopped itself: window messages nested too deep";
    case 0xC0000005: return "access violation (a crash)";
    case 0xC000001D: return "illegal instruction (a crash)";
    case 0xC0000094: return "integer divide by zero (a crash)";
    case 0xC00000FD: return "stack overflow (a crash)";
    case 0xC0000374: return "heap corruption (a crash)";
    case 0xC0000409: return "stack buffer overrun or fast fail (a crash)";
    case 0xC0000135: return "a DLL the program needs was not found";
    case 0xC0000139: return "a function the program needs is missing from a system DLL (Windows too old?)";
    case 0xC000007B: return "bad image format (a 64-bit program on 32-bit Windows, or a broken DLL)";
    case 0xC0000142: return "a DLL failed to initialise";
    case 0xC000013A: return "stopped with Ctrl+C or by closing the console";
    case 0x40010004: return "ended by the debugger or Task Manager";
    case 1: return "ended with code 1 (killed, or the game quit with an error)";
    }
    return (c & 0xF0000000u) == 0xC0000000u ? "a crash" : "an unexpected exit code";
}

/* the Windows edition's files the port reads: COLONIZE.EXE (its pictures), the COLDATA DLLs
   0..8 and COLTEXT0.DLL. COLDATA9.DLL, the patch's, is not required (README.md) */
static int has_game(const char *dir)
{
    static const char *need[] = { "COLONIZE.EXE", "COLTEXT0.DLL", "COLDATA0.DLL", "COLDATA1.DLL", "COLDATA2.DLL",
                                  "COLDATA3.DLL", "COLDATA4.DLL", "COLDATA5.DLL", "COLDATA6.DLL", "COLDATA7.DLL",
                                  "COLDATA8.DLL", 0 };
    char p[MAX_PATH + 16];
    int i;
    if (!dir[0])
        return 0;
    for (i = 0; need[i]; i++) {
        snprintf(p, sizeof p, "%s\\%s", dir, need[i]);
        if (GetFileAttributesA(p) == INVALID_FILE_ATTRIBUTES)
            return 0;
    }
    return 1;
}

/* a disc image with the Windows edition in dir, or in a folder one level down: Steam's
   COLONIZE.ISO first, then GOG's COLONIZE.GOG (raw 2352-byte sectors; the same disc). GOG's
   COLDIG.BIN is a DOS game file, not a disc */
static const char *images[] = { "COLONIZE.ISO", "COLONIZE.GOG", 0 };

static int image_in(const char *dir, const char *name, char *out)
{
    char pat[MAX_PATH + 16];
    WIN32_FIND_DATAA fd;
    HANDLE h;
    snprintf(out, MAX_PATH, "%s\\%s", dir, name);
    if (GetFileAttributesA(out) != INVALID_FILE_ATTRIBUTES)
        return 1;
    snprintf(pat, sizeof pat, "%s\\*", dir);
    if ((h = FindFirstFileA(pat, &fd)) == INVALID_HANDLE_VALUE)
        return 0;
    do
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName[0] != '.') {
            snprintf(out, MAX_PATH, "%s\\%s\\%s", dir, fd.cFileName, name);
            if (GetFileAttributesA(out) != INVALID_FILE_ATTRIBUTES) {
                FindClose(h);
                return 1;
            }
        }
    while (FindNextFileA(h, &fd));
    FindClose(h);
    return 0;
}

/* dir holds the game -- the installed Windows edition, or a disc image (Steam, GOG), which is
   unpacked into a folder of the port's own and dir replaced by it. 0: none of them there */
static int resolve(char *dir)
{
    char img[MAX_PATH], dest[MAX_PATH], base[MAX_PATH], msg[1024], box[MAX_PATH + 1100];
    int k;
    if (!dir[0])
        return 0;
    if (has_game(dir))
        return 1;
    for (k = 0; images[k]; k++)
        if (image_in(dir, images[k], img))
            break;
    if (!images[k])
        return 0;
    if (!SUCCEEDED(SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, base)))
        return 0;
    snprintf(dest, sizeof dest, "%s\\colonization-port\\game", base);
    if (has_game(dest)) {                                /* unpacked before: COLONIZE.EXE is written last */
        char from[MAX_PATH + 16], to[MAX_PATH + 16];
        say("%s: %s: using %s, unpacked from it before", dir, img, dest);
        snprintf(from, sizeof from, "%s\\GUNCOCK.WAV", dest);   /* a folder an older kit unpacked */
        snprintf(to, sizeof to, "%s\\COCKGUN.WAV", dest);
        CopyFileA(from, to, TRUE);
        snprintf(dir, MAX_PATH, "%s", dest);
        return 1;
    }
    say("%s: %s: unpacking it into %s", dir, img, dest);
    if (iso_import(img, dest, msg, sizeof msg)) {
        say("unpacking failed: %s", msg);
        snprintf(box, sizeof box, "%s could not be unpacked:\n\n%s", img, msg);
        MessageBoxA(0, box, TITLE, MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }
    say("unpacked: %s", msg);
    snprintf(dir, MAX_PATH, "%s", dest);
    return 1;
}

/* the next argument of a command line: quoted or not */
static char *next_arg(char **p)
{
    char *s = *p, *start;
    while (*s == ' ')
        s++;
    if (!*s)
        return 0;
    if (*s == '"') {
        start = ++s;
        while (*s && *s != '"')
            s++;
    } else {
        start = s;
        while (*s && *s != ' ')
            s++;
    }
    if (*s)
        *s++ = 0;
    *p = s;
    return start;
}

/* SHA-256 of a file, as 64 hex digits (CryptoAPI: in every Windows and in Wine) */
static int sha256_file(const char *path, char out[65])
{
    HCRYPTPROV prov;
    HCRYPTHASH hash;
    HANDLE f;
    BYTE buf[65536], digest[32];
    DWORD n, len = sizeof digest, i;
    int ok = 0;
    if (!CryptAcquireContextA(&prov, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        return 0;
    if (CryptCreateHash(prov, CALG_SHA_256, 0, 0, &hash)) {
        f = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        if (f != INVALID_HANDLE_VALUE) {
            ok = 1;
            while (ReadFile(f, buf, sizeof buf, &n, 0) && n)
                ok &= CryptHashData(hash, buf, n, 0) != 0;
            CloseHandle(f);
            if (ok && CryptGetHashParam(hash, HP_HASHVAL, digest, &len, 0)) {
                for (i = 0; i < 32; i++)
                    sprintf(out + 2 * i, "%02x", digest[i]);
                out[64] = 0;
            } else
                ok = 0;
        }
        CryptDestroyHash(hash);
    }
    CryptReleaseContext(prov, 0);
    return ok;
}

static void remembered(char *path, int write, char *dir)
{
    char base[MAX_PATH];
    FILE *f;
    if (!SUCCEEDED(SHGetFolderPathA(0, CSIDL_APPDATA, 0, 0, base)))
        return;
    snprintf(path, MAX_PATH, "%s\\colonization-port", base);
    if (write)
        CreateDirectoryA(path, 0);
    strncat(path, "\\gamedir.txt", MAX_PATH - strlen(path) - 1);
    if (write) {
        if ((f = fopen(path, "w"))) {
            fputs(dir, f);
            fclose(f);
        }
    } else if ((f = fopen(path, "r"))) {
        if (!fgets(dir, MAX_PATH, f))
            dir[0] = 0;
        dir[strcspn(dir, "\r\n")] = 0;
        fclose(f);
    }
}

static int pick_folder(char *dir)
{
    BROWSEINFOA bi;
    LPITEMIDLIST id;
    memset(&bi, 0, sizeof bi);
    bi.lpszTitle = "Where is Colonization? Choose the folder of Colonization for Windows (with COLONIZE.EXE), "
                   "or of Steam's or GOG's Colonization (with COLONIZE.ISO or COLONIZE.GOG).";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    CoInitialize(0);
    id = SHBrowseForFolderA(&bi);
    if (!id)
        return 0;
    SHGetPathFromIDListA(id, dir);
    CoTaskMemFree(id);
    return 1;
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
    char self[MAX_PATH], dir[MAX_PATH] = "", store[MAX_PATH], exe[MAX_PATH + 32], sha[65], msg[1024], *slash;
    int i;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    (void)inst; (void)prev; (void)show;

    GetModuleFileNameA(0, self, sizeof self);
    if ((slash = strrchr(self, '\\')))
        *slash = 0;
    open_log(self);
    say("==== launcher %s (%s) started, %s", KIT_VERSION, sizeof(void *) == 8 ? "x64" : "x86", self);
    windows_version();
    say("command line [%s]", cmd ? cmd : "");
    if (cmd && !strncmp(cmd, "--import", 8)) {           /* the scripts' unpacking, under Wine */
        char *p = cmd + 8, *iso = next_arg(&p), *dest = next_arg(&p);
        int rc = iso && dest ? iso_import(iso, dest, msg, sizeof msg) : 2;
        if (!iso || !dest)
            snprintf(msg, sizeof msg, "usage: Colonization.exe --import ISO DEST");
        say("import: %s", msg);
        printf("%s\n", msg);
        fflush(stdout);
        return rc;
    }
    /* where the game is, in this order -- in each folder the installed Windows edition first,
       then Steam's COLONIZE.ISO, then GOG's COLONIZE.GOG (resolve):
         1. the folder given on the command line;
         2. the current folder;
         3. this launcher's folder and the one above it (the kit unzipped into the game's folder:
            the zip has a top folder of its own);
         4. the folder used last time (so a kit unzipped elsewhere does not ask again);
         5. a folder the player picks, asked until one holds the game or the player cancels */
    if (cmd && *cmd) {                                   /* 1 */
        snprintf(dir, sizeof dir, "%s", cmd[0] == '"' ? cmd + 1 : cmd);
        dir[strcspn(dir, "\"")] = 0;
        say("the command line's folder %s: %s", dir, resolve(dir) ? "found" : "nothing there");
        if (!has_game(dir))
            dir[0] = 0;
    }
    if (!dir[0] && GetCurrentDirectoryA(sizeof store, store)) {   /* 2 */
        say("the current folder %s", store);
        if (resolve(store))
            snprintf(dir, sizeof dir, "%s", store);
    }
    for (i = 0; !dir[0] && i < 2; i++) {                 /* 3 */
        snprintf(store, sizeof store, "%s", self);
        if (i == 1) {
            if (!(slash = strrchr(store, '\\')))
                break;
            *slash = 0;
        }
        if (resolve(store))
            snprintf(dir, sizeof dir, "%s", store);
    }
    if (!dir[0]) {                                       /* 4 */
        char last[MAX_PATH] = "";
        remembered(store, 0, last);
        if (last[0] && resolve(last))
            snprintf(dir, sizeof dir, "%s", last);
    }
    say("game folder %s", dir[0] ? dir : "(none found: asking)");
    while (!dir[0]) {                                    /* 5 */
        char picked[MAX_PATH] = "";
        if (!pick_folder(picked)) {
            say("no folder chosen: stopped");
            return 1;
        }
        say("chosen folder %s", picked);
        if (resolve(picked)) {
            snprintf(dir, sizeof dir, "%s", picked);
            break;
        }
        snprintf(msg, sizeof msg, "Colonization was not found in\n%s\n\nChoose the folder of Colonization for "
                 "Windows (the one with COLONIZE.EXE and the COLDATA files), or of Steam's or GOG's "
                 "Colonization (the one with COLONIZE.ISO or COLONIZE.GOG).", picked);
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONINFORMATION);
    }
    snprintf(exe, sizeof exe, "%s\\COLONIZE.EXE", dir);
    if (!sha256_file(exe, sha)) {
        say("%s could not be read (%lu)", exe, GetLastError());
        MessageBoxA(0, "COLONIZE.EXE could not be read.", TITLE, MB_OK | MB_ICONERROR);
        return 2;
    }
    say("COLONIZE.EXE SHA-256 %s: %s", sha, !strcmp(sha, PATCHED) ? "the patched 1995 release" :
        !strcmp(sha, PREPATCH) ? "the original 1995 release, before the patch" : "not a known build");
    if (!strcmp(sha, PREPATCH)) {                        /* the port reads only its pictures: the same */
        char wav[MAX_PATH + 32];
        snprintf(wav, sizeof wav, "%s\\COCKGUN.WAV", dir);
        say("the original release: without the patch's COLDATA9.DLL the Jukebox's Control Panel and the Cheat "
            "menu's dialogs are missing%s", GetFileAttributesA(wav) == INVALID_FILE_ATTRIBUTES ?
            "; COCKGUN.WAV is missing too (it is GUNCOCK.WAV renamed: a copy under that name restores it)" : "");
    }
    if (strcmp(sha, PATCHED) && strcmp(sha, PREPATCH)) {
        snprintf(msg, sizeof msg, "%s\\COLONIZE.EXE is not a build this port knows.\n\nSHA-256 %s\n\nThe port "
                 "needs the 1995 release of Colonization for Windows (SHA-256 %.16s..., or the original's). A "
                 "modified COLONIZE.EXE will not do: the port reads the game's pictures from it.", dir, sha, PATCHED);
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONEXCLAMATION);
        return 4;
    }
    remembered(store, 1, dir);
    snprintf(exe, sizeof exe, "\"%s\\bin\\colonize.exe\"", self);
    if (log_path[0])                                     /* the port's log: this same file */
        SetEnvironmentVariableA("PORT_LOG", log_path);
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    say("starting %s in %s", exe, dir);
    if (!CreateProcessA(0, exe, 0, 0, FALSE, 0, 0, dir, &si, &pi)) {
        DWORD err = GetLastError();
        say("could not start it: error %lu", err);
        snprintf(msg, sizeof msg, "Could not start %s (error %lu).%s%s", exe, err,
                 log_path[0] ? "\n\nDetails are in " : "", log_path);
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONERROR);
        return 5;
    }
    CloseHandle(pi.hThread);
    {   /* wait for the game: how it ended goes in the log, and a crash is said */
        DWORD code = 0, t0 = GetTickCount(), secs;
        char dump[MAX_PATH + 40], *dot;
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &code);
        CloseHandle(pi.hProcess);
        secs = (GetTickCount() - t0) / 1000;
        say("colonize.exe ended after %lu s: exit code %lu (0x%08lX), %s", secs, code, code, exit_meaning(code));
        if (code != 0 && log_path[0]) {
            snprintf(dump, sizeof dump, "%s", log_path);
            if ((dot = strrchr(dump, '.')))
                strcpy(dot, ".dmp");
            snprintf(msg, sizeof msg, "The game stopped: exit code 0x%08lX, %s.\n\nWhat happened is written in\n%s%s%s"
                     "\n\nPlease send %s.", code, exit_meaning(code), log_path,
                     GetFileAttributesA(dump) != INVALID_FILE_ATTRIBUTES ? "\nand\n" : "",
                     GetFileAttributesA(dump) != INVALID_FILE_ATTRIBUTES ? dump : "",
                     GetFileAttributesA(dump) != INVALID_FILE_ATTRIBUTES ? "both files" : "that file");
            MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONEXCLAMATION);
        }
        return code ? 6 : 0;
    }
}
