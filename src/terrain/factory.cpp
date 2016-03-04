
#include "openeaagles/terrain/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/terrain/QuadMap.h"
#include "openeaagles/terrain/ded/DedFile.h"
#include "openeaagles/terrain/dted/DtedFile.h"
#include "openeaagles/terrain/srtm/SrtmHgtFile.h"

#include <cstring>

namespace oe {
namespace terrain {

basic::Object* factory(const char* name)
{
    basic::Object* obj = nullptr;

    if ( std::strcmp(name, QuadMap::getFactoryName()) == 0 ) {
        obj = new QuadMap();
    }
    else if ( std::strcmp(name, DedFile::getFactoryName()) == 0 ) {
        obj = new DedFile();
    }
    else if ( std::strcmp(name, DtedFile::getFactoryName()) == 0 ) {
        obj = new DtedFile();
    }
    else if ( std::strcmp(name, SrtmHgtFile::getFactoryName()) == 0 ) {
        obj = new SrtmHgtFile();
    }

    return obj;
}

}
}
