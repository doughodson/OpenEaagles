
#ifndef __oe_simulation_IAtmosphere_H__
#define __oe_simulation_IAtmosphere_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace simulation {

class IAtmosphere : public base::Component
{
   DECLARE_SUBCLASS(IAtmosphere, base::Component)

public:
   IAtmosphere();

};

}
}

#endif
