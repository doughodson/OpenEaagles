//------------------------------------------------------------------------------
// Form function for the guiGlut object library
//------------------------------------------------------------------------------

#include "openeaagles/gui/glut/glutFF.h"
#include "openeaagles/gui/glut/GlutDisplay.h"
#include "openeaagles/gui/glut/Shapes3D.h"

namespace Eaagles {
namespace Glut {

Basic::Object* glutFormFunc(const char* formname)
{
    // Start out by checking the basic classes (forms)
    Basic::Object* newform = 0;

    // General graphics support
    if ( strcmp(formname, GlutDisplay::getFormName()) == 0 ) {
      newform = new GlutDisplay();
    }
    // glut shapes support
    else if ( strcmp(formname, Sphere::getFormName()) == 0 ) {
      newform = new Sphere();
    }
    else if ( strcmp(formname, Cylinder::getFormName()) == 0 ) {
      newform = new Cylinder();
    }
    else if ( strcmp(formname, Cone::getFormName()) == 0 ) {
      newform = new Cone();
    }
    else if ( strcmp(formname, Cube::getFormName()) == 0 ) {
      newform = new Cube();
    }
    else if ( strcmp(formname, Torus::getFormName()) == 0 ) {
      newform = new Torus();
    }
    else if ( strcmp(formname, Dodecahedron::getFormName()) == 0 ) {
      newform = new Dodecahedron();
    }
    else if ( strcmp(formname, Tetrahedron::getFormName()) == 0 ) {
      newform = new Tetrahedron();
    }
    else if ( strcmp(formname, Icosahedron::getFormName()) == 0 ) {
      newform = new Icosahedron();
    }
    else if ( strcmp(formname, Octahedron::getFormName()) == 0 ) {
      newform = new Octahedron();
    }
    else if ( strcmp(formname, Teapot::getFormName()) == 0 ) {
      newform = new Teapot();
    }

   return newform;
}

} // End Glut namespace
} // End Eaagles namespace

