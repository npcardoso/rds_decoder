#ifndef __RT_H__
#define __RT_H__

#include "../decoder.h"

/**
 * @note See page 25, section 3.1.5.3 in the standard
 */
class RT_decoder : public RDS_decoder {
public:
    RT_decoder (bool version);

    const char * text () const;

    uint size () const;

    bool is_terminated () const;

    bool ready () const;

    virtual std::ostream & write_to (std::ostream & out) const;

protected:
    virtual bool accepts (const group & g) const;

    virtual void process_impl (const group & g,
                               bool new_station);


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