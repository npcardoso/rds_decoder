#include "PIN.h"

#include <cstring>

void PIN_decoder::reset() {
    memset(PIN, 0xFF, 3 * sizeof(uchar));
    memset(slow_labelling, 0, 8 * sizeof(ushort));
}


PIN_decoder::PIN_decoder() {
    reset();
}

bool PIN_decoder::accepts(const group & g) const {
    return g.group_type() == 0x1;
}


void PIN_decoder::process_impl (const group & g,
                                bool new_station){

    if(new_station) { // A new station was tuned in
        reset();
    }

    if(g.group_type_version() == 0) {
        uchar variant_code = g.bits(2, 12, 3);
        slow_labelling[variant_code] = g.bits(2, 0, 12);
    }
}


uchar PIN_decoder::day() const {
    return last_group().bits(3, 11, 5);
}

uchar PIN_decoder::hour() const {
    return last_group().bits(3, 6, 5);
}

uchar PIN_decoder::minute() const {
    return last_group().bits(3, 0, 6);
}

ushort PIN_decoder::paging() const{
    return slow_labelling[0] >> 8 & 0xF;
}

ushort PIN_decoder::extended_country_code() const{
    return slow_labelling[0] & 0xFF;
}

ushort PIN_decoder::TMC_identification() const{
    return slow_labelling[1];
}

ushort PIN_decoder::paging_id() const{
    return slow_labelling[2];
}

ushort PIN_decoder::language_codes() const {
    return slow_labelling[3];
}

ushort PIN_decoder::broadcaster_codes() const{
    return slow_labelling[6];
}

ushort PIN_decoder::EWS_channel_id() const {
    return slow_labelling[7];
}

bool PIN_decoder::linkage_actuator() const {
    const group & g = last_group();
    if(g.group_type_version() == 0)
        return g.bits(2, 15, 1);

    return 0;
}

uchar PIN_decoder::radio_paging_codes() const {
    const group & g = last_group();
    if(g.group_type_version() == 0)
        return g.bits(1, 0, 5);

    return 0;
}
