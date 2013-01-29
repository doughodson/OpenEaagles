//------------------------------------------------------------------------------
// Display
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/Display.h"

#include "openeaagles/basicGL/Font.h"
#include "openeaagles/basicGL/Image.h"
#include "openeaagles/basicGL/Texture.h"
#include "openeaagles/basicGL/Material.h"
#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/Rgba.h"
#include "openeaagles/basic/Hsva.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/PairStream.h"

#include <GL/glu.h>

// if OpenGL extension is not defined by glu.h, try loading glext.h
#ifndef GL_BGR_EXT
#include <GL/glext.h>
#endif

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Display,"Display")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Display)
   "name",                 //  1) Display name
   "colorTable",           //  2) Color table
   "normalFont",           //  3) Normal font; Font or Basic::Identifier
   "left",                 //  4) Left ortho bound
   "right",                //  5) Right ortho bound
   "bottom",               //  6) Bottom ortho bound
   "top",                  //  7) Top ortho bound
   "near",                 //  8) Near ortho bound
   "far",                  //  9) Far ortho bound
   "vpX",                  // 10) Viewport x origin
   "vpY",                  // 11) Viewport y origin
   "vpWidth",              // 12) Viewport width
   "vpHeight",             // 13) Viewport height
   "displays",             // 14) Sub-displays
   "stdLineWidth",         // 15) Standard Line width
   "textures",             // 16) Texture(s)
   "clearColor",           // 17) Clear (Background) color; Basic::Color
   "leftBracketChar",      // 18) Left bracket character;  Basic::String or Basic::Number; default: '['
   "rightBracketChar",     // 19) Right bracket character; Basic::String or Basic::Number; default: ']'
   "reverseVideoBrackets", // 20) Reverse video brackets flag:
                           //     If true, brackets are drawn with reversed video font,
                           //     otherwise follow the field's drawing mode.  default: false
   "fonts",                // 21) List of fonts
   "clearDepth",           // 22) clear depth; range: [ 0, 1 ] or negative for no depth buffer
   "orientation",          // 23) display orientation { normal, cw90, ccw90, inverted } default: normal
   "materials",            // 24) List of material objects
   "antiAliasing",         // 25) Turn on/off anti-aliasing.
END_SLOTTABLE(Display)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Display)
   ON_SLOT(1,setName,Basic::String)
   ON_SLOT(2,setColorTable,Basic::PairStream)
   ON_SLOT(3,setNormalFont,Font)
   ON_SLOT(3,setNormalFont,Basic::Identifier)
   ON_SLOT(4,setSlotLeftOrthoBound,Basic::Number)
   ON_SLOT(5,setSlotRightOrthoBound,Basic::Number)
   ON_SLOT(6,setSlotBottomOrthoBound,Basic::Number)
   ON_SLOT(7,setSlotTopOrthoBound,Basic::Number)
   ON_SLOT(8,setSlotNearOrthoBound,Basic::Number)
   ON_SLOT(9,setSlotFarOrthoBound,Basic::Number)
   ON_SLOT(10,setSlotViewportXOrigin,Basic::Number)
   ON_SLOT(11,setSlotViewportYOrigin,Basic::Number)
   ON_SLOT(12,setSlotViewportWidth,Basic::Number)
   ON_SLOT(13,setSlotViewportHeight,Basic::Number)
   ON_SLOT(14,setSlotSubdisplayStream,Basic::PairStream)
   ON_SLOT(14,setSlotSubdisplaySingle,Display)
   ON_SLOT(15,setSlotStdLineWidth,Basic::Number)
   ON_SLOT(16,setSlotTexturesStream,Basic::PairStream)
   ON_SLOT(16,setSlotTexturesSingle,Texture)
   ON_SLOT(17,setSlotClearColor,Basic::Color)
   ON_SLOT(18,setSlotLeftBracketCharacter,Basic::Number)
   ON_SLOT(18,setSlotLeftBracketCharacter,Basic::String)
   ON_SLOT(19,setSlotRightBracketCharacter,Basic::Number)
   ON_SLOT(19,setSlotRightBracketCharacter,Basic::String)
   ON_SLOT(20,setSlotReverseVideoBrackets,Basic::Number)
   ON_SLOT(21,setFontList,Basic::PairStream)
   ON_SLOT(22,setSlotClearDepth,Basic::Number)
   ON_SLOT(23,setSlotDisplayOrientation,Basic::String)
   ON_SLOT(24,setSlotMaterials,Basic::PairStream)
   ON_SLOT(24,setSlotMaterials,Material)
   ON_SLOT(25,setSlotAntialias,Basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Display::Display()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Display::initData()
{
   name = new Basic::String(" ");
   subdisplays = 0;
   textures = 0;
   stdLinewidth = 1;
   linewidth = 1;

   subdisplayFlg = false;
   antialias = true;
   focusPtr = 0;
   mx = 0;
   my = 0;

   orientation = NORMAL;

   // Z-buffer
   clearDepth = -1.0;

   // Colors
   color.set(1.0f,1.0f,1.0f,1.0f);
   clearColor.set(0.0f,0.0f,0.0f,0.0f);

   {
      colorTable = 0;
      Basic::PairStream* p = defaultColors();
      setColorTable(p);
      p->unref();

      colorName = new Basic::Identifier();

      normColor = 0;
      Basic::Rgba* nc = new Basic::Rgba(0.0, 1.0, 0.0, 1.0); // default: green
      setNormColor( nc );
      nc->unref();

      hiColor = 0;
      Basic::Rgba* hc = new Basic::Rgba(1.0, 0.0, 0.0, 1.0); // default: red
      setHighlightColor( hc );
      hc->unref();
   }

   // Font
   fontList = 0;
   currentFont = 0;
   normalFont    = 0;
   normalFontName = 0;

   // default viewport size
   vpX      = -1;
   vpY      = -1;
   vpWidth  = 300;
   vpHeight = 300;

   // Ortho parameters
   oLeft   = -0.5;
   oRight  = 640.5;
   oBottom = -0.5;
   oTop    = 480.5;
   oNear   = -1;
   oFar    = 1;

   // Brackets
   leftBracketChar = '[';
   rightBracketChar = ']';
   rvBrackets    = false;
   reversedFlg   = false;
   underlinedFlg = false;

   // materials
   materials = 0;

   okToSwap = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Display::copyData(const Display& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   *name = *org.name;
   subdisplayFlg = org.subdisplayFlg;

   orientation = org.orientation;

   if (subdisplays != 0) { subdisplays->unref(); }
   if (org.subdisplays != 0) {
      subdisplays = org.subdisplays->clone();
      processSubdisplays();
   }
   else subdisplays = 0;

   if (textures != 0) { textures->unref(); }
   if (org.textures != 0) {
      textures = org.textures->clone();
      processTextures();
   }
   else textures = 0;

   if (materials != 0) materials->unref();
   if (org.materials != 0) {
      materials = org.materials->clone();
      processMaterials();
   }

   stdLinewidth = org.stdLinewidth;
   linewidth = org.linewidth;

   antialias = org.antialias;
   focusPtr = org.focusPtr;
   mx = org.mx;
   my = org.my;

   clearDepth = org.clearDepth;

   setColorTable(org.colorTable);
   setNormColor(org.normColor);
   setHighlightColor(org.hiColor);
   color = org.color;
   *colorName = *org.colorName;
   clearColor = org.clearColor;

   setFontList(org.fontList);
   setFont( org.currentFont );

   if (org.normalFontName != 0) setNormalFont( org.normalFontName );
   else setNormalFont( org.normalFont );

   oLeft = org.oLeft;
   oRight = org.oRight;
   oBottom = org.oBottom;
   oTop = org.oTop;
   oNear = org.oNear;
   oFar = org.oFar;
   vpX = org.vpX;
   vpY = org.vpY;
   vpWidth = org.vpWidth;
   vpHeight = org.vpHeight;

   leftBracketChar = org.leftBracketChar;
   rightBracketChar = org.rightBracketChar;
   reversedFlg   = org.reversedFlg;
   underlinedFlg = org.underlinedFlg;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Display::deleteData()
{
   if (name != 0) name->unref();
   name = 0;

   if (subdisplays != 0) { subdisplays->unref(); subdisplays = 0; }

   if (textures != 0) { textures->unref(); textures = 0; }

   if (materials != 0) { materials->unref(); materials = 0; }

   if (colorTable != 0) { colorTable->unref(); colorTable = 0; }

   if (normColor != 0) { normColor->unref(); normColor = 0; }

   if (hiColor != 0) { hiColor->unref(); hiColor = 0; }

   if (currentFont != 0) { currentFont->unref(); currentFont = 0; }

   if (normalFont != 0) { normalFont->unref(); normalFont = 0; }
   if (normalFontName != 0) { normalFontName->unref(); normalFontName = 0; }
}


//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Display::updateTC(const LCreal dt)
{
   BaseClass::updateTC(dt);

   // Update any sub-displays ...
   if (subdisplays != 0) {
      Basic::List::Item* item = subdisplays->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = dynamic_cast<Basic::Pair*>(item->getValue());
         if (pair != 0) {
            Display* obj = dynamic_cast<Display*>( pair->object() );
            if (obj != 0) obj->tcFrame(dt);
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Display::reset()
{
   BaseClass::reset();
   if (subdisplays != 0) {
      // Reset all of our sub-displays
      Basic::List::Item* item = subdisplays->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Component* obj = (Component*)( pair->object() );
         if (obj != 0) obj->reset();
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// select() -- select this display
//------------------------------------------------------------------------------
void Display::select()
{
   glLineWidth(linewidth);
   lcColor4v(color.ptr());
}

//------------------------------------------------------------------------------
// keyboardEvent() -- pass keyboard events to the focus
//------------------------------------------------------------------------------
void Display::keyboardEvent(const int key)
{
   Display* focusDisplay = dynamic_cast<Display*>( focusPtr );
   if ( focusDisplay != 0 && focusDisplay != this) {
      // When our focus is a Display ...
      focusDisplay->keyboardEvent(key);

   }
   else {
      // When our focus is NOT at a Display ...

      // Null pointer?  Try to set to our current subpage
      if (focusPtr == 0 && subpage() != 0) focusPtr = subpage();
      if (isMessageEnabled(MSG_DEBUG)) {
         std::cout << "Display::keyboardEvent(" << this << "," << key << "), focusPtr = " << focusPtr << std::endl;
      }
      if (focusPtr != 0) focusPtr->event(key);
   }

}

//------------------------------------------------------------------------------
// onMouseEnter() - called when the mouse enters the display
//------------------------------------------------------------------------------
void Display::onMouseEnter()
{
   // Derive from this class and do what you want with this function call
}

//------------------------------------------------------------------------------
// onMouseExit() - called when the mouse exits the display
//------------------------------------------------------------------------------
void Display::onMouseExit()
{
   // Derive from this class and do what you want with this function call
}

//------------------------------------------------------------------------------
// buttonEvent() -- pass button events to the focus
//------------------------------------------------------------------------------
void Display::buttonEvent(const int b)
{
   // Null pointer?  Try to set to our current subpage
   if (focusPtr == 0 && subpage() != 0) focusPtr = subpage();

   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << "Display::buttonEvent(" << this << "," << b << "), focusPtr = " << focusPtr << std::endl;
   //}
   if (focusPtr != 0) focusPtr->event(b);
}

//------------------------------------------------------------------------------
// mouseEvent() -- pass mouse input events to the focus
//------------------------------------------------------------------------------
void Display::mouseEvent(const int, const int, const int x, const int y)
{
   // keep track of our mouse state
   setMouse(x,y);
}

//------------------------------------------------------------------------------
// setMouse() -- sets the current mouse position
//------------------------------------------------------------------------------
void Display::setMouse(const int x, const int y, Display* const subdisplay)
{
   int lx = x;
   int ly = y;

   if (subdisplay != 0) {
      // When we're called from a sub-display,
      //   offset the coordinates and set the focus to the sub-display
      GLsizei sdX, sdY;
      subdisplay->getViewportOrigin(&sdX, &sdY);
      lx = x + sdX;
      ly = y + sdY;
        if (focus() != 0 && focus() != subdisplay) {
            // if our previous focus was a display, exit it properly
            if (focus()->isClassType(typeid(Display))) {
                BasicGL::Display* dis = (BasicGL::Display*)focus();
                dis->onMouseExit();
            }
      focus(subdisplay);
            // enter our new mouse display
            subdisplay->onMouseEnter();
        }
   }
   else {
        // if we aren't a subdisplay, but we are a display, we
        // still need to call our entry and exit routines
        if (focus() != 0 && focus()->isClassType(typeid(Display))) {
            BasicGL::Display* dis = (BasicGL::Display*)focus();
            dis->onMouseExit();
        }

      // When we are NOT called from a sub-display,
      //   reset the focus to one of our own pages or components
      if (focus() != 0) {
         if (focus()->isClassType(typeid(Display))) {
            focus( subpage() );
         }
      }
      else focus( subpage() );
   }

   // Set our new mouse coordinates
   mx = lx;
   my = ly;

   // Send these coordinates to our parent display
   Display* parentDisplay = (Display*) findContainerByType(typeid(Display));
   if (parentDisplay != 0) {
      parentDisplay->setMouse(lx,ly,this);
   }

   // If we have no focus whatsoever at the end, we take the focus
    if (focusPtr == 0) {
        // call our entry procedure!
        onMouseEnter();
        focus(this);

    }
}

//------------------------------------------------------------------------------
// focus() -- set/return the input 'focus'
//------------------------------------------------------------------------------
Graphic* Display::focus() const
{
   return focusPtr;
}

Graphic* Display::focus(Graphic* s)
{
   return (focusPtr = s);
}

//------------------------------------------------------------------------------
// clear() --
//------------------------------------------------------------------------------
void Display::clear()
{
   if (clearDepth >= 0.0f) {
      glClearDepth(clearDepth);
      glEnable(GL_DEPTH_TEST);
   }
   else {
      glDisable(GL_DEPTH_TEST);
   }

   osg::Vec4f cc = getClearColor();
   glClearColor(cc[Basic::Color::RED], cc[Basic::Color::GREEN], cc[Basic::Color::BLUE], cc[Basic::Color::ALPHA]);

   if (clearDepth >= 0.0f)
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
   else
      glClear(GL_COLOR_BUFFER_BIT);
}

//------------------------------------------------------------------------------
// swapbuffers() --
//------------------------------------------------------------------------------
void Display::swapbuffers()
{
}

//-----------------------------------------------------------------------------
// hide our display --
//-----------------------------------------------------------------------------
void Display::hide()
{
}

//-----------------------------------------------------------------------------
// show our display --
//-----------------------------------------------------------------------------
void Display::show()
{
}

//------------------------------------------------------------------------------
// setLineWidth() -- change the current linewidth
//------------------------------------------------------------------------------
GLfloat Display::setLinewidth(const GLfloat lw)
{
   GLfloat olw = linewidth;
   if (linewidth != lw) {
      linewidth = lw;
      glLineWidth(linewidth);
   }
   return olw;
}

//------------------------------------------------------------------------------
// setStdLineWidth() --
//------------------------------------------------------------------------------
void Display::setStdLineWidth(const GLfloat lw)
{
   if (lw != stdLinewidth) {
      stdLinewidth = lw;
      //setLinewidth(stdLinewidth);
   }
}

//------------------------------------------------------------------------------
// setStdLineWidth() --
//------------------------------------------------------------------------------
bool Display::setAntialiasing(const bool on)
{
   antialias = on;
   return true;
}

//------------------------------------------------------------------------------
// setOrtho() -- set the ortho parameters (call before init())
//------------------------------------------------------------------------------
void Display::setOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   oLeft   = l;
   oRight  = r;
   oBottom = b;
   oTop    = t;
   oNear   = n;
   oFar    = f;
}

void Display::getOrtho(GLdouble &left, GLdouble &right, GLdouble &bottom, GLdouble &top, GLdouble &x, GLdouble &y) const
{
   left = oLeft;
   right = oRight;
   bottom = oBottom;
   top = oTop;
   x = oNear;
   y = oFar;
}

//-----------------------------------------------------------------------------
// forceOrtho()
//-----------------------------------------------------------------------------
void Display::forceOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   setOrtho(l,r,b,t,n,f);
   reshapeIt(vpWidth,vpHeight);
}

//-----------------------------------------------------------------------------
// drawIt() -- default function to draw the display
//-----------------------------------------------------------------------------
void Display::drawIt()
{
   select();

   glViewport(0,0,vpWidth,vpHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   configure();
   clear();

   if (getDisplayOrientation() != NORMAL) {
      glPushMatrix();
      if (getDisplayOrientation() == CW90)
         glRotated(-90.0, 0.0, 0.0, 1.0);
      else if (getDisplayOrientation() == CCW90)
         glRotated(90.0, 0.0, 0.0, 1.0);
      else
         glRotated(180.0, 0.0, 0.0, 1.0);
   }

   // Draw the display
   draw();

   if (getDisplayOrientation() != NORMAL) {
      glPopMatrix();
   }

   // Swap buffer
   if (okToSwap) swapbuffers();
}

//-----------------------------------------------------------------------------
// reshapeIt() -- default function to reshape the viewport
//-----------------------------------------------------------------------------
void Display::reshapeIt(int w, int h)
{
   vpWidth = w;
   vpHeight = h;

   if (oLeft == DBL_MAX) {
      // DEFAULT values
      GLdouble a = (GLdouble(vpHeight)/GLdouble(vpWidth));
      oLeft   = -1.0;
      oRight  = 1.0;
      oBottom = (-1.0*a);
      oTop    = (1.0*a);
      oNear   = 0.0;
      oFar    = 10.0;
   }

   glViewport(0,0,vpWidth,vpHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//------------------------------------------------------------------------------
// getViewport() -- get the viewport parameters
// setViewport() -- set the viewport parameters
//------------------------------------------------------------------------------
void Display::getViewport(GLint* x, GLint* y, GLsizei* w, GLsizei* h) const
{
   *x = vpX;
   *y = vpY;
   *w = vpWidth;
   *h = vpHeight;
}

void Display::setViewport(const GLint x, const GLint y, const GLsizei w, const GLsizei h)
{
   vpX = x;
   vpY = y;
   vpWidth  = w;
   vpHeight = h;
}

//------------------------------------------------------------------------------
// getViewportOrigin() -- get the viewport origin
// setViewportOrigin() -- set the viewport origin
//------------------------------------------------------------------------------
void Display::getViewportOrigin(GLsizei* x, GLsizei* y) const
{
   *x = vpX;
   *y = vpY;
}

void Display::setViewportOrigin(const GLsizei x, const GLsizei y)
{
   vpX = x;
   vpY = y;
}


//------------------------------------------------------------------------------
// getViewportSize() -- get the viewport size
// setViewportSize() -- set the viewport size
//------------------------------------------------------------------------------
void Display::getViewportSize(GLsizei* w, GLsizei* h) const
{
   *w = vpWidth;
   *h = vpHeight;
}

void Display::setViewportSize(const GLsizei w, const GLsizei h)
{
   vpWidth  = w;
   vpHeight = h;
}

//-----------------------------------------------------------------------------
// setSubscreen() -- setup a temporary sub-viewport
//-----------------------------------------------------------------------------
void Display::setSubscreen(const GLdouble lf, const GLdouble rt, const GLdouble bt, const GLdouble tp)
{
   GLint nx = GLint( GLdouble(vpWidth) * ((lf - oLeft)/(oRight - oLeft)) );
   GLint ny = GLint( GLdouble(vpHeight) * ((bt - oBottom)/(oTop - oBottom)) );

   GLint nw = GLint( GLdouble(vpWidth) * ((rt - lf)/(oRight - oLeft)) );
   GLint nh = GLint( GLdouble(vpHeight) * ((tp - bt)/(oTop - oBottom)) );

   glViewport(nx,ny,nw,nh);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(lf, rt, bt, tp, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//-----------------------------------------------------------------------------
// clearSubscreen() -- clears the temporary sub-viewport
//-----------------------------------------------------------------------------
void Display::clearSubscreen()
{
   reshapeIt(vpWidth,vpHeight);
}

//-----------------------------------------------------------------------------
// setScissor() -- setup a scissor box and enable the test
//-----------------------------------------------------------------------------
void Display::setScissor(const GLdouble scissorLeft, const GLdouble scissorRight,
   const GLdouble sscissorBottom, const GLdouble scissorTop)
{
   // get our coordinates and transform them to window coordinates
   GLdouble objz = 0;

   // we have to get our model and
   GLdouble modelMatrix[16];
   glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
   GLdouble projMatrix[16];
   glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   // these will hold our new values
   GLdouble winx0 = 0;
   GLdouble winy0 = 0;
   GLdouble winz0 = 0;
   GLdouble winx1 = 0;
   GLdouble winy1 = 0;
   GLdouble winz1 = 0;

   // ok, we have all of our data, send it to be converted to window coordinates
   gluProject(scissorLeft, sscissorBottom, objz, modelMatrix, projMatrix, viewport, &winx0, &winy0, &winz0);
   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << winx0 << " " << winy0 << " " << winz0 << std::endl;
   //}

   gluProject(scissorRight, scissorTop, objz, modelMatrix, projMatrix, viewport, &winx1, &winy1, &winz1);
   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << winx1 << " " << winy1 << " " << winz1 << std::endl;

   //std::cout << "X = " << x << "    Y = " << y << std::endl;
   //std::cout << "W = " << width << "    H = " << height << std::endl;
   //}

   // now we are going to scissor a small box using our window coordinates
   glEnable(GL_SCISSOR_TEST);
   if (getDisplayOrientation() == CCW90) {
      // 90 degrees Counter-clockwise rotation
      GLint x = (GLint) winx1;
      GLint y = (GLint) winy0;
      GLsizei width = (GLsizei) (winx0 -  winx1);
      GLsizei height = (GLsizei) (winy1 - winy0);
      glScissor(x, y, width, height);
   }
   else if (getDisplayOrientation() == CW90) {
      // 90 degrees Counter-clockwise rotation
      GLint x = (GLint) winx0;
      GLint y = (GLint) winy1;
      GLsizei width = (GLsizei) (winx1 -  winx0);
      GLsizei height = (GLsizei) (winy0 - winy1);
      glScissor(x, y, width, height);
   }
   else if (getDisplayOrientation() == INVERTED) {
      // Normal
      GLint x = (GLint) winx1;
      GLint y = (GLint) winy1;
      GLsizei width = (GLsizei) (winx0 -  winx1);
      GLsizei height = (GLsizei) (winy0 - winy1);
      glScissor(x, y, width, height);
   }
   else {
      // Normal
      GLint x = (GLint) winx0;
      GLint y = (GLint) winy0;
      GLsizei width = (GLsizei) (winx1 -  winx0);
      GLsizei height = (GLsizei) (winy1 - winy0);
      glScissor(x, y, width, height);
   }
}

//-----------------------------------------------------------------------------
// clearScissor() -- clears (disables) the scissor test
//-----------------------------------------------------------------------------
void Display::clearScissor()
{
   glDisable(GL_SCISSOR_TEST);
}

//------------------------------------------------------------------------------
// setColor() -- change the current color vector
//------------------------------------------------------------------------------
void Display::setColor(const osg::Vec4& newColor)
{
   if (color != newColor) {
      color = newColor;
      colorName->empty();
      lcColor4v(color.ptr());
   }
}

void Display::setColor(const char* cname1)
{
   // Already set? Then leave
   if (*colorName == cname1) return;

   Basic::Color* newColor = getColor(cname1);
   if (newColor != 0) {
      colorName->setStr(cname1);
      color = *(newColor->getRGBA());
      lcColor4v(color.ptr());
   }
}

//------------------------------------------------------------------------------
// setClearColor() -- set the clear color (used by a screen clear)
//------------------------------------------------------------------------------
void Display::setClearColor(const Basic::Color& ccolor)
{
   clearColor = *ccolor.getRGBA();
}

//------------------------------------------------------------------------------
// getTextureByName()  -- get a texture by name
//------------------------------------------------------------------------------
GLuint Display::getTextureByName(const Basic::Identifier* texName)
{
   GLuint tex = 0;
   if (texName != 0 && textures != 0) {
      const Basic::Pair* pair = textures->findByName( *texName );
      if (pair != 0) {
         const Texture* pt = dynamic_cast<const Texture*>( pair->object() );
         if (pt != 0) tex = pt->getTexture();
      }
   }
   if ( tex == 0 && isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Display::getTextureByName(): texture ";
      if (texName != 0) std::cerr << *texName;
      std::cerr << "  not found!" << std::endl;
   }
   return tex;
}

//------------------------------------------------------------------------------
// getMaterial()  -- gets a material by name
//------------------------------------------------------------------------------
Material* Display::getMaterial(const Basic::Identifier* name)
{
   Material* temp = 0;
   if (name !=0 && materials != 0) {
      const Basic::Pair* pair = materials->findByName( *name );
      if (pair != 0) {
         const Material* mat = dynamic_cast<const Material*>( pair->object() );
         if (mat != 0) temp = (Material*)mat;
      }
   }
   if (temp == 0 && isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Display::getMaterial(): material \"";
      if (name != 0) std::cerr << *name;
      std::cerr << "\" not found!" << std::endl;
   }
   return temp;
}


//------------------------------------------------------------------------------
// setNormColor() &  setHighlightColor() -- set default text colors
//------------------------------------------------------------------------------
void Display::setNormColor(const Basic::Color* const nc)
{
   if (normColor != 0) normColor->unref();
   normColor = nc;
   if (normColor != 0) normColor->ref();
}

void Display::setHighlightColor(const Basic::Color* const nc)
{
   if (hiColor != 0) hiColor->unref();
   hiColor = nc;
   if (hiColor != 0) hiColor->ref();
}

//------------------------------------------------------------------------------
// setColorTable() -- set the color table
//------------------------------------------------------------------------------
bool Display::setColorTable(Basic::PairStream* const sctobj)
{
   bool ok = true;
   if (colorTable != 0) colorTable->unref();
   colorTable = sctobj;
   if (colorTable != 0) colorTable->ref();
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Display::setColorTable: No input available" << std::endl;
      }
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Functions to set the various fonts
//------------------------------------------------------------------------------

// setFontList() -- set the font list
bool Display::setFontList(Basic::PairStream* const obj)
{
   bool ok = true;
   if (fontList != 0) fontList->unref();
   fontList = obj;
   if (fontList != 0) fontList->ref();
   return ok;
}

// getFont() -- by name
Font* Display::getFont(const char* const fontName)
{
   Font* ft = 0;
   if (fontList != 0) {
      Basic::Pair* p = fontList->findByName(fontName);
      if (p != 0) ft = (Font*) p->object();
   }
   return ft;
}
// getFont() -- by name (const version)
const Font* Display::getFont(const char* const fontName) const
{
   const Font* ft = 0;
   if (fontList != 0) {
      const Basic::Pair* p = fontList->findByName(fontName);
      if (p != 0) ft = (const Font*) p->object();
   }
   return ft;
}

// getFont() -- by name
Font* Display::getFont(const Basic::Identifier* const fontName)
{
   return getFont(*fontName);
}

// getFont() -- by name (const version)
const Font* Display::getFont(const Basic::Identifier* const fontName) const
{
   return getFont(*fontName);
}

// getFont() -- by index
Font* Display::getFont(const int index)
{
   Font* ft = 0;
   if (fontList != 0) {
      Basic::Pair* p = fontList->getPosition(index+1);
      if (p != 0) ft = (Font*) p->object();
   }
   return ft;
}

// getFont() -- by index (const version)
const Font* Display::getFont(const int index) const
{
   const Font* ft = 0;
   if (fontList != 0) {
      const Basic::Pair* p = fontList->getPosition(index+1);
      if (p != 0) ft = (const Font*) p->object();
   }
   return ft;
}


// selectFont() -- select the current font based on mode flags
void Display::selectFont(const bool reversed, const bool underlined, Font* newFont )
{
   // set our font initially to the one given to us
   Font* font = newFont;
   if (font == 0) {
      // our font is default, since none was specified
      font = getNormalFont();
   }
   // now set this font, if we have one!
   if (font != 0) {
      setFont(font);
      // Save flags for reference
      reversedFlg   = reversed;
      underlinedFlg = underlined;
   }
}

// setFont() -- change the current font
void Display::setFont(Font* newFont)
{
   if (currentFont != newFont) {
      if (currentFont != 0) currentFont->unref();
      currentFont = newFont;
      if (currentFont != 0) {
         currentFont->ref();
      }
   }
}

// getNormalFont() --- (look it up, if needed)
Font* Display::getNormalFont()
{
   if (normalFont == 0 && normalFontName != 0) {
      normalFont = getFont(normalFontName);
      if (normalFont == 0) {
         // Not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Display::getNormalFont(): font name '" << (const char*) *normalFontName << "' not found!" << std::endl;
         }
         normalFontName->unref();
         normalFontName = 0;
      }
   }
   return normalFont;
}

// getNormalFont() --- (const version -- no lookup)
const Font* Display::getNormalFont() const
{
   return normalFont;
}

// setNormalFont() ---
bool Display::setNormalFont(Font* const f)
{
   bool ok = true;
   if (normalFont != 0) normalFont->unref();
   normalFont = f;
   if (normalFont != 0) normalFont->ref();
   return ok;
}

// setNormalFont() --- by name (character string)
bool Display::setNormalFont(const char* const fontName)
{
   if (fontName != 0) {
      if (normalFontName == 0) {
         normalFontName = new Basic::Identifier();
      }
      *normalFontName = fontName;
      setNormalFont( getFont(fontName) );
   }
   else {
      if (normalFontName != 0) { normalFontName->unref(); normalFontName = 0; }
      setNormalFont((Font*)0);
   }
   return true;
}

// setNormalFont() --- by name (Basic::Identifier)
bool Display::setNormalFont(const Basic::Identifier* const fontName)
{
   if (fontName != 0) {
      if (normalFontName == 0) {
         normalFontName = new Basic::Identifier();
      }
      *normalFontName = *fontName;
      setNormalFont( getFont(fontName) );
   }
   else {
      if (normalFontName != 0) { normalFontName->unref(); normalFontName = 0; }
      setNormalFont((Font*)0);
   }
   return true;
}

//------------------------------------------------------------------------------
// text output routines
//------------------------------------------------------------------------------

// outputTextLC() -- output a text at (Cp, Ln)
void Display::outputTextLC(const int ln, const int cp, const char* sp, const int n, const bool vf) const
{
   if (currentFont == 0 || n <= 0) return;
   osg::Vec4 ocolor = getCurrentColor();

   Display* that = (Display*)this;
   // If manual reverse text, draw a background polygon
   // Computer posiiton
   GLdouble x = 0.0;
   GLdouble y = 0.0;
   currentFont->position(ln,cp,x,y);
   GLdouble dx = (currentFont->getCharacterSpacing());
   GLdouble dy = (currentFont->getLineSpacing());
   size_t len = strlen(sp);

   // If manual reverse text, draw a background polygon
   if (reversedFlg) {
      if (vf) {
         GLdouble x1 = 0, y1 = 0;
         //currentFont->position(ln+1, cp, x1, y1);
         glPushMatrix();
         // we have to move over 1/2 a character for our xpos
         x1 = x + (dx / 2);
         y1 = y + dy;
         glTranslated(x1, y1, 0);

         GLdouble myX = (dx * len) * 0.2f;
         GLdouble myY = (dy * len) + (dy / 2);


         GLdouble vv[4][3] = {
            { -myX, -myY, -0.001 }, { -myX, 0, -0.001 }, { myX, 0, -0.001 }, { myX, -myY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }
      else {
         // Offsets to center to polygon
         dx *= 0.1;
         dy *= 0.2;

         GLdouble x0 = x - dx;
         GLdouble y0 = y - dy;
         GLdouble x1 = 0.0;
         GLdouble y1 = 0.0;
         currentFont->position((ln-1),(cp+n),x1,y1);
         x1 = x1 - dx;
         y1 = y1 - dy;
         GLdouble vv[4][3] = {
            { x0, y0, -0.001 }, { x1, y0, -0.001 }, { x1, y1, -0.001 }, { x0, y1, -0.001 }
         };
         glPushMatrix();
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }

      that->setColor(getClearColor());
   }

   if (underlinedFlg) {
      GLdouble height = currentFont->getLineSpacing();
      GLdouble width = currentFont->getCharacterSpacing();

      // only come down about a third for underlining
      glPushMatrix();
      that->setColor(ocolor);

      GLdouble myY = 0, myX = 0;
      if (vf) {
         // do nothing if we are vertical and underlined, because then it wouldn't be UNDERlined!
         //height /= 2;
         //myX = x - height;
         //myY = y - (width * len);
         //glBegin(GL_LINES);
         //    glVertex2f(myX, y + width);
         //    glVertex2f(myX, myY);
         //glEnd();
      }
      else {
         height /= 2;
         myY = y - height;
         myX = x + (width * len);
         //glTranslatef(x, y, 0);
         glBegin(GL_LINES);
         glVertex2d(x, myY);
         glVertex2d(myX, myY);
         glEnd();
      }

      glPopMatrix();
      if (reversedFlg) {
         that->setColor(getClearColor());
      }

   }

   // Output the text
   currentFont->outputText(x,y,sp,n,vf,reversedFlg);

   if (reversedFlg) {
      that->setColor(ocolor);
   }
}

// outputText() -- output a text at current position
void Display::outputText(const char* sp, const int n, const bool vf) const
{
   if (currentFont == 0 || n <= 0) return;

   Display* that = (Display*)this;
   osg::Vec4 ocolor = getCurrentColor();
   // If manual reverse text, draw a background polygon
   if (reversedFlg) {
      // Offsets to center to polygon
      GLdouble cSpace = (currentFont->getCharacterSpacing());
      GLdouble lSpace = (currentFont->getLineSpacing());
      size_t len = strlen(sp);
      // are we vertical?
      if (vf) {
         GLdouble startX = cSpace / 2;
         GLdouble startY = 0;
         if (len > 1) startY = lSpace * (len / 2);
         else startY = lSpace * (len);

         startY -= (lSpace * 0.5);

         glPushMatrix();
         glTranslatef((GLfloat)startX, -(GLfloat)startY, 0);

         // now add a buffer for around the edges
         GLdouble deltaX = startX + (cSpace * 0.1);
         GLdouble deltaY = startY + (lSpace * 1.1);

         GLdouble vv[4][3] = {
            { -deltaX, -deltaY, -0.001 }, { -deltaX, deltaY, -0.001 }, { deltaX, deltaY, -0.001 }, { deltaX, -deltaY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }
      else {
         GLdouble cSpace = currentFont->getCharacterSpacing();
         GLdouble lSpace = currentFont->getLineSpacing();
         size_t len = strlen(sp);

         GLdouble startX = (cSpace * len / 2);
         GLdouble startY = (lSpace / 2);

         glPushMatrix();

         glTranslated(startX, startY, 0);

         GLdouble deltaX = startX + (cSpace * 0.1);
         GLdouble deltaY = startY + (lSpace * 0.1);

         GLdouble vv[4][3] = {
            { -deltaX, -deltaY, -0.001 }, { -deltaX, deltaY, -0.001 }, { deltaX, deltaY, -0.001 }, { deltaX, -deltaY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }
      that->setColor(getClearColor());
   }
   if (underlinedFlg) {
      GLdouble height = currentFont->getLineSpacing();
      GLdouble width = currentFont->getCharacterSpacing();
      size_t len = strlen(sp);

      if (vf) {
         // do nothing if we are vertical and underlined, because then it wouldn't be UNDERlined!
         //height /= 2;
         //myX = x - height;
         //myY = y - (width * len);
         //glBegin(GL_LINES);
         //    glVertex2f(myX, y + width);
         //    glVertex2f(myX, myY);
         //glEnd();
      }
      else {
         // only come down about a third for underlining
         height /= 2;
         that->setColor(ocolor);
         glPushMatrix();
         glBegin(GL_LINES);
         glVertex2d(0, -height);
         glVertex2d(width * len, -height);
         glEnd();
         glPopMatrix();
         if (reversedFlg) {
            that->setColor(getClearColor());
         }

      }

   }

   currentFont->outputText(sp,n,vf,reversedFlg);

   // Switch back to the original color
   if (reversedFlg) that->setColor(ocolor);
}


//------------------------------------------------------------------------------
// draw the brackets
//------------------------------------------------------------------------------
void Display::drawLeftBracket(const int ln, const int cp)
{
   // Select reversed video?
   bool oReversedFlg = reversedFlg;
   if (getReverseVideoBrackets() && !oReversedFlg) {
      // select reverse font
      selectFont(true, underlinedFlg);
   }

   // Draw the left bracket
   char cbuf[2];
   cbuf[0] = getLeftBracketCharacter();
   cbuf[1] = '\0';
   outputTextLC(ln, cp, cbuf, 1, reversedFlg);

   // Switch back to the old font
   if (getReverseVideoBrackets() && !oReversedFlg) {
      selectFont(oReversedFlg, underlinedFlg);
   }
}

void Display::drawRightBracket(const int ln, const int cp)
{
   // Select reversed video?
   bool oReversedFlg = reversedFlg;
   if (getReverseVideoBrackets() && !oReversedFlg) {
      // select reverse font
      selectFont(true, underlinedFlg);
   }

   // Draw the right bracket
   char cbuf[2];
   cbuf[0] = getRightBracketCharacter();
   cbuf[1] = '\0';
   outputTextLC(ln, cp, cbuf, 1, reversedFlg);

   // Switch back to the old font
   if (getReverseVideoBrackets() && !oReversedFlg) {
      selectFont(oReversedFlg, underlinedFlg);
   }
}


//------------------------------------------------------------------------------
// getColor() -- get color by index or color name
//------------------------------------------------------------------------------
Basic::Color* Display::getColor(const char* const colorName)
{
   Basic::Color* cc = 0;
   if (colorTable != 0) {
      Basic::Pair* p = colorTable->findByName(colorName);
      if (p != 0) cc = (Basic::Color*) p->object();
   }
   return cc;
}


Basic::Color* Display::getColor(const int index)
{
   Basic::Color* cc = 0;
   if (colorTable != 0) {
      Basic::Pair* p = colorTable->getPosition(index+1);
      if (p != 0) cc = (Basic::Color*) p->object();
   }
   return cc;
}

//-----------------------------------------------------------------------------
// addColor() -- add a color to the color table.
//-----------------------------------------------------------------------------
void Display::addColor(Basic::Color* cc)
{
   if (cc != 0 && colorTable != 0) {
      int i = colorTable->entries();
      char cbuf[20];
      std::sprintf(cbuf,"%i",i);
      colorTable->put( new Basic::Pair(cbuf, cc) );
   }
}

void Display::addColor(Basic::Pair* pp)
{
   if (pp != 0 && colorTable != 0) {
      Basic::Object* obj = pp->object();
      if (obj->isClassType(typeid(Basic::Color))) {
         colorTable->put( pp );
      }
   }
}

//-----------------------------------------------------------------------------
// defaultColors() -- loads a pre-refed() Pairstream with default colors.
//-----------------------------------------------------------------------------
Basic::PairStream* Display::defaultColors()
{
   // allocate our new colortable
   Basic::PairStream* defColorTable = new Basic::PairStream();

   // black
   {
      Basic::Rgba* color = new Basic::Rgba(0.0f, 0.0f, 0.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("black", color);
      defColorTable->put(pair);
      // now unref our local variables, because our pair ref()'d the Rgba object, and
      // PairStream ref()'d the pair.
      color->unref();
      pair->unref();
   }
   // red
   {
      Basic::Rgba* color = new Basic::Rgba(1.0f, 0.0f, 0.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("red", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // green
   {
      Basic::Rgba* color = new Basic::Rgba(0.0f, 1.0f, 0.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("green", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // yellow
   {
      Basic::Rgba* color = new Basic::Rgba(1.0f, 1.0f, 0.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("yellow", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // blue
   {
      Basic::Rgba* color = new Basic::Rgba(0.0f, 0.0f, 1.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("blue", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // magenta
   {
      Basic::Rgba* color = new Basic::Rgba(1.0f, 0.0f, 1.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("magenta", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // cyan
   {
      Basic::Rgba* color = new Basic::Rgba(0.0f, 1.0f, 1.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("cyan", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // white
   {
      Basic::Rgba* color = new Basic::Rgba(1.0f, 1.0f, 1.0f, 1.0f);
      Basic::Pair* pair = new Basic::Pair("white", color);
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }

   return defColorTable;
}


//------------------------------------------------------------------------------
// setName() -- set the display name
//------------------------------------------------------------------------------
bool Display::setName(Basic::String* const n)
{
   bool ok = false;
   if (n != 0) {
      name->unref();
      n->ref();
      name = n;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setLeftOrthoBound() -- set left orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotLeftOrthoBound(const Basic::Number* const sloobj)
{
   if (sloobj != 0) oLeft = sloobj->getDouble();
   return true;
}

//------------------------------------------------------------------------------
//  setSlotMaterials() -- set our list of materials
//------------------------------------------------------------------------------
bool Display::setSlotMaterials(Basic::PairStream* const msg)
{
   if (materials != 0) materials->unref();
   materials = 0;
   if (msg != 0) {
      materials = msg;
      materials->ref();
      processMaterials();
   }
   return true;
}
//------------------------------------------------------------------------------
// setSlotMaterials -- one material
//------------------------------------------------------------------------------
bool Display::setSlotMaterials(Material* const msg)
{
   bool ok = false;
   if (msg != 0) {
      if (materials != 0) materials->unref();

      materials = new Basic::PairStream();
      materials->put( new Basic::Pair("1",msg) );
      ok = processMaterials();
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotAntialias -- turn on/off anti aliasing
//------------------------------------------------------------------------------
bool Display::setSlotAntialias(const Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) ok = setAntialiasing(x->getBoolean());
   return ok;
}

//------------------------------------------------------------------------------
// setRightOrthoBound() -- set right orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotRightOrthoBound(const Basic::Number* const sroobj)
{
   if (sroobj != 0) oRight = sroobj->getDouble();
   return true;
}

//------------------------------------------------------------------------------
// setBottomOrthoBound() -- set bottom orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotBottomOrthoBound(const Basic::Number* const sbobobj)
{
   if (sbobobj != 0) oBottom = sbobobj->getDouble();  // set bottom ortho bound
   return true;
}

//------------------------------------------------------------------------------
// setTopOrthoBound() --  set top orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotTopOrthoBound(const Basic::Number* const stobobj)
{
   if (stobobj != 0) oTop = stobobj->getDouble();  // set top ortho bound
   return true;
}

//------------------------------------------------------------------------------
// setNearOrthoBound() -- set near orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotNearOrthoBound(const Basic::Number* const snobobj)
{
   if (snobobj != 0) oNear = snobobj->getDouble();
   return true;
}


//------------------------------------------------------------------------------
// setFarOrthoBound() -- set far orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotFarOrthoBound(const Basic::Number* const sfobobj)
{
   if (sfobobj != 0) oFar = sfobobj->getDouble();
   return true;
}

//------------------------------------------------------------------------------
// setViewportXOrigin() -- set viewport x origin
//------------------------------------------------------------------------------
bool Display::setSlotViewportXOrigin(const Basic::Number* const svxoobj)
{
   if (svxoobj != 0) vpX = svxoobj->getInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportYOrigin() -- set viewport y origin
//------------------------------------------------------------------------------
bool Display::setSlotViewportYOrigin(const Basic::Number* const svyoobj)
{
   if (svyoobj) vpY = svyoobj->getInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportWidth() -- set viewport width
//------------------------------------------------------------------------------
bool Display::setSlotViewportWidth(const Basic::Number* const svwobj)
{
   if (svwobj != 0) vpWidth = svwobj->getInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportHeight() -- set viewport height
//------------------------------------------------------------------------------
bool Display::setSlotViewportHeight(const Basic::Number* const svhobj)
{
   if (svhobj != 0) vpHeight = svhobj->getInt();
   return true;
}

//------------------------------------------------------------------------------
//  setSlotSubdisplayStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Display::setSlotSubdisplayStream (Basic::PairStream* const dsobj)
{
   bool ok = false;
   if (dsobj != 0) {
      // When a PairStream (i.e., more than one, a list) of displays
      if (subdisplays != 0) subdisplays->unref();
      subdisplays = dsobj;
      subdisplays->ref();

      ok = processSubdisplays();
   }
   return ok;
}

//------------------------------------------------------------------------------
//  setSlotSubdisplaySingle() -- it takes a display
//------------------------------------------------------------------------------
bool Display::setSlotSubdisplaySingle(Display* const dobj)
{
   bool ok = false;
   if (dobj != 0) {
      if (subdisplays != 0) subdisplays->unref();

      subdisplays = new Basic::PairStream();
      subdisplays->put( new Basic::Pair("1",dobj) );
      ok = processSubdisplays();
   }
   return ok;
}

//------------------------------------------------------------------------------
//  setSlotTexturesStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Display::setSlotTexturesStream (Basic::PairStream* const obj)
{
   bool ok = false;
   if (obj != 0) {
      // When a PairStream (i.e., more than one, a list) of displays
      if (textures != 0) textures->unref();
      textures = obj;
      textures->ref();

      ok = processTextures();
   }
   return ok;
}

//------------------------------------------------------------------------------
//  setSlotTexturesSingle() -- it takes a texture
//------------------------------------------------------------------------------
bool Display::setSlotTexturesSingle(Texture* const obj)
{
   bool ok = false;
   if (obj != 0) {
      if (textures != 0) textures->unref();

      textures = new Basic::PairStream();
      textures->put( new Basic::Pair("1",obj) );
      ok = processTextures();
   }
   return ok;
}

//------------------------------------------------------------------------------
//  setSlotStdLineWidth() -- sets the standard line width for the display
//------------------------------------------------------------------------------
bool Display::setSlotStdLineWidth(const Basic::Number* const obj)
{
   if (obj != 0) setStdLineWidth( obj->getFloat() );
   return true;
}

//------------------------------------------------------------------------------
// setSlotClearColor() -- sets the clear color slot
//------------------------------------------------------------------------------
bool Display::setSlotClearColor(const Basic::Color* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setClearColor(*msg);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotLeftBracketCharacter() -- sets the left bracket character
//------------------------------------------------------------------------------
bool Display::setSlotLeftBracketCharacter(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int num = msg->getInt();
      if (isalnum(num)) {
         setLeftBracketCharacter(char(num));
         ok = true;
      }
   }
   return ok;
}

bool Display::setSlotLeftBracketCharacter(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const char* str = *msg;
      if (str != 0) {
         setLeftBracketCharacter(str[0]);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotRightBracketCharacter() -- sets the right bracket character
//------------------------------------------------------------------------------
bool Display::setSlotRightBracketCharacter(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int num = msg->getInt();
      if (isalnum(num)) {
         setRightBracketCharacter(char(num));
         ok = true;
      }
   }
   return ok;
}

bool Display::setSlotRightBracketCharacter(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const char* str = *msg;
      if (str != 0) {
         setRightBracketCharacter(str[0]);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotReverseVideoBrackets() -- set reverse video brackets flag
//------------------------------------------------------------------------------
bool Display::setSlotReverseVideoBrackets(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setReverseVideoBrackets(msg->getBoolean());
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotClearDepth() -- sets the clear depth buffer slot
//------------------------------------------------------------------------------
bool Display::setSlotClearDepth(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setClearDepth(msg->getReal());
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotDisplayOrientation() -- sets display orientation slot
//                                 ( normal, cw90, ccw90, inverted }
//------------------------------------------------------------------------------
bool Display::setSlotDisplayOrientation(const Basic::String* const msg)
{
   bool ok = false;
   if (*msg == "normal") { setDisplayOrientation(NORMAL); ok = true; }
   else if (*msg == "cw90") { setDisplayOrientation(CW90); ok = true; }
   else if (*msg == "ccw90") { setDisplayOrientation(CCW90); ok = true; }
   else if (*msg == "inverted") { setDisplayOrientation(INVERTED); ok = true; }
   return ok;
}

//------------------------------------------------------------------------------
// processSubdisplays() -- process our subpages; make sure they are all of
//	type Display (or derived from it)and tell them that we are their
//	container.
//------------------------------------------------------------------------------
bool Display::processSubdisplays()
{
   bool ok = true;
   if (subdisplays != 0) {
      // Make sure we have only Pages and tell all of the pages
      // that we are their container.
      const Basic::List::Item* item = subdisplays->getFirstItem();
      while (ok && item != 0) {
         Basic::Pair* p = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Display* g = dynamic_cast<Display*>(p->object());
         if (g != 0) {
            // It MUST be of type Page
            g->container(this);
            g->setSubdisplayFlag(true);
         }
         else {
            // Delete components that are not of Page type
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processSubdisplays(): " << *p->slot() << " is not a Display!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// processTextures() -- process our textures; make sure they are all of
//	type Texture (or derived from it)and tell them to load.
//------------------------------------------------------------------------------
bool Display::processTextures()
{
   bool ok = true;
   if (textures != 0) {
      const Basic::List::Item* item = textures->getFirstItem();
      while (ok && item != 0) {
         Basic::Pair* p = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Texture* g = dynamic_cast<Texture*>(p->object());
         if (g == 0) {
            // It MUST be of type Texture
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processTextures(): " << *p->slot() << " is not a Texture!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// processMaterials() -- process our materials, makes sure all of our items
// are of type Material or derived from it.
//------------------------------------------------------------------------------
bool Display::processMaterials()
{
   bool ok = true;
   if (materials != 0) {
      const Basic::List::Item* item = materials->getFirstItem();
      while (ok && item != 0) {
         Basic::Pair* p = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Material* g = dynamic_cast<Material*>(p->object());
         if (g == 0) {
            // It MUST be of type Material
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processMaterials(): " << *p->slot() << " is not a Material!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}


//-----------------------------------------------------------------------------
// configureIt() -- function to configure the display
//-----------------------------------------------------------------------------
void Display::configure()
{
   // Set antialiasing ON
   if (isAntialiasing()) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
      glEnable(GL_ALPHA_TEST);
   }
   else {
      // Set antialiasing OFF
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
      glDisable(GL_BLEND);
      glBlendFunc(GL_ONE,  GL_ZERO);
      glDisable(GL_ALPHA_TEST);
   }

   // Depth checks
   if (getClearDepth() >= 0.0f) {
      glEnable(GL_DEPTH_TEST);
   }
   else {
      glDisable(GL_DEPTH_TEST);
   }

   // Disable other stuff
   glDisable(GL_DITHER);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);
   glDisable(GL_LOGIC_OP);
   glDisable(GL_STENCIL_TEST);
   glDisable(GL_TEXTURE_1D);
   glDisable(GL_TEXTURE_2D);

   // Default pixel transfer settings
   glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
   glPixelTransferi(GL_RED_SCALE, 1);
   glPixelTransferi(GL_RED_BIAS, 0);
   glPixelTransferi(GL_GREEN_SCALE, 1);
   glPixelTransferi(GL_GREEN_BIAS, 0);
   glPixelTransferi(GL_BLUE_SCALE, 1);
   glPixelTransferi(GL_BLUE_BIAS, 0);
   glPixelTransferi(GL_ALPHA_SCALE, 1);
   glPixelTransferi(GL_ALPHA_BIAS, 0);

#if GL_SGI_index_func
   glDisable(GL_INDEX_TEST_SGI);
#endif
}

//------------------------------------------------------------------------------
// loadTextures() -- This gets called when we want our list of textures
// to "load" themselves.  Not all textures load the same, from Bitmap textures
// to textures loaded by map software.  So we will leave it up to the texture
// itself to load itself appropriately.
//------------------------------------------------------------------------------
void Display::loadTextures()
{
   if (textures != 0) {
      const Basic::List::Item* item = textures->getFirstItem();
      while (item != 0) {
         Basic::Pair* p = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Texture* g = (Texture*)(p->object());
         g->loadTexture();
      }
   }
}


//------------------------------------------------------------------------------
// Read all pixels from the frame buffer and return buffer pointer
//------------------------------------------------------------------------------
Image* Display::readFrameBuffer(unsigned int* const w, unsigned int* const h)
{
   // Get Viewport width and height
   GLsizei vpWidth = 0;
   GLsizei vpHeight = 0;
   getViewportSize(&vpWidth, &vpHeight);
   unsigned int width = (unsigned int) vpWidth;
   unsigned int height = (unsigned int) vpHeight;

   Image* image = readFrameBuffer(0, 0, width, height);

   if (w != 0) *w = width;
   if (h != 0) *h = height;
   return image;
}

//------------------------------------------------------------------------------
// Get selected pixels from the frame buffer and return buffer pointer
//------------------------------------------------------------------------------
Image* Display::readFrameBuffer(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
{
   static const unsigned int MAX_IMAGE_WIDTH  = 2048; // maximum image width
   static const unsigned int MAX_IMAGE_HEIGHT = 2048; // maximum image height
   static const unsigned int PIXEL_SIZE = 3; // pixel size in bytes { GL_BGR_EXT }

   // check input parameters
   if ((width < 1) || (height < 1) || (width > MAX_IMAGE_WIDTH) || (height > MAX_IMAGE_HEIGHT)) {
      std::cout << "Display::readGlBuffer(): Invalid screenshot dimensions" << std::endl;
      return 0;
   }
   if ( (x<0) || (y < 0)) {
      std::cout << "Display::readGlBuffer(): Invalid screenshot location" << std::endl;
      return 0;
   }

   // align width to 4-byte boundary:
   unsigned int size = (width +3) / 4;
   unsigned int w = size * 4;

   GLubyte* pixelData = new GLubyte[PIXEL_SIZE * w * height];
   glReadPixels(x, y, w, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelData);

   Image* newImage = new Image(w, height, PIXEL_SIZE, GL_BGR_EXT, pixelData);

   return newImage;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
Basic::Object* Display::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Display::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   if (name != 0) {
      indent(sout,i+j);
      sout << "name:" << *name << std::endl;
   }

   // Orientation
   if (getDisplayOrientation() != NORMAL) {
      indent(sout,i+j);
      sout << "orientation: ";
      if (isDisplayOrientation(CW90))
         sout << "cw90";
      else if (isDisplayOrientation(CCW90))
         sout << "ccw90";
      else if (isDisplayOrientation(INVERTED))
         sout << "inverted";
      else
         sout << "normal";
      sout << std::endl;
   }

   indent(sout,i+j);
   sout << "left: " << oLeft << std::endl;

   indent(sout,i+j);
   sout << "right: " << oRight << std::endl;

   indent(sout,i+j);
   sout << "bottom: " << oBottom << std::endl;

   indent(sout,i+j);
   sout << "top: " << oTop << std::endl;

   indent(sout,i+j);
   sout << "near: " << oNear << std::endl;

   indent(sout,i+j);
   sout << "far: " << oFar << std::endl;

   indent(sout,i+j);
   sout << "vpX: " << vpX << std::endl;

   indent(sout,i+j);
   sout << "vpY: " << vpY << std::endl;

   indent(sout,i+j);
   sout << "vpWidth: " << vpWidth << std::endl;

   indent(sout,i+j);
   sout << "vpHeight: " << vpHeight << std::endl;

   indent(sout,i+j);
   sout << "colorTable: {" << std::endl;
   colorTable->serialize(sout,i+j+4,slotsOnly);
   indent(sout,i+j);
   sout << "}" << std::endl;

   if (getNormalFont() != 0) {
      indent(sout,i+j);
      sout << "normalFont:" << std::endl;
      indent(sout,i+j+4);
      getNormalFont()->serialize(sout,i+j+4);
   }

   if (subdisplays != 0) {
      indent(sout,i+j);
      sout << "displays: {" << std::endl;
      subdisplays->serialize(sout,i+j+4,slotsOnly);
      indent(sout,i+j);
      sout << "}" << std::endl;
   }

   if (textures != 0) {
      indent(sout,i+j);
      sout << "textures: {" << std::endl;
      textures->serialize(sout,i+j+4,slotsOnly);
      indent(sout,i+j);
      sout << "}" << std::endl;
   }

   indent(sout,i+j);
   sout << "leftBracketChar: " << getLeftBracketCharacter() << std::endl;

   indent(sout,i+j);
   sout << "rightBracketChar: " << getRightBracketCharacter() << std::endl;

   indent(sout,i+j);
   sout << "reverseVideoBrackets: " << getReverseVideoBrackets() << std::endl;


   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // End BasicGL namespace
} // End Eaagles namespace
