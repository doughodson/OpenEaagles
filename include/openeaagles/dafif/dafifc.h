//------------------------------------------------------------------------------
// DAFIF Support Utilities and Data 
//------------------------------------------------------------------------------
#ifndef __oe_Dafif_dafifc_H__
#define __oe_Dafif_dafifc_H__

namespace oe {
namespace Dafif {

// Convert MHz to KHz and HHz/
#define MHZ2KHZ(mhz) (mhz*1000.0f)
#define MHZ2HHZ(mhz) (static_cast<int>(mhz*10000.0f+0.5f))

// Convert KHz to MHz and HHz
#define KHZ2MHZ(khz) (khz/1000.0f)
#define KHZ2HHZ(khz) (static_cast<int>(khz*10.0f+0.5f))

// Convert HHz to MHz and KHz
#define HHZ2MHZ(hhz) (static_cast<float>(hhz/10000.0f))
#define HHZ2KHZ(hhz) (static_cast<float>(hhz/10.0f))

// Compare frequencies at HHz level
#define CMP_FREQ(mhz1,mhz2) (MHZ2HHZ(mhz1)==MHZ2HHZ(mhz2))


//------------------------------------------------------------------------------
// Define the DAFIF records 
//------------------------------------------------------------------------------

// DAFIF record field positions and lengths that are common to all records 
const int FORMAT_CODE_POS       = 1;
const int FORMAT_CODE_LEN       = 2;
const int TRANSACTION_CODE_POS  = 3;
const int TRANSACTION_CODE_LEN  = 1;
const int ICAO_CODE_POS         = 4;
const int ICAO_CODE_LEN         = 4;
const int RECORD_NUMBER_POS     = 133;
const int RECORD_NUMBER_LEN     = 6;
const int CYCLE_DATE_POS        = 139;
const int CYCLE_DATE_LEN        = 4;


// Positions and lengths of fields that are common to AIRPORT records 
const int AIRPORT_RECORD_LEN   = 142;
const int AP_KEY_POS           = 8;
const int AP_KEY_LEN           = 7;
const int AP_IDENT_POS         = 55;
const int AP_IDENT_LEN         = 4;
const int AP_NAME_POS          = 15;
const int AP_NAME_LEN          = 38;
const int AP_TYPE_POS          = 108;
const int AP_TYPE_LEN          = 1;
const int AP_CCODE_POS         = 8;
const int AP_CCODE_LEN         = 2;
const int AP_STATE_POS         = 53;
const int AP_STATE_LEN         = 2;
const int AP_WAC_POS           = 121;
const int AP_WAC_LEN           = 4;
const int AP_LATITUDE          = 59;
const int AP_LONGITUDE         = 68;
const int AP_ELEVATION         = 103;
const int AP_MAGVAR            = 109;


// Positions and lengths of fields that are common to RUNWAY records 
const int RUNWAY_RECORD_LEN     = 284;
const int RW_LE_IDENT_POS       = 18;
const int RW_LE_LATITUDE_POS    = (142+21);
const int RW_LE_LONGITUDE_POS   = (142+30);
const int RW_LE_ELEVATION_POS   = (142+59);
const int RW_LE_SLOPE_POS       = (142+65);
const int RW_LE_MAGHDG_POS      = 25;

const int RW_HE_IDENT_POS       = 15;
const int RW_HE_LATITUDE_POS    = 49;
const int RW_HE_LONGITUDE_POS   = 58;
const int RW_HE_ELEVATION_POS   = 87;
const int RW_HE_SLOPE_POS       = 93;
const int RW_HE_MAGHDG_POS      = 21;

const int RW_XE_IDENT_LEN       = 3;
const int RW_XE_SLOPE_LEN       = 3;
const int RW_XE_MAGHDG_LEN      = 4;

const int RW_KEY_POS     = 8;
const int RW_KEY_LEN     = 13;
const int RW_APKEY_POS   = 8;
const int RW_IDENT_POS   = 15;
const int RW_IDENT_LEN   = 6;
const int RW_NAME_POS    = 0;
const int RW_NAME_LEN    = 0;
const int RW_TYPE_POS    = 0;
const int RW_TYPE_LEN    = 0;
const int RW_CCODE_POS   = 0;
const int RW_CCODE_LEN   = 0;
const int RW_STATE_POS   = 0;
const int RW_STATE_LEN   = 0;
const int RW_WAC_POS     = 0;
const int RW_WAC_LEN     = 0;
const int RW_LATITUDE    = RW_HE_LATITUDE_POS;
const int RW_LONGITUDE   = RW_HE_LONGITUDE_POS;
const int RW_ELEVATION   = RW_HE_ELEVATION_POS;
const int RW_MAGVAR      = 0;
const int RW_WIDTH_POS   = 34;
const int RW_WIDTH_LEN   = 5;
const int RW_LENGTH_POS  = 29;
const int RW_LENGTH_LEN  = 5;


// Positions and lengths of fields that are common to ILS records 
const int ILS_RECORD_LEN    = 284;
const int ILS_KEY_POS       = 8;
const int ILS_KEY_LEN       = 11;
const int ILS_IDENT_POS     = (142+38);
const int ILS_IDENT_LEN     = 5;
const int ILS_NAME_POS      = 21;
const int ILS_NAME_LEN      = 38;
const int ILS_TYPE_POS      = 18;
const int ILS_TYPE_LEN      = 1;
const int ILS_CLTYPE_POS    = 19;
const int ILS_CLTYPE_LEN    = 1;
const int ILS_CCODE_POS     = 0;
const int ILS_CCODE_LEN     = 0;
const int ILS_STATE_POS     = 0;
const int ILS_STATE_LEN     = 0;
const int ILS_WAC_POS       = 0;
const int ILS_WAC_LEN       = 0;
const int ILS_LATITUDE      = 90;
const int ILS_LONGITUDE     = 99;
const int ILS_ELEVATION     = 85;
const int ILS_MAGVAR        = (142+48);
const int ILS_CHANNEL_POS    = 67;
const int ILS_CHANNEL_LEN    = 3;
const int ILS_FREQUENCY_POS  = 59;
const int ILS_FREQUENCY_LEN  = 8;
const int ILS_GSA_POS        = 71;
const int ILS_GSA_LEN        = 3;
const int ILS_LOC_POS        = 74;
const int ILS_LOC_LEN        = 6;
const int ILS_APKEY_POS      = ILS_KEY_POS;
const int ILS_RW_ID_POS      = ILS_KEY_POS;
const int ILS_RW_ID_LEN      = 10;


// Positions and lengths of fields that are common to NAVAID records
const int NAVAID_RECORD_LEN = 284;
const int NA_KEY_POS        = 8;
const int NA_KEY_LEN        = 9;
const int NA_IDENT_POS      = 8;
const int NA_IDENT_LEN      = 4;
const int NA_NAME_POS       = 19;
const int NA_NAME_LEN       = 38;
const int NA_TYPE_POS       = 12;
const int NA_TYPE_LEN       = 1;
const int NA_CCODE_POS      = 13;
const int NA_CCODE_LEN      = 2;
const int NA_STATE_POS      = 17;
const int NA_STATE_LEN      = 2;
const int NA_WAC_POS        = 57;
const int NA_WAC_LEN        = 4;
const int NA_LATITUDE       = 95;
const int NA_LONGITUDE      = 104;
const int NA_ELEVATION      = (142+51);
const int NA_MAGVAR         = (142+39);
const int NA_SLAVE_VAR_POS   = (142+30);
const int NA_KEY_CODE_POS    = 15;
const int NA_KEY_CODE_LEN    = 2;
const int NA_CHANNEL_POS     = 70;
const int NA_CHANNEL_LEN     = 3;
const int NA_CHANNEL_TYPE_POS = 73;
const int NA_FREQUENCY_POS   = 61;
const int NA_FREQUENCY_LEN   = 8;
const int NA_POWER_POS       = 88;
const int NA_POWER_LEN       = 4;
const int NA_RANGE_POS       = 92;
const int NA_RANGE_LEN       = 3;
const int NA_RADIO_CLASS_POS = 74;
const int NA_RADIO_CLASS_LEN = 5;


// Positions and lengths of fields that are common to WAYPOINT records
const int WAYPOINT_RECORD_LEN  = 142;
const int WP_KEY_POS    =  8;
const int WP_KEY_LEN    =  10;
const int WP_IDENT_POS  =  8;
const int WP_IDENT_LEN  =  8;
const int WP_NAME_POS   = 24;
const int WP_NAME_LEN   = 25;
const int WP_TYPE_POS   = 21;
const int WP_TYPE_LEN   = 3;
const int WP_CCODE_POS  = 16;
const int WP_CCODE_LEN  = 2;
const int WP_STATE_POS  = 18;
const int WP_STATE_LEN  = 2;
const int WP_WAC_POS    = 58;
const int WP_WAC_LEN    = 4;
const int WP_LATITUDE   = 62;
const int WP_LONGITUDE  = 71;
const int WP_ELEVATION  = 0;
const int WP_MAGVAR     = 106;

} // End Dafif namespace
} // End oe namespace

#endif
