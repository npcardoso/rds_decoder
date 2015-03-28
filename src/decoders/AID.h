#ifndef __AID_H__
#define __AID_H__

#include "../decoder.h"

/**
 * @note See page 27, section 3.1.5.4 in the standard
 */
class AID_decoder : public RDS_decoder {
public:
    uchar application_group_type_code () const;

    uint message_bits () const;

    uint application_id () const;

    std::ostream & write_to (std::ostream & out) const;

protected:
    virtual bool accepts (const group & g) const;

    inline virtual void process_impl (const group & g,
                                      bool new_station) {}
};

#endif