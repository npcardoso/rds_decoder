#include "frequency.h"

#include <cmath>


ulong calculate_alternative_frequency (uchar freq_bits,
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
