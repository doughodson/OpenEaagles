// ------------------------------------------------------------
// Class: VPFIndexTable
//
// Description: Associated Index table with all tables which have
// variable length text or coordinate strings
// ------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VPFIndexTable_H__
#define __Eaagles_Maps_Vpf_VPFIndexTable_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/maps/vpfMap/VPFDataType.h"

namespace Eaagles {
namespace Basic { class String; }
namespace Maps {
namespace Vpf {

class VPFDataType;

class VPFIndexTable : public Basic::Object {
   DECLARE_SUBCLASS(VPFIndexTable,Basic::Object)

public:
    VPFIndexTable();

    virtual void loadIndexTableFromFile(const char* pathname, const char* filename, const int t = -1);
    virtual void getRecordPosition(const int idx, int& offset, int& length);

    int getNumRecords()     { return numEntries; }
    bool isLoaded()         { return loaded; }

private:
    int type;
    int numEntries;
    int numBytes;
    int recordSize;
    bool loaded;
    Basic::String* fullPath;
};

} // End Vpf namespace
} // End Maps namespace
} // End Eaagles namespace

#endif

