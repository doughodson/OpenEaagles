
#include <iostream>

#include "openeaagles/base/distributions/Lognormal.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Lognormal,"Lognormal")

// slot table for this class type
BEGIN_SLOTTABLE(Lognormal)
  "sigma",              // 1
  "mu",                 // 2
  "gamma",              // 3
END_SLOTTABLE(Lognormal)
//  map slot table to handles
BEGIN_SLOT_MAP(Lognormal)
  ON_SLOT(1, setSlotSigma, Number)
  ON_SLOT(2, setSlotMu, Number)
  ON_SLOT(3, setSlotGamma, Number)
END_SLOT_MAP()

EMPTY_DELETEDATA(Lognormal)

Lognormal::Lognormal()
{
  STANDARD_CONSTRUCTOR()
}

void Lognormal::copyData(const Lognormal& org, const bool)
{
  BaseClass::copyData(org);
  sigma = org.sigma;
  mu = org.mu;
  gamma = org.gamma;
}

double Lognormal::draw()
{
  double u(0.0), r2(0.0);
  do {
      // choose x,y in uniform square (-1,-1) to (+1,+1)
      u = -1 + 2 * drawHalfOpen();
      const double v = -1 + 2 * drawHalfOpen();
      // see if it is in the unit circle
      r2 = u * u + v * v;
  } while (r2 > 1.0 || r2 == 0);
  const double normal = u * std::sqrt(-2.0 * std::log(r2) / r2);
  double z =  std::exp(sigma * normal + mu);
  // shift for location
  z += gamma;
  return z;
}

bool Lognormal::setSlotSigma(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setSigma(x->getDouble());
  return ok;
}

bool Lognormal::setSlotMu(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setMu(x->getDouble());
  return ok;
}

bool Lognormal::setSlotGamma(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setGamma(x->getDouble());
  return ok;
}

std::ostream& Lognormal::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
  int j = 0;
  if ( !slotsOnly ) {
//    indent(sout,i);
    sout << "( " << getFactoryName() << std::endl;
    j = 4;
  }

  // serialize base class
  BaseClass::serialize(sout,i+j,true);

  indent(sout,i+j);
  sout << "sigma: " << sigma << std::endl;
  indent(sout,i+j);
  sout << "mu: " << mu << std::endl;
  indent(sout,i+j);
  sout << "gamma: " << gamma << std::endl;

  if ( !slotsOnly ) {
    indent(sout,i);
    sout << ")" << std::endl;
  }

  return sout;
}

}
}
