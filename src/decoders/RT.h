#ifndef __RT_H__
#define __RT_H__

#include "../worker.h"

/**
 * @note See page 25, section 3.1.5.3 in the standard
 */
class RT_decoder : public RDS_decoder {
    public:
    RT_decoder();

    virtual void process (const group & g);

    const char * text() const;

    uint size() const;

    virtual bool ready() const;

    private:

    void reset(ushort PI_code,
               uchar AB_flag,
               uchar chars_per_group);



    private:
    char txt[65];

    bool terminated;
    uint chars_per_group;
    uint last_pos;
    uchar AB_flag;
};

#endif
