#include "BASIC.h"

#include "../constants.h"

#include <cstring>


BASIC_decoder::BASIC_decoder () {
    reset();
}

bool BASIC_decoder::accepts (const group & g) const {
    return g.group_type() == 0x0;
}

void BASIC_decoder::process_impl (const group & g,
                                  bool new_station) {
    if (new_station) {
        // A new station was tuned in
        reset();
    }

    uchar selector = g.bits(1, 0, 2);
    uchar control_bit = g.bits(1, 2, 1);
    uchar offset = 2 * selector;


    // PS_name
    name[offset] = g.bits(3, 8, 8);
    name[offset + 1] = g.bits(3, 0, 8);

    // Decoder Identification
    if (selector == 0) {
        stereo = control_bit;
    } else if (selector == 1) {
        artificial_head = control_bit;
    } else if (selector == 2) {
        compressed = control_bit;
    } else if (selector == 3) {
        static_PTY = !control_bit;
    }

    // alternative frequency
    if (g.group_type_version() == 0) {
        uchar af1 = g.bits(2, 8, 8);
        uchar af2 = g.bits(2, 0, 8);

        if (freq_list.update(af1, af2)) {
            alternative_freqs_count = freq_list.size();
            memcpy(alternative_freqs,
                   freq_list.elements(),
                   sizeof(ulong) * alternative_freqs_count);
        }
    } else {
        // No AF
        alternative_freqs_count = 0;
    }
}

const char * BASIC_decoder::PS_name () const {
    return name;
}

bool BASIC_decoder::traffic_announcement () const {
    return last_group().bits(1, 4, 1);
}

bool BASIC_decoder::is_music () const {
    return last_group().bits(1, 3, 1);
}

bool BASIC_decoder::is_speech () const {
    return !is_music();
}

bool BASIC_decoder::is_stereo () const {
    return stereo;
}

bool BASIC_decoder::is_artificial_head () const {
    return artificial_head;
}

bool BASIC_decoder::is_compressed () const {
    return compressed;
}

bool BASIC_decoder::is_static_PTY () const {
    return static_PTY;
}

uint BASIC_decoder::af_count () const {
    return alternative_freqs_count;
}

const ulong * BASIC_decoder::af () const {
    return alternative_freqs;
}

bool BASIC_decoder::ready () const {
    for (int i = 0; i < 8; i++) {
        if (name[i] == 0)
            return false;
    }

    return true;
}

void BASIC_decoder::reset () {
    this->stereo = false;
    this->artificial_head = false;
    this->compressed = false;
    this->static_PTY = true;

    this->alternative_freqs_count = 0;

    memset(name, 0, 9 * sizeof(char));
}

std::ostream & BASIC_decoder::write_to (std::ostream & out) const {
    out << group_acronyms[0] << ":" << std::endl;
    out << "\tPS name: \"" << PS_name() << "\"" << std::endl;
    out << "\tstereo: " << is_stereo() << std::endl;
    out << "\tartificial_head: " << is_artificial_head() << std::endl;
    out << "\tcompressed: " << is_compressed() << std::endl;
    out << "\tstatic_pty: " << is_static_PTY() << std::endl;
    out << "\tTA: " << traffic_announcement() << std::endl;
    out << "\tAFs (" << (int) af_count() << "):";

    for (int i = 0; i < af_count(); i++) {
        if (i % 2)
            out << "\t";
        else
            out << std::endl << "\t";

        out.width(10);
        out.fill(' ');

        out << af()[i] / 1e6 << " MHz";
    }

    out << std::endl;

    return out;
}