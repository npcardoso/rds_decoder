#include "RT.h"

#include "../constants.h"

#include <cstring>

RT_decoder::RT_decoder (bool version) {
    this->version = version;

    reset();
}

bool RT_decoder::accepts (const group & g) const {
    return g.group_type() == 0x2 && g.group_type_version() == version;
}

void RT_decoder::process_impl (const group & g,
                               bool new_station) {
    bool AB_flag = last_group().bits(1, 4, 1);
    bool cur_AB_flag = g.bits(1, 4, 1);


    if (new_station ||
        AB_flag != cur_AB_flag) {
        // A new station was tuned in

        reset();
    }


    uint offset = g.bits(1, 0, 4) * chars_per_group();

    if (last_pos < offset + chars_per_group())
        last_pos = offset + chars_per_group();

    if (g.group_type_version() == 0) {
        txt[offset] = g.bits(2, 8, 8);
        txt[offset + 1] = g.bits(2, 0, 8);
        txt[offset + 2] = g.bits(3, 8, 8);
        txt[offset + 3] = g.bits(3, 0, 8);
    } else {
        txt[offset] = g.bits(3, 8, 8);
        txt[offset + 1] = g.bits(3, 0, 8);
    }

    for (int i = 0; i < chars_per_group(); i++) {
        if (txt[offset + i] == 0x0D) {
            // Messages shorter than 64 bytes must be terminated with a CR
            txt[offset + i] = '\0';
            last_pos = offset + i;
            terminated = true;
        } else if (txt[offset + i] == 0x0A) {
            // 0x0A represents a newline
            txt[offset + i] = '\n';
        }
    }
}

const char * RT_decoder::text () const {
    return txt;
}

uint RT_decoder::size () const {
    return last_pos;
}

bool RT_decoder::is_terminated () const {
    return terminated;
}

bool RT_decoder::ready () const {
    if (!terminated && size() < chars_per_group() * 16)
        return false;

    for (int i = 0; i < size(); i++) {
        if (txt[i] == 0)
            return false;
    }

    return true;
}

void RT_decoder::reset () {
    this->last_pos = 0;
    this->terminated = false;

    memset(txt, 0, 65 * sizeof(char));
}

std::ostream & RT_decoder::write_to (std::ostream & out) const {
    out << group_acronyms[2] << ":" << std::endl;
    out << "\ttext (" << size() << "): \"" << text() << "\"" << std::endl;
    out << "\tterminated: " << (is_terminated() ? "true" : "false") << std::endl;
    out << "\tready: " << (ready() ? "true" : "false") << std::endl;

    return out;
}