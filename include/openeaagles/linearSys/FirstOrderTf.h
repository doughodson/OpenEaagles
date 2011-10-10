//------------------------------------------------------------------------------
// FirstOrderTf class -- General first order transfer function
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_FirstOrderTf_H__
#define __Eaagles_LinearSystem_FirstOrderTf_H__

#include "openeaagles/linearSys/DiffEquation.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  FirstOrderTf
// Base class:  Basic::Object -> ScalerFunc-> DiffEquation -> FirstOrderTf
//
// Description: Models a first order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coeffients.
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
//------------------------------------------------------------------------------
class FirstOrderTf : public DiffEquation {
    DECLARE_SUBCLASS(FirstOrderTf,DiffEquation)

public:
   FirstOrderTf();
   FirstOrderTf(const unsigned int rate, const LCreal n1, const LCreal n2, const LCreal d1, const LCreal d2);

   // Get Parameters
   LCreal getN1() const          { return n1; }
   LCreal getN2() const          { return n2; }
   LCreal getD1() const          { return d1; }
   LCreal getD2() const          { return d2; }

   // Set Parameters
   virtual bool setN1(const LCreal v);
   virtual bool setN2(const LCreal v);
   virtual bool setD1(const LCreal v);
   virtual bool setD2(const LCreal v);

   // ScalerFunc class interface
   virtual bool isValid() const;

protected:
   // ScalerFunc class protected interface
   virtual void initialize();

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 2;

   // Parameters and initial values
   LCreal n1;
   LCreal n2;
   LCreal d1;
   LCreal d2;
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
