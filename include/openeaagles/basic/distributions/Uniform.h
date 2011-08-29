
#ifndef UniformH
#define UniformH

#include "../Rng.h"

namespace Eaagles {
namespace Basic {

class Number;

class Uniform: public Rng
{
  DECLARE_SUBCLASS(Uniform, Rng)

public:

  Uniform();

  double draw();

  bool setMin(const double x)                           { min = x; return true; }
  double getMin() const                                 { return min; }
  bool setSlotMin(const Number* const);

  bool setMax(const double x)                           { max = x; return true; }
  double getMax() const                                 { return max; }
  bool setSlotMax(const Number* const);

protected:

  double min;
  double max;


};

} // End Basic namespace
} // End Eaagles namespace

#endif
