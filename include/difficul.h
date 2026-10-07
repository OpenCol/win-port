/* difficul.h -- the five difficulty levels, the byte at SEG20:0x77a8.
 *
 * Source: the game's DIFFICULTY TEXT resource, one level a line, lowest first. The code agrees:
 * `choose_difficulty` stores the menu answer less one, and `king_welcome` names the level
 * with the byte as an index into the word table. tools/check_headers.py re-reads the resource.
 *
 * Name a difficulty only where the code COMPARES it. Most uses are arithmetic
 * (`(8 - g_difficulty) * t >> 3`, `g_difficulty * 200`), and there the byte is a number.
 */
#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#define DIFFICULTY_DISCOVERER       0
#define DIFFICULTY_EXPLORER         1
#define DIFFICULTY_CONQUISTADOR     2
#define DIFFICULTY_GOVERNOR         3
#define DIFFICULTY_VICEROY          4

#define DIFFICULTY_COUNT            5

#endif
