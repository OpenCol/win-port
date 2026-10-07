/* MSFileSize: candidate for 1070:829f (106 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * The size of an open MSFile through fstat: 0 when the file has no handle, 0 with an
 * error report when fstat fails, else st_size. The 30-byte struct stat is Borland's.
 */

#include "colonization.h"

struct stat_rtl {
    short st_dev, st_ino, st_mode, st_nlink, st_uid, st_gid, st_rdev;
    long st_size;
    long st_atime, st_mtime, st_ctime;
};
#include "records.h"
extern int fstat_rtl(short handle, struct stat_rtl *st);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_969a(void *ctx, char *msg);

long MSFileSize(MSFile *file)
{
    struct stat_rtl st;

    if (file->handle == 0)
        return 0;
    if (fstat_rtl(file->handle, &st) != 0) {
        report_error_1070_969a(&g_error_log, "Bad file handle in MSFileSize");
        return 0;
    }
    return st.st_size;
}
