
#ifndef __oe_models_AbstractAtmosphere_H__
#define __oe_models_AbstractAtmosphere_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace models {

class AbstractAtmosphere : public base::Component
{
   DECLARE_SUBCLASS(AbstractAtmosphere, base::Component)

public:
   AbstractAtmosphere();
};

}
}

#endif
