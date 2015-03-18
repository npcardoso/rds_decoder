#include "PIN.h"

#include <cstring>

void PIN_decoder::reset(ushort PI_code) {
    memset(PIN, 0xFF, 3 * sizeof(uchar));
}


PIN_decoder::PIN_decoder() {
    reset(0);
}


void PIN_decoder::process (const group & g) {
    if(g.group_type() != 0x1)
        return;

    if(g.PI_code() != PI_code()) { // A new station was tuned in
        reset(g.PI_code());
    }


    PIN[0] = g.bits(3, 11, 5); //Day
    PIN[1] = g.bits(3, 6, 5); //Hour
    PIN[2] = g.bits(3, 0, 6); //Minute

}

bool PIN_decoder::ready() const {
    return day() != 0xFF &&
        minute() != 0xFF &&
        hour() != 0xFF;
}



uchar PIN_decoder::day() const {
    return PIN[0];
}

uchar PIN_decoder::hour() const {
    return PIN[1];
}

uchar PIN_decoder::minute() const {
    return PIN[2];
}
