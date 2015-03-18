#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "types.h"

#include <iostream>

namespace block {
/**
 * @note see page 70, Annex A, figure A.1 in the standard
 */
ushort apply_offset_word(ushort check_word,
                         uint block_type);

ushort get_info_word(ulong buf);

ushort get_check_word(ulong buf);

/**
 * @brief Reads a block from "in".
 * This function does not perform any stream synchronization.
 * @return A 26 byte word.
 */
ulong read(std::istream & in);

/**
 * @note See page 72, Annex B, figure B.1 in the standard
 */
ushort calculate_syndrome(ushort info_word);

/**
 * @brief Returns the next block type.
 * @return The block type or -1 on error.
 */
int next_type(ushort cur_type);

/**
 * @brief Detects the block type.
 * @return Current block type or -1 on error.
 */
int detect_type(ushort info_word, ushort check_word);

/**
 * @brief Checks a block for errors.
 * @note When block_type == 2 and the check fails, this function tries to use block_type == 5 (C' instead of C).
 * @return Current block type or -1 on error.
 */
int check (ushort info_word, ushort check_word, uint block_type);

/**
 * @brief Aligns stream with blocks.
 * @return Current block type or -1 on error.
 */
int sync (std::istream & in);
}

#endif
