//------------------------------------------------------------------------------
// Class: GlutDisplay
//------------------------------------------------------------------------------
#ifndef __Eaagles_Glut_GlutDisplay_H__
#define __Eaagles_Glut_GlutDisplay_H__

#include "openeaagles/basicGL/Display.h"

namespace Eaagles {
namespace Glut {

//------------------------------------------------------------------------------
// Class:  GlutDisplay
//
// Description: Manages the Display as a GLUT Window
//
// Notes:
//    1) All subdisplays (see our base class 'displays' slot) must also be
//    GlutDisplay objects and are created as GLUT subwindows.
//
//    2) The default event handler, onEscKey(), will call the shutdownNotification()
//    function and exit (main windows only).  Override the onEscKey() event
//    handler as required.
//
//    3) By default, the clear depth value (see BasicGL::Display slot 'clearDepth')
//    is not set, which is disables the GL_DEPTH_TEST and does the depth buffer
//    is not enabled.  To enabled the depth buffer and the depth test, set the
//    clear depth values using the 'clearDepth' slot.
//
//    4) The size and position of a subwindow is maintained as a ratio [ 0 .. 1 ] of
//    its main window.  These ratios are computed by createSubWindow() using the main
//    window and subwindow's viewport parameters (vpX, vpY, vpWidth and vpHeight).  They are
//    used whenever the main window is reshaped with subwindow reshaping (resizeSubwindows)
//    enabled.  They can be changed and the subwindow reshaped using reshapeSubWindow().
//
// Factory name: GlutDisplay
// Slots:
//    fullScreen        (Boolean)   ! Flag to set full screen mode  -- Main windows only -- (default: false)
//    idleSleepTime     (Number)    ! Idle callback sleep time (MS) -- Main windows only -- (default: 40)
//    resizeSubwindows  (Boolean)   ! Resize our subwindows on a reshape (default: false)
//    pickWidth         (Number)    ! Width of the pick area in screen coordinates(default: 10)
//    pickHeight        (Number)    ! Height of the pick area in screen coordinates(default: 10)
//    accumBuff         (Boolean)   ! Enable the accumulation buffer (default: false)
//    stencilBuff       (Boolean)   ! Enable the stencil buffer (default: false)
//
// Events:
//    ESC_KEY     -- calls onEscKey() event handler; see note #2.
//
//------------------------------------------------------------------------------
class GlutDisplay : public BasicGL::Display {
   DECLARE_SUBCLASS(GlutDisplay, BasicGL::Display)

public:
   GlutDisplay();

   // Creates the window and returns the window ID (or -1 if failed)
   // Will also create any subdisplays (subwindows), as well
   virtual int createWindow();

   // Returns the window id (or -1 if not valid)
   int getWindowId() const;

   // Is this our window ID?
   bool isWindowId(const int id) const;

   // Is this window in 'full screen' mode?
   bool isFullScreen() const;

   // Is accumulation buffer enabled?
   bool isAccumBuff() const;

   // Is stencil buffer enabled?
   bool isStencilBuff() const;

   // Gets Width of the pick area in screen coordinates
   GLdouble getPickWidth() const;

   // Gets Height of the pick area in screen coordinates
   GLdouble getPickHeight() const;

   // Gets/sets the idle sleep time in milliseconds.  This is used by
   // the idle time callback, idleCB(), to release the CPU.
   unsigned int getIdleSleepTime() const;
   virtual bool setIdleSleepTime(const unsigned int ms);

   // Enable/disable resizing of our subwindows.
   virtual bool setResizeWindows(const bool flg);

   // Subwindow position [ x y ] within our main window as a ratio of its main window (see note #4)
   const osg::Vec2d& getSubwindowPosition() const;

   // Subwindow size [ width height ] as a ratio of its main window (see note #4)
   const osg::Vec2d& getSubwindowSize() const;

   // Slot functions
   bool setSlotFullScreen(const Basic::Number* const msg);
   bool setSlotIdleSleepTime(const Basic::Number* const msg);
   bool setSlotResizeWindows(const Basic::Number* const msg);
   bool setSlotPickWidth(const Basic::Number* const msg);
   bool setSlotPickHeight(const Basic::Number* const msg);
   bool setSlotAccumBuff(const Basic::Number* const msg);
   bool setSlotStencilBuff(const Basic::Number* const msg);

   void select() override;                       // Selects this display.
   void hide() override;
   void show() override;
   void swapBuffers() override;
   void reshapeIt(int w, int h) override;
   void mouseEvent(const int button, const int state, const int x, const int y) override;

   void reset() override;
   bool event(const int event, Basic::Object* const obj = nullptr) override;

protected:
   // Creates a subwindow and returns the window ID (or -1 if failed)
   virtual int createSubWindow(const int mainWin);

   // Reshape subwindow
   virtual void reshapeSubWindow();

   // Reshape this subwindow using the subwindows position and size (see note #4)
   virtual bool reshapeSubWindow(const osg::Vec2d& position, const osg::Vec2d& size);

   virtual bool onEscKey();
   virtual void specialEvent(const int key);

   // Handle passive mouse motion events
   virtual void passiveMotionEvent(const int x, const int y);

   // Handle mouse motion events with either mouse key pressed
   virtual void mouseMotionEvent(const int x, const int y);

   static bool registerGlutDisplay(const int id, GlutDisplay* const display);
   static bool unregisterGlutDisplay(const int id);
   static GlutDisplay* findRegisteredGlutDisplay(const int id);
   static int findRegisteredWinId(const GlutDisplay* dp);

   // pick/select stuff
   virtual Graphic* pick(const int item = -1);
   void printSelectBuffer(const GLint hits, const GLuint sbuff[]);
   void clearSelectBuffer(GLuint sbuff[], const int size);
   virtual Graphic* findSelected(const GLint hits, const GLuint sbuff[], const int item = 0);

private:
   enum { MAX_DISPLAYS = 500 };     // Maximum number of GlutDisplay classes
   static const float CLICK_TIME;

private:
   void initData();

   // GLUT callback functions
   static void drawFuncCB();
   static void reshapeItCB(int w, int h);
   static void idleCB();
   static void keyboardFuncCB(unsigned char key, int x, int y);
   static void specialFuncCB(int key, int x, int y);
   static void mouseFuncCB(int button, int state, int x, int y);
   static void passiveMotionFuncCB(int x, int y);
   static void motionFuncCB(int x, int y);
   static void entryFuncCB(int state);

   int   winId;                     // Window ID
   bool  accumBuff;                 // Accumulation buffer enabled
   bool  stencilBuff;               // Stencil buffer enabled
   GLdouble pickWidth;              // Width of the pick area
   GLdouble pickHeight;             // Height of the pick area
   bool  okToResize;                // Ok to resize our subwindows (main windows only)

   // main window only data
   bool  fullScreenFlg;             // Full screen flag (main window only)
   unsigned int idleSleepTimeMS;    // Idle callback sleep time (MS)

   // Subwindow only data
   int   mainWinId;                 // Main window ID (sub-window only)
   osg::Vec2d swPosition;           // Our X and Y positions as a ratio [ 0 ... 1 ] of our main window
   osg::Vec2d swSize;               // Our width and height as a ratio [ 0 ... 1 ] of our main window

   // Registered GlutDisplay list
   static int idList[MAX_DISPLAYS];                    // List of window IDs
   static GlutDisplay* displayList[MAX_DISPLAYS];      // Display List
   static int numGlutDisplays;                         // Number of GlutDisplays
   Graphic* picked;                                    // our "picked graphic"
};

//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------

inline int GlutDisplay::getWindowId() const                         { return winId;           }
inline bool GlutDisplay::isWindowId(const int id) const             { return id == winId;     }
inline bool GlutDisplay::isFullScreen() const                       { return fullScreenFlg;   }
inline GLdouble GlutDisplay::getPickWidth() const                   { return pickWidth;       }
inline GLdouble GlutDisplay::getPickHeight() const                  { return pickHeight;      }
inline bool GlutDisplay::isAccumBuff() const                        { return accumBuff;       }
inline bool GlutDisplay::isStencilBuff() const                      { return stencilBuff;     }
inline unsigned int GlutDisplay::getIdleSleepTime() const           { return idleSleepTimeMS; }
inline const osg::Vec2d& GlutDisplay::getSubwindowPosition() const  { return swPosition;      }
inline const osg::Vec2d& GlutDisplay::getSubwindowSize() const      { return swSize;          }

} // End Glut namespace
} // End Eaagles namespace

#endif
