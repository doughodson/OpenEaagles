//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/gui/glut/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/gui/glut/GlutDisplay.h"
#include "openeaagles/gui/glut/Shapes3D.h"

#include <cstring>

namespace Eaagles {
namespace Glut {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // General graphics support
    if ( std::strcmp(name, GlutDisplay::getFactoryName()) == 0 ) {
      obj = new GlutDisplay();
    }
    // glut shapes support
    else if ( std::strcmp(name, Sphere::getFactoryName()) == 0 ) {
      obj = new Sphere();
    }
    else if ( std::strcmp(name, Cylinder::getFactoryName()) == 0 ) {
      obj = new Cylinder();
    }
    else if ( std::strcmp(name, Cone::getFactoryName()) == 0 ) {
      obj = new Cone();
    }
    else if ( std::strcmp(name, Cube::getFactoryName()) == 0 ) {
      obj = new Cube();
    }
    else if ( std::strcmp(name, Torus::getFactoryName()) == 0 ) {
      obj = new Torus();
    }
    else if ( std::strcmp(name, Dodecahedron::getFactoryName()) == 0 ) {
      obj = new Dodecahedron();
    }
    else if ( std::strcmp(name, Tetrahedron::getFactoryName()) == 0 ) {
      obj = new Tetrahedron();
    }
    else if ( std::strcmp(name, Icosahedron::getFactoryName()) == 0 ) {
      obj = new Icosahedron();
    }
    else if ( std::strcmp(name, Octahedron::getFactoryName()) == 0 ) {
      obj = new Octahedron();
    }
    else if ( std::strcmp(name, Teapot::getFactoryName()) == 0 ) {
      obj = new Teapot();
    }

    return obj;
}

}  // end namespace Glut
}  // end namespace Eaagles

