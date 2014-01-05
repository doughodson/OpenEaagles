//------------------------------------------------------------------------------
// Classes: Table, Table1, Table2, Table3, Table4, Table5
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Tables_H__
#define __Eaagles_Basic_Tables_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/Functions.h"

namespace Eaagles {
namespace Basic {

class List;
class Number;

//==============================================================================
// Class: Table
//
// Description: Abstract table class ---
//
//    Used as a base for derived table classes that maintain multi-dimensional,
//    dependent data tables, along with their independent variable breakpoint
//    tables, and provides linear function interpolation of the dependent
//    variable data.
//
//
// Slots:
//    data        <List>      ! Dependant variable data. (default: 0)
//    extrapolate <Boolean>   ! Extrapolate beyond the given data table limits (default: 0)
//
// Notes:
//    1) There are several static lfi() functions that are the core linear
//       function interpolators used by the tables classes.  If you want to
//       maintain your own breakpoint and data tables, you may call these
//       directly, otherwise use our derived table classes.
//
//    2) The isValid() function will return true only if all of the required
//       dependent and independent data has been set.
//
//    3) For large tables, use the storageFactory() function to create the FStorage
//       object (see Functions.h) that will maintain the table's previous search
//       values.  This object is an optional parameter to the non-static lfi()
//       functions.
//
//    4) If a dependent variable exceeds a breakpoint table data then the lfi()
//       result is clamped at the last known dependent value.  If the extrapolate
//       flag is true, we'll extrapolate beyond the given data table.
//
// Exceptions:
//      ExpInvalidTable
//          Thrown by Table derived classes' lfi(), minX(), maxX(), minY(),
//          maxY(), minZ(), maxZ(), minW(), and maxW() methods when the table's
//          data set is invalid.
//
//      ExpInvalidVector
//          Thrown by Table's method loadVector() when it's passed an invalid
//          vector.
//
//      ExpInvalidFStorage 
//          Thrown by Table's storage class when the data in FStorage in incorrect. 
//
//==============================================================================
class Table : public Object {
    DECLARE_SUBCLASS(Table, Object)

public:
   Table();
   Table(const LCreal* dtbl, const unsigned int dsize); 

   // Returns a pointer to the dependent variable data table. 
   const LCreal* getDataTable() const    { return dtable; }

   // Returns the number of entries in the data table
   virtual unsigned int tableSize() const = 0;
   virtual bool setDataTable(const List* const msg);

   // Returns the min and max values of the dependent variable data table
   virtual void findMinMax(LCreal* minValue, LCreal* maxValue) const;

   // Returns true if extrapolation beyond the table's data is enabled.
   bool isExtrapolationEnabled() const   { return extFlg; }

   // Sets the extrapolation enabled flag.
   bool setExtrapolationEnabled(const bool flg);
   virtual bool setExtrapolationEnabled(const Number* const msg);

   // Data storage factory (pre-ref()'d)
   virtual FStorage* storageFactory() const;
 
   // ---
   // Static 1D Linear Function Interpolator
   //    x       - Independent variable #1
   //    x_data  - Table of independent variable #1 breakpoints
   //    nx      - Size of x_data table
   //    xbp     - Pointer to the previous X breakpoint (optional)
   //    a_data  - Table of dependent variable data
   //    eFlg    - Extrapolation enabled flag
   // ---

   static LCreal lfi(
         const LCreal x,
         const LCreal *x_data, const unsigned int nx,
         const LCreal *a_data,
         const bool eFlg, 
         unsigned int* const xbp=0
      );

   // ---
   // Static 2D Linear Function Interpolator
   //    Same as 1D plus ...
   //       y       - Independent variable #2
   //       y_data  - Table of independent variable #2 breakpoints
   //       ny      - Size of y_data table
   //       ybp     - Pointer to the previous Y breakpoint (optional)
   // ---

   static LCreal lfi(
         const LCreal x, const LCreal y,
         const LCreal *x_data, const unsigned int nx,
         const LCreal *y_data, const unsigned int ny,
         const LCreal *a_data,
         const bool eFlg,
         unsigned int* const xbp=0,
         unsigned int* const ybp=0
      );

   // ---
   // Static 3D Linear Function Interpolator
   //    Same as 2D plus ...
   //       z       - Independent variable #3
   //       z_data  - Table of independent variable #3 breakpoints
   //       nz      - Size of z_data table
   //       zbp     - Pointer to the previous Z breakpoint (optional)
   // ---

   static LCreal lfi(
         const LCreal x, const LCreal y, const LCreal z,
         const LCreal *x_data, const unsigned int nx,
         const LCreal *y_data, const unsigned int ny,
         const LCreal *z_data, const unsigned int nz,
         const LCreal *a_data,
         const bool eFlg, 
         unsigned int* const xbp=0,
         unsigned int* const ybp=0, 
         unsigned int* const zbp=0
      );

   // ---
   // Static 4D Linear Function Interpolator
   //    Same as 3D plus ...
   //       w       - Independent variable #4
   //       w_data  - Table of independent variable #4 breakpoints
   //       nw      - Size of w_data table
   //       wbp     - Pointer to the previous W breakpoint (optional)
   // ---

   static LCreal lfi(
         const LCreal x, const LCreal y, const LCreal z, const LCreal w,
         const LCreal *x_data, const unsigned int nx,
         const LCreal *y_data, const unsigned int ny,
         const LCreal *z_data, const unsigned int nz,
         const LCreal *w_data, const unsigned int nw,
         const LCreal *a_data,
         const bool eFlg, 
         unsigned int* const xbp=0,
         unsigned int* const ybp=0, 
         unsigned int* const zbp=0,
         unsigned int* const wbp=0
      );

   // ---
   // Static 5D Linear Function Interpolator
   //    Same as 4D plus ...
   //       v       - Independent variable #5
   //       v_data  - Table of independent variable #5 breakpoints
   //       nv      - Size of v_data table
   //       vbp     - Pointer to the previous V breakpoint (optional)
   // ---

   static LCreal lfi(
         const LCreal x, const LCreal y, const LCreal z, const LCreal w, const LCreal v,
         const LCreal *x_data, const unsigned int nx,
         const LCreal *y_data, const unsigned int ny,
         const LCreal *z_data, const unsigned int nz,
         const LCreal *w_data, const unsigned int nw,
         const LCreal *v_data, const unsigned int nv,
         const LCreal *a_data,
         const bool eFlg, 
         unsigned int* const xbp=0,
         unsigned int* const ybp=0, 
         unsigned int* const zbp=0,
         unsigned int* const wbp=0, 
         unsigned int* const vbp=0
      );

   // Object class functions
   virtual bool isValid() const;

public: // Exceptions

    class ExpInvalidTable : public Object::Exception {
        public:
            ExpInvalidTable() : Exception() {}
            virtual const char* getDescription() const
                            { return "table is invalid"; }
    };

    class ExpInvalidVector : public Object::Exception {
        public:
            ExpInvalidVector() : Exception() {}
            virtual const char* getDescription() const
                            { return "table vector is invalid"; }
    };

    class ExpInvalidFStorage : public Object::Exception {
        public:
            ExpInvalidFStorage() : Exception() {}
            virtual const char* getDescription() const
                            { return "Incorrect type of FStorage"; }
    };


protected:
   virtual bool loadData(const List& list, LCreal* const table) = 0;
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const = 0;
   static bool loadVector(const List& list, LCreal** table, unsigned int* n);
   static void printVector(std::ostream& sout, const LCreal* table, const unsigned int n);

   bool    valid;     // Table is valid

private:
   LCreal* dtable;    // Data Table
   unsigned int nd;   // Number of data points
   bool    extFlg;    // Extrapolation enabled flag
};


//==============================================================================
// Class: Table1 
//
// Description: 1D LFI data table
//
// Factory name: Table1
// Slots:
//    x    <List>  Independent variable #1 (iv1) points.
//==============================================================================
class Table1 : public Table {
    DECLARE_SUBCLASS(Table1, Table)

public:
   Table1();
   Table1(const LCreal* dtbl, const unsigned int dsize,
             const LCreal* xtbl, const unsigned int xsize);

   // Returns the number of x breakpoints.
   unsigned int getNumXPoints() const { return nx; }

   // Returns a pointer to the breakpoint data for x. 
   const LCreal* getXData() const     { return xtable; }

   LCreal getMinX() const;    // Min value of the X (iv1) breakpoints
   LCreal getMaxX() const;    // Max value of the X (iv1) breakpoints

   // 1D Linear Function Interpolator: returns the result of f(x) using linear interpolation.
   virtual LCreal lfi(const LCreal iv1, FStorage* const s = 0) const;

   // Load the X (iv1) breakpoints
   virtual bool setXBreakpoints1(const List* const bkpts);

   // Table class functions
   virtual unsigned int tableSize() const;

   // Object class functions
   virtual bool isValid() const;

protected:
   virtual bool loadData(const List& list, LCreal* const table);
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const;

private:
   LCreal* xtable;    // X Breakpoint Table
   unsigned int nx;   // Number of x breakpoints
};


//==============================================================================
// Class: Table2 
//
// Description: 2D LFI data table
//
// Factory name: Table2
// Slots:
//    y    <List>  Independent variable #2 (iv2) points.
//
//==============================================================================
class Table2 : public Table1 {
    DECLARE_SUBCLASS(Table2, Table1)

public:
   Table2();
   Table2(const LCreal* dtbl, const unsigned int dsize,
            const LCreal* xtbl, const unsigned int xsize,
            const LCreal* ytbl, const unsigned int ysize);

   // Returns the number of y breakpoints.
   unsigned int getNumYPoints() const { return ny; }

   // Returns a pointer to the breakpoint data for y. 
   const LCreal* getYData() const     { return ytable; }

   LCreal getMinY() const;    // Min value of the Y (iv2) breakpoints
   LCreal getMaxY() const;    // Max value of the Y (iv2) breakpoints

   // 2D Linear Function Interpolator: returns the result of f(x,y) using linear interpolation.
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, FStorage* const s = 0) const;

   // Load the Y (iv2) breakpoints
   virtual bool setYBreakpoints2(const List* const bkpts);

   // Base table class functions
   virtual LCreal lfi(const LCreal iv1, FStorage* const s = 0) const;
   virtual unsigned int tableSize() const;

   // Object class functions
   virtual bool isValid() const;

protected:
   virtual bool loadData(const List& list, LCreal* const table);
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const;

private:
   LCreal* ytable;    // Y Breakpoint Table
   unsigned int ny;   // Number of y breakpoints
};


//==============================================================================
// Class: Table3 
//
// Description: 3D LFI data table
//
// Factory name: Table3
// Slots:
//    z    <List>  Independent variable #3 (iv3) points.
//
//==============================================================================
class Table3 : public Table2 {
   DECLARE_SUBCLASS(Table3, Table2)

public:
   Table3();
   Table3(const LCreal* dtbl, const unsigned int dsize,
          const LCreal* xtbl, const unsigned int xsize,
          const LCreal* ytbl, const unsigned int ysize,
          const LCreal* ztbl, const unsigned int zsize);

   // Returns the number of z breakpoints.
   unsigned int getNumZPoints() const { return nz; }

   // LCreal* getZData()
   const LCreal* getZData() const     { return ztable; }

   LCreal getMinZ() const;    // Min value of the Z (iv3) breakpoints
   LCreal getMaxZ() const;    // Max value of the Z (iv3) breakpoints

   // 3D Linear Function Interpolator: returns the result of f(x,y,z) using linear interpolation.
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const s = 0) const;

   // Loads the Z (iv3) breakpoints
   virtual bool setZBreakpoints3(const List* const bkpts);

   // Base table class functions
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, FStorage* const s = 0) const;
   virtual unsigned int tableSize() const;

   // Object interface
   virtual bool isValid() const;

protected:
   virtual bool loadData(const List& list, LCreal* const table);
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const;

private:
   LCreal* ztable;    // Z Breakpoint Table
   unsigned int nz;   // Number of z breakpoints
};


//==============================================================================
// Class: Table4 
//
// Description: 4D LFI data table
//
// Factory name: Table4
// Slots:
//    w    <List>  Independent variable #4 (iv4) points.
//
//==============================================================================
class Table4 : public Table3 {
   DECLARE_SUBCLASS(Table4, Table3)

public:
   Table4();
   Table4(const LCreal* dtbl, const unsigned int dsize,
          const LCreal* xtbl, const unsigned int xsize,
          const LCreal* ytbl, const unsigned int ysize,
          const LCreal* ztbl, const unsigned int zsize,
          const LCreal* wtbl, const unsigned int wsize);

   // Returns the number of w breakpoints.
   unsigned int getNumWPoints() const { return nw; }

   // Returns a pointer to the breakpoint data for w. 
   const LCreal* getWData() const     { return wtable; }

   LCreal getMinW() const;    // Min value of the W (iv4) breakpoints
   LCreal getMaxW() const;    // Max value of the W (iv4) breakpoints

   // 4D Linear Function Interpolator: returns the result of f(x,y,z,w) using linear interpolation.
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, FStorage* const s = 0) const;

   // Loads the W (iv4) breakpoints
   virtual bool setWBreakpoints4(const List* const bkpts);

   // Base table class functions
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, FStorage* const s = 0) const;
   virtual unsigned int tableSize() const;

   // Object interface
   virtual bool isValid() const;

protected:
   virtual bool loadData(const List& list, LCreal* const table);
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const;

private:
   LCreal* wtable;    // W Breakpoint Table
   unsigned int nw;   // Number of w breakpoints
};

//==============================================================================
// Class: Table5 
//
// Description: 5D LFI data table
//
// Factory name: Table5
// Slots:
//    v    <List>  Independent variable #5 (iv5) points.
//
//==============================================================================
class Table5 : public Table4 {
   DECLARE_SUBCLASS(Table5, Table4)

public:
   Table5();
   Table5(const LCreal* dtbl, const unsigned int dsize,
          const LCreal* xtbl, const unsigned int xsize,
          const LCreal* ytbl, const unsigned int ysize,
          const LCreal* ztbl, const unsigned int zsize,
          const LCreal* wtbl, const unsigned int wsize,
          const LCreal* vtbl, const unsigned int vsize);

   // Returns the number of v breakpoints.
   unsigned int getNumVPoints() const { return nv; }

   // Returns a pointer to the breakpoint data for v. 
   const LCreal* getVData() const     { return vtable; }

   LCreal getMinV() const;    // Min value of the V (iv5) breakpoints
   LCreal getMaxV() const;    // Max value of the V (iv5) breakpoints

   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, const LCreal iv5, FStorage* const s = 0) const;

   // Loads the V (iv5) breakpoints
   virtual bool setVBreakpoints5(const List* const bkpts);

   // Base table class functions
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, const LCreal iv4, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, const LCreal iv3, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, const LCreal iv2, FStorage* const s = 0) const;
   virtual LCreal lfi(const LCreal iv1, FStorage* const s = 0) const;
   virtual unsigned int tableSize() const;

   // Object interface
   virtual bool isValid() const;

protected:
   virtual bool loadData(const List& list, LCreal* const table);
   virtual void printData(std::ostream& sout, const LCreal* table, const unsigned int indent) const;

private:
   LCreal* vtable;     // V Breakpoint Table
   unsigned int nv;    // Number of v breakpoints
};

} // End Basic namespace
} // End Eaagles namespace

#endif
