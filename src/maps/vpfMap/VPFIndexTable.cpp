// ------------------------------------------------------------
// Class: VPFIndexTable
// ------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VPFIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/basic/String.h"
// for opening and closing files
#include <fstream>

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VPFIndexTable,"VPFIndexTable")
EMPTY_SERIALIZER(VPFIndexTable)

EMPTY_COPYDATA(VPFIndexTable)

VPFIndexTable::VPFIndexTable()
{
    type = 1; //VPFDirectory::EDX;
    //for (int i = 0; i < MAX_ROWS; i++) {
    //    data[i][0] = 0;
    //    data[i][1] = 0;
    //}
    numEntries = 0;
    numBytes = 0;
    loaded = false;
    recordSize = sizeof(int) * 2;
    fullPath = 0;
}

void VPFIndexTable::deleteData()
{
    if (fullPath != 0) {
        fullPath->unref();
        fullPath = 0;
    }
}

// get a record quickly from our index file
void VPFIndexTable::getRecordPosition(const int idx, int& offset, int& length)
{
    // we know our record size is exactly 2 integers, and we know the offset is our numBytes,
    // because numBytes is the header length in the VPFTable associated with this index table,
    if (fullPath != 0 && !fullPath->isEmpty()) {
        std::ifstream stream;
        stream.open(fullPath->getString());
        if (!stream.fail()) {
            // our record size is actually the same size of our num entires and numBytes, which
            // are both integers, so we can use that to seek 8 + idx * 8 to get the record
            stream.seekg(recordSize + ((idx - 1) * recordSize), std::ios::beg);
            int off = 0, len = 0;
            stream.read((char*)&off, sizeof(off));
            stream.read((char*)&len, sizeof(len));
            offset = off;
            length = len;
            //std::cout << "OFFSET AND LENGTH FOR RECORD " << idx << " = " << offset << ", " << length << std::endl;
            stream.close();
        }
        else std::cout << "FAILED TO OPEN INDEX FILE " << fullPath->getString() << std::endl;
    }
}

void VPFIndexTable::loadIndexTableFromFile(const char* pathname, const char* filename, const int t)
{
#if defined DEBUG_OUTPUT
    std::cout << "INDEX PATH AND FILE = " << pathname << filename << std::endl;
#endif
    type = t;
    fullPath = new Basic::String(pathname);
    fullPath->catStr(filename);
    std::ifstream inStream;
    inStream.open(fullPath->getString(), std::ios::in | std::ios::binary);
    if (inStream.fail()) std::cerr << "VPFIndexTable::loadIndexFromFile(), failed to open file " << filename << std::endl;
    else {
        // number of entries in the index (should match associated table number of records)
        inStream.read((char*)&numEntries, sizeof(numEntries));
        //std::cout << "NUM ENTRIES = " << numEntries << std::endl;
        // number of bytes in table header
        inStream.read((char*)&numBytes, sizeof(numBytes));
        //std::cout << "NUM BYTES IN HEADER = " << numBytes << std::endl;
        loaded = true;
    }
    inStream.close();
#if defined DEBUG_OUTPUT
    std::cout << "CLOSE FILE = " << filename << std::endl;
#endif
}



} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

