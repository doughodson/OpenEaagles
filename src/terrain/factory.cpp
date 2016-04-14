
#include "openeaagles/terrain/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/terrain/QuadMap.h"
#include "openeaagles/terrain/ded/DedFile.h"
#include "openeaagles/terrain/dted/DtedFile.h"
#include "openeaagles/terrain/srtm/SrtmHgtFile.h"

#include <string>

namespace oe {
namespace terrain {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == QuadMap::getFactoryName() ) {
        obj = new QuadMap();
    }
    else if ( name == DedFile::getFactoryName() ) {
        obj = new DedFile();
    }
    else if ( name == DtedFile::getFactoryName() ) {
        obj = new DtedFile();
    }
    else if ( name == SrtmHgtFile::getFactoryName() ) {
        obj = new SrtmHgtFile();
    }

    return obj;
}

}
}
