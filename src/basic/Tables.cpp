//------------------------------------------------------------------------------
// Table, Table1, Table2, Table3, Table4, Table5 and TableStorage
//------------------------------------------------------------------------------

#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class TableStorage
//==============================================================================
class TableStorage : public FStorage {
   DECLARE_SUBCLASS(TableStorage, FStorage)
public:
   TableStorage() { xbp = 0; ybp = 0; zbp = 0; wbp = 0; vbp = 0; }
   unsigned int xbp, ybp, zbp, wbp, vbp;
};

IMPLEMENT_SUBCLASS(TableStorage,"TableStorage")
EMPTY_SLOTTABLE(TableStorage)
EMPTY_COPYDATA(TableStorage)
EMPTY_DELETEDATA(TableStorage)
EMPTY_SERIALIZER(TableStorage)

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
   dtable = 0;
   nd = 0;
}

Table::Table(const LCreal* dtbl, const unsigned int dsize) 
   : valid(false), dtable(0), nd(0), extFlg(false)
{
    STANDARD_CONSTRUCTOR()
    if (dtbl != 0 && dsize > 0) {   /* Copy the data table */
        dtable = new LCreal[dsize];
        if (dtable != 0) {
            for (unsigned int i = 0; i < dsize; i++) dtable[i] = dtbl[i];
            nd = dsize;
        }
    }
}

Table::Table(const Table& org) : valid(false), extFlg(false)
{ 
    STANDARD_CONSTRUCTOR()
    dtable = 0;
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
    return 0;
}

void Table::copyData(const Table& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && dtable != 0) { delete[] dtable; dtable = 0; }

    // Copy new data
    nd = org.nd;
    if (org.dtable != 0) {
        dtable = new LCreal[nd];
        for (unsigned int i = 0; i < nd; i++) dtable[i] = org.dtable[i];
    }
    else dtable = 0;
    valid = org.valid;
    extFlg = org.extFlg;
}

void Table::deleteData()
{
    if (dtable != 0) delete[] dtable;
    dtable = 0;
    nd = 0;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table::isValid() const
{
   return (nd >= 1) && (dtable != 0) && (tableSize() == nd) && BaseClass::isValid();
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
   if (msg != 0) {
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
    if (sdtobj != 0) {
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
                if (dtable != 0) delete[] dtable;
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
        sout << "( " << getFormName() << std::endl;
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
    if (table != 0) {
        for (unsigned int i = 0; i < n; i++) sout << " " << table[i];
    }
    sout << " ]";
}

//==============================================================================
// Class Table1
//==============================================================================
IMPLEMENT_SUBCLASS(Table1, "Table1")

BEGIN_SLOTTABLE(Table1)
    "x",           // X breakpoints
END_SLOTTABLE(Table1)

BEGIN_SLOT_MAP(Table1)
    ON_SLOT(1,setXBreakpoints1,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table1::Table1() : Table()
{
   STANDARD_CONSTRUCTOR()
   xtable = 0;
   nx = 0;
}

Table1::Table1(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize)
                   : Table(dtbl, dsize), xtable(0), nx(0)
{
    STANDARD_CONSTRUCTOR()
    if (xtbl != 0 && xsize > 0) {   /* Copy the x breakpoints */
        xtable = new LCreal[xsize];
        if (xtable != 0) {
            for (unsigned int i = 0; i < xsize; i++) xtable[i] = xtbl[i];
            nx = xsize;
            valid = isValid();
        }
    }
}

void Table1::copyData(const Table1& org, const bool cc)
{
    BaseClass::copyData(org);
  
    // Delete old data
    if (!cc && xtable != 0) { delete[] xtable; xtable = 0; }

    // Copy new data
    nx = org.nx;
    if (org.xtable != 0) {
        xtable = new LCreal[nx];
        for (unsigned int i = 0; i < nx; i++) xtable[i] = org.xtable[i];
    }
    else xtable = 0;
    valid = isValid();
}

void Table1::deleteData()
{
    if (xtable != 0) delete[] xtable;       
    xtable = 0;
    nx = 0;
}

//------------------------------------------------------------------------------
// Load a 1D vector with nx values.
// Example:  [ 1 2 3 ]
//------------------------------------------------------------------------------
bool Table1::loadData(const List& list, LCreal* const table)
{
    // Make sure we have the proper number of entries in the list
    unsigned int n1 = list.entries();
    if (n1 <= 0 || n1 != nx) return false;
    
    // Transfer numbers from the list to a temp table
    LCreal* p = new LCreal[nx];
    unsigned int n2 = list.getNumberList(p, nx);
    bool ok = (nx == n2);
    if (ok) {
        // all is well -- copy the data
        for( unsigned int i = 0; i < nx; i++) table[i] = p[i];
        valid = isValid();
    }
    delete[] p;
    return ok;
}
  
//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table1::isValid() const
{
    return (nx >= 1) && (xtable != 0) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table1::tableSize() const
{
    return nx;
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty 
//------------------------------------------------------------------------------
LCreal Table1::getMinX() const
{
    if (xtable != 0 && nx > 0)
        return (xtable[0] < xtable[nx - 1]) ? xtable[0] : xtable[nx - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table1::getMaxX() const
{
    if (xtable != 0 && nx > 0)
        return (xtable[0] < xtable[nx - 1]) ? xtable[nx - 1] : xtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  1D LFI
//------------------------------------------------------------------------------
LCreal
Table1::lfi(const LCreal iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled(), &s->xbp);
   }
   else {
      return Table::lfi(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled());
   }
}

//------------------------------------------------------------------------------
// setXBreakpoints1() -- for Table1
//------------------------------------------------------------------------------
bool Table1::setXBreakpoints1(const List* const sxb1obj)
{
    if (sxb1obj != 0) {
        loadVector(*sxb1obj, &xtable, &nx);
        valid = isValid();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table1::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table1::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout, i + j);
    sout << "x: ";
    printVector(sout, xtable, nx);
    sout << std::endl;

    BaseClass::serialize(sout, i + j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
//  Print 1D table
//------------------------------------------------------------------------------
void Table1::printData(std::ostream& sout, const LCreal* tbl, const unsigned int ns) const
{
    indent(sout, ns);
    printVector(sout, tbl, nx);
    sout << std::endl;
}


//==============================================================================
// Class Table2
//==============================================================================
IMPLEMENT_SUBCLASS(Table2, "Table2")

BEGIN_SLOTTABLE(Table2)
    "y",           // Y breakpoints
END_SLOTTABLE(Table2)

BEGIN_SLOT_MAP(Table2)
    ON_SLOT(1,setYBreakpoints2,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table2::Table2() : Table1()
{
   STANDARD_CONSTRUCTOR()
   ytable = 0;
   ny = 0;
}

Table2::Table2(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize)
                   : Table1(dtbl, dsize, xtbl, xsize), ytable(0), ny(0)
{
    STANDARD_CONSTRUCTOR()
    if (ytbl != 0 && ysize > 0) {   /* Copy the y breakpoints */
        ytable = new LCreal[ysize];
        if (ytable != 0) {
            for (unsigned int i = 0; i < ysize; i++) ytable[i] = ytbl[i];
            ny = ysize;
            valid = isValid();
        }
    }
}

void Table2::copyData(const Table2& org, const bool cc)
{
    BaseClass::copyData(org);
  
    // Delete old data
    if (!cc && ytable != 0) { delete[] ytable; ytable = 0; }

    // Copy new data
    ny = org.ny;
    if (org.ytable != 0) {
        ytable = new LCreal[ny];
        for (unsigned int i = 0; i < ny; i++) ytable[i] = org.ytable[i];
    }
    else ytable = 0;
    valid = isValid();
}

void Table2::deleteData()
{
    if (ytable != 0) delete[] ytable;   
    ytable = 0;
    ny = 0;
}

//------------------------------------------------------------------------------
// 2D table is input as a list of 1D vectors.
// Example:  { [ 11 12 13 ] [ 21 22 23 ] [ 31 32 33 ] }
//------------------------------------------------------------------------------
bool Table2::loadData(const List& list, LCreal* const table)
{ 
    // Make sure we have the proper number of entries in the list
    unsigned int n1 = list.entries();
    bool ok = (n1 > 0 && n1 == ny);

    // Process each item in the list
    unsigned int i = 0;
    unsigned int k = BaseClass::tableSize();
    const List::Item* item = list.getFirstItem();
    while (ok && item != 0) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != 0) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != 0) {
                ok &= BaseClass::loadData(*slist, &table[i]);
                i += k;
            }
        }
        item = item->getNext();
    }
    if (ok) valid = isValid();
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table2::isValid() const
{
    return (ny >= 1) && (ytable != 0) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- returns the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table2::tableSize() const
{
    return ny * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty 
//------------------------------------------------------------------------------
LCreal Table2::getMinY() const
{
    if (ytable != 0 && ny > 0)
        return (ytable[0] < ytable[ny - 1]) ? ytable[0] : ytable[ny - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table2::getMaxY() const
{
    if (ytable != 0 && ny > 0)
        return (ytable[0] < ytable[ny - 1]) ? ytable[ny - 1] : ytable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  2D LFIs
//------------------------------------------------------------------------------
LCreal
Table2::lfi(const LCreal iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   }
   else {
      return Table::lfi( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

LCreal
Table2::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   }
   else {
      return Table::lfi( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setYBreakpoints2() -- for Table2 
//------------------------------------------------------------------------------
bool Table2::setYBreakpoints2(const List* const syb2obj)
{
    if (syb2obj != 0) {
        loadVector(*syb2obj, &ytable, &ny);
        valid = isValid();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table2::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table2::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout, i + j);
    sout << "y: ";
    printVector(sout, ytable, ny);
    sout << std::endl;

    BaseClass::serialize(sout, i + j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
//  Print 2D table
//------------------------------------------------------------------------------
void Table2::printData(std::ostream& sout, const LCreal* tbl, const unsigned int ns) const
{
    indent(sout, ns);
    sout << "{" << std::endl;

    if (tbl != 0) {
        unsigned int j = 0;
        unsigned int k = BaseClass::tableSize();
        for (unsigned int i = 0; i < ny; i++) {
            BaseClass::printData(sout, &tbl[j], ns + 4);
            j += k;
        }
    }

    indent(sout, ns);
    sout << "}" << std::endl;
}


//==============================================================================
// Class Table3
//==============================================================================
IMPLEMENT_SUBCLASS(Table3, "Table3")

BEGIN_SLOTTABLE(Table3)
    "z",           // Z breakpoints
END_SLOTTABLE(Table3)

BEGIN_SLOT_MAP(Table3)
    ON_SLOT(1,setZBreakpoints3,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table3::Table3() : Table2()
{
   STANDARD_CONSTRUCTOR()
   ztable = 0;
   nz = 0;
}

Table3::Table3(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize)
                   : Table2(dtbl, dsize, xtbl, xsize, ytbl, ysize),
                     ztable(0), nz(0)
{
    STANDARD_CONSTRUCTOR()
    if (ztbl != 0 && zsize > 0) {   /* Copy the z breakpoints */
        ztable = new LCreal[zsize];
        if (ztable != 0) {
            for (unsigned int i = 0; i < zsize; i++) ztable[i] = ztbl[i];
            nz = zsize;
            valid = isValid();
        }
    }
}

void Table3::copyData(const Table3& org, const bool cc)
{
    BaseClass::copyData(org);
  
    // Delete old data
    if (!cc && ztable != 0) { delete[] ztable; ztable = 0; }

    // Copy new data
    nz = org.nz;
    if (org.ztable != 0) {
        ztable = new LCreal[nz];
        for (unsigned int i = 0; i < nz; i++) ztable[i] = org.ztable[i];
    }
    else ztable = 0;
    valid = isValid();
}

void Table3::deleteData()
{
    if (ztable != 0) delete[] ztable;       
    ztable = 0;
    nz = 0;
}

//------------------------------------------------------------------------------
// 3D table is input as a list of 2D sub-tables.
// Example:  { { [ 111 112 113 ] [ 121 122 123 ] [ 131 132 133 ] }
//             { [ 211 212 213 ] [ 221 222 223 ] [ 231 232 233 ] }
//             { [ 311 312 313 ] [ 321 322 323 ] [ 331 332 333 ] } }
//------------------------------------------------------------------------------
bool Table3::loadData(const List& list, LCreal* const table)
{
    // Make sure we have the proper number of entries in the list
    unsigned int n1 = list.entries();
    bool ok = (n1 > 0 && n1 == nz);

    // Process each item in the list
    unsigned int i = 0;
    unsigned int k = BaseClass::tableSize();
    const List::Item* item = list.getFirstItem();
    while (ok && item != 0) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != 0) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != 0) {
                ok &= BaseClass::loadData(*slist, &table[i]);
                i += k;
            }
        }
        item = item->getNext();
    }
    if (ok) valid = isValid();
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table3::isValid() const
{
    return (nz >= 1) && (ztable != 0) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table3::tableSize() const
{
    return nz * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty 
//------------------------------------------------------------------------------
LCreal Table3::getMinZ() const
{
    if (ztable != 0 && nz > 0)
        return (ztable[0] < ztable[nz - 1]) ? ztable[0] : ztable[nz - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table3::getMaxZ() const
{
    if (ztable != 0 && nz > 0)
        return (ztable[0] < ztable[nz - 1]) ? ztable[nz - 1] : ztable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  3D LFIs
//------------------------------------------------------------------------------
LCreal
Table3::lfi(const LCreal iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const LCreal* y_data = getYData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return Table::lfi( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table3::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return Table::lfi( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table3::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return Table::lfi( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setZBreakpoints3() -- for Table3
//------------------------------------------------------------------------------
bool Table3::setZBreakpoints3(const List* const szb3obj)
{
    if (szb3obj != 0) {
        loadVector(*szb3obj, &ztable, &nz);
        valid = isValid();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table3::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table3::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout, i + j);
    sout << "z: ";
    printVector(sout, ztable, nz);
    sout << std::endl;

    BaseClass::serialize(sout, i + j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
//  Print 3D table
//------------------------------------------------------------------------------
void Table3::printData(std::ostream& sout, const LCreal* tbl, const unsigned int ns) const
{
    indent(sout, ns);
    sout << "{" << std::endl;

    if (tbl != 0) {
        unsigned int j = 0;
        unsigned int k = BaseClass::tableSize();
        for (unsigned int i = 0; i < nz; i++) {
            BaseClass::printData(sout, &tbl[j], ns + 4);
            j += k;
        }
    }

    indent(sout, ns);
    sout << "}" << std::endl;
}


//==============================================================================
// Class Table4
//==============================================================================
IMPLEMENT_SUBCLASS(Table4, "Table4")

BEGIN_SLOTTABLE(Table4)
    "w",           // W breakpoints
END_SLOTTABLE(Table4)

BEGIN_SLOT_MAP(Table4)
    ON_SLOT(1,setWBreakpoints4,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table4::Table4() : Table3()
{
   STANDARD_CONSTRUCTOR()
   wtable = 0;
   nw = 0;
}
Table4::Table4(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize,
                   const LCreal* wtbl, const unsigned int wsize)
                   : Table3(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize),
                     wtable(0), nw(0)
{
    STANDARD_CONSTRUCTOR()
    if (wtbl != 0 && wsize > 0) {   /* Copy the w breakpoints */
        wtable = new LCreal[wsize];
        if (wtable != 0) {
            for (unsigned int i = 0; i < wsize; i++) wtable[i] = wtbl[i];
            nw = wsize;
            valid = isValid();
        }
    }
}

void Table4::copyData(const Table4& org, const bool cc)
{
    BaseClass::copyData(org);
  
    // Delete old data
    if (!cc && wtable != 0) { delete[] wtable; wtable = 0; }

    // Copy new data
    nw = org.nw;
    if (org.wtable != 0) {
        wtable = new LCreal[nw];
        for (unsigned int i = 0; i < nw; i++) wtable[i] = org.wtable[i];
    }
    else wtable = 0;
    valid = isValid();
}

void Table4::deleteData()
{
    if (wtable != 0) delete[] wtable;       
    wtable = 0;
    nw = 0;
}

//------------------------------------------------------------------------------
// 4D table is input as a list of 3D sub-tables.
// example:  { { { [ 1111 1112 1113 ] [ 1121 1122 1123 ] [ 1131 1132 1133 ] } }
//             { { [ 1211 1212 1213 ] [ 1221 1222 1223 ] [ 1231 1232 1233 ] } }
//             { { [ 1311 1312 1313 ] [ 1321 1322 1323 ] [ 1331 1332 1333 ] } }
//
//             { { [ 2111 2112 2113 ] [ 2121 2122 2123 ] [ 2131 2132 2133 ] } }
//             { { [ 2211 2212 2213 ] [ 2221 2222 2223 ] [ 2231 2232 2233 ] } }
//             { { [ 2311 2312 2313 ] [ 2321 2322 2323 ] [ 2331 2332 2333 ] } }
//
//             { { [ 3111 3112 3113 ] [ 3121 3122 3123 ] [ 3131 3132 3133 ] } }
//             { { [ 3211 3212 3213 ] [ 3221 3222 3223 ] [ 3231 3232 3233 ] } }
//             { { [ 3311 3312 3313 ] [ 3321 3322 3323 ] [ 3331 3332 3333 ] } }
//
//             { { [ 4111 4112 4113 ] [ 4121 4122 4123 ] [ 4131 4132 4133 ] } }
//             { { [ 4211 4212 4213 ] [ 4221 4222 4223 ] [ 4231 4232 4233 ] } }
//             { { [ 4311 4312 4313 ] [ 4321 4322 4323 ] [ 4331 4332 4333 ] } } }
//------------------------------------------------------------------------------
bool Table4::loadData(const List& list, LCreal* const table)
{
    // Make sure we have the proper number of entries in the list
    unsigned int n1 = list.entries();
    bool ok = (n1 > 0 && n1 == nw);

    // Process each item in the list
    unsigned int i = 0;
    unsigned int k = BaseClass::tableSize();
    const List::Item* item = list.getFirstItem();
    while (ok && item != 0) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != 0) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != 0) {
                ok &= BaseClass::loadData(*slist, &table[i]);
                i += k;
            }
        }
        item = item->getNext();
    }
    if (ok) valid = isValid();
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table4::isValid() const
{
    return (nw >= 1) && (wtable != 0) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table4::tableSize() const           
{
    return nw * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty 
//------------------------------------------------------------------------------
LCreal Table4::getMinW() const
{
    if (wtable != 0 && nw > 0)
        return (wtable[0] < wtable[nw - 1]) ? wtable[0] : wtable[nw - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table4::getMaxW() const
{
    if (wtable != 0 && nw > 0)
        return (wtable[0] < wtable[nw - 1]) ? wtable[nw - 1] : wtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}


//------------------------------------------------------------------------------
//  4D LFIs
//------------------------------------------------------------------------------
LCreal
Table4::lfi(const LCreal iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception
    
   const LCreal* y_data = getYData();
   const LCreal* z_data = getZData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, y_data[0], z_data[0], wtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return Table::lfi( iv1, y_data[0], z_data[0], wtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table4::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception
    
   const LCreal* z_data = getZData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return Table::lfi( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table4::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception
    
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return Table::lfi( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table4::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception
    
   if (f != 0) {
       TableStorage* s = dynamic_cast<TableStorage*>(f);
       if (s == 0) throw new ExpInvalidFStorage();

       return Table::lfi( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
                           getYData(), getNumYPoints(), getZData(),
                           getNumZPoints(), getWData(), getNumWPoints(),
                           getDataTable(), isExtrapolationEnabled(),
                           &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
       return Table::lfi( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
                           getYData(), getNumYPoints(), getZData(),
                           getNumZPoints(), getWData(), getNumWPoints(),
                           getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setWBreakpoints4() -- For Table4 
//------------------------------------------------------------------------------
bool Table4::setWBreakpoints4(const List* const swb4obj)
{
    if (swb4obj != 0) {
        loadVector(*swb4obj, &wtable, &nw);
        valid = isValid();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table4::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table4::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout, i + j);
    sout << "w: ";
    printVector(sout, wtable, nw);
    sout << std::endl;

    BaseClass::serialize(sout, i + j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
//  print 4D table
//------------------------------------------------------------------------------
void Table4::printData(std::ostream& sout, const LCreal* tbl, const unsigned int ns) const
{
    indent(sout, ns);
    sout << "{" << std::endl;

    if (tbl != 0) {
        unsigned int j = 0;
        unsigned int k = BaseClass::tableSize();
        for (unsigned int i = 0; i < nw; i++) {
            BaseClass::printData(sout, &tbl[j], ns + 4);
            j += k;
        }
    }

    indent(sout, ns);
    sout << "}" << std::endl;
}


//==============================================================================
// Class Table5
//==============================================================================
IMPLEMENT_SUBCLASS(Table5, "Table5")

BEGIN_SLOTTABLE(Table5)
    "v",           // V breakpoints
END_SLOTTABLE(Table5)

BEGIN_SLOT_MAP(Table5)
    ON_SLOT(1,setVBreakpoints5,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Table5::Table5() : Table4()
{
   STANDARD_CONSTRUCTOR()
   vtable = 0;
   nv = 0;
}

Table5::Table5(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize,
                   const LCreal* wtbl, const unsigned int wsize,
                   const LCreal* vtbl, const unsigned int vsize)
                   : Table4(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize, wtbl, wsize),
                     vtable(0), nv(0)
{
    STANDARD_CONSTRUCTOR()
    if (vtbl != 0 && vsize > 0) {   /* Copy the v breakpoints */
        vtable = new LCreal[vsize];
        if (vtable != 0) {
            for (unsigned int i = 0; i < vsize; i++) vtable[i] = vtbl[i];
            nv = vsize;
            valid = isValid();
        }
    }
}

void Table5::copyData(const Table5& org, const bool cc)
{
    BaseClass::copyData(org);
  
    // Delete old data
    if (!cc && vtable != 0) { delete[] vtable; vtable = 0; }

    // Copy new data
    nv = org.nv;
    if (org.vtable != 0) {
        vtable = new LCreal[nv];
        for (unsigned int i = 0; i < nv; i++) vtable[i] = org.vtable[i];
    }
    else vtable = 0;
    valid = isValid();
}

void Table5::deleteData()
{
    if (vtable != 0) delete[] vtable;       
    vtable = 0;
    nv = 0;
}

//------------------------------------------------------------------------------
// 5D table is input as a list of 4D sub-tables.
//------------------------------------------------------------------------------
bool Table5::loadData(const List& list, LCreal* const table)
{
    // Make sure we have the proper number of entries in the list
    unsigned int n1 = list.entries();
    bool ok = (n1 > 0 && n1 == nv);

    // Process each item in the list
    unsigned int i = 0;
    unsigned int k = BaseClass::tableSize();
    const List::Item* item = list.getFirstItem();
    while (ok && item != 0) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != 0) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != 0) {
                ok &= BaseClass::loadData(*slist, &table[i]);
                i += k;
            }
        }
        item = item->getNext();
    }
    if (ok) valid = isValid();
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table5::isValid() const
{
    return (nv >= 1) && (vtable != 0) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table5::tableSize() const           
{
    return nv * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty 
//------------------------------------------------------------------------------
LCreal Table5::getMinV() const
{
    if (vtable != 0 && nv > 0)
        return (vtable[0] < vtable[nv - 1]) ? vtable[0] : vtable[nv - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table5::getMaxV() const
{
    if (vtable != 0 && nv > 0)
        return (vtable[0] < vtable[nv - 1]) ? vtable[nv - 1] : vtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  5D LFIs
//------------------------------------------------------------------------------
LCreal
Table5::lfi(const LCreal iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const LCreal* y_data = getYData();
   const LCreal* z_data = getZData();
   const LCreal* w_data = getWData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return Table::lfi( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

LCreal
Table5::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const LCreal* z_data = getZData();
   const LCreal* w_data = getWData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return Table::lfi( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

LCreal
Table5::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const LCreal* w_data = getWData();
   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return Table::lfi( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

LCreal
Table5::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return Table::lfi( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table5::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, const LCreal iv5, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != 0) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == 0) throw new ExpInvalidFStorage();

      return Table::lfi( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return Table::lfi( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setVBreakpoints5() -- For Table5 
//------------------------------------------------------------------------------
bool Table5::setVBreakpoints5(const List* const swb5obj)
{
    if (swb5obj != 0) {
        loadVector(*swb5obj, &vtable, &nv);
        valid = isValid();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Table
//------------------------------------------------------------------------------
Object* Table5::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Table5::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
        if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout, i + j);
    sout << "v: ";
    printVector(sout, vtable, nv);
    sout << std::endl;

    BaseClass::serialize(sout, i + j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
//  Print 5D table
//------------------------------------------------------------------------------
void Table5::printData(std::ostream& sout, const LCreal* tbl, const unsigned int ns) const
{
    indent(sout, ns);
    sout << "{" << std::endl;

    if (tbl != 0) {
        unsigned int j = 0;
        unsigned int k = BaseClass::tableSize();
        for (unsigned int i = 0; i < nv; i++) {
            BaseClass::printData(sout, &tbl[j], ns + 4);
            j += k;
        }
    }

    indent(sout, ns);
    sout << "}" << std::endl;
}



//==============================================================================
// Table's static Linear Function Interpolators
//==============================================================================

//------------------------------------------------------------------------------
// lfi - One dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal Table::lfi(
         const LCreal x,            // Independent variable #1
         const LCreal *x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal *a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp    // Previous X breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nx == 1) return a_data[0];   // Early out with the only point

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nx - 1;
   int delta = 1;
   if (x_data[1] < x_data[0]) {
      // Reverse order of breakpoints
      low = nx - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int x2 = 0;
   if (x <= x_data[low]) {
      // At or below the 'low' end
      x2 = low + delta;
      if (!eFlg) {
         // No extrapolate; early out with the low end point
         if (xbp != 0) *xbp = x2;
         return(a_data[low]);
      }
   }
   else if (x >= x_data[high]) {
      // At or above the 'high' end
      x2 = high;
      if (!eFlg) {
         // No extrapolate; early out with the high end point
         if (xbp != 0) *xbp = x2;
         return(a_data[high]);
      }
   }
   else {
      if (xbp == 0) {
         // Simple linear search
         x2 = low + delta;
         while (x > x_data[x2]) { x2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         x2 = *xbp;
         if (x2 >= nx) x2 = 0;                         // safety check
         while (x > x_data[x2]) { x2 += delta; }       // search up
         while (x < x_data[x2-delta]) { x2 -= delta; } // search down
         *xbp = x2;
      }
   }

   // ---
   // Linear interpolation
   // ---
   unsigned int x1 = x2 - delta;
   LCreal m = (x - x_data[x1]) / (x_data[x2] - x_data[x1]);
   return m * (a_data[x2] - a_data[x1]) + a_data[x1];
} 

//------------------------------------------------------------------------------
// lfi - Two dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal Table::lfi(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal *x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal *y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal *a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp    // Previous Y breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (ny == 1) {
      return Table::lfi(x, x_data, nx, &a_data[0], eFlg, xbp);
   }
    
   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = ny - 1;
   int delta = 1;
   if (y_data[1] < y_data[0]) {
      // Reverse order of breakpoints
      low = ny - 1;
      high = 0;
      delta = -1;
   }
    
   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int y2 = 0;
   if (y <= y_data[low]) {
      // At or below the 'low' end
      y2 = low + delta;
      if (!eFlg) {
         // Early out at the 'low' end
         if (ybp != 0) *ybp = y2;
         unsigned int ax = nx * low;
         return Table::lfi(x, x_data, nx, &a_data[ax], eFlg, xbp);
      }
   }
   else if (y >= y_data[high]) {
      // At or above the 'high' end
      y2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (ybp != 0) *ybp = y2;
         unsigned int ax = nx * high;
         return Table::lfi(x, x_data, nx, &a_data[ax], eFlg, xbp);
      }
   }
   else {
      // Find the breakpoints we're between
      if (ybp == 0) {
         // Simple linear search
         y2 = low + delta;
         while (y > y_data[y2]) { y2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         y2 = *ybp;
         if (y2 >= ny) y2 = 0;                         // safty check
         while (y > y_data[y2]) { y2 += delta; }       // search up
         while (y < y_data[y2-delta]) { y2 -= delta; } // search down
         *ybp = y2;
      }
   }

   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   unsigned int y1 = y2 - delta;
   unsigned int ax1 = nx * y1;
   LCreal a1 = Table::lfi(x, x_data, nx, &a_data[ax1], eFlg, xbp);
   unsigned int ax2 = nx * y2;
   LCreal a2 = Table::lfi(x, x_data, nx, &a_data[ax2], eFlg, xbp);

   // ---
   // Final linear interpolation
   // ---
   LCreal m = (y - y_data[y1]) / (y_data[y2] - y_data[y1]);
   return m * (a2 - a1) + a1;
} 

//------------------------------------------------------------------------------
// lfi - Three dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal Table::lfi(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal *x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal *y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal *z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal *a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp    // Previous Z breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nz == 1) {
      return Table::lfi(x, y, x_data, nx, y_data, ny, &a_data[0], eFlg, xbp, ybp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nz - 1;
   int delta = 1;
   if (z_data[1] < z_data[0]) {
      // Reverse order of breakpoints
      low = nz - 1;
      high = 0;
      delta = -1;
   }
    
   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int z2 = 0;
   if (z <= z_data[low]) {
      // At or below the 'low' end
      z2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (zbp != 0) *zbp = z2;
         unsigned int ax = nx * ny * low;
         return Table::lfi(x, y, x_data, nx, y_data, ny, &a_data[ax], eFlg, xbp, ybp);
      }
   }
   else if (z >= z_data[high]) {
      // At or above the 'high' end
      z2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (zbp != 0) *zbp = z2;
         unsigned int ax = nx * ny * high;
         return Table::lfi(x, y, x_data, nx, y_data, ny, &a_data[ax], eFlg, xbp, ybp);
      }
   }
   else {
      // Find the breakpoints we're between
      if (zbp == 0) {
         // Simple linear search
         z2 = low + delta;
         while (z > z_data[z2]) { z2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         z2 = *zbp;
         if (z2 >= nz) z2 = 0;                         // safty check
         while (z > z_data[z2]) { z2 += delta; }       // search up
         while (z < z_data[z2-delta]) { z2 -= delta; } // search down
         *zbp = z2;
      }
   }


   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   unsigned int z1 = z2 - delta;
   unsigned int ax1 = nx * ny * z1;
   LCreal a1 = Table::lfi(x, y, x_data, nx, y_data, ny, &a_data[ax1], eFlg, xbp, ybp);
   unsigned int ax2 = nx * ny * z2;
   LCreal a2 = Table::lfi(x, y, x_data, nx, y_data, ny, &a_data[ax2], eFlg, xbp, ybp);

   // ---
   // Final linear interpolation
   // ---
   LCreal m = (z - z_data[z1]) / (z_data[z2] - z_data[z1]);
   return m * (a2 - a1) + a1;
} 

//------------------------------------------------------------------------------
// lfi - Four dimension Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal Table::lfi(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal w,            // Independent variable #4
         const LCreal *x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal *y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal *z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal *w_data,      // Table of independent variable #4 breakpoints
         const unsigned int nw,     // Size of w_data table
         const LCreal *a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp,   // Previous Z breakpoint (optional)
         unsigned int* const wbp    // Previous W breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nw == 1) {
      return Table::lfi(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[0], eFlg, xbp, ybp, zbp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nw - 1;
   int delta = 1;
   if (w_data[1] < w_data[0]) {
      // Reverse order of breakpoints
      low = nw - 1;
      high = 0;
      delta = -1;
   }
    
   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int w2 = 0;
   if (w <= w_data[low]) {
      // At or below the 'low' end
      w2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (wbp != 0) *wbp = w2;
         unsigned int ax = nx * ny * nz * low;
         return Table::lfi(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax], eFlg, xbp, ybp, zbp);
      }
   }
   else if (w >= w_data[high]) {
      // At or above the 'high' end
      w2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (wbp != 0) *wbp = w2;
         unsigned int ax = nx* ny * nz * high;
         return Table::lfi(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax], eFlg, xbp, ybp, zbp);
      }
   }
   else {
      // Find the breakpoints we're between
      //w2 = low + delta;
      //while (w >= w_data[w2]) { w2 += delta; }
      if (wbp == 0) {
         // Simple linear search
         w2 = low + delta;
         while (w > w_data[w2]) { w2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         w2 = *wbp;
         if (w2 >= nw) w2 = 0;                         // safty check
         while (w > w_data[w2]) { w2 += delta; }       // search up
         while (w < w_data[w2-delta]) { w2 -= delta; } // search down
         *wbp = w2;
      }
   }

   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   unsigned int w1 = w2 - delta;
   unsigned int ax1 = nx * ny * nz * w1;
   LCreal a1 = Table::lfi(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax1], eFlg, xbp, ybp, zbp);
   unsigned int ax2 = nx * ny * nz * w2;
   LCreal a2 = Table::lfi(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax2], eFlg, xbp, ybp, zbp);

   // ---
   // Final linear interpolation
   // ---
   LCreal m  = (w - w_data[w1]) / (w_data[w2] - w_data[w1]);
   return m * (a2 - a1) + a1;
} 

//------------------------------------------------------------------------------
// lfi - Five dimension Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal Table::lfi(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal w,            // Independent variable #4
         const LCreal v,            // Independent variable #5
         const LCreal *x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal *y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal *z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal *w_data,      // Table of independent variable #4 breakpoints
         const unsigned int nw,     // Size of w_data table
         const LCreal *v_data,      // Table of independent variable #5 breakpoints
         const unsigned int nv,     // Size of v_data table
         const LCreal *a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp,   // Previous Z breakpoint (optional)
         unsigned int* const wbp,   // Previous W breakpoint (optional)
         unsigned int* const vbp    // Previous V breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nv == 1) {
        return Table::lfi(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[0], eFlg, xbp, ybp, zbp, wbp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nv - 1;
   int delta = 1;
   if (v_data[1] < v_data[0]) {
      // Reverse order of breakpoints
      low = nv - 1;
      high = 0;
      delta = -1;
   }
    
   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int v2 = 0;
   if (v <= v_data[low]) {
      // At or below the 'low' end
      v2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (vbp != 0) *vbp = v2;
         unsigned int ax = nx * ny * nz * nw * low;
         return Table::lfi(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax], eFlg, xbp, ybp, zbp, wbp);
      }
   }
   else if (v >= v_data[high]) {
      // At or above the 'high' end
      v2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (vbp != 0) *vbp = v2;
         unsigned int ax = nx* ny * nz * nw * high;
         return Table::lfi(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax], eFlg, xbp, ybp, zbp, wbp);
      }
   }
   else {
      // Find the breakpoints we're between
      //v2 = low + delta;
      //while (v >= v_data[v2]) { v2 += delta; }
      if (vbp == 0) {
         // Simple linear search
         v2 = low + delta;
         while (v > v_data[v2]) { v2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         v2 = *vbp;
         if (v2 >= nv) v2 = 0;                         // safty check
         while (v > v_data[v2]) { v2 += delta; }       // search up
         while (v < v_data[v2-delta]) { v2 -= delta; } // search down
         *vbp = v2;
      }
   }


   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   unsigned int v1 = v2 - delta;
   unsigned int ax1 = nx * ny * nz * nw * v1;
   LCreal a1 = Table::lfi(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax1], eFlg, xbp, ybp, zbp, wbp);
   unsigned int ax2 = nx * ny * nz * nw * v2;
   LCreal a2 = Table::lfi(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax2], eFlg, xbp, ybp, zbp, wbp);

   // ---
   // Final linear interpolation
   // ---
   LCreal m  = (v - v_data[v1]) / (v_data[v2] - v_data[v1]);
   return m * (a2 - a1) + a1;
} 

} // End Basic namespace
} // End Eaagles namespace
