/* types.h -- the port's own integer types (roadmap section 1, "Game integers").
 *
 * Borland's `int` is 16 bits, and the game's behaviour depends on it: values wrap at 16 bits
 * and the width and signedness of a compare decide the branch. Game-logic `int`/`unsigned`
 * become i16/u16 as files are ported (W1 onward). The platform layer keeps Win32 types.
 */
#ifndef PORT_TYPES_H
#define PORT_TYPES_H

#include <stdint.h>

typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;

#endif
