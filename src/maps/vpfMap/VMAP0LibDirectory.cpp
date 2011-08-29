#include "openeaagles/maps/vpfMap/VMAP0LibDirectory.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VMAP0LibDirectory,"VMAP0LibDirectory")
EMPTY_SERIALIZER(VMAP0LibDirectory)


VMAP0LibDirectory::VMAP0LibDirectory()
{
    STANDARD_CONSTRUCTOR()
    for (int i = 0; i < MAX_COVERAGES; i++) coverages[i] = 0;
    numCoverages = 0;
}

void VMAP0LibDirectory::copyData(const VMAP0LibDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMAP0LibDirectory::deleteData()
{
    for (int i = 0; i < MAX_COVERAGES; i++) {
        if (coverages[i] != 0) {
            coverages[i]->unref();
            coverages[i] = 0;
        }
    }
}

void VMAP0LibDirectory::loadTables()
{
    // load up our tables that we own
    bool ok = false;
    bool cvgOk = false;
    // first, open our coverage attribute table, then read back the data to us
    Basic::String* string = new Basic::String(getPath());
    createTable(VPFDirectory::CAT);
    VPFTable* table = getTable(VPFDirectory::CAT);
    if (table != 0) {
        ok = table->loadTableFromFile(string->getString(), "cat", VPFDirectory::CAT);
        cvgOk = ok;
        #if 0
        if (ok) {
            std::cout << "Coverage Attribute Table: " << std::endl;
            for (int i = 0; i < tables[VPFDirectory::CAT].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::CAT].getNumberOfColumns(); j++) {
                    std::cout << "ROW, COLUMN, VALUE = " << i+1 << ", " << j+1 << ", " << tables[VPFDirectory::CAT].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
        #endif
    }

    // library header table
    table = 0;
    createTable(VPFDirectory::LHT);
    table = getTable(VPFDirectory::LHT);
    if (table != 0) {
        ok = table->loadTableFromFile(string->getString(), "lat", VPFDirectory::LHT);
    }

    table = 0;
    createTable(VPFDirectory::GRT);
    table = getTable(VPFDirectory::GRT);
    if (table != 0) {
        // geographic reference table
        ok = table->loadTableFromFile(string->getString(), "grt", VPFDirectory::GRT);
    }

    #if 0
    table = getTable(VPFDirectory::DQX);
    if (table != 0) {
        // data quality index
        string.empty();
        string.catStr(getPath());
        string.catStr("/dqx");
        ok = table->loadTableFromFile(string.getString(), VPFDirectory::DQX);
        if (ok) {
            std::cout << "DATA QUALITY INDEX:" << std::endl;
            for (int i = 0; i < tables[VPFDirectory::DQX].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::DQX].getNumberOfColumns(); j++) {
                    std::cout << "ROW # " << i+1 << ", COLUMN # " << j+1 << " VALUE = " << tables[VPFDirectory::DQX].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
    }
    #endif

    #if 0
    table = getTable(VPFDirectory::DQT);
    if (table != 0) {
        // data quality table
        string.empty();
        string.catStr(getPath());
        string.catStr("/dqt");
        ok = table->loadTableFromFile(string.getString(), VPFDirectory::DQT);
        if (ok) {
            std::cout << "DATA QUALITY TABLE:" << std::endl;
            for (int i = 0; i < tables[VPFDirectory::DQT].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::DQT].getNumberOfColumns(); j++) {
                    std::cout << "ROW # " << i+1 << ", COLUMN # " << j+1 << " VALUE = " << tables[VPFDirectory::DQT].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
    }
    #endif

    //table = 0;
    //createTable(VPFDirectory::LINEAGE);
    //table = getTable(VPFDirectory::LINEAGE);
    //if (table != 0) {
    //    // lineage
    //    string->empty();
    //    string->catStr(getPath());
    //    string->catStr("/lineage.doc");
    //    ok = table->loadTableFromFile(string->getString(), VPFDirectory::LINEAGE);
    //}

    // now we have all our data type and values loaded from the rference directory, we must descend into our coverage directories
    // read our coverage attribute table to determine the names of our coverages
    if (cvgOk) {
        table = 0;
        // our file is good, read through and open our directories
        table = getTable(VPFDirectory::CAT);
        //VPFDataType* t = 0;
        size_t size = 0;
        size_t count = 0;
        for (int i = 0; i < table->getNumberOfRows() && i < MAX_COVERAGES; i++) {
            count = 0;
            size = 0;
            // our value for the library comes in as an 8 character name with trailing spaces, so we need to truncate it to a * character 
            // text value with no trailing spaces to put it into our pathname
            //t = table->getTableValue(i+1, 2);
            //t = 0;
            //if (t != 0 && numCoverages < MAX_COVERAGES)  {
            //    string->empty();
            //    char* val = (char*)table->getTableValue(i+1, 2)->getValue();
            //    size = strlen(val);
            //    // make sure we terminate at the correct area
            //    while (val[count] != ' ' && count < size) count++;
            //    val[count] = NULL;
            //    string->catStr(getPath());
            //    string->catStr("/");
            //    string->catStr(val);
            //    if (coverages[numCoverages] == 0) coverages[numCoverages] = new VMAP0RefCoverageDirectory();
            //    coverages[numCoverages]->setSlotPath(string);
            //    numCoverages++;
            //}
        }
    }
}

}; // end Vpf namespace
}; // end Maps namespace 
}; // end Eaagles namespace
