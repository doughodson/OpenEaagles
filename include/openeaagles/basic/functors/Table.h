
#ifndef __Eaagles_Basic_Table_H__
#define __Eaagles_Basic_Table_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/functors/Functions.h"

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

   bool isValid() const override;

public:
    // Exceptions
    class ExpInvalidTable : public Object::Exception {
        public:
            ExpInvalidTable() : Exception() {}
            const char* getDescription() const override          { return "table is invalid"; }
    };

    class ExpInvalidVector : public Object::Exception {
        public:
            ExpInvalidVector() : Exception() {}
            const char* getDescription() const override          { return "table vector is invalid"; }
    };

    class ExpInvalidFStorage : public Object::Exception {
        public:
            ExpInvalidFStorage() : Exception() {}
            const char* getDescription() const override          { return "Incorrect type of FStorage"; }
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
// Class TableStorage
//==============================================================================
class TableStorage : public FStorage {
   DECLARE_SUBCLASS(TableStorage, FStorage)
public:
   TableStorage() { xbp = 0; ybp = 0; zbp = 0; wbp = 0; vbp = 0; }
   unsigned int xbp, ybp, zbp, wbp, vbp;
};


} // End Basic namespace
} // End Eaagles namespace

#endif
