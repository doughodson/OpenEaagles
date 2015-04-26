//==============================================================================
// SrtmHgtFile --
//
//     ---- Extracted from documentation files from SRTM data.
//
//
// 3.0 Data Formats
// The names of individual data tiles refer to the longitude and latitude of the lower-left (southwest)
// corner of the tile (this follows the DTED convention as opposed to the GTOPO30 standard). For
// example, the coordinates of the lower-left corner of tile N40W118 are 40 degrees north latitude
// and 118 degrees west longitude. To be more exact, these coordinates refer to the geometric center of
// the lower left sample, which in the case of SRTM3 data will be about 90 meters in extent.
//
// SRTM1 data are sampled at one arc-second of latitude and longitude and each file contains 3601
// lines and 3601 samples. The rows at the north and south edges as well as the columns at the east
// and west edges of each cell overlap and are identical to the edge rows and columns in the adjacent cell.
//
// SRTM3 data are sampled at three arc-seconds and contain 1201 lines and 1201 samples with
// similar overlapping rows and columns. This organization also follows the DTED convention.
// Unlike DTED, however, 3 arc-second data are generated in each case by 3x3 averaging of the 1
// arc-second data - thus 9 samples are combined in each 3 arc-second data point. Since the primary error
// source in the elevation data has the characteristics of random noise this reduces that error by
// roughly a factor of three.
//
// This sampling scheme is sometimes called a "geographic projection", but of course it is not actually
// a projection in the mapping sense. It does not possess any of the characteristics usually present
// in true map projections, for example it is not conformal, so that if it is displayed as an image
// geographic features will be distorted. However it is quite easy to handle mathematically,
// can be easily imported into most image processing and GIS software packages, and multiple cells
// can be assembled easily into a larger mosaic (unlike the pesky UTM projection, for example.)
//
// 3.1 DEM File (.HGT)
// The DEM is provided as 16-bit signed integer data in a simple binary raster. There are no header
// or trailer bytes embedded in the file. The data are stored in row major order
// (all the data for row 1, followed by all the data for row 2, etc.).
//
// All elevations are in meters referenced to the WGS84/EGM96 geoid as documented at http://www.NGA.mil/GandG/wgsegm/.
//
// Byte order is Motorola ("big-endian") standard with the most significant byte first.
// Since they are signed integers elevations can range from -32767 to 32767 meters,
// encompassing the range of elevation to be found on the Earth.
//
// These data also contain occasional voids from a number of causes such as shadowing,
// phase unwrapping anomalies, or other radar-specific causes. Voids are flagged with the value -32768.
//
// 4.0 Notes and Hints for SRTM Data Users
// 4.1 Data Encoding
// Because the DEM data are stored in a 16-bit binary format, users must be aware of how the bytes
// are addressed on their computers. The DEM data are provided in Motorola or IEEE byte order,
// which stores the most significant byte first ("big endian"). Systems such as Sun SPARC and
// Silicon Graphics workstations and Macintosh computers use the Motorola byte order. The Intel
// byte order, which stores the least significant byte first ("little endian"), is used on DEC Alpha
// systems and most PCs. Users with systems that address bytes in the Intel byte order may have to
// "swap bytes" of the DEM data unless their application software performs the conversion during ingest.
//
//==============================================================================

#include "openeaagles/terrain/srtm/SrtmHgtFile.h"
#include "openeaagles/basic/Number.h"

#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>

namespace Eaagles {
namespace Terrain {

//===========================================================================================
// SRTM file format definition - same as DTED minus all of the headers, footer, and checksums
//===========================================================================================


//==============================================================================
// SrtmHgtFile class
//==============================================================================

IMPLEMENT_SUBCLASS(SrtmHgtFile,"SrtmHgtFile")
EMPTY_SLOTTABLE(SrtmHgtFile)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
SrtmHgtFile::SrtmHgtFile()
{
   STANDARD_CONSTRUCTOR()
   voidValue = -32768; // Voids in a SRTM database are represented by this value
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void SrtmHgtFile::copyData(const SrtmHgtFile& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void SrtmHgtFile::deleteData()
{
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Load a SRTM cell into memory
//------------------------------------------------------------------------------
bool SrtmHgtFile::loadData()
{
    // Compute the filename (why can't we be given just one filename string!??)
    std::string temp_filename;
    const char* p = getPathname();
    if (p != nullptr)
    {
        temp_filename += p;
        temp_filename += '/';
    }
    p = getFilename();
    if (p != nullptr)
        temp_filename += p;

    std::string srtmFilename(p);

    // Open the terrain file.
    const char* filename = temp_filename.c_str();
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if ( in.fail() )
    {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "SrtmHgtFile::loadData() ERROR, could not open file: " << filename << std::endl;
        }
        return false;
    }

    in.seekg(0, std::ios::end);
    std::streamoff byteSize = in.tellg();
    in.seekg(0, std::ios::beg);
    int nameSize = static_cast<int>(srtmFilename.size());
    if (nameSize < 11 || !determineSrtmInfo(srtmFilename.substr(nameSize - 11, 11), byteSize))
    {
        clearData();
        in.close();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "SrtmHgtFile::loadData() ERROR in determining SRTM type: " << filename << std::endl;
        }
        return false;
    }

    // Read elevation data from the SRTM file
    if (! readSrtmData(in))
    {
        clearData();
        in.close();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "SrtmHgtFile::loadData() ERROR reading data from file: " << filename << std::endl;
        }
        return false;
    }

    // Close the file
    in.close();
    return true;
}

bool SrtmHgtFile::determineSrtmInfo(const std::string& srtmFilename, std::streamoff size)
{
    // Extract the number of latitude and longitude lines
    unsigned int num_lat;
    unsigned int num_lon;
    // Extract the data intervals for latitude and longitude
    switch (size) {
       case 2884802:
         latSpacing = 3.0 / 3600.0;
         lonSpacing = 3.0 / 3600.0;
         num_lat = 1201;
         num_lon = 1201;
         break;
      case 25934402:
         latSpacing = 1.0 / 3600.0;
         lonSpacing = 1.0 / 3600.0;
         num_lat = 3601;
         num_lon = 3601;
         break;
      default:
         return false;
    }

    // valid SRTM file extensions
    std::string ext1(".hgt");
    std::string ext2(".HGT");
    if (srtmFilename.substr(7, 4) != ".hgt" && srtmFilename.substr(7, 4) != ".HGT") {
        return false;
    }

    // nXXwXXX.hgt   srtm1 srtm3
    int swcLatitude = std::atoi(srtmFilename.substr(1, 2).c_str());
    int swcLongitude = std::atoi(srtmFilename.substr(4, 3).c_str());
    char ns = static_cast<char>(std::tolower(srtmFilename[0]));
    char ew = static_cast<char>(std::tolower(srtmFilename[3]));
    if ((ns != 'n' && ns != 's') || (ew != 'e' && ew != 'w')) {
        return false;
    }
    if (ns == 's') {
        swcLatitude = -swcLatitude;
    }
    if (ew == 'w') {
        swcLongitude = -swcLongitude;
    }
    setLatitudeSW( swcLatitude );
    setLongitudeSW( swcLongitude );
    setLatitudeNE( swcLatitude + 1 );
    setLongitudeNE( swcLongitude + 1 );

    nptlat = num_lat;
    nptlong = num_lon;

    return true;
}

//------------------------------------------------------------------------------
// Read elevation data from SRTM file
//------------------------------------------------------------------------------
bool SrtmHgtFile::readSrtmData(std::istream& in)
{
    if (nptlat < 1 || nptlong < 1)
    {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "SrtmHgtFile::readSrtmData: SRTM headers indicate an empty file." << std::endl;
        }
        return false;
    }

    // Allocate the elevation array
    columns = new short*[nptlong];
    for(unsigned int i=0; i<nptlong; i++)
    {
        columns[i] = new short[nptlat];
    }

    // Read the elevation array.
    for(unsigned int lat=0; lat<nptlat; lat++)
    {
        //unsigned long checksum = 0;

        // Read elevation values for record
        LCreal minElev0 = 99999.0;
        LCreal maxElev0 = 0.0;
        for(unsigned int lon=0; lon<nptlong; lon++)
        {
            unsigned char values[2];
            in.read(reinterpret_cast<char*>(values), sizeof(values));
            if (in.fail() || in.gcount() < sizeof(values))
            {
                if (isMessageEnabled(MSG_ERROR)) {
                    std::cerr << "SrtmHgtFile::readSrtmData: error reading data value." << std::endl;
                }
                return false;
            }

            short height  = readValue(values[0], values[1]);
            columns[lon][nptlat-lat-1] = height;

            // check if this is the new min or max elevation
            if (height != voidValue)
            {
                if (height < minElev0) minElev0 = height;
                if (height > maxElev0) maxElev0 = height;
            }
        }
        setMinElevation(minElev0);
        setMaxElevation(maxElev0);
    }
    return true;
}

//------------------------------------------------------------------------------
// Interpret signed-magnitude values from SRTM file
//------------------------------------------------------------------------------
short SrtmHgtFile::readValue(const unsigned char hbyte, const unsigned char lbyte)
{
    // The data is stored as 2 byte characters (sign and magnitude)
    // with high byte first.  The high bit is the sign bit.  Check for
    // sign bit and then turn it off and set SIGN_VAL accordingly.
    short height = 0;
    short sign_val = 1;
    unsigned char nhbyte = hbyte;

    if (hbyte & ~0177)
    {
        // sign bit set
        nhbyte   = hbyte & 0177;
        sign_val = -1;
    }
    height = (256 * static_cast<short>(nhbyte) + static_cast<short>(lbyte)) * sign_val;

    return height;
}

//------------------------------------------------------------------------------
// serialize() --
//------------------------------------------------------------------------------
std::ostream& SrtmHgtFile::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Terrain namespace
} // End Eaagles namespace
