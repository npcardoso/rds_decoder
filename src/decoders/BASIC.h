#ifndef __BASIC_H__
#define __BASIC_H__

#include "../worker.h"
#include "../frequency.h"

/**
 * @note See page 21, section 3.1.5.1 in the standard
 */
class BASIC_decoder : public RDS_decoder {
public:
    BASIC_decoder ();

    const char * PS_name () const;

    bool traffic_announcement () const;

    bool is_music () const;
    bool is_speech () const;

    bool is_stereo () const;
    bool is_artificial_head () const;
    bool is_compressed () const;
    bool is_static_PTY () const;

    uint af_count () const;
    const ulong * af () const;


    bool ready () const;

    std::ostream & write_to (std::ostream & out) const;

protected:
    virtual bool accepts (const group & g) const;

    virtual void process_impl (const group & g,
                               bool new_station);

private:
    void reset ();

private:
    char name[9];

    bool stereo;
    bool artificial_head;
    bool compressed;
    bool static_PTY;

    frequency_list freq_list;
    ulong alternative_freqs[25];
    uint alternative_freqs_count;
};

#endif