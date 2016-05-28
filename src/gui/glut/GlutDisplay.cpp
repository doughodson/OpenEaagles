//------------------------------------------------------------------------------
// Glut Display
//------------------------------------------------------------------------------

#include "openeaagles/gui/glut/GlutDisplay.h"

#include "openeaagles/base/Color.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/util/system.h"

#include <cstdlib>

#include <GL/glut.h>
//#include <GL/freeglut.h>

#if !defined(WIN32)
#include <sys/time.h>
#endif

namespace oe {
namespace glut {

IMPLEMENT_SUBCLASS(GlutDisplay,"GlutDisplay")

int GlutDisplay::idList[GlutDisplay::MAX_DISPLAYS];                     // List of window IDs
GlutDisplay* GlutDisplay::displayList[GlutDisplay::MAX_DISPLAYS];       // Display List
int GlutDisplay::numGlutDisplays = 0;                                   // Number of  registered GlutDisplays

const float GlutDisplay::CLICK_TIME = 0.5f;                             // our double click timeout
const unsigned int DEFAULT_IDLE_SLEEP = 40;                             // default idle CB sleep time

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(GlutDisplay)
   "fullScreen",           // 1) Full screen flag     -- Main windows only --
   "idleSleepTime",        // 2) Idle sleep time (MS; default: 40ms) -- Main windows only --
   "resizeSubwindows",     // 3) resize our subwindows on a reshape
   "pickWidth",            // 4) Width of the pick area (default: 10)
   "pickHeight",           // 5) Height of the pick area (default: 10)
   "accumBuff",            // 6) Enable the accumulation buffer (default: false)
   "stencilBuff",          // 7) Enable the stencil buffer (default: false)
END_SLOTTABLE(GlutDisplay)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(GlutDisplay)
   ON_SLOT(1,setSlotFullScreen,base::Number)
   ON_SLOT(2,setSlotIdleSleepTime,base::Number)
   ON_SLOT(3,setSlotResizeWindows,base::Number)
   ON_SLOT(4,setSlotPickWidth,base::Number)
   ON_SLOT(5,setSlotPickHeight,base::Number)
   ON_SLOT(6,setSlotAccumBuff,base::Number)
   ON_SLOT(7,setSlotStencilBuff,base::Number)
END_SLOT_MAP()

//-----------------------------------------------------------------------------
// Event handler macro
//-----------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(GlutDisplay)
   ON_EVENT(ESC_KEY,onEscKey)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
GlutDisplay::GlutDisplay()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// InitData
//------------------------------------------------------------------------------
void GlutDisplay::initData()
{
   winId = -1;
   fullScreenFlg = false;
   pickWidth = 10.0;
   pickHeight = 10.0;
   accumBuff = false;
   stencilBuff = false;
   idleSleepTimeMS = DEFAULT_IDLE_SLEEP;
   okToResize = false;
   picked = nullptr;

   mainWinId = -1;
   swPosition.set(0.0, 0.0);
   swSize.set(50.0, 50.0);
}

//------------------------------------------------------------------------------
// copyData() -- copy our objects data
//------------------------------------------------------------------------------
void GlutDisplay::copyData(const GlutDisplay& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   winId = org.winId;
   pickWidth = org.pickWidth;
   pickHeight = org.pickHeight;
   accumBuff = org.accumBuff;
   stencilBuff = org.stencilBuff;

   idleSleepTimeMS = org.idleSleepTimeMS;
   fullScreenFlg = org.fullScreenFlg;
   okToResize = org.okToResize;

   mainWinId = org.mainWinId;
   swPosition = org.swPosition;
   swSize = org.swSize;

   if (picked != nullptr) picked->unref();
   picked = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- copy our objects data
//------------------------------------------------------------------------------
void GlutDisplay::deleteData()
{
   if (picked != nullptr) picked->unref();
   picked = nullptr;
}

//-----------------------------------------------------------------------------
// Reset the display
//-----------------------------------------------------------------------------
void GlutDisplay::reset()
{
   BaseClass::reset();
}

//-----------------------------------------------------------------------------
// onEscKey() - from event handler
//-----------------------------------------------------------------------------
bool GlutDisplay::onEscKey()
{
   if (isMainDisplay()) {
      shutdownNotification();
      if (isMessageEnabled(MSG_INFO)) {
         std::cout<<"oe::glut::GlutDisplay::onEscKey()Exit by the ESC key!"<<std::endl;
      }
#ifdef __FREEGLUT_EXT_H__     /* freeglut only */
      glutLeaveMainLoop();
      return true;
#else
      std::exit(0);
#endif
   }
   return false;
}

//-----------------------------------------------------------------------------
// setIdleSleepTime() - Sets the idle callback's sleep time
//-----------------------------------------------------------------------------
bool GlutDisplay::setIdleSleepTime(const unsigned int ms)
{
   idleSleepTimeMS = ms;
   return true;
}

//-----------------------------------------------------------------------------
// createWindow() -- create the main window
//-----------------------------------------------------------------------------
int GlutDisplay::createWindow()
{
   winId = -1;

#ifdef __FREEGLUT_EXT_H__     /* freeglut only */
      glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

   unsigned int wmode = GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA;
   if (getClearDepth() >= 0.0f) { wmode = wmode | GLUT_DEPTH; }
   if (accumBuff) { wmode = wmode | GLUT_ACCUM; }
   if (stencilBuff) { wmode = wmode | GLUT_STENCIL; }
   glutInitDisplayMode( wmode );

   GLint  vpX(0), vpY(0);                   // our initial viewport position
   GLsizei vpWidth(0), vpHeight(0);    // our initial viewport size
   getViewport(&vpX, &vpY, &vpWidth, &vpHeight);
   glutInitWindowPosition(vpX, vpY);
   glutInitWindowSize(vpWidth, vpHeight);
   winId = glutCreateWindow(getName());
   if (winId > 0) {
      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "GlutDisplay::createWindow() name = " << getName() << ", winId = " << winId << std::endl;
      }

      // Configure the new window
      if (fullScreenFlg) glutFullScreen();
      glutDisplayFunc(drawFuncCB);
      glutReshapeFunc(reshapeItCB);
      glutIdleFunc(idleCB);
      glutPassiveMotionFunc(passiveMotionFuncCB);
      glutMotionFunc(motionFuncCB);
      glutKeyboardFunc(keyboardFuncCB);
      glutSpecialFunc(specialFuncCB);
      glutMouseFunc(mouseFuncCB);
      glutEntryFunc(entryFuncCB);
      registerGlutDisplay(winId, this);
      glutSetWindow(winId);
      configure();
      loadTextures();

      // Create sub windows (if any)
      if (subDisplays() != nullptr) {
         base::List::Item* item = subDisplays()->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = dynamic_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
               GlutDisplay* dobj = dynamic_cast<GlutDisplay*>( pair->object() );
               if (dobj != nullptr) dobj->createSubWindow(winId);
            }
            item = item->getNext();
         }
      }

      // Select this window
      select();
   }

   return winId;
}

//-----------------------------------------------------------------------------
// createSubWindow() -- create the screen for a subwindow
//-----------------------------------------------------------------------------
int GlutDisplay::createSubWindow(const int mainId)
{
   GLint  vpX(0), vpY(0);                   // our initial viewport position
   GLsizei vpWidth(0), vpHeight(0);    // our initial viewport size
   getViewport(&vpX, &vpY, &vpWidth, &vpHeight);

   // Must have a width and height
   winId = -1;
   if (vpWidth == 0 || vpHeight == 0) return winId;

   mainWinId = mainId;

   unsigned int wmode = GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA;
   if (getClearDepth() >= 0.0f) { wmode = wmode | GLUT_DEPTH; }
   if (accumBuff) { wmode = wmode | GLUT_ACCUM; }
   if (stencilBuff) { wmode = wmode | GLUT_STENCIL; }
   glutInitDisplayMode( wmode );

   winId = glutCreateSubWindow (mainWinId, vpX, vpY, vpWidth, vpHeight);
   if (winId > 0) {
      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "GlutDisplay::createSubWindow() name = " << getName() << ", winId = " << winId << std::endl;
      }

      // compute our sub-display to main display ratios
      const GlutDisplay* pMainWin = findRegisteredGlutDisplay(mainWinId);
      if (pMainWin != nullptr) {
         GLint mainWinX = 0, mainWinY = 0;
         GLsizei mainWinWidth = 0, mainWinHeight = 0;
         pMainWin->getViewport(&mainWinX, &mainWinY, &mainWinWidth, &mainWinHeight);

         const double widthRatio  = static_cast<double>(vpWidth)  / static_cast<double>(mainWinWidth);
         const double heightRatio = static_cast<double>(vpHeight) / static_cast<double>(mainWinHeight);

         const double xRatio = static_cast<double>(vpX) / static_cast<double>(mainWinWidth);
         const double yRatio = static_cast<double>(vpY) / static_cast<double>(mainWinHeight);

         swPosition.set(xRatio, yRatio);
         swSize.set(widthRatio, heightRatio);
      }

      glutDisplayFunc(drawFuncCB);
      glutReshapeFunc(reshapeItCB);
      glutMouseFunc(mouseFuncCB);
      glutKeyboardFunc(keyboardFuncCB);
      glutSpecialFunc(specialFuncCB);
      glutPassiveMotionFunc(passiveMotionFuncCB);
      glutMotionFunc(motionFuncCB);
      glutEntryFunc(entryFuncCB);
      registerGlutDisplay(winId, this);
      glutSetWindow(winId);
      configure();
      loadTextures();

      // Create sub windows (if any)
      if (subDisplays() != nullptr) {
         base::List::Item* item = subDisplays()->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = dynamic_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
               GlutDisplay* dobj = dynamic_cast<GlutDisplay*>( pair->object() );
               if (dobj != nullptr) dobj->createSubWindow(winId);
            }
            item = item->getNext();
         }
      }
   }
   return winId;
}

//------------------------------------------------------------------------------
// swapBuffers() --
//------------------------------------------------------------------------------
void GlutDisplay::swapBuffers()
{
   glutSwapBuffers();
}

//------------------------------------------------------------------------------
// select() -- select this display
//------------------------------------------------------------------------------
void GlutDisplay::select()
{
   glutSetWindow(winId);
   BaseClass::select();
}

//-----------------------------------------------------------------------------
// hide our glut window (set ourself to the current window first)
//-----------------------------------------------------------------------------
void GlutDisplay::hide()
{
   glutSetWindow(winId);
   glutHideWindow();
}

//-----------------------------------------------------------------------------
// show our glut window (set us first)
//-----------------------------------------------------------------------------
void GlutDisplay::show()
{
   glutSetWindow(winId);
   glutShowWindow();
}

//-----------------------------------------------------------------------------
// Set resize windows flag
//-----------------------------------------------------------------------------
bool GlutDisplay::setResizeWindows(const bool flg)
{
   okToResize = flg;
   return true;
}

//-----------------------------------------------------------------------------
// reshape it function, for reshaping our subdisplays
//-----------------------------------------------------------------------------
void GlutDisplay::reshapeIt(int w, int h)
{
   //std::cout << "reshapeIt() winID = " << winId;
   //std::cout << "; size(" << w << ", " << h << ")";
   //std::cout << std::endl;
   // make sure we have a min height and width or our displays will get destroyed
   if (w > 10 && h > 10) {
      BaseClass::reshapeIt(w, h);

      if (subDisplays() != nullptr && okToResize) {

         // go through and put our new numbers in
         base::List::Item* item = subDisplays()->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
               GlutDisplay* gd = dynamic_cast<GlutDisplay*>(pair->object());
               if (gd != nullptr) gd->reshapeSubWindow();
            }
            item = item->getNext();
         }

         // Restore our window ID
         glutSetWindow(this->getWindowId());
      }
   }
}

//-----------------------------------------------------------------------------
// Reshape subwindow using the subwindows position and size (see note #4)
//-----------------------------------------------------------------------------
bool GlutDisplay::reshapeSubWindow(const osg::Vec2d& position, const osg::Vec2d& size)
{
   //std::cout << "reshapeSubWindow(p,s) winID = " << winId << std::endl;
   bool ok = false;
   if (position.x() >= 0 && position.x() <= 1.0 && position.y() >= 0 && position.y() <= 1.0) {
      if (size.x() >= 0 && size.x() <= 1.0 && size.y() >= 0 && size.y() <= 1.0) {
         swPosition = position;
         swSize = size;
         reshapeSubWindow();
         ok = true;
      }
   }
   return ok;
}

//-----------------------------------------------------------------------------
// reshape a sub-window
//-----------------------------------------------------------------------------
void GlutDisplay::reshapeSubWindow()
{
   if (mainWinId >= 0) {
      const GlutDisplay* pMainWin = findRegisteredGlutDisplay(mainWinId);
      if (pMainWin != nullptr) {
         GLint mainWinX = 0, mainWinY = 0;
         GLsizei mainWinWidth = 0, mainWinHeight = 0;
         pMainWin->getViewport(&mainWinX, &mainWinY, &mainWinWidth, &mainWinHeight);

         // we have our new viewport width and height
         // multiply it by our ratio and reset our width and height
         const int newX = static_cast<int>(swPosition.x() * static_cast<double>(mainWinWidth) + 0.5);
         const int newY = static_cast<int>(swPosition.y() * static_cast<double>(mainWinHeight) + 0.5);
         const int newWidth = static_cast<int>(swSize.x() * static_cast<double>(mainWinWidth) + 0.5);
         const int newHeight = static_cast<int>(swSize.y() * static_cast<double>(mainWinHeight) + 0.5);

         setViewport(newX, newY, newWidth, newHeight);

         //std::cout << "reshapeSubWindow() winID = " << winId;
         //std::cout << "; mainWinID = " << mainWinId;
         //std::cout << "; pos(" << newX << ", " << newY << ")";
         //std::cout << "; size(" << newWidth << ", " << newHeight << ")";
         //std::cout << std::endl;

         // now resize
         glutSetWindow(winId);
         glutPositionWindow(newX, newY);
         glutReshapeWindow(newWidth, newHeight);
         reshapeIt(newWidth, newHeight);
         glutPostWindowRedisplay( winId );
      }
   }
}

//-----------------------------------------------------------------------------
// pick() -- Perform for select/pick operation; returns the selected (picked)
//           graphic or zero(0) if nothing was selected.
//
// 1) When item == 0, returns nearest (by depth buffer) selected entry.
// 2) When item < 0, returns furthest (by depth buffer) selected entry.
// 3) When item > 0, returns the item'th selected entry or the first entry if
//    there are less than 'item' entries
// 4) Returns zero(0) when there are no entries in the select buffer or if the
//    Graphic for the select ID is not found.
//-----------------------------------------------------------------------------
graphics::Graphic* GlutDisplay::pick(const int item)
{
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT,viewport);

   // make sure we are starting at 0, 0
   int xm = 0, ym = 0;

   getMouse(&xm,&ym);
   int x = xm;
   int y = viewport[3] - ym;

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluPickMatrix(x, y, getPickWidth(), getPickHeight(), viewport);

   // Get our ortho parameters
   GLdouble oLeft(0), oRight(0), oBottom(0), oTop(0), oNear(0), oFar(0);
   getOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);

   glOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);

   if (getDisplayOrientation() != NORMAL) {
      glPushMatrix();
      if (getDisplayOrientation() == CW90)
         glRotated(-90.0, 0.0, 0.0, 1.0);
      else if (getDisplayOrientation() == CCW90)
         glRotated(90.0, 0.0, 0.0, 1.0);
      else
         glRotated(180.0, 0.0, 0.0, 1.0);
   }

   static const unsigned int MAX_BUFF_SIZE = 1024;
   GLuint sbuff[MAX_BUFF_SIZE];
   clearSelectBuffer(sbuff,MAX_BUFF_SIZE);
   glSelectBuffer(MAX_BUFF_SIZE, sbuff);
   glRenderMode(GL_SELECT);

   glInitNames();
   draw();

   GLint hits = glRenderMode(GL_RENDER);

   if (getDisplayOrientation() != NORMAL) glPopMatrix();

   Graphic* selected = findSelected(hits, sbuff, item);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

   return selected;
}

//-----------------------------------------------------------------------------
// clearSelectBuffer() -- clears our pick select buffer
//-----------------------------------------------------------------------------
void GlutDisplay::clearSelectBuffer(GLuint sbuff[], const int size)
{
   for (int j = 0; j < size; j++) sbuff[j] = 0;
}

//-----------------------------------------------------------------------------
// findSelected() -- find the selected graphic from the 'select buffer'.
//
// 1) When item == 0, returns nearest (by depth buffer) entry.
// 2) When item < 0, returns furthest (by depth buffer) entry.
// 3) When item > 0, returns the item'th selected entry or the first entry if
//    there are less than 'item' entries
// 4) Returns zero(0) when there are no entries in the select buffer or if the
//    Graphic for the select ID is not found.
//-----------------------------------------------------------------------------
graphics::Graphic* GlutDisplay::findSelected(const GLint hits, const GLuint sbuff[], const int item)
{
   Graphic* sel = nullptr;
   GLuint id = 0;

   // ---
   // Build a list of hit names with their z values
   // ---
   static const unsigned int MAX_HITS = 64;
   GLuint hitList[MAX_HITS];
   GLuint zminList[MAX_HITS];
   GLuint zmaxList[MAX_HITS];
   unsigned int hitCnt = 0;

   unsigned int idx = 0;
   for (GLint hit = 1; hit <= hits && hitCnt < MAX_HITS; hit++) {
      GLuint n   = sbuff[idx++];     // Number of select names
      GLuint zmin = sbuff[idx++];    // Min Depth
      GLuint zmax = sbuff[idx++];    // Max Depth
      if (n > 0) {
         hitList[hitCnt] = sbuff[idx];
         zminList[hitCnt] = zmin;
         zmaxList[hitCnt] = zmax;
         hitCnt++;
      }
      for (GLuint i = 0; i < n; i++) idx++; // move past the stack
   }

   // ---
   // We have at least one hit
   // ---
   if (hitCnt > 0) {

      // Select this item (if it exists)
      if (item > 0 && item <= static_cast<int>(hitCnt)) {
         id = hitList[item-1];
      }

      // Or look for the nearest
      else if (item == 0) {
         // (find the smallest zmin)
         GLuint h = hitList[0];
         GLuint zmin = zminList[0];
         for (unsigned int i = 1; i < hitCnt; i++) {
            if (zminList[i] < zmin) {
               h = hitList[i];
               zmin = zminList[i];
            }
         }
         id = h;
      }

      // Or look for the furthest
      else if (item < 0) {
         // (find the largest zmax)
         GLuint h = hitList[0];
         GLuint zmax = zmaxList[0];
         for (unsigned int i = 1; i < hitCnt; i++) {
            if (zmaxList[i] > zmax) {
               h = hitList[i];
               zmax = zmaxList[i];
            }
         }
         id = h;
      }

   }

   // ---
   // Find the Graphic with this id
   // ---
   if (id > 0) {
      base::Pair* pair = findBySelectName(id);
      if (pair != nullptr) {
         sel = static_cast<Graphic*>(pair->object());
      }
   }

   return sel;
}

// printSelectBuffer() -- print the selection buffer
void GlutDisplay::printSelectBuffer(const GLint hits, const GLuint sbuff[])
{
   int idx = 0;
   int hitCnt = 0;
   std::cout << std::hex << "hits: " << hits << std::endl;
   while (hitCnt < hits) {
      unsigned int n = sbuff[idx++];
      std::cout << "Hit: " << n;
      std::cout << " ( " << sbuff[idx++];
      std::cout << ", " << sbuff[idx++];
      std::cout << ") Name stack = { ";
      for (unsigned int i = 0; i < n; i++) {
         std::cout << sbuff[idx++] << " ";
      }
      std::cout << "}" << std::endl;
      hitCnt++;
   }
   std::cout << std::dec << std::endl;
}

//-----------------------------------------------------------------------------
// specialEvent()
//-----------------------------------------------------------------------------
void GlutDisplay::specialEvent(const int key)
{
   if (key == GLUT_KEY_LEFT)           keyboardEvent(BACK_SPACE);
   else if (key == GLUT_KEY_RIGHT)     keyboardEvent(FORWARD_SPACE);
   else if (key == GLUT_KEY_UP)        keyboardEvent(UP_ARROW_KEY);
   else if (key == GLUT_KEY_DOWN)      keyboardEvent(DOWN_ARROW_KEY);
   else if (key == GLUT_KEY_PAGE_UP)   keyboardEvent(PAGE_UP_KEY);
   else if (key == GLUT_KEY_PAGE_DOWN) keyboardEvent(PAGE_DOWN_KEY);
   else if (key == GLUT_KEY_HOME)      keyboardEvent(HOME_KEY);
   else if (key == GLUT_KEY_END)       keyboardEvent(END_KEY);
   else if (key == GLUT_KEY_INSERT)    keyboardEvent(INSERT_KEY);
   else if (key == GLUT_KEY_F1)        keyboardEvent(F1_KEY);
   else if (key == GLUT_KEY_F2)        keyboardEvent(F2_KEY);
   else if (key == GLUT_KEY_F3)        keyboardEvent(F3_KEY);
   else if (key == GLUT_KEY_F4)        keyboardEvent(F4_KEY);
   else if (key == GLUT_KEY_F5)        keyboardEvent(F5_KEY);
   else if (key == GLUT_KEY_F6)        keyboardEvent(F6_KEY);
   else if (key == GLUT_KEY_F7)        keyboardEvent(F7_KEY);
   else if (key == GLUT_KEY_F8)        keyboardEvent(F8_KEY);
   else if (key == GLUT_KEY_F9)        keyboardEvent(F9_KEY);
   else if (key == GLUT_KEY_F10)       keyboardEvent(F10_KEY);
   else if (key == GLUT_KEY_F11)       keyboardEvent(F11_KEY);
   else if (key == GLUT_KEY_F12)       keyboardEvent(F12_KEY);
   else { /* ignore */ }
}



//-----------------------------------------------------------------------------
// Mouse motion (with either key pressed) event handler
//-----------------------------------------------------------------------------
void GlutDisplay::mouseMotionEvent(const int x, const int y)
{
   if (isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::mouseEvent(): " << std::endl;
   }

   if (picked != nullptr) {
      Graphic* selected = pick();
      if (selected != nullptr) {
         if (selected != picked) {
            picked->event(ON_CANCEL);
            picked->unref();
            picked = nullptr;
         }
         selected->event(ON_MOTION);

      }
      else {
         if (picked != nullptr) {
            picked->event(ON_CANCEL);
            picked->unref();
            picked = nullptr;
         }
      }
   }
   setMouse(x,y);
}


//-----------------------------------------------------------------------------
// passive mouse motion (no mouse key pressed) event handler
//-----------------------------------------------------------------------------
void GlutDisplay::passiveMotionEvent(const int x, const int y)
{
   if (isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::mouseEvent(): " << std::endl;
   }

   // set our mouse to the current position and update our number of clicks
   // do another pick, just to make sure
   setMouse(x,y);
}


//-----------------------------------------------------------------------------
// passive button event handler
//-----------------------------------------------------------------------------
void GlutDisplay::mouseEvent(const int button, const int state, const int x, const int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      Graphic* selected = pick();
      if (selected != nullptr) {
         selected->event(INPUT_LEFT_EDGE);
         if (picked != nullptr) picked->unref();
         picked = selected;
         picked->ref();
      }
   }
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      if (picked != nullptr) {
         picked->event(ON_SINGLE_CLICK);
         picked->unref();
         picked = nullptr;
      }
   }
   setMouse(x,y);
}

//-----------------------------------------------------------------------------
// registerGlutDisplay() --
//   register (add to list) this GlutDisplay & GLUT window ID
//-----------------------------------------------------------------------------
bool GlutDisplay::registerGlutDisplay(const int id, GlutDisplay* const display)
{
   bool ok = false;
   if (numGlutDisplays < MAX_DISPLAYS) {
      idList[numGlutDisplays] = id;
      displayList[numGlutDisplays] = display;
      numGlutDisplays++;
      ok = true;
   }
   return ok;
}

//-----------------------------------------------------------------------------
// unregisterGlutDisplay() --
//   unregister (remove from list) this GLUT window ID
//-----------------------------------------------------------------------------
bool GlutDisplay::unregisterGlutDisplay(const int id)
{
   bool ok = false;
   for (int i = 0; !ok && i < numGlutDisplays; i++) {
      if (id == idList[i]) {
         idList[i] = -1;
         ok = true;
      }
   }
   return ok;
}

//-----------------------------------------------------------------------------
// findRegisteredGlutDisplay() -- Find the GlutDisplay for this GLUT window ID
//-----------------------------------------------------------------------------
GlutDisplay* GlutDisplay::findRegisteredGlutDisplay(const int id)
{
   GlutDisplay* found = nullptr;
   for (int i = 0; found == nullptr && i < numGlutDisplays; i++) {
      if (id == idList[i]) {
         found = displayList[i];
      }
   }
   return found;
}

//-----------------------------------------------------------------------------
// findRegisteredWinId() -- Find the GLUT window ID for this GlutDisplay
//-----------------------------------------------------------------------------
int GlutDisplay::findRegisteredWinId(const GlutDisplay* dp)
{
   int found = 0;
   for (int i = 0; found == 0 && i < numGlutDisplays; i++) {
      if (dp == displayList[i]) {
         found = idList[i];
      }
   }
   return found;
}

//-----------------------------------------------------------------------------
// GLUT idle time; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::idleCB()
{
   int id = glutGetWindow();
   double time = base::getComputerTime();

   // N-1 Time
   static double time0 = time;

   // Compute delta time
   double dt = (time - time0);
   time0 = time;

   // Update displays
   unsigned int sleepFor = DEFAULT_IDLE_SLEEP;
   for (int i = 0; i < numGlutDisplays; i++) {
      if (idList[i] >= 0) {
         glutSetWindow(idList[i]);
         displayList[i]->updateData( static_cast<double>(dt) );
         if (displayList[i]->isMainDisplay()) {
            sleepFor = displayList[i]->getIdleSleepTime();
         }
         displayList[i]->drawIt();
      }
   }
   glutSetWindow(id);

   // sleep to let other's use the CPU
   base::msleep(sleepFor);
}


//-----------------------------------------------------------------------------
// GLUT draw; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::drawFuncCB()
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::drawFuncCB(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->drawIt();
}


//-----------------------------------------------------------------------------
// GLUT reshape; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::reshapeItCB(int w, int h)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::reshapeItCB(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->reshapeIt(w,h);
}


//-----------------------------------------------------------------------------
// GLUT keyboard event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::keyboardFuncCB(unsigned char key, int, int)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::keyboardFuncCB(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->keyboardEvent(key);
}


//-----------------------------------------------------------------------------
// GLUT special key event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::specialFuncCB(int key, int, int)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::specialFuncCB(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->specialEvent(key);
}


//-----------------------------------------------------------------------------
// GLUT mouse button event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::mouseFuncCB(int button, int state, int x, int y)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::mouseFuncCBStatic(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->mouseEvent(button,state,x,y);
}


//-----------------------------------------------------------------------------
// GLUT passive mouse motion event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::passiveMotionFuncCB(int x, int y)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::specialFuncCB(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->passiveMotionEvent(x, y);
}


//-----------------------------------------------------------------------------
// GLUT mouse motion event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::motionFuncCB(int x, int y)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::mouseFuncCBStatic(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) p->mouseMotionEvent(x,y);
}


//-----------------------------------------------------------------------------
// GLUT mouse enter/left window event; static callback function
//-----------------------------------------------------------------------------
void GlutDisplay::entryFuncCB(int state)
{
   int id = glutGetWindow();
   GlutDisplay* p = findRegisteredGlutDisplay(id);

   if (p != nullptr && p->isMessageEnabled(MSG_DEBUG)) {
      std::cout << "GlutDisplay::mouseFuncCBStatic(): id = " << id << ", p = " << p << std::endl;
   }

   if (p != nullptr) {
      if (state == GLUT_LEFT) {
         p->onMouseExit();
      }
      else if (state == GLUT_ENTERED) {
         p->onMouseEnter();
      }
   }
}

//-----------------------------------------------------------------------------
// Slot functions
//-----------------------------------------------------------------------------

// setSlotFullScreen()
bool GlutDisplay::setSlotFullScreen(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      fullScreenFlg = msg->getBoolean();
      ok = true;
   }
   return ok;
}

// setSlotIdleSleepTime()
bool GlutDisplay::setSlotIdleSleepTime(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int i = msg->getInt();
      if (i >= 0) {
         unsigned int ms = static_cast<unsigned int>(i);
         ok = setIdleSleepTime(ms);
      }
   }
   return ok;
}

// setSlotResizeWindows()
bool GlutDisplay::setSlotResizeWindows(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setResizeWindows(msg->getBoolean());
   }
   return ok;
}

// pickWidth -- Width of the pick area
bool GlutDisplay::setSlotPickWidth(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      GLdouble s = msg->getDouble();
      if (s > 0) {
         pickWidth = s;
         ok = true;
      }
      else {
         std::cerr << "GlutDisplay::setSlotPickWidth() error; pick size (" << s << ") must be greater than zero!" << std::endl;
      }
   }
   return ok;
}

// pickHeight -- Height of the pick area
bool GlutDisplay::setSlotPickHeight(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      GLdouble s = msg->getDouble();
      if (s > 0) {
         pickHeight = s;
         ok = true;
      }
      else {
         std::cerr << "GlutDisplay::setSlotPickHeight() error; pick size (" << s << ") must be greater than zero!" << std::endl;
      }
   }
   return ok;
}

bool GlutDisplay::setSlotAccumBuff(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      accumBuff = msg->getBoolean();
      ok = true;
   }
   return ok;
}

bool GlutDisplay::setSlotStencilBuff(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      stencilBuff = msg->getBoolean();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
base::Object* GlutDisplay::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& GlutDisplay::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "fullScreen: " << (isFullScreen() ? "true" : "false") << std::endl;

   indent(sout,i+j);
   sout << "resizeSubwindows: " << (okToResize ? "true" : "false") << std::endl;

   indent(sout,i+j);
   sout << "idleSleepTime: " << idleSleepTimeMS << std::endl;

   indent(sout,i+j);
   sout << "pickWidth: " << getPickWidth() << std::endl;

   indent(sout,i+j);
   sout << "pickHeight: " << getPickHeight() << std::endl;

   indent(sout,i+j);
   sout << "accumBuff: " << (isAccumBuff() ? "true" : "false") << std::endl;

   indent(sout,i+j);
   sout << "stencilBuff: " << (isStencilBuff() ? "true" : "false") << std::endl;

   BaseClass::serialize(sout,i+j, true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
