
#include "openeaagles/ubf/soar/factory.h"
#include "openeaagles/ubf/soar/SoarArbiter.h"

#include "openeaagles/base/Object.h"

namespace oe {
namespace soar {

base::Object* factory(const std::string& name)
{
   base::Object* obj = nullptr;

   if (name == SoarArbiter::getFactoryName()) {
      obj = new SoarArbiter();
   }

   return obj;
}

}
}

