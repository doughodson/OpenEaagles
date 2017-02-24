
#ifndef __oe_base_Table_H__
#define __oe_base_Table_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/functors/Functions.hpp"

namespace oe {
namespace base {
class List;
class Number;

//------------------------------------------------------------------------------
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
//    1) The isValid() function will return true only if all of the required
//       dependent and independent data has been set.
//
//    2) For large tables, use the storageFactory() function to create the FStorage
//       object (see Functions.h) that will maintain the table's previous search
//       values.  This object is an optional parameter to the non-static lfi()
//       functions.
//
//    3) If a dependent variable exceeds a breakpoint table data then the lfi()
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
//------------------------------------------------------------------------------
class Table : public Object
{
    DECLARE_SUBCLASS(Table, Object)

public:
   Table();
   Table(const double* dtbl, const unsigned int dsize);

   // Returns a pointer to the dependent variable data table.
   const double* getDataTable() const                            { return dtable; }

   // Returns the number of entries in the data table
   virtual unsigned int tableSize() const = 0;
   virtual bool setDataTable(const List* const msg);

   // Returns the min and max values of the dependent variable data table
   virtual void findMinMax(double* minValue, double* maxValue) const;

   // Returns true if extrapolation beyond the table's data is enabled.
   bool isExtrapolationEnabled() const                           { return extFlg; }

   // Sets the extrapolation enabled flag.
   bool setExtrapolationEnabled(const bool flg);
   virtual bool setExtrapolationEnabled(const Number* const msg);

   // Data storage factory (pre-ref()'d)
   virtual FStorage* storageFactory() const;

   virtual bool isValid() const override;

public:
    // Exceptions
    class ExpInvalidTable : public Object::Exception {
        public:
            ExpInvalidTable() : Exception() {}
            virtual const char* getDescription() const override          { return "table is invalid"; }
    };

    class ExpInvalidVector : public Object::Exception {
        public:
            ExpInvalidVector() : Exception() {}
            virtual const char* getDescription() const override          { return "table vector is invalid"; }
    };

    class ExpInvalidFStorage : public Object::Exception {
        public:
            ExpInvalidFStorage() : Exception() {}
            virtual const char* getDescription() const override          { return "Incorrect type of FStorage"; }
    };


protected:
   virtual bool loadData(const List& list, double* const table) = 0;
   virtual void printData(std::ostream& sout, const double* table, const unsigned int indent) const = 0;
   static bool loadVector(const List& list, double** table, unsigned int* n);
   static void printVector(std::ostream& sout, const double* table, const unsigned int n);

   bool valid {};        // Table is valid

private:
   double* dtable {};    // Data Table
   unsigned int nd {};   // Number of data points
   bool extFlg {};       // Extrapolation enabled flag
};

//------------------------------------------------------------------------------
// Class TableStorage
//------------------------------------------------------------------------------
class TableStorage : public FStorage {
   DECLARE_SUBCLASS(TableStorage, FStorage)
public:
   TableStorage();
   unsigned int xbp {}, ybp {}, zbp {}, wbp {}, vbp {};
};

}
}

#endif
