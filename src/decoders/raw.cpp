#include "raw.h"

#include <bitset>

raw_decoder::raw_decoder (uchar group_type,
                          bool group_type_version) {
    this->group_type = group_type;
    this->group_type_version = group_type_version;
}

bool raw_decoder::accepts (const group & g) const {
    return g.group_type() == group_type &&
           g.group_type_version() == group_type_version;
}

std::ostream & raw_decoder::write_to (std::ostream & out) const {
    const group & g = last_group();


    out << (int) g.group_type();
    out << (g.group_type_version() ? "B" : "A");
    out << " (" << g.group_type_string() << "): " << std::endl;

    out << "\tPI code: \"0x" << std::hex << g.PI_code() << "\"" << std::endl;
    out << "\tcoverage: \"" << g.coverage_area_string() << "\"";
    out << " (" << (int) g.coverage_area() << ")" << std::endl;
    out << "\ttraffic_program: " << (bool) g.traffic_program() << std::endl;
    out << "\tprogram_type: \"" << g.program_type_string() << "\"";
    out << " (" << (int) g.program_type() << ")" << std::endl;
    out << "\tbits: " << std::endl;

    uchar b = g.bits(1, 0, 5);
    out << "\t\t   " << std::bitset<5> (b);
    out << " (0x";
    out.fill('0');
    out.width(2);
    out << std::hex << (int) b << ")";
    out << " (" << (char) (isprint(b) ? b : '*') << ")" << std::endl;

    for (int i = 2; i < 4; i++) {
        for (int j = 2; j--;) {
            b = g.bits(i, 8 * j, 8);
            out << "\t\t" << std::bitset<8> (b);
            out << " (0x" << std::hex;
            out.fill('0');
            out.width(2);
            out << (int) b << ")";
            out << " (" << (char) (isprint(b) ? b : '*') << ")" << std::endl;
        }
    }

    out << std::dec;

    return out;
}