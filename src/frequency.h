#ifndef __FREQUENCY_H__
#define __FREQUENCY_H__

#include "types.h"

ulong calculate_alternative_frequency (uchar freq_bits,
                                       bool LFMF);

class frequency_list {
    public:
    frequency_list();

    bool update(uchar af1,
                uchar af2);

    uint size() const;

    const ulong * elements() const;

    private:
    void reset(uint target_elems);

    private:
    bool LFMF;
    uint target_elems;
    uint n_elems;
    ulong elems[25];
};

#endif
