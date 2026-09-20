/* col/types.h -- the widths the game is written in, spelled so they stay those widths.
 *
 * Borland's `int` is 16 bits and the program depends on it: the map code wraps at 16
 * bits, and the width of a compare is what decides JL from JB. A host compiler's `int`
 * is 32 bits, so a record written with bare `int` has a different layout on the two
 * targets and the host tests would be testing a different program.
 *
 * So the generated header spells every multi-byte field through these names. `char`
 * stays `char` on purpose -- it is signed under Borland's default and under the host
 * flags the test harness passes, and keeping it lets a string still be a string.
 */
#ifndef COL_TYPES_H
#define COL_TYPES_H

typedef signed char    col_i8;
typedef unsigned char  col_u8;
#ifdef COL_HOST
typedef short          col_i16;
typedef unsigned short col_u16;
typedef int            col_i32;
typedef unsigned int   col_u32;
#else
typedef int            col_i16;     /* 16 bits under Borland C++ 4.52 */
typedef unsigned int   col_u16;
typedef long           col_i32;
typedef unsigned long  col_u32;
#endif

typedef char col_assert_widths[sizeof(col_i16) == 2 && sizeof(col_i32) == 4 ? 1 : -1];

#endif
