#include "group.h"

#include "constants.h"

#include <string.h>



group::group(uint group_id,
             const ushort * g) {
    this->g_id = group_id;
    memcpy(this->group_buf, g, 4 * sizeof(ushort));
}

uint group::group_id() const {
    return g_id;
}

ushort group::PI_code() const {
    return bits(0);
}

uchar group::coverage_area() const {
    return bits(0, 8, 4);
}

std::string group::coverage_area_string() const {
    return coverage_area_codes[coverage_area()];
}

uchar group::group_type() const {
    return bits(1, 12, 4);
}

uchar group::group_type_version() const {
    return bits(1, 11, 1);
}

std::string group::group_type_string() const {
    return group_acronyms[group_type()];
}

uchar group::TP() const {
    return bits(1, 10, 1);
}

uchar group::PTY() const {
    return bits(1, 5, 5);
}

std::string group::PTY_string() const {
    return pty_table[PTY()];
}




ushort group::bits(uint block_id,
                   uint start_bit,
                   uint width) const {
    ushort b = group_buf[block_id];

    b >>= start_bit;
    b &= (1 << width) - 1;

    return b;
}
