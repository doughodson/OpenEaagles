// -------------------------------------------------------------------------------
// Class: VPFDirectory
// Description: Directory object which will understand it's VPFTables
// Vector Product Format
// -------------------------------------------------------------------------------
#ifndef	__Eaagles_Maps_Vpf_VPFDirectory_H__
#define __Eaagles_Maps_Vpf_VPFDirectory_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/maps/vpfMap/VPFTable.h"

namespace Eaagles {
namespace Basic { class String; }
namespace Maps {
namespace Vpf {

class VPFTable;

class VPFDirectory : public Basic::Component {
   DECLARE_SUBCLASS(VPFDirectory,Basic::Component)

public:
    VPFDirectory();

    // Vector Product Format table types (main types of tables - other tables, such as feature tables, are
    // defined at a lower level)
    enum { DHT = 0, LAT, CAT, DQT, LHT, GRT, FCS, LINEAGE, MAX_TABLES };

    virtual void loadTables();
    const char* getPath()   { return path; }
    virtual void setType(const int x)   { type = x; }

    int getType()   { return type; }

    // Tables
    void createTable(const int type);
    VPFTable* getTable(const int type) { if (tables[type] != 0) return tables[type]; else return 0; }

    bool setSlotPath(const Basic::String* const sfpobj);

private:
    char* path;                       // Path name to our database directory
    int type;                         // Type of directory we are
    VPFTable* tables[MAX_TABLES];     // Tables

};

}; // End Vpf namespace
}; // End Maps namespace 
}; // End Eaagles namespace

#endif // __EAAGLES_MAPS_VPF_VPFDIRECTORY_H__
