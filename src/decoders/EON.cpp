#include "EON.h"

#include "../constants.h"

#include "../frequency.h"

#include <cstring>

other_network::other_network() {
    PI = 0;
    PTY = 0;
    TA = false;
    TP = false;
    PIN = 0;

    linkage_info = 0;

    memset(name, 0, 9 * sizeof(uchar));
    memset(alternative_freqs, 0, 12 * sizeof(uint));
}


const char * other_network::PS_name() const{
    return name;
}

bool other_network::traffic_announcement () const {
    return TA;
}

bool other_network::traffic_program () const {
    return TP;
}

uchar other_network::program_type () const {
    return PTY;
}

ushort other_network::program_item_number () const {
    return PIN;
}

const uint * other_network::alternative_frequencies() const {
    return alternative_freqs;
}



EON_decoder::EON_decoder() {
    reset();
}

void EON_decoder::reset() {
    ONs.clear();
}

bool EON_decoder::accepts(const group & g) const {
    return g.group_type() == 0xE;
}


void EON_decoder::process_impl (const group & g,
                                bool new_station){

    if(new_station) { // A new station was tuned in
        reset();
    }

    ushort PI_ON = g.bits(3);
    other_network & ON = ONs[PI_ON];

    ON.PI = PI_ON;
    ON.TP = g.bits(1, 4, 1);

    if(g.group_type_version() == 0) {
        uchar variant_code = g.bits(1, 0, 4);
        uchar w1 = g.bits(2, 8, 8);
        uchar w2 = g.bits(2, 0, 8);
        if(variant_code <= 3) { // PS_name
            ON.name[variant_code * 2] = w1;
            ON.name[variant_code * 2 + 1] = w2;
        }
        else if(variant_code <= 9) { // AF
            ON.alternative_freqs[(variant_code - 4) * 2] =
                calculate_alternative_frequency(w1, false);
            ON.alternative_freqs[(variant_code - 4) * 2 + 1] =
                calculate_alternative_frequency(w2, false);
        }
        else if(variant_code == 12) { // Linkage Information
            ON.linkage_info = g.bits(2);
        }
        else if(variant_code == 13) { // PTY/TA
            ON.PTY = g.bits(2, 11, 5);
            ON.TA = g.bits(2, 0, 1);
        }
        else if(variant_code == 13) { // PIN
            ON.PIN = g.bits(2);
        }
    }
    else {
        ON.TA = g.bits(1, 3, 1);
    }
}

const std::map<ushort, other_network> & EON_decoder::other_networks () const {
    return ONs;
}

std::ostream & EON_decoder::write_to (std::ostream & out) const {
    std::map<ushort, other_network>::const_iterator it = other_networks().begin();
    out << group_acronyms[14] << ":" << std::endl;

    while(it != other_networks().end()) {
        const other_network & ON = (it++)->second;
        out << "\tPS name: \"" << ON.PS_name() << "\"" << std::endl;
        out << "\t\tprogram item number: " << (int) ON.program_item_number() << std::endl;
        out << "\t\tpty: " << (int) ON.program_type() << std::endl;
        out << "\t\tTA: " << (int) ON.traffic_announcement() << std::endl;
        out << "\t\tTP: " << (int) ON.traffic_program() << std::endl;
        out << "\t\tAFs:";
        for(int i = 0; i < 12; i++)
            out << " "<< ON.alternative_frequencies()[i];
        out <<std::endl;
    }

    return out;
}
