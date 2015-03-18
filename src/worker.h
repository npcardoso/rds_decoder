#ifndef __WORKER_H__
#define __WORKER_H__

#include "group.h"

#include <iostream>


class RDS_decoder {
    public:
    virtual void process(const group & g) = 0;

    inline virtual void rx_error () {}

    inline virtual void sync() {}

    inline virtual void no_sync() {}

    ushort PI_code() const;

    inline virtual bool ready() const { return false; };

    protected:

    /**
     * @brief Updates PI_code
     * @return true if PI_code changed, false otherwise
     */
    bool update_PI_code(ushort PI_code);
    private:

    ushort PI;
};


class RDS_worker {
    public:
    void run (std::istream & in,
              RDS_decoder & decoder);

    private:
    int read(std::istream & in,
             ushort * group_buf);

    int sync (std::istream & in);
};

#endif
