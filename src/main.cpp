#include "decoders/RT.h"
#include "decoders/BASIC.h"
#include "decoders/PIN.h"

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

    virtual void process (const group & g){
/*        if(g.group_type() != 0x2 &&
           g.group_type() != 0x1 &&
           g.group_type() != 0x0)
            return;
*/
        cout << "PI: " << std::bitset<16> (g.PI_code());
        cout << " | ";
        cout << "Coverage: " << g.coverage_area_string();
        cout << " | ";

        cout << "Gtype: " << std::bitset<4> (g.group_type()) << (int)g.group_type_version();
        cout << " (" << g.group_type_string() << ")";
        cout << " | ";

        cout << "TP: " << (int)g.TP();
        cout << " | ";

        cout << "PTY: " << std::bitset<5> (g.PTY());
        cout << " (" << g.PTY_string() << ")";
        cout << " | ";

        cout << std::bitset<5> (g.bits(1, 0, 5));
        cout << " | ";

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
        rt.process(g);
        if(rt.ready())
            cout << rt.text() << "  || " << rt.size() << ", " << rt.ready() << endl;


        basic.process(g);
        if(basic.ready()) {
            cout << basic.PS_name() << " ||";
            cout << " stereo: " << basic.is_stereo();
            cout << ", artificial_head: " << basic.is_artificial_head();
            cout << ", compressed: " << basic.is_compressed();
            cout << ", static_pty: " << basic.is_static_PTY();
            cout << ", TA: " << basic.traffic_announcement();
            cout << ", TP: " << basic.traffic_program();
            cout << ", AFs: " << (int)basic.af_count() << " (";
            for(int i = 0; i < basic.af_count(); i++)
                cout << " "<< basic.af()[i];
            cout << ")" <<endl;
        }

        pin.process(g);
        if(pin.ready()) {
            cout << "PIN (day: " << pin.day();
            cout << ", " << pin.hour();
            cout << ", " << pin.minute();
            cout << ")" << endl;


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
