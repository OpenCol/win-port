/* file_dialog: candidate for 1070:8399 (241 bytes).  Verdict: RELOCATABLE.
 *
 * The common open/save dialog: OPENFILENAME zeroed and filled (filter, the caller's path
 * buffer of 256, title "Select a File", default extension, flags 0x80e plus OFN_FILEMUSTEXIST
 * (0x1000) when `must_exist` is set -- the open wrappers' flag -- ), GetSaveFileName or GetOpenFileName. A result other than 1
 * reports any CommDlgExtendedError through report_error_1070_a08c and returns 0.
 */

#include "colonization.h"

typedef struct {
    unsigned long lStructSize; unsigned hwndOwner; unsigned hInstance;
    const char *lpstrFilter; char *lpstrCustomFilter; unsigned long nMaxCustFilter;
    unsigned long nFilterIndex; char *lpstrFile; unsigned long nMaxFile;
    char *lpstrFileTitle; unsigned long nMaxFileTitle; const char *lpstrInitialDir;
    const char *lpstrTitle; unsigned long Flags; unsigned nFileOffset; unsigned nFileExtension;
    const char *lpstrDefExt; unsigned long lCustData; void *lpfnHook; const char *lpTemplateName;
} OPENFILENAME;   /* 0x48 bytes */

extern char GetOpenFileName(OPENFILENAME *ofn);
extern char GetSaveFileName(OPENFILENAME *ofn);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_a08c(void *ctx, char *msg, long code);

int file_dialog(char *path, const char *filter, const char *ext, char must_exist, char save)
{
    long err;
    char ok;
    char *p = path;
    OPENFILENAME ofn;

    memset_rtl(&ofn, 0, 0x48);
    ofn.lStructSize = 0x48;
    ofn.hwndOwner = 0;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = p;
    ofn.nMaxFile = 0x100;
    ofn.lpstrTitle = "Select a File";
    ofn.lpstrDefExt = ext;
    if (must_exist)
        ofn.Flags = 0x180e;
    else
        ofn.Flags = 0x80e;
    if (save)
        ok = GetSaveFileName(&ofn);
    else
        ok = GetOpenFileName(&ofn);
    if ((int)ok != 1) {
        err = CommDlgExtendedError();
        if (err != 0)
            report_error_1070_a08c(&g_error_log, "Error: GetOpenFileName returned ", (long)(int)err);
        return 0;
    }
    return 1;
}
