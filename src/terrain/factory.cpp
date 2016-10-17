
#include "openeaagles/terrain/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/terrain/QuadMap.hpp"
#include "openeaagles/terrain/ded/DedFile.hpp"
#include "openeaagles/terrain/dted/DtedFile.hpp"
#include "openeaagles/terrain/srtm/SrtmHgtFile.hpp"

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
