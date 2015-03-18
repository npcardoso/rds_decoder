#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <string>

/**
 * @note See page 95, Annex F in the standard
 */
extern const std::string pty_table[32];

/**
 * @note See page 71, Annex D, table D.1 in the standard
 */
extern const std::string pi_country_codes[15][5];

/**
 * @note See page 72, Annex D, table D.2 in the standard
 */
extern const std::string coverage_area_codes[16];

extern const std::string group_acronyms[16];

/* see page 84, Annex J in the standard */
extern const std::string language_codes[44];

/* see page 12 in ISO 14819-1 */
extern const std::string tmc_duration[8][2];

extern const std::string label_descriptions[16];

#endif
