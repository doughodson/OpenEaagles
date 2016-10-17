
#include "openeaagles/models/factory.hpp"

#include "openeaagles/base/Object.hpp"

// dynamics models
#include "openeaagles/models/dynamics/JSBSimModel.hpp"
#include "openeaagles/models/dynamics/RacModel.hpp"
#include "openeaagles/models/dynamics/LaeroModel.hpp"

// sensor models
#include "openeaagles/models/sensors/Gmti.hpp"
#include "openeaagles/models/sensors/Tws.hpp"
#include "openeaagles/models/sensors/Stt.hpp"

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
