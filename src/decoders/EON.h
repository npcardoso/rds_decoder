#ifndef __EON_H_00073331efdd74a8fd8440d203bf29baf9337dc4__
#define __EON_H_00073331efdd74a8fd8440d203bf29baf9337dc4__

#include "../worker.h"

#include <map>

class EON_decoder;

class other_network {
    public:
    other_network();

    ushort PI_code() const;

    const char * PS_name() const;

    bool traffic_announcement() const;
    bool traffic_program() const;

    uchar program_type() const;

    ushort program_item_number () const;

    /**
     * @return A pointer to a 12 position array where positions 0 and
     * 1 hold Method A AFs and positions 2--11 hold Method B Afs
     */
    const uint * alternative_frequencies() const;

    private:
    ushort PI;
    uchar PTY;
    bool TA;
    bool TP;
    ushort PIN;
    char name[9];
    uint alternative_freqs[12];
    ushort linkage_info;
    friend class EON_decoder;
};

/**
 * @note See page 38, section 3.1.5.19 in the standard
 */
class EON_decoder : public RDS_decoder {
    public:

    public:
    EON_decoder();

    const std::map<ushort, other_network> & other_networks () const;

    std::ostream & write_to (std::ostream & out) const;

    protected:
    virtual bool accepts(const group & g) const;

    virtual void process_impl (const group & g,
                               bool new_station);

    private:
    void reset();

    private:
    std::map<ushort, other_network> ONs;
};
#endif
