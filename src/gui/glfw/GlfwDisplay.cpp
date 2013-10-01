#ifdef OE_SHADERS
   // shader class includes GLEW
   #include "openeaagles/basicGL2.0/core/Shader.h"
#else 
   #include "GLEW/glew.h"
#endif
#include "openeaagles/gui/glfw/GlfwDisplay.h"
#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

#include <GLFW/glfw3.h>

namespace Eaagles {
namespace Glfw {

IMPLEMENT_SUBCLASS(GlfwDisplay,"GlfwDisplay")
EMPTY_SERIALIZER(GlfwDisplay)

GLFWwindow* GlfwDisplay::wins[GlfwDisplay::MAX_DISPLAYS];               // List of window structures
GlfwDisplay* GlfwDisplay::displayList[GlfwDisplay::MAX_DISPLAYS];       // Display List
int GlfwDisplay::numGlfwDisplays = 0;                                   // Number of  registered GlfwDisplays

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(GlfwDisplay)
   "glMajorVersion",       // 1) OpenGL major version we want to use (not guaranteed)
   "glMinorVersion",       // 2) OpenGL minor version we want to use (not guaranteed)
   "glCompatibility",       // 3) Do we load the openGL compatibility extensions (for backwards compatibility)
END_SLOTTABLE(GlfwDisplay)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(GlfwDisplay)
   ON_SLOT(1,setSlotMajorVersion,Basic::Number)
   ON_SLOT(2,setSlotMinorVersion, Basic::Number)
   ON_SLOT(3,setSlotCompatibility, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
GlfwDisplay::GlfwDisplay()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// InitData
//------------------------------------------------------------------------------
void GlfwDisplay::initData()
{
	myWin = 0;
   // default to no version hints (which means we will get the latest OpenGL WITH the
   // compatibility loaded)
   glMajorVersion = 0;
   glMinorVersion = 0;
   compatibility = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy our objects data
//------------------------------------------------------------------------------
void GlfwDisplay::copyData(const GlfwDisplay& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   myWin = org.myWin;
   glMajorVersion = org.glMajorVersion;
   glMinorVersion = org.glMinorVersion;
   compatibility = org.compatibility;
}

//------------------------------------------------------------------------------
// deleteData() -- copy our objects data
//------------------------------------------------------------------------------
void GlfwDisplay::deleteData()
{
	myWin = 0;
}

// ---
// isWindowShutdown() - is our window closing (or closed)
// ---
bool GlfwDisplay::isWindowShutdown() const
{
	if (myWin != 0) return (glfwWindowShouldClose(myWin) != 0);
	else return true;
}

//-----------------------------------------------------------------------------
// Reset the GlfwDisplay
//-----------------------------------------------------------------------------
void GlfwDisplay::reset()
{
   BaseClass::reset();
}

// ---
// cursorMove() - called when the mouse move callback is instantiated on this object
// ---
void GlfwDisplay::cursorMove(const double x, const double y)
{
   // get the last reported mouse button from the window, to see if it is a   
   // passive or active motion
   int mouseButton = glfwGetMouseButton(myWin, GLFW_MOUSE_BUTTON_LEFT);
   if (mouseButton == GLFW_PRESS) {
      mouseMotionEvent((int)x, (int)y);
      //std::cout << "MOUSE MOTION EVENT = " << x << ", " << y << std::endl;
   }
   else {
      passiveMotionEvent((int)x, (int)y);
      //std::cout << "PASSIVE MOTION EVENT = " << x << ", " << y << std::endl;
   }

}

//-----------------------------------------------------------------------------
// createWindow() -- create the main window
//-----------------------------------------------------------------------------
int GlfwDisplay::createWindow()
{
   GLint  vpX(0), vpY(0);                          // our initial viewport position
   GLsizei vpWidth(0), vpHeight(0);                // our initial viewport size
   getViewport(&vpX, &vpY, &vpWidth, &vpHeight);
   
   // Let's try to use OpenGL 3.2 or later, which will remove all deprecated functions, and
   // force us to use the 3.2+ core API.
   // check our gl major and minor version

   // antialiasing!
   if (isAntialiasing()) glfwWindowHint(GLFW_SAMPLES, 4);
   if (glMajorVersion > 0) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
      if (glMajorVersion >= 3 && glMinorVersion >= 2) {
         if (compatibility) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
         }
         // load core only
         else glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      }
   }

	myWin = glfwCreateWindow(vpWidth, vpHeight, getName(), NULL, NULL);

   if (myWin == 0) {
      std::cout << "GlfwDisplay::createWindow() - gflwCreateWIndow FAILED!" << std::endl;
   }
   else {
      // set our callbacks
      glfwSetWindowSizeCallback(myWin, reshapeCB);
      glfwSetCursorPosCallback(myWin, cursorMoveCB);
      glfwSetMouseButtonCallback(myWin, mouseButtonCB);
      glfwSetKeyCallback(myWin, keyCB);
      glfwSetWindowCloseCallback(myWin, closeEventCB);
      glfwSetWindowFocusCallback(myWin, focusCB);
      // Register the display
      registerGlfwDisplay(myWin, this);

      // set our position
      glfwSetWindowPos(myWin, vpX, vpY);
   
      // set our context quickly, then initialize glew
      glfwMakeContextCurrent(myWin);
      GLenum glewInitResult;
      glewExperimental = GL_TRUE;
	   glewInitResult = glewInit();

	   if (GLEW_OK != glewInitResult) {
		   std::cerr << "ERROR: " << glewGetErrorString(glewInitResult) << std::endl;
		   exit(EXIT_FAILURE);
	   }
      else {
         if (glMajorVersion > 0) {
            std::cout << "INFO: OpenGL version requested = " << glMajorVersion << "." << glMinorVersion << std::endl;
         }
         std::cout << "INFO: Actual OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
         if (compatibility) std::cout << "INFO: compatibility extensions supported." << std::endl;
      }

      // OpenGL 2 implementation
#ifdef OE_SHADERS
      // generate and create our vertex array 
      GLuint myArrayID = 0;
      glGenVertexArrays(1, &myArrayID);
      setVertexArrayID(myArrayID);
	   glBindVertexArray(myArrayID);

      // create our shaders - this HAS to be done after our context is setup.
      Eaagles::Basic::PairStream* myShaders = getShaders();
      if (myShaders != 0) {
         Eaagles::Basic::List::Item* item = myShaders->getFirstItem();
         while (item != 0) {
            Eaagles::Basic::Pair* pair = (Eaagles::Basic::Pair*)item->getValue();
            if (pair != 0) {
               BasicGL2_0::Shader* shdr = dynamic_cast<BasicGL2_0::Shader*>(pair->object());
               if (shdr != 0) shdr->createAndLinkShaders();
            }
            item = item->getNext();
         }
      }
#endif
   }

   // load our textures
   loadTextures();

   return 0;
}


//-----------------------------------------------------------------------------
// registerGlfwDisplay() --
//   register (add to list) this GlfwDisplay & GLUT window ID
//-----------------------------------------------------------------------------
bool GlfwDisplay::registerGlfwDisplay(GLFWwindow* win, GlfwDisplay* const display)
{
   bool ok = false;
   if (numGlfwDisplays < MAX_DISPLAYS) {
      wins[numGlfwDisplays] = win;
      displayList[numGlfwDisplays] = display;
      numGlfwDisplays++;
      ok = true;
   }
   return ok;
}

//-----------------------------------------------------------------------------
// unregisterGlfwDisplay() --
//   unregister (remove from list) this GLUT window ID
//-----------------------------------------------------------------------------
bool GlfwDisplay::unregisterGlfwDisplay(GLFWwindow* win)
{
   bool ok = false;
   for (int i = 0; !ok && i < numGlfwDisplays; i++) {
      if (win == wins[i]) {
         wins[i] = 0;
         ok = true;
      }
   }
   return ok;
}

//-----------------------------------------------------------------------------
// findRegisteredGlfwDisplay() -- Find the GlfwDisplay for this GLUT window ID
//-----------------------------------------------------------------------------
GlfwDisplay* GlfwDisplay::findRegisteredGlfwDisplay(GLFWwindow* win)
{
   GlfwDisplay* found = 0;
   for (int i = 0; found == 0 && i < numGlfwDisplays; i++) {
      if (win == wins[i]) {
         found = displayList[i];
      }
   }
   return found;
}

//------------------------------------------------------------------------------
// select() -- select this GlfwDisplay
//------------------------------------------------------------------------------
void GlfwDisplay::select()
{
   if (myWin != 0) glfwMakeContextCurrent(myWin);
}

//------------------------------------------------------------------------------
// swapbuffers() --
//------------------------------------------------------------------------------
void GlfwDisplay::swapBuffers()
{
	glfwSwapBuffers(myWin);
}

// SLOT FUNCTIONS
// ---
bool GlfwDisplay::setSlotMajorVersion(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      glMajorVersion = x->getInt();
      ok = true;
   }

   return ok;
}
bool GlfwDisplay::setSlotMinorVersion(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      glMinorVersion = x->getInt();
      ok = true;
   }

   return ok;
}
bool GlfwDisplay::setSlotCompatibility(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      compatibility = x->getBoolean();
      ok = true;
   }

   return ok;
}

//-----------------------------------------------------------------------------
// Mouse motion (with either key pressed) event handler
//-----------------------------------------------------------------------------
void GlfwDisplay::mouseMotionEvent(const int x, const int y)
{    
   //setMouse(x,y);
}


//-----------------------------------------------------------------------------
// passive mouse motion (no mouse key pressed) event handler
//-----------------------------------------------------------------------------
void GlfwDisplay::passiveMotionEvent(const int x, const int y)
{    
   // set our mouse to the current position and update our number of clicks
   // do another pick, just to make sure
   //setMouse(x,y);
}
// ---
// CALLBACKS
// ---
void GlfwDisplay::reshapeCB(GLFWwindow* win, const int w, const int h)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->reshapeIt(w, h);   
}

// handles when the cursor moves... the user can check the status
// of the mouse buttons to determine passive or active motion
void GlfwDisplay::cursorMoveCB(GLFWwindow* win, const double x, const double y)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->cursorMove(x, y);
}
// mouse button callbacks - wheel is not called here
void GlfwDisplay::mouseButtonCB(GLFWwindow* win, int button, int state, int mods)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->mouseButtonEvent(button, state, mods);
}
// keyboard callback, with action (GL_PRESS, RELEASE) and any modification keys
// (CTRL, SHIFT)
void GlfwDisplay::keyCB(GLFWwindow* win, int key, int scanCode, int action, int mods)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->keyboardEvent(key, action, mods);
}
// called when the given window closes.
void GlfwDisplay::closeEventCB(GLFWwindow* win)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->closeEvent();
}
// called when a window gains or loses focus (GL_TRUE for gain focus)
void GlfwDisplay::focusCB(GLFWwindow* win, int state)
{
   GlfwDisplay* myDisplay = findRegisteredGlfwDisplay(win);
   if (myDisplay != 0) myDisplay->focus(state);
}

// ---
// mouse button handler
// ---
void GlfwDisplay::mouseButtonEvent(const int button, const int state, const int mods)
{    
   std::cout << "MOUSE BUTTON, STATE, MODS = " << button << ", " << state << ", " << mods << std::endl;
}
void GlfwDisplay::keyboardEvent(const int key, const int state, const int mods)
{    
   std::cout << "KEYBOARD , STATE, MODS = " << key << ", " << state << ", " << mods << std::endl;
   if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_WORLD_2 && state == GLFW_RELEASE) {
      // GLFW doesn't differenciate between lower and upper characters, so we have to convert it ourself.
      // It defaults to upper case.
      if ((mods & GLFW_MOD_SHIFT) == 0) {
         int newKey = key + 32;
         // convert to lower
         BaseClass::keyboardEvent(newKey);
      }
      else BaseClass::keyboardEvent(key);

//      int numMons = 0;
//      GLFWmonitor** mons = glfwGetMonitors(&numMons);
//      int xPos = 0, yPos = 0;
//      int width = 0, height = 0;
//      for (int i = 0; i < numMons; i++) {
//         std::cout << "MONITOR " << i+1 << " : " << std::endl;
//         glfwGetMonitorPos(mons[i], &xPos, &yPos);
//         std::cout << "VIEWPORT POSITION = " << xPos << ", " << yPos << std::endl;
//         glfwGetMonitorPhysicalSize(mons[i], &width, &height);
//         std::cout << "PHYSICAL SIZE (MM) = " << width << "mm by " << height << "mm" << std::endl;
//         const char* name = glfwGetMonitorName(mons[i]);
//         std::cout << "MONITOR NAME = " << name << std::endl;
//         int numModes = 0;
//         const GLFWvidmode* mode = glfwGetVideoModes(mons[i], &numModes);
//         std::cout << "VIDEO MODES SUPPORTED: " << std::endl;
//         for (int j = 0; j < numModes; j++) {
//            std::cout << "REFRESH RATE = " << mode->refreshRate << std::endl;
//            std::cout << "(" << mode->width << " x " << mode->height << ")" << std::endl;
//            std::cout << "RGB Depth = " << mode->redBits << ", " << mode->greenBits << ", " << mode->blueBits << std::endl;
//         }
//      }
   }
}
void GlfwDisplay::closeEvent()
{
   std::cout << "GlfwDisplay IS CLOSING NOW" << std::endl;
}
void GlfwDisplay::focus(const int state)
{
   if (state == GL_TRUE) std::cout << "GlfwDisplay GOT FOCUS!" << std::endl;
   else std::cout << "GlfwDisplay LOST FOCUS!" << std::endl;
}

//------------------------------------------------------------------------------
// getSlotByIndex() - needed for slottable
//------------------------------------------------------------------------------
Basic::Object* GlfwDisplay::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} 
} 
