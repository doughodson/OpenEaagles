//------------------------------------------------------------------------------
// Form function for the 'terrain' library
//------------------------------------------------------------------------------
#include "openeaagles/terrain/terrainFF.h"
#include "openeaagles/terrain/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Terrain {

Basic::Object* terrainFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Terrain::terrainFormFunc() is deprecated - use Terrain::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Terrain namespace
} // End oe namespace
