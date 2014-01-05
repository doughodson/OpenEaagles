//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/gui/glut/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/gui/glut/GlutDisplay.h"
#include "openeaagles/gui/glut/Shapes3D.h"

namespace Eaagles {
namespace Glut {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // General graphics support
    if ( strcmp(name, GlutDisplay::getFactoryName()) == 0 ) {
      obj = new GlutDisplay();
    }
    // glut shapes support
    else if ( strcmp(name, Sphere::getFactoryName()) == 0 ) {
      obj = new Sphere();
    }
    else if ( strcmp(name, Cylinder::getFactoryName()) == 0 ) {
      obj = new Cylinder();
    }
    else if ( strcmp(name, Cone::getFactoryName()) == 0 ) {
      obj = new Cone();
    }
    else if ( strcmp(name, Cube::getFactoryName()) == 0 ) {
      obj = new Cube();
    }
    else if ( strcmp(name, Torus::getFactoryName()) == 0 ) {
      obj = new Torus();
    }
    else if ( strcmp(name, Dodecahedron::getFactoryName()) == 0 ) {
      obj = new Dodecahedron();
    }
    else if ( strcmp(name, Tetrahedron::getFactoryName()) == 0 ) {
      obj = new Tetrahedron();
    }
    else if ( strcmp(name, Icosahedron::getFactoryName()) == 0 ) {
      obj = new Icosahedron();
    }
    else if ( strcmp(name, Octahedron::getFactoryName()) == 0 ) {
      obj = new Octahedron();
    }
    else if ( strcmp(name, Teapot::getFactoryName()) == 0 ) {
      obj = new Teapot();
    }

    return obj;
}

}  // end namespace Glut
}  // end namespace Eaagles

