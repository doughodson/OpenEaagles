//------------------------------------------------------------------------------
// vpfFormFunc() - initializes vpf objects from formname
//------------------------------------------------------------------------------

#include "openeaagles/maps/vpfMap/vpfMapFF.h"
#include "openeaagles/maps/vpfMap/VMAP0FeatureClass.h"
#include "openeaagles/maps/vpfMap/VMAP0LibDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0MainDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0RefCoverageDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0ReferenceDirectory.h"
#include "openeaagles/maps/vpfMap/VPFDataType.h"
#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VPFIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/maps/vpfMap/VPFSpatialIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFTable.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

Basic::Object* vpfMapFormFunc(const char* formname)
{
    // Start out by checking the basic classes (forms)
    Basic::Object* newform = 0;

    // Feature Class
    if ( strcmp(formname, VMAP0FeatureClass::getFormName()) == 0 ) {
        newform = new VMAP0FeatureClass();
    }
    // Library directory
    else if ( strcmp(formname, VMAP0LibDirectory::getFormName()) == 0 ) {
        newform = new VMAP0LibDirectory();
    }
    // Main directory
    else if ( strcmp(formname, VMAP0MainDirectory::getFormName()) == 0 ) {
        newform = new VMAP0MainDirectory();
    }
    // Reference Coverage  directory
    else if ( strcmp(formname, VMAP0RefCoverageDirectory::getFormName()) == 0 ) {
        newform = new VMAP0RefCoverageDirectory();
    }
    // Reference directory
    else if ( strcmp(formname, VMAP0ReferenceDirectory::getFormName()) == 0 ) {
        newform = new VMAP0ReferenceDirectory();
    }
    // Data Type
    else if ( strcmp(formname, VPFDataType::getFormName()) == 0 ) {
        newform = new VPFDataType();
    }
    // Top Level Directory
    else if ( strcmp(formname, VPFDirectory::getFormName()) == 0 ) {
        newform = new VPFDirectory();
    }
    // Index Table
    else if ( strcmp(formname, VPFIndexTable::getFormName()) == 0 ) {
        newform = new VPFIndexTable();
    }
    // Record
    else if ( strcmp(formname, VPFRecord::getFormName()) == 0 ) {
        newform = new VPFRecord();
    }
    // Spatial Index File
    else if ( strcmp(formname, VPFSpatialIndexTable::getFormName()) == 0 ) {
        newform = new VPFSpatialIndexTable();
    }
    // Table
    else if ( strcmp(formname, VPFTable::getFormName()) == 0 ) {
        newform = new VPFTable();
    }

    return newform;
}

} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

