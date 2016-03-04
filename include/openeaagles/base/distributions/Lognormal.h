
#ifndef __oe_base_Lognormal_H__
#define __oe_base_Lognormal_H__

#include "../Rng.h"

//
// Lognormal - this class returns a random variate from the lognormal distribution.
// Input parameters:
//     sigma : continuous parameter
//     mu    : continuous parameter
//     gamma : continuous location parameter (gamma = 0 for 2 parameter lognormal)
//

namespace oe {
namespace basic {

class Number;

class Lognormal: public Rng
{
  DECLARE_SUBCLASS(Lognormal, Rng)

public:

  Lognormal();

  double draw() override;

  bool setSigma(const double x)                       { sigma = x; return true; }
  double getSigma() const                             { return sigma; }
  bool setSlotSigma(const Number* const);

  bool setMu(const double x)                          { mu = x; return true; }
  double getMu() const                                { return mu; }
  bool setSlotMu(const Number* const);

  bool setGamma(const double x)                       { gamma = x; return true; }
  double getGamma() const                             { return gamma; }
  bool setSlotGamma(const Number* const);

protected:

  double sigma;
  double mu;
  double gamma;

};

} // End basic namespace
} // End oe namespace

#endif

