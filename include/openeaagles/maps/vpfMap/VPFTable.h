// --------------------------------------------------------------
// Class: VpfTable
// Description: Default vector product format table
// 
// --------------------------------------------------------------
#ifndef	__Eaagles_Maps_Vpf_VPFTable_H__
#define __Eaagles_Maps_Vpf_VPFTable_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic { class List; class String; }
namespace Maps {
namespace Vpf {

class VPFIndexTable;
class VPFRecord;

class VPFTable : public Basic::Object {
   DECLARE_SUBCLASS(VPFTable,Basic::Object)

public:
    VPFTable();

    struct ColumnDefinitions {
        char name[255];
        char dataType;
        char number[255];
        char keyType;
        char columnDesc[255];
        char valueDescTableName[255];
        char thematicIndex[255];
        char narrTable[255];
    };

    // Set functions
    virtual void setHeaderLength(const int x);
    virtual void setByteOrder(char x);
    virtual void setTableDesc(char* x);
    virtual void setNarrTable(char* x);
    // Column functions
    virtual void setCurrentColumn(const int x);
    virtual void setColumnName(char* x);
    virtual void setColumnDataType(char x);
    virtual void setColumnNumber(char* x);
    virtual void setColumnKeyType(char x);
    virtual void setColumnDesc(char* x);
    virtual void setColumnValueDescTableName(char* x);
    virtual void setColumnThematicIndex(char* x);
    virtual void setColumnNarrTable(char* x);

    // Get functions
    int         getHeaderLength()               { return headerLength; }
    const char  getByteOrder()                  { return byteOrder; }
    const char* getTableDesc()                  { return tableDesc; }
    const char* getNarrTable()                  { return narrTable; }
    const char* getColumnName()                 { return columns[cCol].name; }
    const char  getColumnDataType()             { return columns[cCol].dataType; }
    const char* getColumnNumber()               { return columns[cCol].number; }
    const char  getColumnKeyType()              { return columns[cCol].keyType; }
    const char* getColumnDesc()                 { return columns[cCol].columnDesc; }
    const char* getColumnValueDescTableName()   { return columns[cCol].valueDescTableName; }
    const char* getColumnThematicIndex()        { return columns[cCol].thematicIndex; }
    const char* getColumnNarrTable()            { return columns[cCol].narrTable; }
    VPFIndexTable* getIndexTable()              { return idxTable; }

    int getNumberOfRows()                       { return numRows; }
    int getNumberOfColumns()                    { return numColumns; }
    bool isLoaded()                             { return loaded; }

    // Specific set function (for directories to know what type we are)
    virtual void setType(const int x)           { type = x; }
    int getType()                               { return type; }
    bool isType(const int x)                    { if (type == x) return true; else return false; }

    // Size of one record (if we are fixed length)
    int getRecordSize()                         { return recordSize; }
    VPFRecord* getRecord(const int idx);             

    virtual bool loadTableFromFile(const char* pathname, const char* filename, const int xType = -1);
    static const int MAX_COLUMNS = 50;
    static const int MAX_ROWS = 5000;

    virtual void loadIndexFile();

private:
    void determineRecordSize();
    int headerLength;   
    char byteOrder;
    char tableDesc[255];
    char narrTable[255];
    int cCol;                                 // Column we are on
    int numColumns;                           // Number of columns in our column list
    int numRows;                              // Number of data elements (rows of data) we have
    bool loaded;                              // Have we already read the file?
    int recordSize;                           // Size of 1 record (if we aren't variable length)
    Basic::String* path;                      // Path file
    Basic::String* name;                      // Name of our table (basically the name of the file)

    ColumnDefinitions columns[MAX_COLUMNS];
    VPFRecord* records[MAX_ROWS];             // Holds our records

    VPFIndexTable* idxTable;                  // Holds our associated index table (if we need one)
    int type;                                 // Type of table we are (header table, feature table, etc...)
};

}; // End Vpf namespace
}; // End Maps namespace 
}; // End Eaagles namespace

#endif
