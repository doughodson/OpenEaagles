//------------------------------------------------------------------------------
// Class: OpenGL GLFW3 Display
//------------------------------------------------------------------------------
#ifndef __Eaagles_Glfw_GlfwDisplay_H__
#define __Eaagles_Glfw_GlfwDisplay_H__

#include "openeaagles/basicGL/Display.h"

struct GLFWwindow;

namespace Eaagles {
namespace Glfw {

class GlfwDisplay : public BasicGL::Display {
   DECLARE_SUBCLASS(GlfwDisplay, BasicGL::Display)

public:
   GlfwDisplay();

   // Creates the window 
   virtual int createWindow();

   bool isWindowShutdown() const;
   
   // GLFW specific functions
   GLFWwindow* getWindowInstance();
   void cursorMove(const double x, const double y);
   // mouse event is slightly different for GLFW
   void mouseButtonEvent(const int button, const int state, const int mods);
   // keyboard events
   void keyboardEvent(const int key, const int action, const int mods);
   // window close event
   void closeEvent();
   // window level focus events
   void focus(const int state);

   // Handle passive mouse motion events
   virtual void passiveMotionEvent(const int x, const int y);

   // Handle mouse motion events with either mouse key pressed
   virtual void mouseMotionEvent(const int x, const int y);

protected:
   // slot functions
   bool setSlotMajorVersion(const Basic::Number* const x);
   bool setSlotMinorVersion(const Basic::Number* const x);
   bool setSlotCompatibility(const Basic::Number* const x);

   virtual void select();                       
   virtual void swapBuffers();

   // Functions to register the display
   static bool registerGlfwDisplay(GLFWwindow* win, GlfwDisplay* const display);
   static bool unregisterGlfwDisplay(GLFWwindow* win);
   static GlfwDisplay* findRegisteredGlfwDisplay(GLFWwindow* win);

private:
   void initData();
   enum { MAX_DISPLAYS = 500 };     // Maximum number of GlfwDisplay classes

   // GLFW callbacks
   static void reshapeCB(GLFWwindow* win, const int w, const int h);
   static void cursorMoveCB(GLFWwindow* win, const double x, const double y);
   static void mouseButtonCB(GLFWwindow* win, int button, int state, int mods);
   static void keyCB(GLFWwindow* win, int key, int scanCode, int action, int mods);
   static void closeEventCB(GLFWwindow* win);
   static void focusCB(GLFWwindow* win, int state);

   GLFWwindow* myWin;				// holds the pointer to our GLFW 3 window
   int glMajorVersion;           // openGL major version we would like to use
   int glMinorVersion;           // openGL minor version we would like to use
   bool compatibility;           // are we using the compatibility extensions?

   static GLFWwindow* wins[MAX_DISPLAYS];             // List of window structures
   static GlfwDisplay* displayList[MAX_DISPLAYS];     // Display List
   static int numGlfwDisplays;                        // Number of GlfwDisplays
};

inline GLFWwindow* GlfwDisplay::getWindowInstance()     { return myWin; }

} 
} 
#endif
