//------------------------------------------------------------------------------
// FirstOrderTf class -- General first order transfer function
//------------------------------------------------------------------------------
#ifndef __oe_linearsystem_FirstOrderTf_H__
#define __oe_linearsystem_FirstOrderTf_H__

#include "openeaagles/linearsystem/DiffEquation.h"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  FirstOrderTf
// Base class:  base::Object -> ScalerFunc-> DiffEquation -> FirstOrderTf
//
// Description: Models a first order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
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
   FirstOrderTf(const unsigned int rate, const double n1, const double n2, const double d1, const double d2);

   // Get Parameters
   double getN1() const          { return n1; }
   double getN2() const          { return n2; }
   double getD1() const          { return d1; }
   double getD2() const          { return d2; }

   // Set Parameters
   virtual bool setN1(const double v);
   virtual bool setN2(const double v);
   virtual bool setD1(const double v);
   virtual bool setD2(const double v);

   virtual bool isValid() const override;

protected:
   virtual void initialize() override;

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 2;

   // Parameters and initial values
   double n1;
   double n2;
   double d1;
   double d2;
};

}
}

#endif
