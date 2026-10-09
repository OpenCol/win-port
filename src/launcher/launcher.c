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
 * COLONIZE.EXE's SHA-256 must be the patched 1995 release's. The original 1995 release (before
 * the patch) is recognised and named; the kit does not carry the patch (BLOCKED.md, W10).
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>

#define PATCHED  "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
#define PREPATCH "ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650"
#define TITLE "Colonization"

static int has_game(const char *dir)
{
    char p[MAX_PATH + 16];
    snprintf(p, sizeof p, "%s\\COLONIZE.EXE", dir);
    return GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES;
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
        "C:\\Program Files (x86)\\GOG Galaxy\\Games\\Colonization", "C:\\COLONIZE", "C:\\MPS\\COLONIZE", 0 };
    char self[MAX_PATH], dir[MAX_PATH] = "", store[MAX_PATH], exe[MAX_PATH + 32], sha[65], msg[1024], *slash;
    int i, from_store = 0;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    (void)inst; (void)prev; (void)show;

    GetModuleFileNameA(0, self, sizeof self);
    if ((slash = strrchr(self, '\\')))
        *slash = 0;
    if (cmd && *cmd) {                                   /* 1. the command line */
        snprintf(dir, sizeof dir, "%s", cmd[0] == '"' ? cmd + 1 : cmd);
        dir[strcspn(dir, "\"")] = 0;
    }
    if (!dir[0] && has_game(self))                       /* 2. this folder */
        snprintf(dir, sizeof dir, "%s", self);
    if (!dir[0]) {                                       /* 3. last time */
        remembered(store, 0, dir);
        from_store = dir[0] != 0;
    }
    for (i = 0; !dir[0] && usual[i]; i++)                /* 4. the usual places */
        if (has_game(usual[i]))
            snprintf(dir, sizeof dir, "%s", usual[i]);
    while (!has_game(dir)) {                             /* 5. ask */
        if (dir[0] && !from_store) {
            snprintf(msg, sizeof msg, "There is no COLONIZE.EXE in\n%s\n\nChoose the folder Colonization "
                     "for Windows is installed in.", dir);
            MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONINFORMATION);
        }
        from_store = 0;
        if (!pick_folder(dir))
            return 1;
    }
    snprintf(exe, sizeof exe, "%s\\COLONIZE.EXE", dir);
    if (!sha256_file(exe, sha)) {
        MessageBoxA(0, "COLONIZE.EXE could not be read.", TITLE, MB_OK | MB_ICONERROR);
        return 2;
    }
    if (!strcmp(sha, PREPATCH)) {
        snprintf(msg, sizeof msg, "%s\\COLONIZE.EXE is the original 1995 release.\n\nThis port is built from "
                 "the updated 1995 release (the official patch, which also fixed lock-ups and Windows 95 "
                 "support). Apply that patch to your copy, then start again.", dir);
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONEXCLAMATION);
        return 3;
    }
    if (strcmp(sha, PATCHED)) {
        snprintf(msg, sizeof msg, "%s\\COLONIZE.EXE is not a build this port knows.\n\nSHA-256 %s\n\nThe port "
                 "needs the updated 1995 release of Colonization for Windows (SHA-256 %.16s...). A patched or "
                 "modified COLONIZE.EXE will not do: the port reads the game's data from it.", dir, sha, PATCHED);
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONEXCLAMATION);
        return 4;
    }
    remembered(store, 1, dir);
    snprintf(exe, sizeof exe, "\"%s\\bin\\colonize.exe\"", self);
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    if (!CreateProcessA(0, exe, 0, 0, FALSE, 0, 0, dir, &si, &pi)) {
        snprintf(msg, sizeof msg, "Could not start %s (error %lu).", exe, GetLastError());
        MessageBoxA(0, msg, TITLE, MB_OK | MB_ICONERROR);
        return 5;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
