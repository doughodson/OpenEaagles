// --------------------------------------------------------------
// Class: VPFSpatialIndexTable
// Descr: Spacial index file which contains the minimum bounding 
// rectangle (mbr) information about each primitive.  This allows
// for quick referencing of primitives by area
// 
// ---------------------------------------------------------------
#ifndef	__Eaagles_Maps_Vpf_VPFSpatialIndexTable_H__
#define __Eaagles_Maps_Vpf_VPFSpatialIndexTable_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
   namespace Basic { class String; }
namespace Maps {
namespace Vpf {

class VPFSpatialIndexTable : public Basic::Object {
   DECLARE_SUBCLASS(VPFSpatialIndexTable,Basic::Object)

public:
    VPFSpatialIndexTable();

    virtual void loadIndexTableFromFile(const char* pathname, const char* filename, const int t = -1);

    bool isLoaded()         { return loaded; }

    int findPrimitivesBySpatialQuery(const float lat, const float lon, int primIds[], const float width = 0, const float height = 0);

private:
    void convertDegsToSpatialPoint(const float lat, const float lon, int& x, int& y);
    int type;
    int numPrims;
    float mbrX1;
    float mbrX2;
    float mbrY1;
    float mbrY2;
    int numNodes;
    // Offset from the beginning of our header (24 bytes)
    int headerByteOffset;
    // Size of our bin (numNodes * 8 bytes)
    int binSize;
    bool loaded;
    Basic::String* fullPath;
};

}; // End Vpf namespace
}; // End Maps namespace 
}; // End Eaagles namespace

#endif
