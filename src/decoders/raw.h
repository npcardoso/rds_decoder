#ifndef __RAW_H__
#define __RAW_H__

#include "../worker.h"
#include "../frequency.h"

/**
 * This decoder doesn't perform any operations. It is only used to
 * print the group's bits.
 */
class raw_decoder : public RDS_decoder {
public:
    raw_decoder (uchar group_type,
                 bool group_type_version);

    std::ostream & write_to (std::ostream & out) const;

protected:
    virtual bool accepts (const group & g) const;

    inline virtual void process_impl (const group & g,
                                      bool new_station) {}

private:
    uchar group_type;
    bool group_type_version;
};

#endif