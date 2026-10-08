/* Borland C++ 4.52 runtime routines the reconstructions call, on the host's C library.
 * flags: -P
 *
 * The corpus names the runtime by what the shipped code calls -- STRCPY for the large-model
 * _fstrcpy, sprintf_rtl, L1_346c for the routine at 1000:346c -- and the game's own source
 * never defines them: they came from CWL.LIB. This file is the port's (src/host, roadmap
 * W2), not imported. It compiles as C++ (the flags line) for operator delete, and every
 * definition has C linkage, as the header declares them.
 *
 * It does not include colonization.h: the header's `FILE` is the game's opaque record, and
 * here it is the host's. The prototypes below repeat the header's; C linkage matches the
 * symbols by name.
 *
 * rand/srand are Borland's generator exactly (seed * 0x015A4E35 + 1, bits 16..30), because
 * the game's random choices -- and the lockstep oracle (W3) -- depend on the sequence.
 */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <sys/stat.h>
#include <new>

extern "C" {

char *STRCPY(char *d, char *s) { return strcpy(d, s); }
char *STRCAT(char *d, char *s) { return strcat(d, s); }
unsigned STRLEN(const char *s) { return (unsigned)strlen(s); }
int STRCMP(const char *a, const char *b) { return strcmp(a, b); }
int STRNCMP(const char *a, const char *b, unsigned n) { return strncmp(a, b, n); }
char *STRNCPY(char *d, const char *s, unsigned n) { return strncpy(d, s, n); }
char *STRCHR(const char *s, int c) { return (char *)strchr(s, c); }
char *strrchr_rtl(const char *s, int c) { return (char *)strrchr(s, c); }
int STRICMP(char *a, char *b) { return _stricmp(a, b); }
int STRNICMP(const char *a, const char *b, unsigned n) { return _strnicmp(a, b, n); }
char *STRUPR(char *s) { for (char *p = s; *p; p++) *p = (char)toupper((unsigned char)*p); return s; }
char *STRLWR(char *s) { for (char *p = s; *p; p++) *p = (char)tolower((unsigned char)*p); return s; }
int toupper_rtl(int c) { return toupper(c); }
int tolower_rtl(int c) { return tolower(c); }

void *memset_rtl(void *p, int c, unsigned n) { return memset(p, c, n); }
void *memcpy_rtl(void *d, void *s, unsigned n) { return memcpy(d, s, n); }

/* 1000:0c72: not identified by signature (analysis/rtl-names.json); its one caller copies a
   file name less its extension, which is strncpy's contract. */
char *L1_0c72(char *dst, const char *src, unsigned n) { return strncpy(dst, src, n); }

static char *to_radix(long v, char *buf, int radix)
{
    char tmp[40];
    int i = 0, neg = 0;
    unsigned long u;
    if (radix == 10 && v < 0) {
        neg = 1;
        u = (unsigned long)(-v);
    } else
        u = (unsigned long)v;
    do {
        int d = (int)(u % (unsigned)radix);
        tmp[i++] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
        u /= (unsigned)radix;
    } while (u);
    char *p = buf;
    if (neg)
        *p++ = '-';
    while (i)
        *p++ = tmp[--i];
    *p = 0;
    return buf;
}

/* itoa: Borland's int is 16 bits, so the value is taken as one */
char *LTOA1(int v, char *buf, int radix) { return to_radix(radix == 10 ? (long)(short)v : (long)(unsigned short)v, buf, radix); }
void ltoa_rtl(long v, char *buf, int radix) { to_radix(v, buf, radix); }
long ATOL(void *p) { return atol((const char *)p); }
long LABS(long v) { return v < 0 ? -v : v; }

int sprintf_rtl(char *buf, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsprintf(buf, fmt, ap);
    va_end(ap);
    return n;
}

int printf_rtl(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vprintf(fmt, ap);
    va_end(ap);
    return n;
}

static unsigned long borland_seed = 1;
int rand_rtl(void)
{
    borland_seed = borland_seed * 0x015A4E35UL + 1;
    return (int)((borland_seed >> 16) & 0x7FFF);
}
void srand_rtl(unsigned seed) { borland_seed = (unsigned short)seed; }

long time_rtl(long *t)
{
    long now = (long)time(0);
    if (t)
        *t = now;
    return now;
}

FILE *fopen_rtl(char *name, char *mode) { return fopen(name, mode); }
int FCLOSE(FILE *f) { return fclose(f); }
unsigned fread_rtl(void *p, unsigned size, unsigned n, FILE *f) { return (unsigned)fread(p, size, n, f); }
unsigned fwrite_rtl(void *p, unsigned size, unsigned n, FILE *f) { return (unsigned)fwrite(p, size, n, f); }

/* 1000:346c is FLENGTH.CAS (analysis/rtl-names.json): filelength(handle) */
long L1_346c(int handle) { return _filelength(handle); }

/* Borland's struct stat, as MSFileSize.c declares it (the port lays it out its own way;
   both files see the same definition) */
struct stat_rtl {
    short st_dev, st_ino, st_mode, st_nlink, st_uid, st_gid, st_rdev;
    long st_size;
    long st_atime, st_mtime, st_ctime;
};
int fstat_rtl(short handle, struct stat_rtl *st)
{
    struct _stat s;
    if (_fstat(handle, &s) != 0)
        return -1;
    memset(st, 0, sizeof *st);
    st->st_mode = (short)s.st_mode;
    st->st_size = (long)s.st_size;
    st->st_atime = (long)s.st_atime;
    st->st_mtime = (long)s.st_mtime;
    st->st_ctime = (long)s.st_ctime;
    return 0;
}

void *mem_new(unsigned n) { return malloc(n); }
void mem_free(void *p) { free(p); }
void __bdele_qnv_rtl(void *p) { ::operator delete(p); }

void assert_fail(char *cond, char *file, char *msg, int line)
{
    const char *log = getenv("PORT_LOG");
    FILE *f = log ? fopen(log, "a") : 0;
    if (f) {
        fprintf(f, "game: assertion failed: %s, %s (%s) line %d\n", cond, msg ? msg : "", file, line);
        fclose(f);
    }
    fprintf(stderr, "Assertion failed: %s, %s (%s) line %d\n", cond, msg ? msg : "", file, line);
    abort();
}

/* The corpus's name for the error-log writer it never reconstructed: the message goes to
   stderr until the log window exists (W4). */
void stub_reporter(void *ctx, char *msg)
{
    const char *log = getenv("PORT_LOG");
    FILE *f = log ? fopen(log, "a") : 0;
    (void)ctx;
    fprintf(stderr, "%s\n", msg);
    if (f) {
        fprintf(f, "game: %s\n", msg);
        fclose(f);
    }
}

}  /* extern "C" */
