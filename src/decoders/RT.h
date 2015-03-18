#ifndef __RT_H__
#define __RT_H__

#include "../worker.h"

/**
 * @note See page 25, section 3.1.5.3 in the standard
 */
class RT_decoder : public RDS_decoder {
    public:
    RT_decoder();

    const char * text() const;

    uint size() const;

    bool ready() const;

    private:

    void reset(uchar AB_flag,
               uchar chars_per_group);



    protected:
    virtual bool accepts(const group & g) const;

    virtual void process_impl (const group & g,
                               bool new_station);


    private:
    char txt[65];

    bool terminated;
    uint chars_per_group;
    uint last_pos;
    uchar AB_flag;
};

#endif
