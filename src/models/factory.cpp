
#include "openeaagles/models/factory.h"

#include "openeaagles/base/Object.h"

// dynamics models
#include "openeaagles/models/dynamics/JSBSimModel.h"
#include "openeaagles/models/dynamics/RacModel.h"
#include "openeaagles/models/dynamics/LaeroModel.h"

// sensor models
#include "openeaagles/models/sensors/Gmti.h"
#include "openeaagles/models/sensors/Tws.h"
#include "openeaagles/models/sensors/Stt.h"

#include <cstring>

namespace oe {
namespace models {

base::Object* factory(const char* name)
{
   base::Object* obj = nullptr;

   // dynamics models
   if (std::strcmp(name, RacModel::getFactoryName()) == 0) {              // RAC
      obj = new RacModel();
   }
   else if (std::strcmp(name, JSBSimModel::getFactoryName()) == 0) {      // JSBSim
      obj = new JSBSimModel();
   }
   else if (std::strcmp(name, LaeroModel::getFactoryName()) == 0) {       // Laero
      obj = new LaeroModel();
   }

   // sensor models
   if (std::strcmp(name, Gmti::getFactoryName()) == 0) {
      obj = new Gmti();
   }
   else if (std::strcmp(name, Stt::getFactoryName()) == 0) {
      obj = new Stt();
   }
   else if (std::strcmp(name, Tws::getFactoryName()) == 0) {
      obj = new Tws();
   }

   return obj;
}

}
}
