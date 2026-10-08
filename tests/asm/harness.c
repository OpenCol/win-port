/* The port's replacements for the game's assembly (src/asm), run on a job file: the native side
 * of tests/asm.py (roadmap W5).
 *
 *     asm_harness.exe JOB OUT
 *
 * JOB, little-endian 32-bit words: the call count, then per call
 *     function, seed, buffer count, per buffer (size, bytes padded to 4),
 *     argument count, per argument (kind, a, b)
 * an argument being a 16-bit word (kind 0, a), a long (1, a) or a pointer into a buffer
 * (2, buffer a, offset b). OUT, per call: a status (0 returned, 1 faulted), the return value,
 * the seed after (DGROUP:0x3f00's), then every buffer as it was left.
 */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void asm_wipe_rows(long, long, long, long, long, long, int, int, int, int, int, int, int, int, int);
void asm_copy_bits(void *, void *, int, int, int, int, int, int, int, int, int, int);
void asm_fill_rect(unsigned long, int, int, int, int, int, int, int);
void asm_draw_sprite(long, void *, long, unsigned char, int, int, int, int, int, int, int, int, int, int, int, int);
void asm_draw_mono_sprite(long, void *, long, unsigned char, int, int, int, int, int, int, int, int, int, int, int,
                          int, unsigned char);
void asm_decode_delta_flc(long, long, int, int, int, unsigned char);
void asm_decode_byte_run(long, long, int, int, int, unsigned char);
void asm_decode_literal(long, long, int, int, int, unsigned char);
int asm_lzw_decode(long, long, long, int, int, int, int);
int L17_01dc(int);
void L17_01f0(long);
long L17_01fe(void);
extern uint32_t g_dgroup_3f00_seed;

enum { WIPE, COPY, FILL, SPRITE, MONO, DELTA, BYTE_RUN, LITERAL, LZW, RANDOM_BELOW, SET_SEED, GET_SEED };

static uint32_t rd(FILE *f)
{
    unsigned char b[4];
    if (fread(b, 1, 4, f) != 4) {
        fprintf(stderr, "asm_harness: short job file\n");
        exit(2);
    }
    return b[0] | b[1] << 8 | b[2] << 16 | (uint32_t)b[3] << 24;
}

static void wr(FILE *f, uint32_t v)
{
    unsigned char b[4] = { v & 0xff, v >> 8 & 0xff, v >> 16 & 0xff, v >> 24 };
    fwrite(b, 1, 4, f);
}

static jmp_buf fault;

static void on_fault(int sig)
{
    signal(sig, on_fault);
    longjmp(fault, 1);
}

int main(int argc, char **argv)
{
    FILE *in, *out;
    uint32_t ncalls, k;
    if (argc != 3 || !(in = fopen(argv[1], "rb")) || !(out = fopen(argv[2], "wb"))) {
        fprintf(stderr, "usage: asm_harness JOB OUT\n");
        return 2;
    }
    ncalls = rd(in);
    for (k = 0; k < ncalls; k++) {
        uint32_t fn = rd(in), seed = rd(in), nbuf = rd(in), nargs, i;
        unsigned char *buf[16];
        uint32_t size[16];
        intptr_t a[24];
        volatile long ret = 0;
        volatile int status = 0;
        for (i = 0; i < nbuf; i++) {
            size[i] = rd(in);
            buf[i] = malloc(size[i] + 4);
            if (fread(buf[i], 1, (size[i] + 3) & ~3u, in) != ((size[i] + 3) & ~3u))
                return 2;
        }
        nargs = rd(in);
        for (i = 0; i < nargs; i++) {
            uint32_t kind = rd(in), x = rd(in), y = rd(in);
            a[i] = kind == 0 ? (intptr_t)(int16_t)x : kind == 1 ? (intptr_t)(int32_t)x : (intptr_t)(buf[x] + y);
        }
        g_dgroup_3f00_seed = seed;
        signal(SIGSEGV, on_fault);
        if (setjmp(fault))
            status = 1;
        else
            switch (fn) {
            case WIPE:
                asm_wipe_rows(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12],
                              a[13], a[14]);
                break;
            case COPY:
                asm_copy_bits((void *)a[0], (void *)a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10],
                              a[11]);
                break;
            case FILL:
                asm_fill_rect((unsigned long)a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
                break;
            case SPRITE:
                asm_draw_sprite(a[0], (void *)a[1], a[2], (unsigned char)a[3], a[4], a[5], a[6], a[7], a[8], a[9],
                                a[10], a[11], a[12], a[13], a[14], a[15]);
                break;
            case MONO:
                asm_draw_mono_sprite(a[0], (void *)a[1], a[2], (unsigned char)a[3], a[4], a[5], a[6], a[7], a[8],
                                     a[9], a[10], a[11], a[12], a[13], a[14], a[15], (unsigned char)a[16]);
                break;
            case DELTA:
                asm_decode_delta_flc(a[0], a[1], a[2], a[3], a[4], (unsigned char)a[5]);
                break;
            case BYTE_RUN:
                asm_decode_byte_run(a[0], a[1], a[2], a[3], a[4], (unsigned char)a[5]);
                break;
            case LITERAL:
                asm_decode_literal(a[0], a[1], a[2], a[3], a[4], (unsigned char)a[5]);
                break;
            case LZW:
                ret = asm_lzw_decode(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
                break;
            case RANDOM_BELOW:
                ret = L17_01dc(a[0]);
                break;
            case SET_SEED:
                L17_01f0(a[0]);
                break;
            case GET_SEED:
                ret = L17_01fe();
                break;
            default:
                status = 1;
            }
        wr(out, status);
        wr(out, (uint32_t)ret);
        wr(out, g_dgroup_3f00_seed);
        for (i = 0; i < nbuf; i++) {
            fwrite(buf[i], 1, size[i], out);
            free(buf[i]);
        }
    }
    fclose(out);
    return 0;
}
