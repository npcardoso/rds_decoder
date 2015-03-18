#ifndef __DECODER_CPP__
#define __DECODER_CPP__

#include "decoder.h"

ushort g_init[4] = {0x0,0x0,0x0,0x0};

RDS_decoder::RDS_decoder() : last_g(0, g_init) {}

bool RDS_decoder::process(const group & g) {
    if(!accepts(g))
        return false;

    bool new_station = g.PI_code() != last_g.PI_code();

    this->last_g = g;

    process_impl(g, new_station);

    return true;
}

const group & RDS_decoder::last_group() const {
    return last_g;
}

#endif
