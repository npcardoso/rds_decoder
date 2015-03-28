#include "AID.h"

#include "../constants.h"

bool AID_decoder::accepts (const group & g) const {
    return g.group_type() == 0x3 &&
           g.group_type_version() == 0;
}

uchar AID_decoder::application_group_type_code () const {
    return last_group().bits(1, 0, 5);
}

uint AID_decoder::message_bits () const {
    return last_group().bits(2);
}

uint AID_decoder::application_id () const {
    return last_group().bits(3);
}

std::ostream & AID_decoder::write_to (std::ostream & out) const {
    out << group_acronyms[3] << ":" << std::endl;
    out << "\tapplication id: 0x" << std::hex << (int) application_id() << std::endl;

    out << "\tapp group type code: 0x" << std::hex;
    out.width(4);
    out << (int) application_group_type_code() << std::endl;

    out << "\tmessage bits: 0x" << std::hex;
    out.width(4);
    out << message_bits() << std::endl;
    out << std::dec;

    return out;
}