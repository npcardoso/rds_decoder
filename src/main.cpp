#include "decoders/RT.h"
#include "decoders/BASIC.h"
#include "decoders/PIN.h"
#include "decoders/EON.h"
#include "decoders/raw.h"
#include "constants.h"

#include <iostream>
#include <cassert>
#include <list>

#include <memory>

using namespace std;


class verbose_decoder : public RDS_decoder {
public:

    list<std::shared_ptr<RDS_decoder> > decoders;

protected:

    virtual bool accepts (const group & g) const {
        return true;
    }

    virtual void process_impl (const group & g,
                               bool new_station) {
        cout << "------------------------------------------------------------------------" << endl;

        for (auto & dec : decoders) {
            if (dec->process(g))
                dec->write_to(cout);
        }
    }

    virtual void sync () {
        cerr << "synced!" << endl;
    }

    virtual void no_sync () {
        cerr << "lost sync!" << endl;
    }

    virtual void rx_error () {
        cerr << "rx_error!" << endl;
    }
};


int main () {
    RDS_worker worker;
    verbose_decoder decoder;


    for (int i = 0; i < 16; i++) {
        decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new raw_decoder(i, 0)));
        decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new raw_decoder(i, 1)));
    }

    decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new RT_decoder(0)));
    decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new RT_decoder(1)));
    decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new BASIC_decoder()));
    decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new EON_decoder()));
    decoder.decoders.push_back(std::shared_ptr<RDS_decoder> (new PIN_decoder()));


    worker.run(cin, decoder);

    return 0;
}