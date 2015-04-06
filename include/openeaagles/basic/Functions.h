//------------------------------------------------------------------------------
// Classes ---
//    Base function:                      Function
//    Polynomial function:                Polynomial
//    Generic multi-variable functions:   Func1, Func2, Func3, Func4 and Func5
//    Function storage:                   FStorage
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Functions_H__
#define __Eaagles_Basic_Functions_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

class FStorage;
class List;
class Table;

//==============================================================================
// Class: Function
//
// Description: Base for the derived, multi-dimensional function classes.
//
// Notes:
//    1) Use the storageFactory() function to create the FStorage object that
//       will maintain the previous function call values (i.e., integration).
//
//   virtual FStorage* storageFactory() const;
//       Data storage factory (pre-ref()'d)
//   virtual bool setSlotLfiTable(const Table* const msg);
//       Set Slot Tables
//
// Slots:
//    table    <Table>     ! Optional LFI table
//
//==============================================================================
class Function : public Object {
    DECLARE_SUBCLASS(Function, Object)

public:
   Function();

   virtual FStorage* storageFactory() const;    // Data storage factory (pre-ref()'d)
   virtual bool setSlotLfiTable(const Table* const msg);

protected:
   const Table* getTable() const {  return table; }

private:
   SPtr<const Table> table;   // Optional LFI Table
};

//==============================================================================
// Class: Func1
// Description: Generic 1-Dimensional function; f(iv1)
// Factory name: Func1
//==============================================================================
class Func1 : public Function {
    DECLARE_SUBCLASS(Func1, Function)
public:
   Func1();

   virtual double f(const double iv1, FStorage* const s = 0) const;

   bool setSlotLfiTable(const Table* const msg) override;
};

//==============================================================================
// Class: Func2
// Description: Generic 2-Dimensional function; f(iv1, iv2)
// Factory name: Func2
//==============================================================================
class Func2 : public Function {
    DECLARE_SUBCLASS(Func2, Function)
public:
   Func2();

   virtual double f(const double iv1, const double iv2, FStorage* const s = 0) const;

   bool setSlotLfiTable(const Table* const msg) override;
};

//==============================================================================
// Class: Func3
// Description: Generic 3-Dimensional function; f(iv1, iv2, iv3)
// Factory name: Func3
//==============================================================================
class Func3 : public Function {
   DECLARE_SUBCLASS(Func3, Function)
public:
   Func3();

   virtual double f(const double iv1, const double iv2, const double iv3, FStorage* const s = 0) const;

   bool setSlotLfiTable(const Table* const msg) override;
};

//==============================================================================
// Class: Func4
// Description: Generic 4-Dimensional function; f(iv1, iv2, iv3, iv4)
// Factory name: Func4
//==============================================================================
class Func4 : public Function {
   DECLARE_SUBCLASS(Func4, Function)
public:
   Func4();

   virtual double f(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = 0) const;

   bool setSlotLfiTable(const Table* const msg) override;
};

//==============================================================================
// Class: Func5
// Description: Generic 5-Dimensional function; f(iv1, iv2, iv3, iv4, iv5)
// Factory names: Func5
//==============================================================================
class Func5 : public Function {
   DECLARE_SUBCLASS(Func5, Function)
public:
   Func5();

   virtual double f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s = 0) const;

   bool setSlotLfiTable(const Table* const msg) override;
};

//==============================================================================
// Class: Polynomial
// Description: Polynomial function
//                f(x) = a0 + a1*x + a2*x^2 + ... + aN*x^N
//
// Factory names: Polynomial
// Slots:
//    coefficients   <Basic::List>  ! Constant coefficients vector: [ a0 a1 a2 ... aN ]
//
// Notes
//    1) The degree of the polynomial is determined by the size of the coefficients vector.
//
//          coefficients     degree            result
//          ------------    --------     --------------------------------
//             none            < 0        0
//              1               0         a0
//              2               1         a0 + a1 * x
//              3               2         a0 + a1 * x + a2 * x^2
//              M             N = M-1     a0 + a1 * x + a2 * x^2 + ... + aN * x^N
//
//    2) Storage is not used.
//
//==============================================================================
class Polynomial : public Func1 {
    DECLARE_SUBCLASS(Polynomial, Func1)

public:
   // Highest allowed degree of polynomial
   static const unsigned int MAX_DEGREE = 32;

public:
   Polynomial();

   int getDegree() const                  { return (m-1); }
   const double* getCoefficients() const  { return a; }

   double f(const double x, FStorage* const s = 0) const override;

protected:
   virtual bool setSlotCoefficients(const List* const msg);
   bool setCoefficients(const double* const coeff, const unsigned short n);

private:
   void initData();
   static const unsigned short MAX_COEFF = (MAX_DEGREE+1);

   double  a[MAX_COEFF];   // Constant coefficients vector
   unsigned short m;       // Number of coefficients (degree + 1)
};

//==============================================================================
// class FStorage
// Description: Generic storage class for past values of function calls.
//              Use Function::storageFactory() to generate pre-ref()'d
//              FStorage objects.
//==============================================================================
class FStorage : public Object {
   DECLARE_SUBCLASS(FStorage, Object)
public:
   FStorage();
};

} // End Basic namespace
} // End Eaagles namespace

#endif
