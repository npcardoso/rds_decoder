#ifndef __BASIC_H__
#define __BASIC_H__

#include "../worker.h"

class frequency_list {
    public:
    frequency_list();

    bool update(uchar af1,
                uchar af2);

    uint size() const;

    const ulong * elements() const;

    private:
    void reset(uint target_elems);

    private:
    bool LFMF;
    uint target_elems;
    uint n_elems;
    ulong elems[25];
};

/**
 * @note See page 21, section 3.1.5.1 in the standard
 */
class BASIC_decoder : public RDS_decoder {
    public:
    BASIC_decoder();

    virtual void process (const group & g);

    const char * PS_name() const;

    bool traffic_announcement () const;
    bool traffic_program() const;

    bool is_music() const;
    bool is_speech() const;

    bool is_stereo() const;
    bool is_artificial_head() const;
    bool is_compressed() const;
    bool is_static_PTY() const;

    uint af_count() const;
    const ulong * af() const;


    virtual bool ready() const;

    private:

    void reset(ushort PI_code);

    private:
    char name[9];

    bool tp_bit;
    bool ta_bit;
    bool ms_bit;

    bool stereo;
    bool artificial_head;
    bool compressed;
    bool static_PTY;

    frequency_list freq_list;
    ulong alternative_freqs[25];
    uint alternative_freqs_count;


};

#endif