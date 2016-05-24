//------------------------------------------------------------------------------
// Classes: DtedFile
//------------------------------------------------------------------------------
#ifndef __oe_terrain_DtedFile_H__
#define __oe_terrain_DtedFile_H__

#include "../DataFile.h"

namespace oe {
   namespace base { class Number; }
namespace terrain {

//------------------------------------------------------------------------------
// Class: DtedFile
//
// Description: DTED data loader.
//
// Factory name: DtedFile
// Slots:
//
//------------------------------------------------------------------------------
class DtedFile : public DataFile
{
    DECLARE_SUBCLASS(DtedFile,DataFile)

public:
    DtedFile();

protected:
   bool isVerifyChecksum() const { return verifyChecksum; }
   virtual bool setSlotVerifyChecksum(const base::Number* const msg);

private:
    // Interpret signed-magnitude values from DTED file
    static short readValue(const unsigned char hbyte, const unsigned char lbyte);
    static long readValue(const unsigned char hbyte, const unsigned char byte1, const unsigned char byte2, const unsigned char lbyte);

    // Read in cell parameters from DTED headers
    bool readDtedHeaders(std::istream& in);
    bool readDtedData(std::istream& in);

   bool loadData() override;

   bool verifyChecksum;    // verify the file checksum flag
};


} // End terrain namespace
}

#endif
