/* The release kit's Windows launcher (roadmap W10): Colonization.exe beside bin\colonize.exe.
 *
 * The port reads the game's own files -- COLONIZE.EXE (its initial data), the COLDATA DLLs,
 * the pictures and sounds -- from its working directory, and saves there, as the original
 * did. No game file is in the kit. This finds the player's copy, checks it is the build the
 * port was made from, and starts the port in it:
 *   1. the folder given on the command line;
 *   2. this launcher's own folder (the kit unpacked into the game folder);
 *   3. the folder chosen last time (%APPDATA%\colonization-port\gamedir.txt);
 *   4. the usual install places (GOG, the 1995 CD's default);
 *   5. otherwise it asks, with the folder picker, and remembers the answer.
 * A folder with no COLONIZE.EXE but Steam's COLONIZE.ISO (the DOS release's disc image, which
 * carries the Windows edition too), in it or in a folder one level down, is unpacked into a game
 * folder of the port's own, %LOCALAPPDATA%\colonization-port\game (isoimport.c), and that one
 * is used -- once: it is remembered like any other.
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

static int has_game(const char *dir)
{
    char p[MAX_PATH + 16];
    snprintf(p, sizeof p, "%s\\COLONIZE.EXE", dir);
    return dir[0] && GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES;
}

/* COLONIZE.ISO in dir, or in a folder one level down (Steam's install folder holds it) */
static int find_iso(const char *dir, char *iso)
{
    char pat[MAX_PATH + 16];
    WIN32_FIND_DATAA fd;
    HANDLE h;
    if (!dir[0])
        return 0;
    snprintf(iso, MAX_PATH, "%s\\COLONIZE.ISO", dir);
    if (GetFileAttributesA(iso) != INVALID_FILE_ATTRIBUTES)
        return 1;
    snprintf(pat, sizeof pat, "%s\\*", dir);
    if ((h = FindFirstFileA(pat, &fd)) == INVALID_HANDLE_VALUE)
        return 0;
    do
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName[0] != '.') {
            snprintf(iso, MAX_PATH, "%s\\%s\\COLONIZE.ISO", dir, fd.cFileName);
            if (GetFileAttributesA(iso) != INVALID_FILE_ATTRIBUTES) {
                FindClose(h);
                return 1;
            }
        }
    while (FindNextFileA(h, &fd));
    FindClose(h);
    return 0;
}

/* dir holds the game, or holds COLONIZE.ISO and is replaced by the folder it is unpacked into */
static int resolve(char *dir)
{
    char iso[MAX_PATH], dest[MAX_PATH], base[MAX_PATH], msg[1024], box[MAX_PATH + 1100];
    if (has_game(dir))
        return 1;
    if (!find_iso(dir, iso))
        return 0;
    if (!SUCCEEDED(SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, base)))
        return 0;
    snprintf(dest, sizeof dest, "%s\\colonization-port\\game", base);
    say("%s: no COLONIZE.EXE, but %s: unpacking it into %s", dir, iso, dest);
    if (iso_import(iso, dest, msg, sizeof msg)) {
        say("unpacking failed: %s", msg);
        snprintf(box, sizeof box, "%s could not be unpacked:\n\n%s", iso, msg);
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
    bi.lpszTitle = "Where is Colonization installed? (the folder with COLONIZE.EXE)";
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
    static const char *usual[] = {
        "C:\\GOG Games\\Colonization", "C:\\GOG Games\\Sid Meier's Colonization",
        "C:\\Program Files (x86)\\GOG Galaxy\\Games\\Colonization", "C:\\COLONIZE", "C:\\MPS\\COLONIZE",
        /* Steam's DOS release, whose COLONIZE.ISO is unpacked (resolve) */
        "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Sid Meier's Colonization",
        "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Colonization", 0 };
    char self[MAX_PATH], dir[MAX_PATH] = "", store[MAX_PATH], exe[MAX_PATH + 32], sha[65], msg[1024], *slash;
    int i, from_store = 0;
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
    if (cmd && *cmd) {                                   /* 1. the command line */
        snprintf(dir, sizeof dir, "%s", cmd[0] == '"' ? cmd + 1 : cmd);
        dir[strcspn(dir, "\"")] = 0;
    }
    if (dir[0])
        resolve(dir);
    if (!dir[0]) {                                       /* 2. this folder */
        snprintf(store, sizeof store, "%s", self);
        if (resolve(store))
            snprintf(dir, sizeof dir, "%s", store);
    }
    if (!dir[0]) {                                       /* 3. last time */
        remembered(store, 0, dir);
        from_store = dir[0] != 0;
    }
    for (i = 0; !dir[0] && usual[i]; i++) {              /* 4. the usual places */
        snprintf(store, sizeof store, "%s", usual[i]);
        if (resolve(store))
            snprintf(dir, sizeof dir, "%s", store);
    }
    say("game folder %s: %s", dir[0] ? dir : "(none found)", dir[0] && has_game(dir) ? "has COLONIZE.EXE" : "no COLONIZE.EXE");
    while (!resolve(dir)) {                              /* 5. ask */
        if (dir[0] && !from_store) {
            snprintf(msg, sizeof msg, "There is no COLONIZE.EXE (nor Steam's COLONIZE.ISO) in\n%s\n\nChoose "
                     "the folder Colonization for Windows is installed in, or Steam's Colonization folder.", dir);
            MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONINFORMATION);
        }
        from_store = 0;
        if (!pick_folder(dir)) {
            say("no folder chosen: stopped");
            return 1;
        }
        say("chosen folder %s", dir);
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
