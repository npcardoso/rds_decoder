#include "decoders/RT.h"
#include "decoders/BASIC.h"
#include "decoders/PIN.h"
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

    virtual bool accepts(const group & g) const {
        return true;
        return g.group_type() == 0x2 ||
            g.group_type() != 0x1 ||
            g.group_type() != 0x0;
    }

    virtual void process_impl (const group & g,
                               bool new_station){
        cout << "------------------------" << endl;
        //cout << "PI: " << std::bitset<16> (g.PI_code());
        //cout << " | ";
        //cout << "Coverage: " << g.coverage_area_string();
        //cout << " | ";

        //cout << "Gtype: " << std::bitset<4> (g.group_type()) << (int)g.group_type_version();
        cout << "(" << g.group_type_string() << ")";
        //cout << " | ";

        //cout << "TP: " << (int)g.TP();
        //cout << " | ";

        //cout << "PTY: " << std::bitset<5> (g.PTY());
        //cout << " (" << g.PTY_string() << ")";
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
        if(rt.process(g) && rt.ready())
            cout << rt.text() << "  || " << rt.size() << ", " << rt.ready() << endl;


        if(basic.process(g) && basic.ready()) {
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

        if(pin.process(g)) {
            cout << "PIN (" << (int) pin.day();
            cout << ", " << (int) pin.hour();
            cout << ", " << (int) pin.minute();
            cout << ") ";
            cout << ", paging: " << (int) pin.paging();
            cout << ", extended_country_code: " << (int) pin.extended_country_code();
            cout << ", TMC_identification: " << (int) pin.TMC_identification();
            cout << ", paging_id: " << (int) pin.paging_id();
            cout << ", language_codes: " << (int) pin.language_codes();
            cout << " (" << language_codes[pin.language_codes()] << ")";

            cout << ", broadcaster_codes: " << (int) pin.broadcaster_codes();
            cout << ", EWS_channel_id: " << (int) pin.EWS_channel_id();
            cout << ", linkage_actuator: " << (int) pin.linkage_actuator();

            cout << endl;


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
