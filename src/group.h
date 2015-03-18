#ifndef __GROUP_H__
#define __GROUP_H__

#include "types.h"

#include <string>

class group {
    public:
    group(uint group_id,
          const ushort * g);

    uint group_id() const;

    ushort PI_code() const;
    uchar coverage_area() const;
    std::string coverage_area_string() const;

    uchar group_type() const;
    uchar group_type_version() const;
    std::string group_type_string() const;

    uchar TP() const;
    uchar PTY() const;
    std::string PTY_string() const;

    ushort bits(uint block_id,
                uint start_bit=0,
                uint width=16) const;

    private:
    ushort group_buf[4];
    uint g_id;
};

#endif
