#include "block.h"

namespace block {

/**
 * @note See page 12, Section 2.1, figure 8 in the standard
 */
static const uint info_word_len = 16;
static const uint check_word_len = 10;
static const uint block_len = info_word_len + check_word_len;


/**
 * @note See page 72, Annex B, figure B.1 in the standard
 */
static const ushort syndrome_matrix [] = {0x077, 0x2E7, 0x3AF, 0x30B,
                                          0x359, 0x370, 0x1B8, 0x0DC,
                                          0x06E, 0x037, 0x2C7, 0x3BF,
                                          0x303, 0x35D, 0x372,0x1B9};
/*
  Python:

  binary = ["000001110111", "001011100111", "001110101111", "001100001011",
  "001101011001", "001101110000", "000110111000", "000011011100",
  "000001101110", "000000110111", "001011000111", "001110111111",
  "001100000011", "001101011101", "001101110010", "000110111001"]
  for ee in [hex(int(a, 2)) for a in binary]:
    print("%s," %ee)
*/


/**
 * @note See page 59, Annex C, table C.1 in the standard
 * @note Offset word C' has been put at the end
 */

static const uint offset_word[5]={252,408,360,436,848};
static const uint offset_pos[5]={0,1,2,3,2};
static const char * const offset_name[]={"A","B","C","D","C'"};


/* see page 70, Annex A, figure A.1 in the standard */
ushort apply_offset_word(ushort check_word, uint block_type) {

    return check_word ^ offset_word[block_type];
}

ushort get_info_word(ulong buf) {
    return (buf >> check_word_len) & ((1 << info_word_len) - 1);
}

ushort get_check_word(ulong buf) {
    return buf & ((1 << check_word_len) - 1);
}

ulong read(std::istream & in) {
    ulong buf;

    for(uint i = 0; i < block_len && in.good(); i++) {
        uchar tmp;
        in >> tmp;

        // push the new bit onto the buffer
        buf <<= 1;
        buf += tmp & 1;
    }

    return buf;
}



ushort calculate_syndrome(ushort info_word) {
    ushort ret = 0;

    for(uint i = 0; i < info_word_len; i++) {
        if(info_word & (1 << (info_word_len - (i + 1))))
            ret ^= syndrome_matrix[i];
    }
    return ret;
}

int next_type(ushort cur_type) {
    if(cur_type < 0 || cur_type > 4)
        return -1;

    if(cur_type == 4)
        return 3;

    return (cur_type + 1) % 4;
}

int detect_type(ushort info_word, ushort check_word) {
    for(uint i = 0; i < 5; i++) {
        if(calculate_syndrome(info_word) == apply_offset_word(check_word, i))
            return i;
    }

    return -1;
}

int check (ushort info_word, ushort check_word, uint block_type) {
    ushort syn = calculate_syndrome(info_word);

    if(syn == apply_offset_word(check_word, block_type))
            return block_type;
    // Handle C and C'
    else if(block_type == 2 && (syn == apply_offset_word(check_word, 4)))
            return 4;
    else if(block_type == 4 && (syn == apply_offset_word(check_word, 2)))
            return 2;

    return -1;
}


int sync (std::istream & in) {
    ulong buf;

    while(in.good()) {
        uchar tmp;
        in >> tmp;

        // push the new bit onto the buffer
        buf <<= 1;
        buf += tmp & 1;

        int block_type = block::detect_type(block::get_info_word(buf),
                                            block::get_check_word(buf));

        if(block_type >= 0)
            return block_type;
    }
    return -1;
}
}
