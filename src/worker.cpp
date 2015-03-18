#include "worker.h"

#include "block.h"

RDS_worker::RDS_worker() {
    group_id = 0;
}

void RDS_worker::run (std::istream & in,
                      RDS_decoder & decoder) {
    static const int max_errors = 10;

    ushort group_buf[4];
    bool synced = false;
    int errors = 0;

    while(in.good()){
        if(errors > max_errors) {
            synced = false;
            decoder.no_sync();
        }

        if(!synced) {
            int tmp = sync(in);
            if(tmp < 0)
                continue;
            synced = true;
            errors = 0;
            decoder.sync();
        }

        int tmp = read(in, group_buf);

        if(tmp < 0) {
            errors++;

            decoder.rx_error();
            continue;
        }

        if(-errors < max_errors)
            errors--;

        decoder.process(group(group_id++, group_buf));
    }
}

int RDS_worker::read(std::istream & in,
                     ushort * group_buf) {
    int cur_block_type = 0;

    int i = 0;

    while(in.good()) {
        ulong b = block::read(in);
        ushort info_word = block::get_info_word(b);

        cur_block_type = block::check(info_word,
                                      block::get_check_word(b),
                                      cur_block_type);



        if(cur_block_type < 0)
            return -1;


        group_buf[i++] = info_word;

        cur_block_type = block::next_type(cur_block_type);

        if(cur_block_type == 0)
            break;
    }

    return 0;
}

int RDS_worker::sync (std::istream & in) {
    int block_type = -1;
    while(in.good()) {
        if(block_type < 0)
            block_type = block::sync(in);

        if(block_type < 0)
            continue;

        if(block::next_type(block_type) ==  0) { // At this point the stream is synced
            return 0;
        }

        ulong buf = block::read(in);
        block_type = block::check(block::get_info_word(buf),
                                  block::get_check_word(buf),
                                  block::next_type(block_type));
    }

    return -1;
}
