// -------------------------------------------------------------------------------
// Class: VMAP0LibDirectory
// Description: VMAP level 0 library directory which contains
// 
// - lat (library attribute table)

// Subroutines:
// loadTables() - 
//      void VMAP0LibDirectory::loadTables()

// -------------------------------------------------------------------------------
#ifndef	__Eaagles_Maps_Vpf_VMAP0LibDirectory_H__
#define __Eaagles_Maps_Vpf_VMAP0LibDirectory_H__

#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0RefCoverageDirectory.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

class VPFTable;
class VMAP0RefCoverageDirectory;

class VMAP0LibDirectory : public VPFDirectory {
   DECLARE_SUBCLASS(VMAP0LibDirectory,VPFDirectory)

public:
    VMAP0LibDirectory();

    virtual void loadTables();

private:
    // Holds table information for this directory
    static const int MAX_COVERAGES = 15;

    VMAP0RefCoverageDirectory* coverages[MAX_COVERAGES];
    int numCoverages;
};

}; // End Vpf namespace
}; // End Maps namespace 
}; // End Eaagles namespace

#endif
