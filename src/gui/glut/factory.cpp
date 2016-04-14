
#include "openeaagles/gui/glut/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/gui/glut/GlutDisplay.h"
#include "openeaagles/gui/glut/Shapes3D.h"

#include <string>

namespace oe {
namespace glut {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // General graphics support
    if ( name == GlutDisplay::getFactoryName() ) {
      obj = new GlutDisplay();
    }
    // glut shapes support
    else if ( name == Sphere::getFactoryName() ) {
      obj = new Sphere();
    }
    else if ( name == Cylinder::getFactoryName() ) {
      obj = new Cylinder();
    }
    else if ( name == Cone::getFactoryName() ) {
      obj = new Cone();
    }
    else if ( name == Cube::getFactoryName() ) {
      obj = new Cube();
    }
    else if ( name == Torus::getFactoryName() ) {
      obj = new Torus();
    }
    else if ( name == Dodecahedron::getFactoryName() ) {
      obj = new Dodecahedron();
    }
    else if ( name == Tetrahedron::getFactoryName() ) {
      obj = new Tetrahedron();
    }
    else if ( name == Icosahedron::getFactoryName() ) {
      obj = new Icosahedron();
    }
    else if ( name == Octahedron::getFactoryName() ) {
      obj = new Octahedron();
    }
    else if ( name == Teapot::getFactoryName() ) {
      obj = new Teapot();
    }

    return obj;
}

}
}

