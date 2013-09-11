// --------------------------------------------------------------
// Class: VpfTable
// --------------------------------------------------------------

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"

#include "openeaagles/maps/vpfMap/VPFDataType.h"
#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VPFTable.h"
#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/maps/vpfMap/VPFIndexTable.h"
#include <fstream>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VPFTable,"VPFTable")
EMPTY_SERIALIZER(VPFTable)
EMPTY_COPYDATA(VPFTable)

VPFTable::VPFTable()
{
    STANDARD_CONSTRUCTOR()
    headerLength = 0;
    cCol = 0;
    numColumns = 0;
    for (int i = 0; i < MAX_ROWS; i++) {
        records[i] = 0;
    }
    idxTable = 0;
    numRows = 0;
    loaded = false;
    recordSize = 0;
    path = 0;
    name = 0;
    tableDesc[0] = '\0';
    narrTable[0] = '\0';
}

void VPFTable::deleteData()
{
    // delete all of our data objects
    for (int i = 0; i < MAX_ROWS; i++) {
        if (records[i] != 0) records[i]->unref();
        records[i] = 0;
    }
    if (idxTable != 0) idxTable->unref();
    if (path != 0) {
        path->unref();
        path = 0;
    }
    if (name != 0) {
        name->unref();
        name = 0;
    }
}

// set functions
void VPFTable::setHeaderLength(const int x)         { headerLength = x; }
void VPFTable::setByteOrder(char x)                 { byteOrder = x; }
void VPFTable::setTableDesc(char* x)                { lcStrcpy(tableDesc, sizeof(tableDesc), x); }
void VPFTable::setNarrTable(char* x)                { lcStrcpy(narrTable, sizeof(narrTable), x); }
// column set functions
void VPFTable::setCurrentColumn(const int x)        { cCol = (x-1); }
void VPFTable::setColumnName(char* x)               { lcStrcpy(columns[cCol].name, sizeof(columns[cCol].name), x); }
void VPFTable::setColumnDataType(char x)            { columns[cCol].dataType = x; }
void VPFTable::setColumnNumber(char* x)             { lcStrcpy(columns[cCol].number, sizeof(columns[cCol].number), x); }
void VPFTable::setColumnKeyType(char x)             { columns[cCol].keyType = x; }
void VPFTable::setColumnDesc(char *x)               { lcStrcpy(columns[cCol].columnDesc, sizeof(columns[cCol].columnDesc), x); }
void VPFTable::setColumnValueDescTableName(char* x) { lcStrcpy(columns[cCol].valueDescTableName, sizeof(columns[cCol].valueDescTableName), x ); }
void VPFTable::setColumnThematicIndex(char* x)      { lcStrcpy(columns[cCol].thematicIndex, sizeof(columns[cCol].thematicIndex), x); }
void VPFTable::setColumnNarrTable(char* x)          { lcStrcpy(columns[cCol].narrTable, sizeof(columns[cCol].narrTable), x); }

bool VPFTable::loadTableFromFile(const char* pathname, const char* filename, const int xType)
{
    // this loads the basic information from the file, not all the records themself
    if (path == 0) path = new Basic::String(pathname);
    else path->setStr(pathname);
    bool ok = false;
    if (name == 0) name = new Basic::String(filename);
    else name->setStr(filename);
#if defined DEBUG_OUTPUT
    std::cout << "Path and file = " << pathname << filename << std::endl << std::endl;
#endif
    // standard input instream
    std::ifstream inStream;
    // open our database header table first, to get information
    // now create a file and pathname
    Basic::String* temp = new Basic::String(pathname);
    temp->catStr(filename);
    inStream.open(temp->getString(), std::ios::in | std::ios::binary);
    temp->unref();
    if (inStream.fail()) {
        std::cout << "FAILED to open file " << pathname << filename << std::endl;
    }
    else {
        ok = true;
        // start reading our file
        headerLength = 0;
        inStream.seekg(0, std::ios::beg);
        // header length
        inStream.read((char*)(&headerLength), sizeof(headerLength));
        // byte order information
        char byteOrder;
        inStream.read(&byteOrder, sizeof(byteOrder));
        // now we skip over the semicolon
        inStream.seekg(1, std::ios::cur);
        // Database header table description
        char tableDesc[255];
        inStream.getline(tableDesc, sizeof(tableDesc), ';');
        // optional narrative table
        char narrTable[255];
        inStream.getline(narrTable, sizeof(narrTable), ';');
        //if (narrTable[0] == '-') std::cout << "Narrative Table Empty!" << std::endl;
        // column definitions
        char columnDefs[1000];
        inStream.getline(columnDefs, sizeof(columnDefs), ';');
        // sort our column defs
        char tempCol[100][255];
        int row = 0;
        int column = 0;
        for (unsigned int i = 0; i < strlen(columnDefs); i++) {
            // end of column definition
            if (columnDefs[i] == ':') {
                tempCol[row++][column] = 0;
                numColumns++;
                column = 0;
            }
            else {
                tempCol[row][column++] = columnDefs[i];
            }
        }

        // populate our database header table
        setType(xType);
        setByteOrder(byteOrder);
        setTableDesc(tableDesc);
        setNarrTable(narrTable);

        // go through and set our column definitions
        int idx = 0;
        int x = 0;
        char holder[255];
        bool hasIdx = false;
        for (int i = 0; i < row; i++) {
            idx = 0;
            x = 0;
            // set the column as current
            setCurrentColumn(i+1);
            // name of the column
            while (tempCol[i][idx] != '=') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnName(holder);
            // skip comma
            idx++;
            // data type
            setColumnDataType(tempCol[i][idx++]);
            // skip the comma, reset our temporary index
            idx++;
            x = 0;
            // column number field
            while (tempCol[i][idx] != ',') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnNumber(holder);
            if (strcmp(getColumnNumber(),"*") == 0) hasIdx = true;
            // skip comma
            idx++;
            // column key type
            setColumnKeyType(tempCol[i][idx++]);
            // skip comma & reset temp index
            idx++;
            x = 0;
            // column description
            while (tempCol[i][idx] != ',') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnDesc(holder);
            // skip comma & reset temp index
            idx++;
            x = 0;
            // column value description table name
            while (tempCol[i][idx] != ',') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnValueDescTableName(holder);
            // comma and reset
            idx++;
            x = 0;
            // thematic index
            while (tempCol[i][idx] != ',') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnThematicIndex(holder);
            // comm, reset temp idx, then narrative table
            idx++;
            x = 0;
            while (tempCol[i][idx] != ',') holder[x++] = tempCol[i][idx++];
            holder[x] = '\0';
            setColumnNarrTable(holder);
#if defined DEBUG_OUTPUT
            std::cout << "COLUMN # " << i+1 << " = " << tempCol[i] << std::endl;
#endif
            // this will determine our record size, but if we have an index file the
            // record size gets reset to 0.
            if (!hasIdx) determineRecordSize();
        }
        // now at the end, load our index file if we need to
        if (hasIdx) loadIndexFile();
        //std::cout << std::endl;

        // if we are a certain type of table, we need specific information
        // library attribute table has to be read in, because we need to know our specific libraries
        // this is only done for high level tables, which have a small amount of data but
        // specific data on the coverages we need
        //if (type != -1) storeContentsFromFile(inStream);
        loaded = true;
        // before we go, let's figure out how big our file is
#if defined DEBUG_OUTPUT
        std::cout << "CLOSE FILE = " << filename << std::endl;
        std::cout << std::endl << std::endl;
#endif
        inStream.close();
        // ok, we know our record size, but we don't know how many records we have, but we will simply
        // get the records as we need them
    }
    return ok;
}

void VPFTable::loadIndexFile()
{
    // ok, we need to take our file name, and replace the last letter with x
    // reset our record size
    recordSize = 0;
    char x[4];
    strncpy(x, name->getString(), 3);
    x[2] = 'x';
    x[3] = 0;
    // now load the file
    if (idxTable == 0) idxTable = new VPFIndexTable();
    if (!idxTable->isLoaded()) {
        idxTable->loadIndexTableFromFile(path->getString(), x);
    }
}

VPFRecord* VPFTable::getRecord(const int idx)
{
    if (idx < MAX_ROWS && path != 0 && name != 0) {
        if (records[idx-1] == 0) {
            // we haven't created this record yet, so let's do it
            records[idx-1] = new VPFRecord();
            // this will load the record for us
            Basic::String* string = new Basic::String(path->getString());
            string->catStr(name->getString());
            records[idx-1]->createRecord(this, string->getString(), idx);
            string->unref();
            //std::cout << "CREATED RECORD NUMBER AND TABLE NAME = " << idx << ", " << this->getType() << std::endl;
            // if our record is invalid, it means we have reached the end of the record
            bool eor = records[idx-1]->isEOR();
            if (eor) {
                // we have to delete our record, because it's was not valid
                records[idx-1]->unref();
                records[idx-1] = 0;
                return 0;
            }
            else return records[idx-1];
        }
        else {
            if (records[idx-1]->isEOR()) return 0;
            return records[idx-1];
        }
    }
    return 0;
}

void VPFTable::determineRecordSize()
{
    // determine our total record size
    char x = getColumnDataType();
    int num = atoi(getColumnNumber());

    if (num != 0) {
        if (x == 'I') recordSize += sizeof(int) * num;
        else if (x == 'S') recordSize += sizeof(short int) * num;
        else if (x == 'T') recordSize += num;
        else if (x == 'D') recordSize += num * 20;
        else if (x == 'F') recordSize += sizeof(float) * num;
        else if (x == 'R') recordSize += sizeof(double) * num;
        else if (x == 'Z') recordSize += sizeof(float) * 3;
    }
    //std::cout << "RECORD SIZE = " << recordSize << std::endl;
}

} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

