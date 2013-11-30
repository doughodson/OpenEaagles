// -------------------------------------------------------------------------------
// Class: VPFDirectory
// -------------------------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_SUBCLASS(VPFDirectory,"VPFDirectory")
EMPTY_SERIALIZER(VPFDirectory)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(VPFDirectory)
    "path",                 // Path to the vpf database directory
END_SLOTTABLE(VPFDirectory)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(VPFDirectory)
    ON_SLOT(1, setSlotPath, Basic::String)
END_SLOT_MAP()

VPFDirectory::VPFDirectory()
{
    STANDARD_CONSTRUCTOR()
    path = 0;
    for (int i = 0; i < MAX_TABLES; i++) tables[i] = 0;
}


void VPFDirectory::copyData(const VPFDirectory& org, const bool cc)
{
    BaseClass::copyData(org);
    if (!cc) {
        if (path != 0) delete[] path;
    }
    path = 0;
}

void VPFDirectory::deleteData()
{
    for (int i = 0; i < MAX_TABLES; i++) {
        if (tables[i] != 0) tables[i]->unref();
        tables[i] = 0;
    }
}

void VPFDirectory::createTable(const int x)
{
    if (x < MAX_TABLES) {
        if (tables[x] == 0) tables[x] = new VPFTable();
    }
}

//------------------------------------------------------------------------------
//  setSlotPath() - sets the path to database directory
//------------------------------------------------------------------------------
bool VPFDirectory::setSlotPath(const Basic::String* const sfpobj)
{
    bool ok = true;
    if (sfpobj != 0) {
        size_t j = sfpobj->len();
        if (j > 0) {
            path = new char[j+1];
            lcStrcpy(path,j+1,*sfpobj);
        }
        else {
            std::cerr << "VPFDirectory::setPath: Invalid path value" << std::endl;
            ok = false;
        }
    }

    // ok, this is our directory, which means we should own certain table files
    loadTables();

    return ok;
}

void VPFDirectory::loadTables()
{
    // this is where our directories can load up their specific tables and directories
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* VPFDirectory::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

