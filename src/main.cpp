#include "decoders/RT.h"
#include "decoders/BASIC.h"
#include "decoders/PIN.h"
#include "decoders/EON.h"
#include "constants.h"

#include <iostream>
#include <cassert>
#include <bitset>

#include <cstring>

using namespace std;




class RDS_verbose : public RDS_decoder {
    public:
    RT_decoder rt;
    BASIC_decoder basic;
    PIN_decoder pin;
    EON_decoder eon;

    virtual bool accepts(const group & g) const {
        return true;
        return g.group_type() == 0x2 ||
            g.group_type() != 0x1 ||
            g.group_type() != 0x0;
    }

    virtual void process_impl (const group & g,
                               bool new_station){
        cout << "------------------------" << endl;
        cout << (int) g.group_type() << (g.group_type_version()?"B":"A");
        cout << " (" << g.group_type_string() << ") | ";
        cout << std::bitset<5> (g.bits(1, 0, 5)) << " | ";
        for(int i = 2; i < 4; i++) {
            for(int j = 2; j--;) {
              uchar b = g.bits(i, 8 * j, 8);
              cout << std::bitset<8>(b) << "(";
              cout.fill('0');
              cout.width(2);
              cout << hex<< (int)b << ")";
              cout << "(" << (char)(isprint(b)?b:'*') << ")";
          }
          cout << " | ";
        }
        cout << endl;
        cout << dec;

        if(rt.process(g)) {
            rt.write_to(cout);
        }
        else if(basic.process(g)) {
            basic.write_to(cout);
        }
        else if(pin.process(g)) {
            pin.write_to(cout);
        }
        else if(eon.process(g)) {
            eon.write_to(cout);
        }

    }


    virtual void sync() {
        cerr << "synced!" << endl;
    }

    virtual void no_sync() {
        cerr << "lost sync!" << endl;
    }

    virtual void rx_error() {
        cerr << "rx_error!" << endl;
    }


};

int main(){
    RDS_worker worker;
    RDS_verbose decoder;
    worker.run(cin, decoder);

    return 0;
}
