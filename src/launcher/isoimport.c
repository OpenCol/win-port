/* Steam's COLONIZE.ISO into a game folder (roadmap W10): the launchers' import.
 *
 * Steam and GOG sell Colonization's DOS edition, and its disc image carries the Windows edition
 * too -- the original 1995 release, before the patch -- in its INSTALL folder: Steam's
 * COLONIZE.ISO (2048-byte sectors), GOG's COLONIZE.GOG (raw 2352-byte Mode 1 sectors, where the
 * compressed program is named COLONIZE.$00, not ._00; the same bytes). This
 * reads that folder straight out of the ISO 9660 image and writes a game folder the port can run
 * in: the 54 data files as they are, each checked by SHA-256 (they are the patched install's,
 * byte for byte), and COLONIZE.EXE decompressed from COLONIZE._00 and checked too.
 *
 * COLONIZE._00 is an ARCV archive (Eschalon's InstallWrap): a 59-byte header, a 16-byte CHNK
 * header, then LZHUF -- adaptive Huffman over a 4096-byte LZSS ring. The decoder is
 * col-win-re's tools/arcv_extract.py, which read every step out of the disc's SETUP.EXE; its
 * position tables are Okumura's (LZHUF.C, 1988), built here from their definition, so SETUP.EXE
 * is not needed. The ring starts at 0xdc3, not LZHUF.C's N - F: the installer's own choice.
 *
 * Of the patched release's files, the disc lacks COLDATA9.DLL, which the 1995 patch added: a
 * folder made from the ISO plays without the Jukebox's Control Panel and the Cheat menu's dialogs
 * (README.md). COCKGUN.WAV, the patch's other new file, is the disc's GUNCOCK.WAV renamed (the
 * same bytes; the patched program plays cockgun.wav, the original guncock.wav), so it is written
 * as a copy: without it Windows would play its default sound in its place (sndPlaySound flag 1,
 * no SND_NODEFAULT). Nothing here is game data: only names and hashes.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isoimport.h"

#define SECTOR 2048
#define PREPATCH "ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650"

/* the data files of the disc's INSTALL folder, as the patched install has them */
static const struct { const char *name, *sha256; } files[] = {
    { "1812.WAV", "429e5bc3b496ae811a36818506918ec226ab60b5aa2dfb1ead32961d2dd969fb" },
    { "AMEN.WAV", "68e35fec3ea1c9b07046bee245be731c3ab41d29a19924d09f5aa6d3885a83dc" },
    { "AMER2.MP", "ea928f06e70564be997aa56575096c86cd7a985d41e584ba818c5c853b399dcc" },
    { "ARROW.WAV", "222ccc075983a56fec11164b6bbf65404e0183c3fca7f1c6c2a96913508d214e" },
    { "BIGEXPLO.WAV", "2cd46fe8278b3c8c79654c2a0bbe9ddbb84d07c5c615a4c399c014f78f710950" },
    { "BIGGUN.WAV", "2b6ac5c2d7b02f1cac27c9824c33196bc45aeafd39a4bc958d31b97f47c26230" },
    { "BLTDIRT.WAV", "32bbc3ec833958d40ec6c93995281437b890399cbcc3dc60f45cb778acf8eb5c" },
    { "BLTH20.WAV", "b8145862b039a596bf33ad2acd795128f0c3b23998da19e9d20b2b5727165e57" },
    { "BOARD.WAV", "90a7cb8aa98dd70937a561764e8d7d75148050468b9adb7fefa92b61c8813fcc" },
    { "BOOM1.WAV", "75b4008c4292ff980b95525301e17f71820cae1e266b129c220d8a9c364790ad" },
    { "BOOM2.WAV", "837935847750d30149ed881ababd0335e2f797977e567239bb0ed3cfa69124cb" },
    { "BOOM3.WAV", "5028667531e4fd83ade2fb7356207cb0f3b264851b47dc727b49d25fcaf01223" },
    { "BOOM4.WAV", "54c8a5f8b5f935914d3b299ce9845c2d9ca31ca2d5b19f290b6d5883712c4e3c" },
    { "CANNON0.WAV", "250b27fff1efdf78f93d474225714556b47ce49b814ea0347d40c3b0e42cdb27" },
    { "CANNON1.WAV", "6d06b016913c4e81349bc46c462ff2672e84fcf20bf70cde2bb22bee2a1d8f0f" },
    { "COLDATA0.DLL", "863b5f267045016f81ca25443e1a8760e10b05318647b639bfd737e58c252f68" },
    { "COLDATA1.DLL", "f998e59c804dd59ade6fd44876d925a0a97be05a1f0bec98d406a8ecd8498444" },
    { "COLDATA2.DLL", "758205638e60e59e57d1bf0d375c76055ae455d07b7a6c5f3a2de2b5617c0289" },
    { "COLDATA3.DLL", "ccdb3255293fb4a6630636aed52be49694acf91bfb707330b8bf6555a5cef7e7" },
    { "COLDATA4.DLL", "50d1b6053d95bd872f3be54b059b14f50d0ca40ac49289054a7253c908134e9f" },
    { "COLDATA5.DLL", "af695e3b113f57d8206057194f13e524125c90446be1ceb239cd80f7f5b8ef98" },
    { "COLDATA6.DLL", "d17698b4db1324c792448775b0d31d4d2ef6c1c2f38ea73a97a890e41e688957" },
    { "COLDATA7.DLL", "b80948fe01a8c3f34047db36b1f8e506527a5aa2140848e693c61849c4d713f4" },
    { "COLDATA8.DLL", "bd35533cefad152db27c81612390356c281412b46a944a09f804d0703fe4b207" },
    { "COLTEXT0.DLL", "b085a960d0557809d959aab5dc93a140957eda42bf553c04d5f1a6d9ac90068a" },
    { "CRACKER1.WAV", "fb50b733f9cb23ef463cf14f19835f260060bd110c95b5cfec68241e4e2c4915" },
    { "CRACKER2.WAV", "3c3ff0883caf4b93adba4df873a59c8a47afa4791b682208178198693587f69a" },
    { "CRACKER3.WAV", "b2cb5451eb922239f5966e988f8f33bc65de046ccaa8d1ba57af654bdb854635" },
    { "CRACKER4.WAV", "8b1277c5b0e5f7e708922e02eb673f45653c4b44924ec410d951957423e47c32" },
    { "DRAGOONS.WAV", "410c2e2fbc7c35ffb455aa6e577ebafe31375794426dbea2623f7cc8a588e07f" },
    { "DUTCH.WAV", "c9fde41e3bddad05e9f5338a6b3176fcc7d11a2a76622906852e257cabd19371" },
    { "ENGLAND.WAV", "1f16eb07021668a3b4abc086026bd14e2cdb759d89c6be49e3d92cd44f847774" },
    { "EXPLO1.WAV", "2521e94987da74d164e619a47c8bb66e167b665a41fcee6472f95833d3c73c60" },
    { "FIRE1.WAV", "b79dd6ab657f5960edc51be2b35bd66a3a2e3e263365bfc72ce888b32ec762e7" },
    { "FRANCE.WAV", "4be717bb68792ce5ba63463eaf9cbfd7ae18c2885704f91d83ce94626b5a6f91" },
    { "GOLDCITY.WAV", "f8664c99b7eb34aec9a87180a9a720ed47a16571e9888a6380c532cf6dce9f26" },
    { "GUNCOCK.WAV", "0f78acd2869fd0450472ed33767969d30892992378b791dd453bdf5719f42055" },
    { "GUNS.WAV", "0a8f9855a500d873c44144c6e151160db1c6fd66b642763763b2838f263cd630" },
    { "GUNWHOOP.WAV", "91fc3decf0eb064973037c09c404047ff7606808508640a3ec3bf2c3e390e5b3" },
    { "HAMMERS.WAV", "e5613968106caae1ba5523733cefb701431963e99ee79d1644a9ad900b741712" },
    { "HORGNWHP.WAV", "29a8cb50f0da031a52a6fef6086909414ea7e6c9ac1143bb60e7607c30c7f948" },
    { "HORSES.WAV", "e7b0a5456f0dc007e7ceb1e316a461e4bd8f284673cb84cbc4745f9c0030f3c3" },
    { "LIBERTY.WAV", "ec79f803026dd4335cacdf915eb8470dc9d160cabd1d1dd03273b5463bee35c2" },
    { "LILEXPLO.WAV", "b0c68e0c8b1c7e7fc1ac6256800c5c328044d4bf5e723ce119005b04e750ffc2" },
    { "LILGUN.WAV", "0b4469284cec129a1bdaf44c829c605d716e6356f8298a680b879da23ca5aeb4" },
    { "MEETKING.WAV", "028b9620bde5b34b38ffe73fd99ff028b7bb5b4a07a19ea7cdfdb3a803409946" },
    { "MUSPARTY.WAV", "33b219e2da4067e6ec7a8b829f6cc8e80eb4bd5903117c045639749fecb78743" },
    { "POMP.WAV", "ce82fd798d80a7f1407c5e588a2520d111e9dad590e7123b5fc882f398fde4c5" },
    { "SHIPSINK.WAV", "0556e9aba51852933fe38560e4576a1d0a134bfb50799c7e63785743889ab3a1" },
    { "SPAIN.WAV", "7bdb931c583ca4219d1f8ee5714a24dd41bc34adf83e6a16aaf2ad1fcc65c3a6" },
    { "WGNSTOP.WAV", "c4557eefccaa756abc5119d6aebee30b832d02918c0722c37f8c93bf82f12b0f" },
    { "WHOOP.WAV", "0c20209d099172032b1c9b998c0cfe9eee70c9a22d6c6f2b40202017e42ec231" },
    { "WHPHORSE.WAV", "c27a26a8b3ac7dc83af60d8076d0349a7e485892994419ab18649bc594627917" },
    { "WINFLC.WAV", "f00f1f905886d1422f227953437f4ebb60c99ebdeeddac4fb4429fd0965e06ce" },
};
#define NFILES (sizeof files / sizeof files[0])

static unsigned long le32(const unsigned char *p) { return p[0] | p[1] << 8 | (unsigned long)p[2] << 16 | (unsigned long)p[3] << 24; }
static unsigned le16(const unsigned char *p) { return p[0] | p[1] << 8; }

static int sha256_buf(const unsigned char *data, unsigned long n, char out[65])
{
    HCRYPTPROV prov;
    HCRYPTHASH hash;
    BYTE digest[32];
    DWORD len = sizeof digest, i;
    int ok = 0;
    if (!CryptAcquireContextA(&prov, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        return 0;
    if (CryptCreateHash(prov, CALG_SHA_256, 0, 0, &hash)) {
        if (CryptHashData(hash, data, n, 0) && CryptGetHashParam(hash, HP_HASHVAL, digest, &len, 0)) {
            for (i = 0; i < 32; i++)
                sprintf(out + 2 * i, "%02x", digest[i]);
            out[64] = 0;
            ok = 1;
        }
        CryptDestroyHash(hash);
    }
    CryptReleaseContext(prov, 0);
    return ok;
}

/* ------------------------------------------------------------------ ISO 9660 */

typedef struct { unsigned long lba, size; int dir; char name[40]; } Entry;

/* the image's sectors: 2048 bytes of data each (an .ISO), or raw CD sectors of 2352 bytes with the
   data at 16 (Mode 1) or 24 (Mode 2 Form 1) -- GOG's COLONIZE.GOG is Mode 1 */
static unsigned long raw_size = SECTOR, raw_data = 0;

static int detect_sectors(FILE *f)
{
    static const struct { unsigned long size, data; } kinds[] = { { 2048, 0 }, { 2352, 16 }, { 2352, 24 } };
    unsigned char id[6];
    unsigned i;
    for (i = 0; i < 3; i++)
        if (!fseek(f, (long)(16 * kinds[i].size + kinds[i].data), SEEK_SET) && fread(id, 1, 6, f) == 6 &&
            id[0] == 1 && !memcmp(id + 1, "CD001", 5)) {
            raw_size = kinds[i].size;
            raw_data = kinds[i].data;
            return 1;
        }
    return 0;
}

static unsigned char *iso_read(FILE *f, unsigned long lba, unsigned long size)
{
    unsigned char *b = malloc(size ? size : 1);
    unsigned long done = 0;
    if (!b)
        return 0;
    if (raw_size != SECTOR) {                    /* raw sectors: the data part of each */
        while (done < size) {
            unsigned long n = size - done < SECTOR ? size - done : SECTOR;
            if (fseek(f, (long)((lba + done / SECTOR) * raw_size + raw_data), SEEK_SET) || fread(b + done, 1, n, f) != n) {
                free(b);
                return 0;
            }
            done += n;
        }
        return b;
    }
    if (fseek(f, (long)(lba * SECTOR), SEEK_SET) || fread(b, 1, size, f) != size) {
        free(b);
        return 0;
    }
    return b;
}

/* a directory's entries: name without ";1" and without a trailing dot */
static int iso_list(FILE *f, unsigned long lba, unsigned long size, Entry *out, int max)
{
    unsigned char *b = iso_read(f, lba, size);
    unsigned long p = 0;
    int n = 0;
    if (!b)
        return -1;
    while (p < size && n < max) {
        unsigned len = b[p], nl;
        char *semi;
        if (len == 0) {                      /* records do not cross a sector: next one */
            p = (p / SECTOR + 1) * SECTOR;
            continue;
        }
        if (p + 33 > size || p + len > size)
            break;
        nl = b[p + 32];
        if (!(nl == 1 && (b[p + 33] == 0 || b[p + 33] == 1)) && nl < sizeof out[n].name) {   /* not . or .. */
            out[n].lba = le32(b + p + 2);
            out[n].size = le32(b + p + 10);
            out[n].dir = (b[p + 25] & 2) != 0;
            memcpy(out[n].name, b + p + 33, nl);
            out[n].name[nl] = 0;
            if ((semi = strchr(out[n].name, ';')) != 0)
                *semi = 0;
            if (out[n].name[0] && out[n].name[strlen(out[n].name) - 1] == '.')
                out[n].name[strlen(out[n].name) - 1] = 0;
            n++;
        }
        p += len;
    }
    free(b);
    return n;
}

static const Entry *find(const Entry *e, int n, const char *name)
{
    int i;
    for (i = 0; i < n; i++)
        if (!_stricmp(e[i].name, name))
            return &e[i];
    return 0;
}

/* ------------------------------------------------------------------ ARCV / LZHUF */

#define N 4096
#define N_CHAR 287                 /* 256 literals, the end, 30 lengths */
#define T (N_CHAR * 2 - 1)         /* 573 */
#define R (T - 1)
#define MAX_FREQ 0x8000

static unsigned freq[T + 1], son[T], prnt[T + N_CHAR];
static unsigned char d_code[256], d_len[256];

static void tables(void)
{
    /* Okumura's d_code: 32 x 0, 16 x 1..3, 8 x 4..11, 4 x 12..23, 2 x 24..47, 1 x 48..63;
       d_len: 32 x 3, 48 x 4, 64 x 5, 48 x 6, 48 x 7, 16 x 8 -- equal to the disc SETUP.EXE's */
    static const struct { int first, last, run; } code[] = {
        { 0, 0, 32 }, { 1, 3, 16 }, { 4, 11, 8 }, { 12, 23, 4 }, { 24, 47, 2 }, { 48, 63, 1 } };
    static const struct { int len, count; } lens[] = { { 3, 32 }, { 4, 48 }, { 5, 64 }, { 6, 48 }, { 7, 48 }, { 8, 16 } };
    int i, c, k, p = 0;
    for (i = 0; i < 6; i++)
        for (c = code[i].first; c <= code[i].last; c++)
            for (k = 0; k < code[i].run; k++)
                d_code[p++] = (unsigned char)c;
    for (i = p = 0; i < 6; i++)
        for (k = 0; k < lens[i].count; k++)
            d_len[p++] = (unsigned char)lens[i].len;
}

static void start_huff(void)
{
    int i, j;
    for (i = 0; i < N_CHAR; i++) {
        freq[i] = 1;
        son[i] = i + T;
        prnt[i + T] = i;
    }
    for (i = 0, j = N_CHAR; j <= R; i += 2, j++) {
        freq[j] = freq[i] + freq[i + 1];
        son[j] = i;
        prnt[i] = prnt[i + 1] = j;
    }
    freq[T] = 0xffff;
    prnt[R] = 0;
}

static void reconst(void)
{
    int i, j, k;
    unsigned f;
    for (i = j = 0; i < T; i++)
        if (son[i] >= T) {
            freq[j] = (freq[i] + 1) >> 1;
            son[j] = son[i];
            j++;
        }
    for (i = 0, j = N_CHAR; j < T; i += 2, j++) {
        f = freq[j] = freq[i] + freq[i + 1];
        for (k = j - 1; f < freq[k]; k--)
            ;
        k++;
        memmove(&freq[k + 1], &freq[k], (j - k) * sizeof freq[0]);
        freq[k] = f;
        memmove(&son[k + 1], &son[k], (j - k) * sizeof son[0]);
        son[k] = i;
    }
    for (i = 0; i < T; i++) {
        k = son[i];
        prnt[k] = i;
        if (k < T)
            prnt[k + 1] = i;
    }
}

static void update(int c)
{
    unsigned i, j, k, l;
    if (freq[R] == MAX_FREQ)
        reconst();
    c = prnt[c + T];
    do {
        k = ++freq[c];
        l = c + 1;
        if (k > freq[l]) {
            while (k > freq[l + 1])
                l++;
            freq[c] = freq[l];
            freq[l] = k;
            i = son[c];
            prnt[i] = l;
            if (i < T)
                prnt[i + 1] = l;
            j = son[l];
            son[l] = i;
            prnt[j] = c;
            if (j < T)
                prnt[j + 1] = c;
            son[c] = j;
            c = l;
        }
    } while ((c = prnt[c]) != 0);
}

static const unsigned char *in, *in_end;
static unsigned bitbuf, bitn;

static unsigned next_byte(void) { return in < in_end ? *in++ : 0; }

static unsigned get_bit(void)
{
    unsigned b;
    if (bitn == 0) {
        bitbuf = (bitbuf | next_byte() << 8) & 0xffff;
        bitn += 8;
    }
    bitn--;
    b = bitbuf >> 15 & 1;
    bitbuf = bitbuf << 1 & 0xffff;
    return b;
}

static unsigned get_byte(void)
{
    unsigned v;
    if (bitn < 8) {
        bitbuf = (bitbuf | next_byte() << (8 - bitn)) & 0xffff;
        bitn += 8;
    }
    v = bitbuf >> 8 & 0xff;
    bitn -= 8;
    bitbuf = bitbuf << 8 & 0xffff;
    return v;
}

/* the ARCV member, decompressed: its size, 0 when it is not one */
static unsigned long arcv_decode(const unsigned char *arc, unsigned long n, unsigned char **out)
{
    static unsigned char text[N];
    unsigned long usize, csize, off, o = 0;
    unsigned r = 0xdc3, c, i, k, pos, src;
    unsigned char *dst;
    if (n < 0x21 || memcmp(arc, "ARCV", 4))
        return 0;
    usize = le32(arc + 0x19);
    csize = le32(arc + 0x1d);
    off = le16(arc + 6) + 16;
    if (off + csize > n || usize > 64ul << 20 || !(dst = malloc(usize)))
        return 0;
    tables();
    start_huff();
    memset(text, ' ', sizeof text);
    in = arc + off;
    in_end = in + csize;
    bitbuf = bitn = 0;
    while (o < usize) {
        c = son[R];
        while (c < T)
            c = son[c + get_bit()];
        c -= T;
        update((int)c);
        if (c == 0x100)
            break;
        if (c < 0x100) {
            dst[o++] = (unsigned char)c;
            text[r] = (unsigned char)c;
            r = (r + 1) & (N - 1);
        } else {
            i = get_byte();
            k = d_len[i] - 2;
            pos = (unsigned)d_code[i] << 6;
            {
                unsigned v = i, b;
                for (b = 0; b < k; b++)
                    v = v << 1 | get_bit();
                pos |= v & 0x3f;
            }
            src = (r - pos - 1) & (N - 1);
            for (k = 0; k < c - 0xff + 1 && o < usize; k++) {
                unsigned char v = text[(src + k) & (N - 1)];
                dst[o++] = v;
                text[r] = v;
                r = (r + 1) & (N - 1);
            }
        }
    }
    *out = dst;
    return o;
}

/* ------------------------------------------------------------------ the import */

static int write_file(const char *dir, const char *name, const unsigned char *data, unsigned long n)
{
    char path[MAX_PATH + 64];
    FILE *f;
    int ok;
    snprintf(path, sizeof path, "%s\\%s", dir, name);
    if (!(f = fopen(path, "wb")))
        return 0;
    ok = fwrite(data, 1, n, f) == n;
    return fclose(f) == 0 && ok;
}

static void make_dirs(const char *dir)
{
    char p[MAX_PATH + 64], *s;
    snprintf(p, sizeof p, "%s", dir);
    for (s = p + 3; *s; s++)                 /* past "C:\" */
        if (*s == '\\' || *s == '/') {
            char c = *s;
            *s = 0;
            CreateDirectoryA(p, 0);
            *s = c;
        }
    CreateDirectoryA(p, 0);
}

int iso_import(const char *iso_path, const char *dest, char *msg, size_t msglen)
{
    FILE *f = fopen(iso_path, "rb");
    unsigned char *pvd = 0, *data;
    Entry root[64], inst[128];
    const Entry *e;
    int nroot, ninst, rc = 1;
    unsigned i;
    char sha[65];
    msg[0] = 0;
    if (!f) {
        snprintf(msg, msglen, "%s could not be opened", iso_path);
        return 1;
    }
    if (!detect_sectors(f) || !(pvd = iso_read(f, 16, SECTOR)) || pvd[0] != 1 || memcmp(pvd + 1, "CD001", 5)) {
        snprintf(msg, msglen, "%s is not an ISO 9660 disc image", iso_path);
        goto out;
    }
    nroot = iso_list(f, le32(pvd + 156 + 2), le32(pvd + 156 + 10), root, 64);
    if (nroot <= 0 || !(e = find(root, nroot, "INSTALL")) || !e->dir) {
        snprintf(msg, msglen, "%s has no INSTALL folder: not the disc with Colonization for Windows", iso_path);
        goto out;
    }
    ninst = iso_list(f, e->lba, e->size, inst, 128);
    make_dirs(dest);
    for (i = 0; i < NFILES; i++) {
        if (!(e = find(inst, ninst, files[i].name))) {
            snprintf(msg, msglen, "the disc has no INSTALL\\%s", files[i].name);
            goto out;
        }
        if (!(data = iso_read(f, e->lba, e->size))) {
            snprintf(msg, msglen, "INSTALL\\%s could not be read", files[i].name);
            goto out;
        }
        if (!sha256_buf(data, e->size, sha) || strcmp(sha, files[i].sha256)) {
            snprintf(msg, msglen, "INSTALL\\%s is not the file this port knows (SHA-256 %.16s...)", files[i].name, sha);
            free(data);
            goto out;
        }
        if (!write_file(dest, files[i].name, data, e->size)) {
            snprintf(msg, msglen, "%s\\%s could not be written", dest, files[i].name);
            free(data);
            goto out;
        }
        free(data);
    }
    /* the patch's COCKGUN.WAV: GUNCOCK.WAV under the name the patched program plays */
    if (!(e = find(inst, ninst, "GUNCOCK.WAV")) || !(data = iso_read(f, e->lba, e->size))) {
        snprintf(msg, msglen, "INSTALL\\GUNCOCK.WAV could not be read");
        goto out;
    }
    if (!write_file(dest, "COCKGUN.WAV", data, e->size)) {
        snprintf(msg, msglen, "%s\\COCKGUN.WAV could not be written", dest);
        free(data);
        goto out;
    }
    free(data);
    /* the compressed program: COLONIZE._00 on Steam's image, COLONIZE.$00 on GOG's -- the same archive */
    if (!(e = find(inst, ninst, "COLONIZE._00")))
        e = find(inst, ninst, "COLONIZE.$00");
    if (!e || !(data = iso_read(f, e->lba, e->size))) {
        snprintf(msg, msglen, "the disc has no INSTALL\\COLONIZE._00");
        goto out;
    }
    {
        unsigned char *exe = 0;
        unsigned long n = arcv_decode(data, e->size, &exe);
        free(data);
        if (!n || !sha256_buf(exe, n, sha) || strcmp(sha, PREPATCH)) {
            snprintf(msg, msglen, "COLONIZE._00 did not decompress to the 1995 COLONIZE.EXE (%lu bytes, SHA-256 %.16s...)",
                     n, n ? sha : "-");
            free(exe);
            goto out;
        }
        if (!write_file(dest, "COLONIZE.EXE", exe, n)) {
            snprintf(msg, msglen, "%s\\COLONIZE.EXE could not be written", dest);
            free(exe);
            goto out;
        }
        free(exe);
    }
    snprintf(msg, msglen, "%u files, COCKGUN.WAV (GUNCOCK.WAV's copy) and COLONIZE.EXE (decompressed, the original "
             "1995 release) written to %s",
             (unsigned)NFILES, dest);
    rc = 0;
out:
    free(pvd);
    fclose(f);
    return rc;
}
