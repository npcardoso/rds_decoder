#ifndef __PIN_H__
#define __PIN_H__

#include "../worker.h"

/**
 * @note See page 23, section 3.1.5.2 in the standard
 */
class PIN_decoder : public RDS_decoder {
    public:
    PIN_decoder();

    uchar day() const;
    uchar hour() const;
    uchar minute() const;

    ushort paging() const;
    ushort extended_country_code() const;
    ushort TMC_identification() const;
    ushort paging_id() const;
    ushort language_codes() const;
    ushort broadcaster_codes() const;
    ushort EWS_channel_id() const;

    bool linkage_actuator() const;

    uchar radio_paging_codes() const;

    protected:
    virtual bool accepts(const group & g) const;

    virtual void process_impl (const group & g,
                               bool new_station);

    private:

    void reset();

    private:
    uchar PIN[3];
    ushort slow_labelling[8];
};

#endif
