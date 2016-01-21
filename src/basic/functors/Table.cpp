
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"

namespace oe {
namespace basic {

//==============================================================================
// Class TableStorage
//==============================================================================
IMPLEMENT_SUBCLASS(TableStorage, "TableStorage")
EMPTY_SLOTTABLE(TableStorage)
EMPTY_DELETEDATA(TableStorage)
EMPTY_SERIALIZER(TableStorage)

TableStorage::TableStorage()
{
   STANDARD_CONSTRUCTOR()
   xbp = 0;
   ybp = 0;
   zbp = 0;
   wbp = 0;
   vbp = 0;
}

void TableStorage::copyData(const TableStorage& org, const bool cc)
{
    BaseClass::copyData(org);
   xbp = org.xbp;
   ybp = org.ybp;
   zbp = org.zbp;
   wbp = org.wbp;
   vbp = org.vbp;
}

//==============================================================================
// Class Table
//  (Note: the static lfi() functions are located at the end of this file)
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Table, "Table")

BEGIN_SLOTTABLE(Table)
    "data",          // Data table
    "extrapolate",   // Extrapolate beyond data
END_SLOTTABLE(Table)

BEGIN_SLOT_MAP(Table)
    ON_SLOT(1,setDataTable,List)
    ON_SLOT(2,setExtrapolationEnabled,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table::Table() : valid(false), extFlg(false)
{
   STANDARD_CONSTRUCTOR()
   dtable = nullptr;
   nd = 0;
}

Table::Table(const LCreal* dtbl, const unsigned int dsize)
   : valid(false), dtable(nullptr), nd(0), extFlg(false)
{
    STANDARD_CONSTRUCTOR()
    if (dtbl != nullptr && dsize > 0) {   /* Copy the data table */
        dtable = new LCreal[dsize];
        if (dtable != nullptr) {
            for (unsigned int i = 0; i < dsize; i++) dtable[i] = dtbl[i];
            nd = dsize;
        }
    }
}

Table::Table(const Table& org) : valid(false), extFlg(false)
{
    STANDARD_CONSTRUCTOR()
    dtable = nullptr;
    nd = 0;
    copyData(org,true);
}

Table::~Table()
{
   STANDARD_DESTRUCTOR()
}

Table& Table::operator=(const Table& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Table* Table::clone() const
{
    return nullptr;
}

void Table::copyData(const Table& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && dtable != nullptr) { delete[] dtable; dtable = nullptr; }

    // Copy new data
    nd = org.nd;
    if (org.dtable != nullptr) {
        dtable = new LCreal[nd];
        for (unsigned int i = 0; i < nd; i++) dtable[i] = org.dtable[i];
    }
    else dtable = nullptr;
    valid = org.valid;
    extFlg = org.extFlg;
}

void Table::deleteData()
{
    if (dtable != nullptr) delete[] dtable;
    dtable = nullptr;
    nd = 0;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table::isValid() const
{
   return (nd >= 1) && (dtable != nullptr) && (tableSize() == nd) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
FStorage* Table::storageFactory() const
{
   return new TableStorage();
}

//------------------------------------------------------------------------------
// setExtrapolationEnabled() -- set the extrapolation enabled flag
//------------------------------------------------------------------------------
bool Table::setExtrapolationEnabled(const bool flg)
{
   extFlg = flg;
   return true;
}

bool Table::setExtrapolationEnabled(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setExtrapolationEnabled( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// findMinMax() -- find the minimum and maximum values of the table
//------------------------------------------------------------------------------
void Table::findMinMax(LCreal* minValue, LCreal* maxValue) const
{
    if (nd > 0) {
        LCreal minv = dtable[0];
        LCreal maxv = dtable[0];
        for (unsigned int i = 1; i < nd; i++) {
            if (dtable[i] < minv) minv = dtable[i];
            if (dtable[i] > maxv) maxv = dtable[i];
        }
        *minValue = minv;
        *maxValue = maxv;
    }
}


//------------------------------------------------------------------------------
// loadVector() --
//------------------------------------------------------------------------------
bool Table::loadVector(const List& list, LCreal** table, unsigned int* nn)
{
    unsigned int n = list.entries();
    if (n <= 0) return false;

    LCreal* p = new LCreal[n];
    unsigned int n2 = list.getNumberList(p, n);
    bool ok = (n == n2);
    if (ok) {
        // Have the data!
        *table = p;
        *nn = n;
    }
    else {
        // Something was wrong, free the table
        delete[] p;
        throw new ExpInvalidVector();     //invalid vector - throw an exception
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setDataTable() -- for Table
//------------------------------------------------------------------------------
bool Table::setDataTable(const List* const sdtobj)
{
    bool ok = true;
    if (sdtobj != nullptr) {
        // First determine the size of the table -- ALL breakpoint data MUST
        // have been set first (order in input file) to determine the size
        // of the data table
        unsigned int ts = tableSize();
        if (ts > 0) {
            // Allocate table space and load the table
            LCreal* p = new LCreal[ts];
            ok = loadData(*sdtobj, p);
            if (ok) {
                // Loading completed, so
                // free up any old data and set to the new.
                if (dtable != nullptr) delete[] dtable;
                dtable = p;
                nd = ts;
            }
            else {
                // Something was wrong!
                delete[] p;
                std::cerr << "Table::setDataTable: Something is wrong!  Data table aborted." << std::endl;
                ok = false;
            }
        } valid = isValid();
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout, i + j, true);

    indent(sout, i + j);
    sout << "data: ";
    printData(sout, dtable, (i + j));

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// printVector() -- print a vector of breakpoints
//------------------------------------------------------------------------------
void Table::printVector(std::ostream& sout, const LCreal* table, const unsigned int n)
{
    sout << "[";
    if (table != nullptr) {
        for (unsigned int i = 0; i < n; i++) sout << " " << table[i];
    }
    sout << " ]";
}

} // End basic namespace
} // End oe namespace
