#include "RT.h"

#include "../constants.h"

#include <cstring>

RT_decoder::RT_decoder() {
    reset(-1, 0);
}

bool RT_decoder::accepts(const group & g) const {
    return g.group_type() == 0x2;
}

void RT_decoder::process_impl (const group & g,
                               bool new_station){


    uchar cur_AB_flag = g.bits(1, 4, 1);
    uint cur_chars_per_group = g.group_type_version()?2:4;

    if(new_station ||
       AB_flag != cur_AB_flag ||
       chars_per_group != cur_chars_per_group) { // A new station was tuned in

        reset(cur_AB_flag,
              cur_chars_per_group);
    }


    uint offset = g.bits(1, 0, 4) * chars_per_group;

    if(last_pos < offset + chars_per_group)
        last_pos = offset + chars_per_group;

    if(g.group_type_version() == 0) {
        txt[offset] = g.bits(2, 8, 8);
        txt[offset + 1] = g.bits(2, 0, 8);
        txt[offset + 2] = g.bits(3, 8, 8);
        txt[offset + 3] = g.bits(3, 0, 8);
    }
    else {
        txt[offset] = g.bits(3, 8, 8);
        txt[offset + 1] = g.bits(3, 0, 8);
    }

    for(int i = 0; i < chars_per_group; i++) {
        if(txt[offset + i] == 0x0D) { // Messages shorter than 64 bytes must be terminated with a CR
            txt[offset + i] = '\0';
            last_pos = offset + i;
            terminated = true;
        }
        else if(txt[offset + i] == 0x0A) { // 0x0A represents a newline
            txt[offset + i] = '\n';
        }

    }
}

const char * RT_decoder::text() const {
    return txt;
}

uint RT_decoder::size() const {
    return last_pos;
}

bool RT_decoder::is_terminated() const {
    return terminated;
}

bool RT_decoder::ready() const {
    if(!terminated && size() < chars_per_group * 16)
        return false;

    for(int i = 0; i < size(); i++) {
        if(txt[i] == 0)
            return false;
    }

    return true;
}

void RT_decoder::reset(uchar AB_flag,
                       uchar chars_per_group) {
    this->AB_flag = AB_flag;
    this->chars_per_group = chars_per_group;
    this->last_pos = 0;
    this->terminated = false;

    memset(txt, 0, 65 * sizeof(char));
}

std::ostream & RT_decoder::write_to (std::ostream & out) const {
    out << group_acronyms[2] << ":" << std::endl;
    out << "\tText ("<< size()<< "): \"" << text() << "\""<< std::endl;
    out << "\tterminated: "<< (is_terminated()?"true":"false") << std::endl;

    return out;
}
