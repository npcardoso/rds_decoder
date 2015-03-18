#include "BASIC.h"

#include <cstring>
#include <cmath>

ulong calculate_alternative_frequency (ushort freq_bits,
                                       bool LFMF) {
    if(freq_bits == 0)
        return -1;

    if(LFMF) {
        // See page 43, Table 12
        if(freq_bits < 16) // LF frequency
            return (153 + (9 * (freq_bits -1))) * 1e3;
        else if(freq_bits < 136) // MF frequency
            return (531 + (9 * (freq_bits -1))) * 1e3;
    }
    else {
        // See page 42, Table 10
        if(freq_bits < 205)
            return (876 + (freq_bits -1)) * 1e5;
    }

    return -1;
}

void frequency_list::reset(uint target_elems) {
    LFMF = false;
    this->target_elems = target_elems;
    n_elems = 0;
}

frequency_list::frequency_list() {
    reset(0);
}

bool frequency_list::update(uchar af1,
                            uchar af2) {

    uchar af[2] = {af1, af2};
    bool LFMF = false;

    for(int i=0; i < 2; i++) {
        // See page 42, Table 10
        if(af[i] >= 224 && af[i] < 250) {
            reset(af[i] - 224);
            if(af[i] == 224)
                return true;
        }
        else if(af[i] == 205){ // Filler code
        }
        else if(af[i] == 250){ // LF/MF code
            LFMF = true;
            continue;
        }
        else if(n_elems < target_elems){
            ulong freq = calculate_alternative_frequency(af[i], LFMF);
            if(!isnan(freq)) {
                elems[n_elems++] = freq;

                if(n_elems == target_elems){
                    return true;
                }
            }
        }

        LFMF = false;
    }

    return false;
}

uint frequency_list::size() const {
    return target_elems;
}

const ulong * frequency_list::elements() const {
    return elems;
}


BASIC_decoder::BASIC_decoder() {
    reset();
}

bool BASIC_decoder::accepts(const group & g) const {
    return g.group_type() == 0x0;
}

void BASIC_decoder::process_impl (const group & g,
                                  bool new_station){

    if(new_station) { // A new station was tuned in
        reset();
    }

    uchar selector = g.bits(1, 0, 2);
    uchar control_bit = g.bits(1, 2, 1);
    uchar offset = 2 * selector;


    // PS_name
    name[offset] = g.bits(3, 8, 8);
    name[offset + 1] = g.bits(3, 0, 8);

    // Decoder Identification
    if(selector == 0) {
        stereo = control_bit;
    }
    else if(selector == 1) {
        artificial_head = control_bit;
    }
    else if(selector == 2) {
        compressed = control_bit;
    }
    else if(selector == 3) {
        static_PTY = !control_bit;
    }

    // alternative frequency
    if(g.group_type_version() == 0){
        uchar af1 = g.bits(2,8, 8);
        uchar af2 = g.bits(2,0, 8);

        if(freq_list.update(af1,af2)) {
            alternative_freqs_count = freq_list.size();
            memcpy(alternative_freqs,
                   freq_list.elements(),
                   sizeof(ulong) * alternative_freqs_count);
        }
    }
    else { //No AF
        alternative_freqs_count = 0;
    }
}


const char * BASIC_decoder::PS_name() const{
    return name;
}

bool BASIC_decoder::traffic_announcement () const {
    return last_group().bits(1, 4, 1);
}

bool BASIC_decoder::traffic_program () const {
    return last_group().TP();
}

bool BASIC_decoder::is_music() const{
    return last_group().bits(1, 3, 1);
}

bool BASIC_decoder::is_speech() const{
    return !ms_bit;
}

bool BASIC_decoder::is_stereo() const{
    return stereo;
}

bool BASIC_decoder::is_artificial_head() const {
    return artificial_head;
}

bool BASIC_decoder::is_compressed() const {
    return compressed;
}

bool BASIC_decoder::is_static_PTY() const {
    return static_PTY;
}

uint BASIC_decoder::af_count() const {
    return alternative_freqs_count;
}

const ulong * BASIC_decoder::af() const {
    return alternative_freqs;
}


bool BASIC_decoder::ready() const {
    for(int i = 0; i < 8; i++) {
        if(name[i] == 0)
            return false;
    }

    return true;
}

void BASIC_decoder::reset() {
    this->tp_bit = false;
    this->ta_bit = false;
    this->ms_bit = false;
    this->stereo = false;
    this->artificial_head = false;
    this->compressed = false;
    this->static_PTY = true;

    this->alternative_freqs_count = 0;

    memset(name, 0, 9 * sizeof(char));
}
