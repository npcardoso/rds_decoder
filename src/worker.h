#ifndef __WORKER_H__
#define __WORKER_H__

#include "decoder.h"

#include <iostream>

class RDS_worker {
    public:
    RDS_worker();

    void run (std::istream & in,
              RDS_decoder & decoder);

    private:
    int read(std::istream & in,
             ushort * group_buf);

    int sync (std::istream & in);

    private:
    uint group_id;
};

#endif
