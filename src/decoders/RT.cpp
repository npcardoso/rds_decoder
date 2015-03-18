#include "RT.h"

#include <cstring>

RT_decoder::RT_decoder() {
    reset(0, -1, 0);
}

void RT_decoder::process (const group & g){
    if(g.group_type() != 0x2)
        return;


    uchar cur_AB_flag = g.bits(1, 4, 1);
    uint cur_chars_per_group = g.group_type_version()?2:4;

    if(g.PI_code() != PI_code() ||
       AB_flag != cur_AB_flag ||
       chars_per_group != cur_chars_per_group) { // A new station was tuned in

        reset(g.PI_code(),
              cur_AB_flag,
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

bool RT_decoder::ready() const {
    if(!terminated && size() < chars_per_group * 16)
        return false;

    for(int i = 0; i < size(); i++) {
        if(txt[i] == 0)
            return false;
    }

    return true;
}

void RT_decoder::reset(ushort PI_code,
                       uchar AB_flag,
                       uchar chars_per_group) {
    update_PI_code(PI_code);

    this->AB_flag = AB_flag;
    this->chars_per_group = chars_per_group;
    this->last_pos = 0;
    this->terminated = false;

    memset(txt, 0, 65 * sizeof(char));
}