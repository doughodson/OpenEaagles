//------------------------------------------------------------------------------
// Table1, Table2, Table3, Table4, Table5
//------------------------------------------------------------------------------

#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/util/lfi.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"

namespace Eaagles {
namespace Basic {

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
   xtable = nullptr;
   nx = 0;
}

Table1::Table1(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize)
                   : Table(dtbl, dsize), xtable(nullptr), nx(0)
{
    STANDARD_CONSTRUCTOR()
    if (xtbl != nullptr && xsize > 0) {   /* Copy the x breakpoints */
        xtable = new LCreal[xsize];
        if (xtable != nullptr) {
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
    if (!cc && xtable != nullptr) { delete[] xtable; xtable = nullptr; }

    // Copy new data
    nx = org.nx;
    if (org.xtable != nullptr) {
        xtable = new LCreal[nx];
        for (unsigned int i = 0; i < nx; i++) xtable[i] = org.xtable[i];
    }
    else xtable = nullptr;
    valid = isValid();
}

void Table1::deleteData()
{
    if (xtable != nullptr) delete[] xtable;
    xtable = nullptr;
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
    return (nx >= 1) && (xtable != nullptr) && BaseClass::isValid();
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
    if (xtable != nullptr && nx > 0)
        return (xtable[0] < xtable[nx - 1]) ? xtable[0] : xtable[nx - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table1::getMaxX() const
{
    if (xtable != nullptr && nx > 0)
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

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_1D(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled(), &s->xbp);
   }
   else {
      return lfi::lfi_1D(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled());
   }
}

//------------------------------------------------------------------------------
// setXBreakpoints1() -- for Table1
//------------------------------------------------------------------------------
bool Table1::setXBreakpoints1(const List* const sxb1obj)
{
    if (sxb1obj != nullptr) {
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
        sout << "( " << getFactoryName() << std::endl;
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
   ytable = nullptr;
   ny = 0;
}

Table2::Table2(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize)
                   : Table1(dtbl, dsize, xtbl, xsize), ytable(nullptr), ny(0)
{
    STANDARD_CONSTRUCTOR()
    if (ytbl != nullptr && ysize > 0) {   /* Copy the y breakpoints */
        ytable = new LCreal[ysize];
        if (ytable != nullptr) {
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
    if (!cc && ytable != nullptr) { delete[] ytable; ytable = nullptr; }

    // Copy new data
    ny = org.ny;
    if (org.ytable != nullptr) {
        ytable = new LCreal[ny];
        for (unsigned int i = 0; i < ny; i++) ytable[i] = org.ytable[i];
    }
    else ytable = nullptr;
    valid = isValid();
}

void Table2::deleteData()
{
    if (ytable != nullptr) delete[] ytable;
    ytable = nullptr;
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
    while (ok && item != nullptr) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != nullptr) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != nullptr) {
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
    return (ny >= 1) && (ytable != nullptr) && BaseClass::isValid();
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
    if (ytable != nullptr && ny > 0)
        return (ytable[0] < ytable[ny - 1]) ? ytable[0] : ytable[ny - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table2::getMaxY() const
{
    if (ytable != nullptr && ny > 0)
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

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_2D( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   }
   else {
      return lfi::lfi_2D( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

LCreal
Table2::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_2D( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   }
   else {
      return lfi::lfi_2D( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setYBreakpoints2() -- for Table2
//------------------------------------------------------------------------------
bool Table2::setYBreakpoints2(const List* const syb2obj)
{
    if (syb2obj != nullptr) {
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
        sout << "( " << getFactoryName() << std::endl;
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

    if (tbl != nullptr) {
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
   ztable = nullptr;
   nz = 0;
}

Table3::Table3(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize)
                   : Table2(dtbl, dsize, xtbl, xsize, ytbl, ysize),
                     ztable(nullptr), nz(0)
{
    STANDARD_CONSTRUCTOR()
    if (ztbl != nullptr && zsize > 0) {   /* Copy the z breakpoints */
        ztable = new LCreal[zsize];
        if (ztable != nullptr) {
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
    if (!cc && ztable != nullptr) { delete[] ztable; ztable = nullptr; }

    // Copy new data
    nz = org.nz;
    if (org.ztable != nullptr) {
        ztable = new LCreal[nz];
        for (unsigned int i = 0; i < nz; i++) ztable[i] = org.ztable[i];
    }
    else ztable = nullptr;
    valid = isValid();
}

void Table3::deleteData()
{
    if (ztable != nullptr) delete[] ztable;
    ztable = nullptr;
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
    while (ok && item != nullptr) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != nullptr) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != nullptr) {
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
    return (nz >= 1) && (ztable != nullptr) && BaseClass::isValid();
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
    if (ztable != nullptr && nz > 0)
        return (ztable[0] < ztable[nz - 1]) ? ztable[0] : ztable[nz - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table3::getMaxZ() const
{
    if (ztable != nullptr && nz > 0)
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_3D( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return lfi::lfi_3D( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table3::lfi(const LCreal iv1, const LCreal iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_3D( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return lfi::lfi_3D( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table3::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_3D( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   }
   else {
      return lfi::lfi_3D( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setZBreakpoints3() -- for Table3
//------------------------------------------------------------------------------
bool Table3::setZBreakpoints3(const List* const szb3obj)
{
    if (szb3obj != nullptr) {
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
        sout << "( " << getFactoryName() << std::endl;
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

    if (tbl != nullptr) {
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
   wtable = nullptr;
   nw = 0;
}
Table4::Table4(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize,
                   const LCreal* wtbl, const unsigned int wsize)
                   : Table3(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize),
                     wtable(nullptr), nw(0)
{
    STANDARD_CONSTRUCTOR()
    if (wtbl != nullptr && wsize > 0) {   /* Copy the w breakpoints */
        wtable = new LCreal[wsize];
        if (wtable != nullptr) {
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
    if (!cc && wtable != nullptr) { delete[] wtable; wtable = nullptr; }

    // Copy new data
    nw = org.nw;
    if (org.wtable != nullptr) {
        wtable = new LCreal[nw];
        for (unsigned int i = 0; i < nw; i++) wtable[i] = org.wtable[i];
    }
    else wtable = nullptr;
    valid = isValid();
}

void Table4::deleteData()
{
    if (wtable != nullptr) delete[] wtable;
    wtable = nullptr;
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
    while (ok && item != nullptr) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != nullptr) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != nullptr) {
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
    return (nw >= 1) && (wtable != nullptr) && BaseClass::isValid();
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
    if (wtable != nullptr && nw > 0)
        return (wtable[0] < wtable[nw - 1]) ? wtable[0] : wtable[nw - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table4::getMaxW() const
{
    if (wtable != nullptr && nw > 0)
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_4D( iv1, y_data[0], z_data[0], wtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return lfi::lfi_4D( iv1, y_data[0], z_data[0], wtable[0], getXData(),
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_4D( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return lfi::lfi_4D( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table4::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_4D( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return lfi::lfi_4D( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

LCreal
Table4::lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
       TableStorage* s = dynamic_cast<TableStorage*>(f);
       if (s == nullptr) throw new ExpInvalidFStorage();

       return lfi::lfi_4D( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
                           getYData(), getNumYPoints(), getZData(),
                           getNumZPoints(), getWData(), getNumWPoints(),
                           getDataTable(), isExtrapolationEnabled(),
                           &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
       return lfi::lfi_4D( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
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
    if (swb4obj != nullptr) {
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
        sout << "( " << getFactoryName() << std::endl;
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

    if (tbl != nullptr) {
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
   vtable = nullptr;
   nv = 0;
}

Table5::Table5(const LCreal* dtbl, const unsigned int dsize,
                   const LCreal* xtbl, const unsigned int xsize,
                   const LCreal* ytbl, const unsigned int ysize,
                   const LCreal* ztbl, const unsigned int zsize,
                   const LCreal* wtbl, const unsigned int wsize,
                   const LCreal* vtbl, const unsigned int vsize)
                   : Table4(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize, wtbl, wsize),
                     vtable(nullptr), nv(0)
{
    STANDARD_CONSTRUCTOR()
    if (vtbl != nullptr && vsize > 0) {   /* Copy the v breakpoints */
        vtable = new LCreal[vsize];
        if (vtable != nullptr) {
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
    if (!cc && vtable != nullptr) { delete[] vtable; vtable = nullptr; }

    // Copy new data
    nv = org.nv;
    if (org.vtable != nullptr) {
        vtable = new LCreal[nv];
        for (unsigned int i = 0; i < nv; i++) vtable[i] = org.vtable[i];
    }
    else vtable = nullptr;
    valid = isValid();
}

void Table5::deleteData()
{
    if (vtable != nullptr) delete[] vtable;
    vtable = nullptr;
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
    while (ok && item != nullptr) {
        const Pair* p = dynamic_cast<const Pair*>(item->getValue());
        if (p != nullptr) {
            const List* slist = dynamic_cast<const List*>(p->object());
            if (slist != nullptr) {
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
    return (nv >= 1) && (vtable != nullptr) && BaseClass::isValid();
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
    if (vtable != nullptr && nv > 0)
        return (vtable[0] < vtable[nv - 1]) ? vtable[0] : vtable[nv - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

LCreal Table5::getMaxV() const
{
    if (vtable != nullptr && nv > 0)
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_5D( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return lfi::lfi_5D( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_5D( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return lfi::lfi_5D( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
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
   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_5D( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return lfi::lfi_5D( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
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

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_5D( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return lfi::lfi_5D( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
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

   if (f != nullptr) {
      TableStorage* s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi::lfi_5D( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   }
   else {
      return lfi::lfi_5D( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
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
    if (swb5obj != nullptr) {
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
        sout << "( " << getFactoryName() << std::endl;
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

    if (tbl != nullptr) {
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

} // End Basic namespace
} // End Eaagles namespace
