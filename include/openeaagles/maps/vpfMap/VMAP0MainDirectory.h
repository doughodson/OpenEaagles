// -------------------------------------------------------------------------------
// Class: VMAP0MainDirectory
//
// Description: VMAP level 0 main directory which contains
// - dht (database header table)
// - lat (library attribute table)
// -------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VMAP0MainDirectory_H__
#define __Eaagles_Maps_Vpf_VMAP0MainDirectory_H__

#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0LibDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0ReferenceDirectory.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

class VPFTable;
class VMAP0ReferenceDirectory;
class VMPA0LibDirectory;

class VMAP0MainDirectory : public VPFDirectory
{
   DECLARE_SUBCLASS(VMAP0MainDirectory,VPFDirectory)

public:
    VMAP0MainDirectory();

    virtual void loadTables();

    VMAP0ReferenceDirectory* getReferenceDir()      { return rference; }

private:
    // Holds our reference library
    VMAP0ReferenceDirectory* rference;              // Reference library directory
    VMAP0LibDirectory* libDirectory;                // Library directory (just one for now)
};

} // End Vpf namespace
} // End Maps namespace
} // End Eaagles namespace

#endif

