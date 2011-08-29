//------------------------------------------------------------------------------
// Form function for the 'terrain' library
//------------------------------------------------------------------------------

#include "openeaagles/terrain/terrainFF.h"

#include "openeaagles/terrain/QuadMap.h"
#include "openeaagles/terrain/ded/DedFile.h"
#include "openeaagles/terrain/dted/DtedFile.h"
#include "openeaagles/terrain/srtm/SrtmHgtFile.h"

namespace Eaagles {
namespace Terrain {

Basic::Object* terrainFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    if ( strcmp(formname, QuadMap::getFormName()) == 0 ) {
        newform = new QuadMap();
    }
    else if ( strcmp(formname, DedFile::getFormName()) == 0 ) {
        newform = new DedFile();
    }
    else if ( strcmp(formname, DtedFile::getFormName()) == 0 ) {
        newform = new DtedFile();
    }
    else if ( strcmp(formname, SrtmHgtFile::getFormName()) == 0 ) {
        newform = new SrtmHgtFile();
    }

    return newform;
}

} // End Terrain namespace
} // End Eaagles namespace
