#include "openeaagles/maps/vpfMap/VMAP0MainDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0ReferenceDirectory.h"
#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/maps/vpfMap/VMAP0LibDirectory.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VMAP0MainDirectory,"VMAP0MainDirectory")
EMPTY_SERIALIZER(VMAP0MainDirectory)


VMAP0MainDirectory::VMAP0MainDirectory()
{
    STANDARD_CONSTRUCTOR()
    rference = 0;
    libDirectory = 0;
}

void VMAP0MainDirectory::copyData(const VMAP0MainDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMAP0MainDirectory::deleteData()
{
    if (rference != 0) {
        rference->unref();
        rference = 0;
    }
    if (libDirectory != 0) {
        libDirectory->unref();
        libDirectory = 0;
    }
}

void VMAP0MainDirectory::loadTables()
{
    Basic::String* string = new Basic::String(getPath());

    // load our database header table
    // create our table first
    createTable(VPFDirectory::DHT);
    VPFTable* table = getTable(VPFDirectory::DHT);
    bool ok = false;
    if (table != 0) ok = table->loadTableFromFile(string->getString(), "dht", VPFDirectory::DHT);

    // this is the main directory, which for a level 0 map, should use 
    // load our library attribute table
    table = 0;
    // create our table
    createTable(VPFDirectory::LAT);
    table = getTable(VPFDirectory::LAT);
    if (table != 0) {
        ok = table->loadTableFromFile(string->getString(), "lat", VPFDirectory::LAT);
        // now create our reference library directory
        if (ok) {
            int index = 1;
            VPFRecord* record = table->getRecord(index);
            while (record != 0) {
                string->empty();
                //std::cout << "LAT INFO = " << record->getData(1) << ", " << record->getData(2) << ", " << record->getData(3) << \
                //    ", " << record->getData(4) << ", " << record->getData(5) << ", " << record->getData(6) << std::endl;
                // column 2 is the column we need
                char* x = (char*)record->getData(2);
                size_t size = strlen(x);
                size_t count = 0;
                while (x[count] != ' ' && count < size) count++;
                x[count] = 0;
                if (strcmp(x, "rference") == 0) {
                    string->catStr(getPath());
                    string->catStr(x);
                    string->catStr("/");
                    if (rference == 0) rference = new VMAP0ReferenceDirectory();
                    rference->setSlotPath(string);
                }
                //else if (strcmp(x, "noamer") == 0) {
                //    string->catStr(getPath());
                //    string->catStr("noamer");
                //    if (libDirectory == 0) libDirectory = new VMAP0LibDirectory();
                //    libDirectory->setSlotPath(string);
                //}
                index++;
                record = table->getRecord(index);
            }

        }
    }
    string->unref();
}

}; // end Vpf namespace
}; // end Maps namespace 
}; // end Eaagles namespace
