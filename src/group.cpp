#include "group.h"

#include <string.h>

/**
 * @note See page 95, Annex F in the standard
 */
const std::string pty_table[32]={
	"None",
	"News",
	"Current Affairs",
	"Information",
	"Sport",
	"Education",
	"Drama",
	"Cultures",
	"Science",
	"Varied Speech",
	"Pop Music",
	"Rock Music",
	"Easy Listening",
	"Light Classics M",
	"Serious Classics",
	"Other Music",
	"Weather & Metr",
	"Finance",
	"Children’s Progs",
	"Social Affairs",
	"Religion",
	"Phone In",
	"Travel & Touring",
	"Leisure & Hobby",
	"Jazz Music",
	"Country Music",
	"National Music",
	"Oldies Music",
	"Folk Music",
	"Documentary",
	"Alarm Test",
	"Alarm-Alarm!"};


/**
 * @note See page 71, Annex D, table D.1 in the standard
 */
const std::string pi_country_codes[15][5]={
	{"DE","GR","MA","__","MD"},
	{"DZ","CY","CZ","IE","EE"},
	{"AD","SM","PL","TR","__"},
	{"IL","CH","VA","MK","__"},
	{"IT","JO","SK","__","__"},
	{"BE","FI","SY","__","UA"},
	{"RU","LU","TN","__","__"},
	{"PS","BG","__","NL","PT"},
	{"AL","DK","LI","LV","SI"},
	{"AT","GI","IS","LB","__"},
	{"HU","IQ","MC","__","__"},
	{"MT","GB","LT","HR","__"},
	{"DE","LY","YU","__","__"},
	{"__","RO","ES","SE","__"},
	{"EG","FR","NO","BY","BA"}};

/**
 * @note See page 72, Annex D, table D.2 in the standard
 */
const std::string coverage_area_codes[16]={
	"Local",
	"International",
	"National",
	"Supra-regional",
	"Regional 1",
	"Regional 2",
	"Regional 3",
	"Regional 4",
	"Regional 5",
	"Regional 6",
	"Regional 7",
	"Regional 8",
	"Regional 9",
	"Regional 10",
	"Regional 11",
	"Regional 12"};

const std::string group_acronyms[16]={
	"BASIC",
	"PIN/SL",
	"RT",
	"AID",
	"CT",
	"TDC",
	"IH",
	"RP",
	"TMC",
	"EWS",
	"___",
	"___",
	"___",
	"___",
	"EON",
	"___"};

/* see page 84, Annex J in the standard */
const std::string language_codes[44]={
	"Unkown/not applicable",
	"Albanian",
	"Breton",
	"Catalan",
	"Croatian",
	"Welsh",
	"Czech",
	"Danish",
	"German",
	"English",
	"Spanish",
	"Esperanto",
	"Estonian",
	"Basque",
	"Faroese",
	"French",
	"Frisian",
	"Irish",
	"Gaelic",
	"Galician",
	"Icelandic",
	"Italian",
	"Lappish",
	"Latin",
	"Latvian",
	"Luxembourgian",
	"Lithuanian",
	"Hungarian",
	"Maltese",
	"Dutch",
	"Norwegian",
	"Occitan",
	"Polish",
	"Portuguese",
	"Romanian",
	"Romansh",
	"Serbian",
	"Slovak",
	"Slovene",
	"Finnish",
	"Swedish",
	"Turkish",
	"Flemish",
	"Walloon"};

/* see page 12 in ISO 14819-1 */
const std::string tmc_duration[8][2]={
	{"no duration given", "no duration given"},
	{"15 minutes", "next few hours"},
	{"30 minutes", "rest of the day"},
	{"1 hour", "until tomorrow evening"},
	{"2 hours", "rest of the week"},
	{"3 hours", "end of next week"},
	{"4 hours", "end of the month"},
	{"rest of the day", "long period"}};

/* optional message content, data field lengths and labels
 * see page 15 in ISO 14819-1 */
const int optional_content_lengths[16]={3,3,5,5,5,8,8,8,8,11,16,16,16,16,0,0};

const std::string label_descriptions[16]={
	"Duration",
	"Control code",
	"Length of route affected",
	"Speed limit advice",
	"Quantifier",
	"Quantifier",
	"Supplementary information code",
	"Explicit start time",
	"Explicit stop time",
	"Additional event",
	"Detailed diversion instructions",
	"Destination",
	"RFU (Reserved for future use)",
	"Cross linkage to source of problem, or another route",
	"Separator",
	"RFU (Reserved for future use)"};



group::group(const ushort * g) {
    memcpy(this->group_buf, g, 4 * sizeof(ushort));
}

ushort group::PI_code() const {
    return bits(0);
}

uchar group::coverage_area() const {
    return bits(0, 8, 4);
}

std::string group::coverage_area_string() const {
    return coverage_area_codes[coverage_area()];
}

uchar group::group_type() const {
    return bits(1, 12, 4);
}

uchar group::group_type_version() const {
    return bits(1, 11, 1);
}

std::string group::group_type_string() const {
    return group_acronyms[group_type()];
}

uchar group::TP() const {
    return bits(1, 10, 1);
}

uchar group::PTY() const {
    return bits(1, 5, 5);
}

std::string group::PTY_string() const {
    return pty_table[PTY()];
}




ushort group::bits(uint block_id,
                   uint start_bit,
                   uint width) const {
    ushort b = group_buf[block_id];

    b >>= start_bit;
    b &= (1 << width) - 1;

    return b;
}
