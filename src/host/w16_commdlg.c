/* COMMDLG: the open and save file dialogs, Win16 on Win32 (roadmap W4). The game's OPENFILENAME
   is its own structure in the port's layout (the field order of Win32's, handles as tokens);
   it is copied into Win32's, the dialog run, and the results copied back.

   Since Vista the dialog is the shell's, a COM object that wants a single-threaded apartment.
   The game's thread has none of its own; should something in the process (a driver, a shell
   extension) have made it multithreaded, the dialog runs on a thread of its own, the game's
   windows painted meanwhile. Each dialog is a line in $PORT_LOG: a player's report of one that
   would not close (0.1.7, 32-bit only) said no more than that. */
#include <stdio.h>
#include <string.h>
#include "w16.h"
#include <commdlg.h>
#include <cderr.h>
#include <objbase.h>
#include <tlhelp32.h>

typedef struct {
    unsigned long lStructSize; unsigned hwndOwner; unsigned hInstance;
    const char *lpstrFilter; char *lpstrCustomFilter; unsigned long nMaxCustFilter;
    unsigned long nFilterIndex; char *lpstrFile; unsigned long nMaxFile;
    char *lpstrFileTitle; unsigned long nMaxFileTitle; const char *lpstrInitialDir;
    const char *lpstrTitle; unsigned long Flags; unsigned nFileOffset; unsigned nFileExtension;
    const char *lpstrDefExt; unsigned long lCustData; void *lpfnHook; const char *lpTemplateName;
} OPENFILENAME_G;

int w16_dialog_open;                    /* 1 + the messages the game took meanwhile (w16_user.c) */
static char cwd[MAX_PATH];
static char filter_buf[512];            /* the game's filter, as Win16's dialog read it */
static const char *style;               /* the dialog's kind, for the log */

/* Win16's dialog put a pattern without wildcards -- the game's "eng1492.sav", its suggested
   name -- in the file name box when the game gave none, and listed what the pattern's
   extension matched. Win32's shell dialog takes it as a file type of its own; here it is
   what it was: the name in the box, and the type "*.sav". */
static const char *win16_filter(const char *f, unsigned long index, char *file, unsigned long nfile,
                                const char *dir)
{
    size_t k = 0;
    unsigned long i;
    for (i = 1; f && *f; i++) {
        const char *pat = f + strlen(f) + 1, *dot;
        size_t nd = strlen(f) + 1, np = strlen(pat) + 1;
        char star[64];
        if (!*pat || k + nd + np + 4 >= sizeof filter_buf)
            return f == filter_buf ? f : 0;
        memcpy(filter_buf + k, f, nd);
        k += nd;
        if (!strpbrk(pat, "*?;") && np < sizeof star - 2) {
            /* the name with the folder the dialog opens in: Win32 hands back a bare name as given */
            if (i == index && file && !file[0] && nfile > np + (dir ? strlen(dir) + 1 : 0))
                _snprintf(file, nfile - 1, "%s%s%s", dir ? dir : "",
                          dir && *dir && dir[strlen(dir) - 1] != '\\' ? "\\" : "", pat);
            dot = strrchr(pat, '.');
            _snprintf(star, sizeof star - 1, "*%s", dot ? dot : ".*");
            star[sizeof star - 1] = 0;
            pat = star;
            np = strlen(star) + 1;
        }
        memcpy(filter_buf + k, pat, np);
        k += np;
        f += nd + strlen(f + nd) + 1;
    }
    filter_buf[k] = 0;
    return filter_buf;
}

/* Windows 7 and 8 (NT 6.x): the dialog of Windows XP, which a hook asks for, not Vista's shell
   dialog -- a tester's Windows 7 Save button did nothing in it (v0.1.9) while Windows 11's
   worked. $PORT_XP_DIALOG=1 or 0 says otherwise. GetVersionEx answers 6.2 to a program without
   a manifest on every later Windows: ntdll's RtlGetVersion does not. */
static UINT_PTR CALLBACK xp_hook(HWND h, UINT m, WPARAM w, LPARAM l)
{
    (void)h; (void)m; (void)w; (void)l;
    return 0;
}

static int xp_dialog(void)
{
    typedef LONG (WINAPI *rtl_version)(OSVERSIONINFOW *);
    const char *e = getenv("PORT_XP_DIALOG");
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    rtl_version rv = ntdll ? (rtl_version)(void (*)(void))GetProcAddress(ntdll, "RtlGetVersion") : 0;
    OSVERSIONINFOW v;
    if (e && *e)
        return *e == '1';
    memset(&v, 0, sizeof v);
    v.dwOSVersionInfoSize = sizeof v;
    return rv && rv(&v) == 0 && v.dwMajorVersion == 6;
}
static DWORD last_error;                /* CommDlgExtendedError's, of whichever thread ran it */

static void in(OPENFILENAMEA *o, const OPENFILENAME_G *g)
{
    memset(o, 0, sizeof *o);
    o->lStructSize = sizeof *o;
    o->hwndOwner = W16_HWND(g->hwndOwner);
    o->lpstrCustomFilter = g->lpstrCustomFilter;
    o->nMaxCustFilter = g->nMaxCustFilter;
    o->nFilterIndex = g->nFilterIndex;
    o->lpstrFile = g->lpstrFile;
    o->nMaxFile = g->nMaxFile;
    o->lpstrFileTitle = g->lpstrFileTitle;
    o->nMaxFileTitle = g->nMaxFileTitle;
    o->lpstrInitialDir = g->lpstrInitialDir;
    /* none given: the current folder, where Win16's dialog opened and the game's saves are;
       Windows' own choice is the folder used last or the user's documents */
    if (!o->lpstrInitialDir && GetCurrentDirectoryA(sizeof cwd, cwd) - 1 < sizeof cwd - 1)
        o->lpstrInitialDir = cwd;
    o->lpstrFilter = win16_filter(g->lpstrFilter, g->nFilterIndex, g->lpstrFile, g->nMaxFile, o->lpstrInitialDir);
    if (!o->lpstrFilter)
        o->lpstrFilter = g->lpstrFilter;
    o->lpstrTitle = g->lpstrTitle;
    /* no hook or template from the game's module: Win32 cannot use them */
    o->Flags = g->Flags & ~(OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_ENABLETEMPLATEHANDLE);
    o->lpstrDefExt = g->lpstrDefExt;
    style = "Vista's";
    if (xp_dialog()) {
        o->Flags |= OFN_EXPLORER | OFN_ENABLEHOOK;
        o->lpfnHook = xp_hook;
        style = "XP's";
    }
}

static void out(OPENFILENAME_G *g, const OPENFILENAMEA *o)
{
    g->nFilterIndex = o->nFilterIndex;
    g->nFileOffset = o->nFileOffset;
    g->nFileExtension = o->nFileExtension;
    g->Flags = (g->Flags & (OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_ENABLETEMPLATEHANDLE | OFN_EXPLORER)) |
               (o->Flags & ~(OFN_ENABLEHOOK | OFN_EXPLORER));
}

typedef struct { OPENFILENAMEA *o; int save; BOOL r; DWORD err; } RUN;

static DWORD WINAPI dialog_thread(void *p)
{
    RUN *x = p;
    HRESULT h = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    x->r = x->save ? GetSaveFileNameA(x->o) : GetOpenFileNameA(x->o);
    x->err = x->r ? 0 : CommDlgExtendedError();
    if (SUCCEEDED(h))
        CoUninitialize();
    return 0;
}

/* the filter's pairs as one line: "Saved Game|eng1492.sav|..." */
static void pairs(char *b, size_t n, const char *f)
{
    size_t k = 0;
    b[0] = 0;
    for (; f && *f; f += strlen(f) + 1) {
        _snprintf(b + k, n - k - 1, "%s%s", k ? "|" : "", f);
        b[n - 1] = 0;
        k = strlen(b);
        if (k + 2 >= n)
            break;
    }
}

/* the process's room: free address space, its largest block, and its threads -- a dialog that
   cannot make its threads does nothing (w16_entry.c) */
static void room(char *b, size_t n)
{
    MEMORY_BASIC_INFORMATION mb;
    char *p = 0;
    unsigned long long total = 0, big = 0;
    int threads = 0;
    DWORD pid = GetCurrentProcessId();
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    while (VirtualQuery(p, &mb, sizeof mb) && (char *)mb.BaseAddress + mb.RegionSize > p) {
        if (mb.State == MEM_FREE) {
            total += mb.RegionSize;
            if (mb.RegionSize > big)
                big = mb.RegionSize;
        }
        p = (char *)mb.BaseAddress + mb.RegionSize;
    }
    if (snap != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te;
        te.dwSize = sizeof te;
        if (Thread32First(snap, &te))
            do
                threads += te.th32OwnerProcessID == pid;
            while (Thread32Next(snap, &te));
        CloseHandle(snap);
    }
    _snprintf(b, n - 1, "%llu MB free, largest %llu MB, %d threads", total >> 20, big >> 20, threads);
    b[n - 1] = 0;
}

static char run(OPENFILENAME_G *g, int save)
{
    OPENFILENAMEA o;
    RUN x;
    char filter[160], before[80], after[80];
    HRESULT h = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    in(&o, g);
    x.o = &o;
    x.save = save;
    w16_dialog_open = 1;
    room(before, sizeof before);
    if (h == RPC_E_CHANGED_MODE) {      /* a multithreaded apartment: a thread of the dialog's own */
        HANDLE t = CreateThread(0, 0, dialog_thread, &x, 0, 0);
        int quit = 0;
        WPARAM code = 0;
        if (!t) {
            x.r = 0;
            x.err = CDERR_INITIALIZATION;
        } else {
            while (MsgWaitForMultipleObjects(1, &t, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0 + 1) {
                MSG m;
                while (PeekMessageA(&m, 0, 0, 0, PM_REMOVE)) {
                    if (m.message == WM_QUIT) {
                        quit = 1;
                        code = m.wParam;
                        continue;
                    }
                    TranslateMessage(&m);
                    DispatchMessageA(&m);
                }
            }
            CloseHandle(t);
            if (quit)
                PostQuitMessage((int)code);
        }
    } else {
        x.r = save ? GetSaveFileNameA(&o) : GetOpenFileNameA(&o);
        x.err = x.r ? 0 : CommDlgExtendedError();
        if (SUCCEEDED(h))
            CoUninitialize();
    }
    last_error = x.err;
    room(after, sizeof after);
    if (w16_dialog_open > 1)
        w16_report("  the game took %d messages from the queue during the dialog", w16_dialog_open - 1);
    w16_dialog_open = 0;
    pairs(filter, sizeof filter, o.lpstrFilter);
    w16_report("%s dialog (%s): filter \"%s\" index %lu, folder \"%s\", flags 0x%lx, apartment %s: %s \"%s\"",
               save ? "save" : "open", style, filter, (unsigned long)g->nFilterIndex,
               o.lpstrInitialDir ? o.lpstrInitialDir : "", (unsigned long)o.Flags,
               h == RPC_E_CHANGED_MODE ? "multithreaded (dialog on its own thread)" : h == S_FALSE ? "single" : "none",
               x.r ? "chose" : "closed", x.r ? o.lpstrFile : "");
    w16_report("  before: %s; after: %s", before, after);
    if (x.err)
        w16_report("  CommDlgExtendedError 0x%lx", (unsigned long)x.err);
    out(g, &o);
    return (char)x.r;
}

char w16_GetOpenFileName(OPENFILENAME_G *g) W16(GetOpenFileName);
char w16_GetOpenFileName(OPENFILENAME_G *g)
{
    if (w16_lockstep_save()) {          /* the scenario's saved game, no dialog (W6) */
        snprintf(g->lpstrFile, g->nMaxFile, "%s", w16_lockstep_save());
        return 1;
    }
    return run(g, 0);
}

char w16_GetSaveFileName(OPENFILENAME_G *g) W16(GetSaveFileName);
char w16_GetSaveFileName(OPENFILENAME_G *g)
{
    if (w16_lockstep_saveas()) {        /* a session's save, no dialog (W7) */
        snprintf(g->lpstrFile, g->nMaxFile, "%s", w16_lockstep_saveas());
        return 1;
    }
    return run(g, 1);
}

unsigned long w16_CommDlgExtendedError(void) W16(CommDlgExtendedError);
unsigned long w16_CommDlgExtendedError(void) { return last_error; }
