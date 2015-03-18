#ifndef __PIN_H__
#define __PIN_H__

#include "../worker.h"

/**
 * @note See page 23, section 3.1.5.2 in the standard
 */
class PIN_decoder : public RDS_decoder {
    public:
    PIN_decoder();

    virtual void process (const group & g);


    uchar day() const;
    uchar hour() const;
    uchar minute() const;

    virtual bool ready() const;

    private:

    void reset(ushort PI_code);

    private:

    uchar PIN[3];
};

#endif
