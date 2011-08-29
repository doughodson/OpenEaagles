//------------------------------------------------------------------------------
// LimitFunc class -- General Limit function
//------------------------------------------------------------------------------
#ifndef __LinearSystem_LimitFunc_H_CCFC289D_E6DF_4512_BBCD_7AFADB2D744A__
#define __LinearSystem_LimitFunc_H_CCFC289D_E6DF_4512_BBCD_7AFADB2D744A__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: LimitFunc
// Base class:  Basic::Object -> ScalerFunc -> LimitFunc
//
// Description: Genaric limits function: limits X to the upper and lower boundries.
//
// GUID: {CCFC289D-E6DF-4512-BBCD-7AFADB2D744A}
//------------------------------------------------------------------------------
class LimitFunc : public ScalerFunc {
    DECLARE_SUBCLASS(LimitFunc,ScalerFunc)

public:
   LimitFunc();
   LimitFunc(const LCreal lower, const LCreal upper);

   LCreal getLowerLimit() const  { return lower; }
   LCreal getUpperLimit() const  { return upper; }

   virtual bool setLowerLimit(const LCreal v);
   virtual bool setUpperLimit(const LCreal v);

   // ScalerFunc class interface
   virtual LCreal g(const LCreal x);
   virtual bool isValid() const;

protected:
   // ScalerFunc class protected interface
   virtual void initialize();

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 1;

   LCreal       lower;      // Lower limit
   LCreal       upper;      // Upper limit
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif      /* __LinearSystem_LimitFunc_H_CCFC289D_E6DF_4512_BBCD_7AFADB2D744A__ */
