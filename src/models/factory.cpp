
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

#include <string>

namespace oe {
namespace models {

base::Object* factory(const std::string& name)
{
   base::Object* obj = nullptr;

   // dynamics models
   if ( name == RacModel::getFactoryName() ) {              // RAC
      obj = new RacModel();
   }
   else if ( name == JSBSimModel::getFactoryName() ) {      // JSBSim
      obj = new JSBSimModel();
   }
   else if ( name == LaeroModel::getFactoryName() ) {       // Laero
      obj = new LaeroModel();
   }

   // sensor models
   if ( name == Gmti::getFactoryName() ) {
      obj = new Gmti();
   }
   else if ( name == Stt::getFactoryName() ) {
      obj = new Stt();
   }
   else if ( name == Tws::getFactoryName() ) {
      obj = new Tws();
   }

   return obj;
}

}
}
