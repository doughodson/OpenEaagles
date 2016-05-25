//------------------------------------------------------------------------------
// Classes: SrtmHgtFile
//------------------------------------------------------------------------------
#ifndef __oe_terrain_SrtmHgtFile_H__
#define __oe_terrain_SrtmHgtFile_H__

#include "../DataFile.h"

namespace oe {
   namespace base { class Number; }
namespace terrain {

//------------------------------------------------------------------------------
// Class: SrtmHgtFile
//
// Description: SRTM data loader.
//
// Adapted from terrain::DtedFile
//
// OpenEaagles version by Christopher Buell
//
// obtain SRTM data files from ftp://e0srp01u.ecs.nasa.gov/srtm/version2
//
// This class read 2 types of SRTM elevation data sets.
// SRTM3 - 3 arc second intervals, equivalent resolution to DTED Level 1
// SRTM1 - 1 arc second intervals, equivalent resolution to DTED Level 2
// Only the elevation data is contained in the data set file.
// The other metadata is interpreted from the file name and file size.
// The SRTM file name are of the format...
//     N00W000.hgt
//     N00E000.hgt
//     S00W000.hgt
//     S00E000.hgt
// where N00/S00 is the latitude of the SW corner
// and W000/E000 is the longitude of the SW corner
// N/S and E/W may be upper or lower case.
// The file extension must be ".hgt" or ".HGT".
// SRTM1 - will have a file size of 25934402 bytes.
// SRTM3 - will have a file size of 2884802 bytes.
// A file of any other size will fail.
//
// A file name must have at least 11 characters
// A file name may have addition path prepended.
// Only the last 11 characters of the file name will be parse for the SW corner.
//
// Factory name: SrtmHgtFile
// Slots:
//   N/A
//
//------------------------------------------------------------------------------
class SrtmHgtFile : public DataFile
{
    DECLARE_SUBCLASS(SrtmHgtFile,DataFile)

public:
    SrtmHgtFile();

private:
    // Interpret signed-magnitude values from SRTM file
    static short readValue(const unsigned char hbyte, const unsigned char lbyte);

    bool readSrtmData(std::istream& in);
    bool determineSrtmInfo(const std::string& srtmFilename, std::streamoff size);

    virtual bool loadData() override;
};


}
}

#endif
