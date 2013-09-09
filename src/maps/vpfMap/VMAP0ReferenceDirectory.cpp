// -------------------------------------------------------------------------------
// Class: VMAP0ReferenceDirectory
// -------------------------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VMAP0ReferenceDirectory.h"
#include "openeaagles/maps/vpfMap/VPFIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VMAP0ReferenceDirectory,"VMAP0ReferenceDirectory")
EMPTY_SERIALIZER(VMAP0ReferenceDirectory)


VMAP0ReferenceDirectory::VMAP0ReferenceDirectory()
{
    STANDARD_CONSTRUCTOR()
    for (int i = 0; i < MAX_COVERAGES; i++) coverages[i] = 0;
}

void VMAP0ReferenceDirectory::copyData(const VMAP0ReferenceDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMAP0ReferenceDirectory::deleteData()
{
    for (int i = 0; i < MAX_COVERAGES; i++) {
        if (coverages[i] != 0) coverages[i]->unref();
        coverages[i] = 0;
    }
}

void VMAP0ReferenceDirectory::loadTables()
{
    bool ok = false;
    bool cvgOk = false;
    // first, open our coverage attribute table, then read back the data to us
    Basic::String* string = new Basic::String(getPath());
    VPFTable* table = 0;

    // create our table
    createTable(VPFDirectory::CAT);
    table = getTable(VPFDirectory::CAT);
    if (table != 0) {
        ok = table->loadTableFromFile(string->getString(), "cat", VPFDirectory::CAT);
        cvgOk = ok;
    }

    // library header table
    table = 0;
    createTable(VPFDirectory::LHT);
    table = getTable(VPFDirectory::LHT);
    if (table != 0) {
        ok = table->loadTableFromFile(string->getString(), "lht", VPFDirectory::LHT);
    }

    table = 0;
    createTable(VPFDirectory::GRT);
    table = getTable(VPFDirectory::GRT);
    if (table != 0) {
        // geographic reference table
        ok = table->loadTableFromFile(string->getString(), "grt", VPFDirectory::GRT);
        if (ok) {
            // go through and read our records
            int count = 1;
            VPFRecord* r = table->getRecord(count);
            while (r != 0) {
                for (int i = 1; i < table->getNumberOfColumns(); i++) {
                    if (r != 0) {   
#if defined DEBUG_OUTPUT
                        std::cout << "COLUMN #" << i << " = " << r->getData(i) << std::endl;
#endif
                    }
                }
                r = table->getRecord(++count);
            }
        }
    }

    //table = 0;
    //createTable(VPFDirectory::DQT);
    //table = getTable(VPFDirectory::DQT);
    //if (table != 0) {
    //    // data quality table
    //    string->empty();
    //    string->catStr(getPath());
    //    string->catStr("/dqt");
    //    ok = table->loadTableFromFile(string->getString(), VPFDirectory::DQT);
    //}

    table = 0;
    createTable(VPFDirectory::LINEAGE);
    table = getTable(VPFDirectory::LINEAGE);
    if (table != 0) {
        // lineage
        ok = table->loadTableFromFile(string->getString(), "lineage.doc", VPFDirectory::LINEAGE);
    }

    // now we have all our data type and values loaded from the rference directory, we must descend into our coverage directories
    // read our coverage attribute table to determine the names of our coverages
    if (cvgOk) {
        table = 0;
        // our file is good, read through and open our directories (should be 4)
        table = getTable(VPFDirectory::CAT);
        if (table != 0) {
            // this is where we get our data
            int index = 1;
            VPFRecord* record = table->getRecord(index);
            while (record != 0) {
                string->empty();
                // column 2 is the column we need
                char* x = (char*)record->getData(2);
                size_t size = strlen(x);
                size_t count = 0;
                while (x[count] != ' ' && count < size) count++;
                x[count] = 0;            

#if 1
                //if (strcmp(x, "polbnd") == 0) {
                //    if (coverages[CVG_POLBND] == 0) coverages[CVG_POLBND] = new VMAP0RefCoverageDirectory();
                //    string->catStr(getPath());
                //    string->catStr(x);
                //    string->catStr("/");
                //    // make sure you set the type first, or it will not create the proper tables
                //    coverages[CVG_POLBND]->setType(CVG_POLBND);
                //    coverages[CVG_POLBND]->setSlotPath(string);
                //}
                if (strcmp(x, "placenam") == 0) {
                    if (coverages[CVG_PLACENAM] == 0) coverages[CVG_PLACENAM] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr(x);
                    string->catStr("/");
                    // make sure you set the type first, or it will not create the proper tables
                    coverages[CVG_PLACENAM]->setType(CVG_PLACENAM);
                    coverages[CVG_PLACENAM]->setSlotPath(string);
                }
#endif

#if 0
                if (strcmp(x, "libref") == 0) {
                    if (coverages[CVG_LIBREF] == 0) coverages[CVG_LIBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_LIBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "placenam") == 0) {
                    if (coverages[CVG_PLACENAM] == 0) coverages[CVG_PLACENAM] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_PLACENAM]->setSlotPath(string);
                }
                else if (strcmp(x, "dbref") == 0) {
                    if (coverages[CVG_DBREF] == 0) coverages[CVG_DBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_DBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "libref") == 0) {
                    if (coverages[CVG_LIBREF] == 0) coverages[CVG_LIBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_LIBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "polbnd") == 0) {
                    if (coverages[CVG_POLBND] == 0) coverages[CVG_POLBND] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_POLBND]->setSlotPath(string);
                }
#endif 
                index++;
                record = table->getRecord(index);
            }
        }
    }
    string->unref();
}

VMAP0RefCoverageDirectory* VMAP0ReferenceDirectory::getCoverage(const int cov)
{
    if (cov < MAX_COVERAGES) return coverages[cov];
    return 0;
}

} // end Vpf namespace
} // end Maps namespace 
} // end Eaagles namespace

