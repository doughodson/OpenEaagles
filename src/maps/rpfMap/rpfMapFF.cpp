#include "openeaagles/maps/rpfMap/rpfMapFF.h"

// Include our moving map stuff
#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

Basic::Object* rpfMapFormFunc(const char* formname)
{    
   // Form generator
   Basic::Object* newform = 0;

   // Map Drawer
   if ( strcmp(formname, MapDrawer::getFormName()) == 0 ) {
      newform = new MapDrawer();
   }
   // CadrgMap
   else if ( strcmp(formname, CadrgMap::getFormName()) == 0 ) {
      newform = new CadrgMap();
   }
    
   return newform;
}

};  // End Rpf namespace
};  // End Maps namespace
};  // End Eaagles namespace
