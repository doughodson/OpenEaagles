//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/terrain/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/terrain/QuadMap.h"
#include "openeaagles/terrain/ded/DedFile.h"
#include "openeaagles/terrain/dted/DtedFile.h"
#include "openeaagles/terrain/srtm/SrtmHgtFile.h"

namespace Eaagles {
namespace Terrain {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    if ( strcmp(name, QuadMap::getFactoryName()) == 0 ) {
        obj = new QuadMap();
    }
    else if ( strcmp(name, DedFile::getFactoryName()) == 0 ) {
        obj = new DedFile();
    }
    else if ( strcmp(name, DtedFile::getFactoryName()) == 0 ) {
        obj = new DtedFile();
    }
    else if ( strcmp(name, SrtmHgtFile::getFactoryName()) == 0 ) {
        obj = new SrtmHgtFile();
    }

    return obj;
}

}  // end namespace Terrain
}  // end namespace Eaagles
