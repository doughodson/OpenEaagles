//------------------------------------------------------------------------------
// Form function for the guiGlut object library
//------------------------------------------------------------------------------

#include "openeaagles/gui/glfw/glfwFF.h"
#include "openeaagles/gui/glfw/GlfwDisplay.h"

namespace Eaagles {
namespace Glfw {

Basic::Object* glfwFormFunc(const char* formname)
{
    // Start out by checking the basic classes (forms)
    Basic::Object* newform = 0;

    // General graphics support
    if ( strcmp(formname, GlfwDisplay::getFormName()) == 0 ) {
      newform = new GlfwDisplay();
    }
   return newform;
}

} // End Glfw namespace
} // End Eaagles namespace

