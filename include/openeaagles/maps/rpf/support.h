//------------------------------------------------------------------------------
// Support functions for the map library.
//
// This contains a lot of enumerated types that allow the CadrgFile to seek to 
// the right portion of the .Toc and other files, all which contain RPF data.
//
// Subroutines:
// swap() - Simple static function to swap bytes.
//      void swap(unsigned char *ptr, int count)
//
// stringToLowercase() - Takes in a string, and makes it lowercase.
//      char* stringToLowercase(char* str)
//
// parseLocations() - At this point we are at the end of the header section 
// and now we need to start reading the location data. - this function 
// takes in a file, and finds the proper locations for our component types.
//      void parseLocations(std::ifstream& fin, Location* locs, int count)
//
//------------------------------------------------------------------------------

#ifndef __oe_maps_rpf_support_H__
#define __oe_maps_rpf_support_H__

#include <fstream>

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char uchar;

namespace oe {
namespace maps {
namespace rpf {

// Swap bytes.
void swap(unsigned char* ptr, int count);
// Change string to lower case
char* stringToLowercase(char* str, int flag);

enum NitfHdrLengths {
    NITF_HDR_NONE = 0, NITF_HDR_SHORT = 413,
    NITF_HDR_LONG = 426
};

#if 0
    static const int LOC_BOUNDARIES     = 3;
    static const int LOC_FRAMES         = 4;
    static const int LOC_COVERAGE       = 6;
    static const int LOC_COMPRESSION    = 8;
    static const int LOC_CLUT           = 9;
    static const int LOC_IMAGE          = 10;
#endif

// Constants

// List of all constants needed to do any CADRG file operations.
// all values were found in MIL-STD-2411-2, section 5.1.1

static const int LOC_HEADER_SECTION                     = 128;
static const int LOC_LOCATION_SECTION                   = 129;
static const int LOC_COVERAGE_SECTION                   = 130;
static const int LOC_COMPRESSION_SECTION                = 131;
static const int LOC_COMPRESSION_LOOKUP_SUBSECTION      = 132;
static const int LOC_COMPRESSION_PARAMETER_SUBSECTION   = 133;
static const int LOC_COLORGRAY_SECTION_SUBHEADER        = 134;
static const int LOC_COLORMAP_SUBSECTION                = 135;
static const int LOC_IMAGE_DESCR_SUBHEADER              = 136;
static const int LOC_IMAGE_DISPLAY_PARAM_SUBHEADER      = 137;
static const int LOC_MASK_SUBSECTION                    = 138;
static const int LOC_COLOR_CONVERTER_SUBSECTION         = 139;
static const int LOC_SPATIAL_DATA_SUBSECTION            = 140;
static const int LOC_ATTRIBUTE_SECTION_SUBHEADER        = 141;
static const int LOC_ATTRIBUTE_SUBSECTION               = 142;
static const int LOC_EXPLICIT_AREAL_TABLE               = 143;
static const int LOC_RELATED_IMAGE_SECTION_SUBHEADER    = 144;
static const int LOC_RELATED_IMAGE_SUBSECTION           = 145;
static const int LOC_REPLACE_UPDATE_SECTION_SUBHEADER   = 146;
static const int LOC_REPLACE_UPDATE_TABLE               = 147;
static const int LOC_BOUNDARY_SECTION_SUBHEADER         = 148;
static const int LOC_BOUNDARY_RECTANGLE_TABLE           = 149;
static const int LOC_FRAME_FILE_INDEX_SUBHEADER         = 150;
static const int LOC_FRAME_FILE_INDEX_SUBSECTION        = 151;
static const int LOC_COLOR_TABLE_SECTION_SUBHEADER      = 152;
static const int LOC_COLOR_TABLE_INDEX_RECORD           = 153;

// Structures - List of all structured needed to hold any data or operation status.

// Header structure - contains all the information included in the TOC header (and other data files 
// following the RPF data structure).
struct Header {
    bool endian;               // Little / big endian indicator
    ushort hdrSectionLength;   // Header section length (bytes)
    char filename[12];         // Filename
    uchar nruInd;              // New/replacement/update indicator
    char govSpecNum[15];       // Governing specification number
    char govSpecdate[8];       // Governing specification date
    char secClass;             // Security classification     
    char secCountryCode[2];    // Security country/international code
    char secRelease[2];        // Security release markings
    uint locSecLoc;            // location section location (where the frame files are)
};

// Location structure - Holds the length of our components (ie, the boundary table, frame table, etc..) based on the 
// component ID (see definitions above), and also the physical location of the components' in the file.
struct Location {
    ushort componentId;
    uint componentLength;
    uint physicalIdx;
};

// Subframe structure - For decompressing the tile images (used by cadrg Frame)
struct Subframe {
    unsigned char image[256][256];
};

// Compression structure - Holds our image compression data.
struct Compression {
    ushort algorithm;     // Algorithm used for compression
    ushort nOffRecs;      // Number of offset records
    ushort nParmOffRecs;  // Number parameter offset records
};

// LookupTable structure - Holds our lookup table.
struct LookupTable {
    ushort id;          // Table ID
    uint  records;      // Num records
    ushort values;      // Value of record
    ushort bitLength;   // Bit length
    uint physOffset;    // Physical offset
};

// Image structure - Holds our image info.
struct Image {
    ushort spectralGroups;
    ushort subframeTables;
    ushort spectralTables;
    ushort spectralLines;
    ushort horizSubframes;
    ushort vertSubframes;
    uint outputCols;
    uint outputRows;
};

// Support functions

// parseLocations() - Reads the proper locations in for the CADRG files.
void parseLocations(std::ifstream& fin, Location* locs, int count);


}
}
}

#endif
