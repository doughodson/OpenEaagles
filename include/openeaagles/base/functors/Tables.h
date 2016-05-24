//------------------------------------------------------------------------------
// Classes: Table1, Table2, Table3, Table4, Table5
//------------------------------------------------------------------------------
#ifndef __oe_base_Tables_H__
#define __oe_base_Tables_H__

#include "openeaagles/base/Object.h"
#include "openeaagles/base/functors/Functions.h"
#include "openeaagles/base/functors/Table.h"

namespace oe {
namespace base {

class List;
class Number;

//==============================================================================
// Class: Table1
//
// Description: 1D LFI data table
//
// Factory name: Table1
// Slots:
//    x    <List>  Independent variable #1 (iv1) points
//==============================================================================
class Table1 : public Table {
    DECLARE_SUBCLASS(Table1, Table)

public:
   Table1();
   Table1(const double* dtbl, const unsigned int dsize,
             const double* xtbl, const unsigned int xsize);

   // Returns the number of x breakpoints
   unsigned int getNumXPoints() const { return nx; }

   // Returns a pointer to the breakpoint data for x
   const double* getXData() const     { return xtable; }

   double getMinX() const;    // Min value of the X (iv1) breakpoints
   double getMaxX() const;    // Max value of the X (iv1) breakpoints

   // 1D Linear Function Interpolator: returns the result of f(x) using linear interpolation
   virtual double lfi(const double iv1, FStorage* const s = nullptr) const;

   // Load the X (iv1) breakpoints
   virtual bool setXBreakpoints1(const List* const bkpts);

   virtual unsigned int tableSize() const override;

   virtual bool isValid() const override;

protected:
   virtual bool loadData(const List& list, double* const table) override;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const override;

private:
   double* xtable;    // X Breakpoint Table
   unsigned int nx;   // Number of x breakpoints
};


//==============================================================================
// Class: Table2
//
// Description: 2D LFI data table
//
// Factory name: Table2
// Slots:
//    y    <List>  Independent variable #2 (iv2) points
//
//==============================================================================
class Table2 : public Table1 {
    DECLARE_SUBCLASS(Table2, Table1)

public:
   Table2();
   Table2(const double* dtbl, const unsigned int dsize,
            const double* xtbl, const unsigned int xsize,
            const double* ytbl, const unsigned int ysize);

   // Returns the number of y breakpoints
   unsigned int getNumYPoints() const { return ny; }

   // Returns a pointer to the breakpoint data for y
   const double* getYData() const     { return ytable; }

   double getMinY() const;    // Min value of the Y (iv2) breakpoints
   double getMaxY() const;    // Max value of the Y (iv2) breakpoints

   // 2D Linear Function Interpolator: returns the result of f(x,y) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const;

   // Load the Y (iv2) breakpoints
   virtual bool setYBreakpoints2(const List* const bkpts);

   virtual double lfi(const double iv1, FStorage* const s = nullptr) const override;
   virtual unsigned int tableSize() const override;

   virtual bool isValid() const override;

protected:
   virtual bool loadData(const List& list, double* const table) override;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const override;

private:
   double* ytable;    // Y Breakpoint Table
   unsigned int ny;   // Number of y breakpoints
};


//==============================================================================
// Class: Table3
//
// Description: 3D LFI data table
//
// Factory name: Table3
// Slots:
//    z    <List>  Independent variable #3 (iv3) points
//
//==============================================================================
class Table3 : public Table2 {
   DECLARE_SUBCLASS(Table3, Table2)

public:
   Table3();
   Table3(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize);

   // Returns the number of z breakpoints.
   unsigned int getNumZPoints() const { return nz; }

   // double* getZData()
   const double* getZData() const     { return ztable; }

   double getMinZ() const;    // Min value of the Z (iv3) breakpoints
   double getMaxZ() const;    // Max value of the Z (iv3) breakpoints

   // 3D Linear Function Interpolator: returns the result of f(x,y,z) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const;

   // Loads the Z (iv3) breakpoints
   virtual bool setZBreakpoints3(const List* const bkpts);

   virtual double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, FStorage* const s = nullptr) const override;
   virtual unsigned int tableSize() const override;

   virtual bool isValid() const override;

protected:
   virtual bool loadData(const List& list, double* const table) override;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const override;

private:
   double* ztable;    // Z Breakpoint Table
   unsigned int nz;   // Number of z breakpoints
};


//==============================================================================
// Class: Table4
//
// Description: 4D LFI data table
//
// Factory name: Table4
// Slots:
//    w    <List>  Independent variable #4 (iv4) points
//
//==============================================================================
class Table4 : public Table3 {
   DECLARE_SUBCLASS(Table4, Table3)

public:
   Table4();
   Table4(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize,
          const double* wtbl, const unsigned int wsize);

   // Returns the number of w breakpoints.
   unsigned int getNumWPoints() const { return nw; }

   // Returns a pointer to the breakpoint data for w
   const double* getWData() const     { return wtable; }

   double getMinW() const;    // Min value of the W (iv4) breakpoints
   double getMaxW() const;    // Max value of the W (iv4) breakpoints

   // 4D Linear Function Interpolator: returns the result of f(x,y,z,w) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = nullptr) const;

   // Loads the W (iv4) breakpoints
   virtual bool setWBreakpoints4(const List* const bkpts);

   virtual double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, FStorage* const s = nullptr) const override;
   virtual unsigned int tableSize() const override;

   virtual bool isValid() const override;

protected:
   virtual bool loadData(const List& list, double* const table) override;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const override;

private:
   double* wtable;    // W Breakpoint Table
   unsigned int nw;   // Number of w breakpoints
};

//==============================================================================
// Class: Table5
//
// Description: 5D LFI data table
//
// Factory name: Table5
// Slots:
//    v    <List>  Independent variable #5 (iv5) points
//
//==============================================================================
class Table5 : public Table4 {
   DECLARE_SUBCLASS(Table5, Table4)

public:
   Table5();
   Table5(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize,
          const double* wtbl, const unsigned int wsize,
          const double* vtbl, const unsigned int vsize);

   // Returns the number of v breakpoints
   unsigned int getNumVPoints() const { return nv; }

   // Returns a pointer to the breakpoint data for v
   const double* getVData() const     { return vtable; }

   double getMinV() const;    // Min value of the V (iv5) breakpoints
   double getMaxV() const;    // Max value of the V (iv5) breakpoints

   virtual double lfi(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s = nullptr) const;

   // Loads the V (iv5) breakpoints
   virtual bool setVBreakpoints5(const List* const bkpts);

   virtual double lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   virtual double lfi(const double iv1, FStorage* const s = nullptr) const override;
   virtual unsigned int tableSize() const override;

   virtual bool isValid() const override;

protected:
   virtual bool loadData(const List& list, double* const table) override;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const override;

private:
   double* vtable;     // V Breakpoint Table
   unsigned int nv;    // Number of v breakpoints
};

}
}

#endif
