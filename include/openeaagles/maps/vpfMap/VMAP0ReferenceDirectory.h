// -------------------------------------------------------------------------------
// Class: VMAP0ReferenceDirectory
//
// Description: VMAP Level 0 reference library, contains tables
// - cat
// - lat
// - grt
// - dqx
// - dqt
// - lineage.doc
// and other libraries
// -------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VMAP0ReferenceDirectory_H__
#define __Eaagles_Maps_Vpf_VMAP0ReferenceDirectory_H__

#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0RefCoverageDirectory.h"

namespace Eaagles {
namespace Basic { class PairStream; }
namespace Maps {
namespace Vpf {

class VPFTable;
class VMAP0RefCoverageDirectory;

class VMAP0ReferenceDirectory : public VPFDirectory {
   DECLARE_SUBCLASS(VMAP0ReferenceDirectory,VPFDirectory)

public:
    VMAP0ReferenceDirectory();

    virtual void loadTables();

    // Defined coverages for VMAP level 0 reference directories
    enum { CVG_DBREF = 0, CVG_LIBREF, CVG_PLACENAM, CVG_POLBND, MAX_COVERAGES };

    // Get functions
    // Get a specific coverage (VMAP0 has 4)
    VMAP0RefCoverageDirectory* getCoverage(const int cov);

private:
    VMAP0RefCoverageDirectory* coverages[MAX_COVERAGES];
};

} // End Vpf namespace
} // End Maps namespace
} // End Eaagles namespace

#endif

