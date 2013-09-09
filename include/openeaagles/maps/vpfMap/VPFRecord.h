// -------------------------------------------------------------------------------
// Class: VPFRecord
//
// Description: This is a single fixed length or variable length object that
// will store a list of VPFDataType values for quick access.
// -------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VPFRecord_H__
#define __Eaagles_Maps_Vpf_VPFRecord_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/maps/vpfMap/VPFTable.h"

namespace Eaagles {
   namespace Basic { class String; }

namespace Maps {
namespace Vpf {

class VPFDataType;

class VPFRecord : public Basic::Object
{
   DECLARE_SUBCLASS(VPFRecord,Basic::Object)

public:
    VPFRecord();

    static const int MAX_COORDS = 500;

    virtual void createRecord(VPFTable* x, const char* file, const int idx);
    
    const char* getData(const int column);

    int getCoordinate(const int column, osg::Vec3 vec[], const int idx, const int max);

    bool isEOR()                { return eor; }

private:
    VPFTable* parent;                          // Our parent table (for header information and column information)
    Basic::String* filename;                   // Name of the file we are going to access
    VPFDataType* data[VPFTable::MAX_COLUMNS];  // Data type
    int index;                                 // Our index number, which we will need for accessing data
    bool eor;                                  // End of record
    int numCoords;                             // Number of coordinates we have
};

} // End Vpf namespace
} // End Maps namespace 
} // End Eaagles namespace

#endif

