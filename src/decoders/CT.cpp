#include "CT.h"

#include "../constants.h"

bool CT_decoder::accepts (const group & g) const {
    return g.group_type() == 0x4 &&
           g.group_type_version() == 0;
}

float CT_decoder::local_time_offset () const {
    return (last_group().bits(3, 5, 1) ? 1 : -1) *
           last_group().bits(3, 0, 5) / 2;
}

uint CT_decoder::modified_julian_date (bool local_time) const {
    return last_group().bits(2, 1, 15) +
           (last_group().bits(1, 0, 2) << 15) +
           (local_time ? local_time_offset() : 0);
}

uint CT_decoder::day (bool local_time) const {
    int mjd = modified_julian_date(local_time);
    int y = ((mjd - 15078.2) / 365.25);
    int m = int((mjd - 14956.1 - int(y * 365.25)) / 30.6001);


    return mjd - 14956 - int(y * 365.25) - int(m * 30.6001);
}

uint CT_decoder::month (bool local_time) const {
    int mjd = modified_julian_date(local_time);
    int y = (mjd - 15078.2) / 365.25;
    int m = (mjd - 14956.1 - int(y * 365.25)) / 30.6001;
    int K = ((m == 14) || (m == 15)) ? 1 : 0;


    return m - 1 + K * 12;
}

uint CT_decoder::year (bool local_time) const {
    int mjd = modified_julian_date(local_time);
    int y = (mjd - 15078.2) / 365.25;
    int m = (mjd - 14956.1 - int(y * 365.25)) / 30.6001;
    int K = ((m == 14) || (m == 15)) ? 1 : 0;


    return 1900 + y + K;
}

uint CT_decoder::hour (bool local_time) const {
    return (last_group().bits(2, 0, 1) << 4) +
           last_group().bits(3, 12, 4) +
           (local_time ? local_time_offset() : 0);
}

uint CT_decoder::minute (bool local_time) const {
    return last_group().bits(3, 6, 6) +
           ((local_time ? local_time_offset() : 0) * 30);
}

uint CT_decoder::week_day (bool local_time) const {
    return (modified_julian_date(local_time) + 2) % 7;
}

uint CT_decoder::week_number_year (bool local_time) const {
    int mjd = modified_julian_date(local_time);
    int w = (mjd / 7) - 2144.64;


    return (w * 28 / 1461.0) - 0.0079;
}

uint CT_decoder::week_number (bool local_time) const {
    int mjd = modified_julian_date(local_time);
    int w = (mjd / 7) - 2144.64;
    int wy = (w * 28 / 1461.0) - 0.0079;


    return w - int((wy * 1461 / 28.0) + 0.41);
}

std::ostream & CT_decoder::write_to (std::ostream & out,
                                     bool local_time) const {
    out << group_acronyms[4] << ":" << std::endl;
    out << "\tmodified julian date (" << (local_time ? "local" : "UTC") << "): " <<
    (int) modified_julian_date(local_time) << std::endl;
    out << "\tlocal time offset: " << local_time_offset() << std::endl;
    out << "\tyear: " << (int) year() << std::endl;
    out << "\tmonth: " << (int) month() << std::endl;
    out << "\tday: " << (int) day() << std::endl;

    out << "\thour: " << (int) hour() << std::endl;
    out << "\tminute: " << (int) minute() << std::endl;

    out << "\tweek day: " << (int) week_day() << std::endl;
    out << "\tweek number: " << (int) week_number() << std::endl;
    out << "\tweek number year: " << (int) week_number_year() << std::endl;

    out << "\tminute: " << (int) minute() << std::endl;

    return out;
}