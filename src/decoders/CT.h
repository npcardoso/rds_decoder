#ifndef __CT_H__
#define __CT_H__

#include "../decoder.h"

/**
 * @note See page 28, section 3.1.5.6 in the standard
 */
class CT_decoder : public RDS_decoder {
public:
    float local_time_offset () const;

    uint modified_julian_date (bool local_time) const;

    /**
     * @note See page 104, annex G in the standard
     */
    uint day (bool local_time=true) const;

    /**
     * @note See page 104, annex G in the standard
     */
    uint month (bool local_time=true) const;

    /**
     * @note See page 104, annex G in the standard
     */
    uint year (bool local_time=true) const;

    uint hour (bool local_time=true) const;
    uint minute (bool local_time=true) const;


    /**
     * @note See page 104, annex G in the standard
     * @return 0 -> Monday, 6 -> Sunday
     */
    uint week_day (bool local_time=true) const;

    /**
     * @note See page 104, annex G in the standard
     */
    uint week_number (bool local_time=true) const;

    /**
     * @note See page 104, annex G in the standard
     */
    uint week_number_year (bool local_time=true) const;


    inline virtual std::ostream & write_to (std::ostream & out) const {
        return write_to(out, true);
    }

    std::ostream & write_to (std::ostream & out,
                             bool local_time) const;

protected:
    virtual bool accepts (const group & g) const;

    inline virtual void process_impl (const group & g,
                                      bool new_station) {}


private:
    void reset ();

    inline uint chars_per_group () const {
        return version ? 2 : 4;
    }

    char txt[65];

    bool version;
    bool terminated;
    uint last_pos;
};

#endif