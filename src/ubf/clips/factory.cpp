
#include "openeaagles/ubf/clips/factory.h"

#include "ClipsArbiter.h"

#include "openeaagles/base/Object.h"

namespace oe {
namespace clips {

base::Object* factory(const std::string& name)
{
   base::Object* obj = nullptr;

   if (name == ClipsArbiter::getFactoryName()) {
      obj = new ClipsArbiter();
   }

   return obj;
}

}
}

