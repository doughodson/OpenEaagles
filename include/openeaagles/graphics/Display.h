
#ifndef __oe_graphics_Display_H__
#define __oe_graphics_Display_H__

#include "Page.h"

namespace oe {

namespace base {
   class Color;
   class PairStream;
   class Identifier;
   class String;
}

namespace graphics {

class Font;
class Image;
class Texture;
class Material;

//------------------------------------------------------------------------------
// Class:  Display
//
// Description:  Display (MFD, CDU, HUD, etc) Manager;
//               provides a OpenGL canvas and manages the textures, fonts,
//               materials, colors, and other attributes use by the display.
//
// Notes:
//
// 1) Using the 'Orientation' enum and functions, the display can be rotated to
//    allow for the physical orientation of the simulator display devices.
//
// 2) The functions setViewport(), setViewportOrigin(), setViewportSize() and
//    setOrtho() all define the display's window or viewport, which usually
//    need to be set before the window is created.
//
// 3) On clear depth() - by default, this value is set to -1, which will
//    DISABLE the GL_DEPTH_TEST.  This in turn will cause all drawing to use the
//    painter's algorithm.  However, if you enable GL_DEPTH_TEST, the algorithm is
//    not adhered to, so the drawing order will appear "reversed".  So, if you are planning
//    on using the Z buffer to adjust graphical depth, be aware that you may need
//    to adjust your graphical positions according to the buffer, and not the order in
//    which they are drawn.
//
//
//
// Factory name: Display
// Slots:
//  name             <String>      ! Display name (default: " ")
//  colorTable       <PairStream>  ! Color table (default: 0)
//  normalFont       <Font>        ! Normal font; Font (default: 0)
//  normalFont       <Identifier>  ! Normal font; base::Identifier (default: 0)
//  left             <Number>      ! Left ortho bound (default: -0.5)
//  right            <Number>      ! Right ortho bound (default: 640.5)
//  bottom           <Number>      ! Bottom ortho bound (default: -0.5)
//  top              <Number>      ! Top ortho bound (default: 480.5)
//  near             <Number>      ! Near ortho bound (default: -1)
//  far              <Number>      ! Far ortho bound (default: 1)
//  vpX              <Number>      ! Viewport x origin (default: -1)
//  vpY              <Number>      ! Viewport y origin (default: -1)
//  vpWidth          <Number>      ! Viewport width (default: 300)
//  vpHeight         <Number>      ! Viewport height (default: 300)
//  displays         <PairStream>  ! Sub-displays, stream (default: 0)
//  displays         <Display>     ! Sub-displays, single (default: 0)
//  stdLineWidth     <Number>      ! Standard Line width (default: 1)
//  textures         <PairStream>  ! Texture, stream (default: 0)
//  textures         <Texture>     ! Texture, single (default: 0)
//  clearColor       <Color>       ! Clear (Background) color (default: 0.0f,0.0f,0.0f,0.0f)
//  leftBracketChar  <Number>      ! Left bracket character (default: '[')
//  leftBracketChar  <String>      ! Left bracket character (default: '[')
//  rightBracketChar <Number>      ! Right bracket character (default: ']')
//  rightBracketChar <String>      ! Right bracket character (default: ']')
//  reverseVideoBrackets  <Number> ! Reverse video brackets flag:
//                                 ! If true, brackets are drawn with reversed video font,
//                                 ! otherwise follow the field's drawing mode.  (default: false)
//  fonts             <PairStream> ! List of fonts (default: none)
//  clearDepth        <number>     ! clear depth; range: [ 0, 1 ] or negative for no depth buffer (default: -1.0)
//  orientation       <String>     ! display orientation { normal, cw90, ccw90, inverted } (default: normal)
//  materials         <Material>   ! List of material objects (default: 0)
//  antiAliasing      <Number>     ! Turn on/off anti-aliasing (default: true)
//
// Exceptions:
//      ExpInvalidDisplayPtr
//          Thrown by display() when displayPtr == null(0)
//          (the display screen pointer is not set).
//
//----------------------------------------------------------------------------
class Display : public Page
{
    DECLARE_SUBCLASS(Display,Page)

public:
   enum Orientation { NORMAL, CW90, CCW90, INVERTED };

public:
   Display();

   bool isMainDisplay() const;                  // Is this a main window
   bool isSubdisplay() const;                   // Is this a subwindow

   const char* getName() const;                 // Returns the display's (window) name
   bool setName(base::String* const n);        // Sets the display's name

   virtual void select();                       // Selects this display.

   bool isAntialiasing() const;                 // Is anti-aliasing enabled?
   bool setAntialiasing(const bool on);         // Set anti-aliasing enabled flag

   Orientation getDisplayOrientation() const;   // Returns the orientation of the display
   bool isDisplayOrientation(const Orientation o) const; // Is this our display orientation?
   void setDisplayOrientation(const Orientation o); // Sets the display orientation

   virtual void clear();                        // Clear the display buffer (and depth buffer if 'clearDepth' is set)
   virtual void swapBuffers();                  // Swaps the front and back buffers (Window system specific)
   virtual void show();                         // show our display (Window system specific)
   virtual void hide();                         // hide our display (Window system specific)

   virtual void drawIt();                       // Selects, configures and draws then swaps buffers
   virtual void reshapeIt(int w, int h);        // Resizes the displays's width and height

   const osg::Vec4& getClearColor() const;      // Returns the clear (background) color as a Vec4 vector (RGBA).
   void setClearColor(const base::Color& ccolor); // Set the display's clear (background) color

   GLclampd getClearDepth() const;              // Returns the value that the depth buffer is cleared to.
   void setClearDepth(const GLclampd depth);    //  Sets the value that the depth buffer is cleared to (see notes)

   virtual void loadTextures();                 // Load the texture table
   const base::PairStream* getTextures() const; // Returns a ptr to the list of textures

   // ---
   // Global (display) graphic parameters
   // ---

   GLfloat getLinewidth() const;                // Returns the current line width
   GLfloat setLinewidth(const GLfloat);         // Sets and returns the current line width

   GLfloat getStdLineWidth() const;             // Returns the standard line width
   void setStdLineWidth(const GLfloat);         // Sets the standard and current line widths

   const osg::Vec4& getCurrentColor() const;    // Returns the current color RGBA vector
   void setColor(const osg::Vec4& color);       // Sets the current color by an RGBA vector.
   void setColor(const char* cname1);           // Sets the current color by name (color table)

   base::Color* getColor(const char* const name); // Returns a color by name from the color table
   base::Color* getColor(const int idx);       // Returns a color by index from the color table

   bool setColorTable(base::PairStream* const list); // Sets the color table to this list of colors
   void addColor(base::Color*);                // Adds a color to the color table
   void addColor(base::Pair*);                 // Adds a color to the color table

   base::PairStream* defaultColors();          // Generates a list of default colors; returns a pre-ref'ed() ptr
                                                //  -- black, red, green, yellow, blue, magenta, cyan, and white.

   // Returns the Material object by material table name
   Material* getMaterial(const base::Identifier* name);

   // Returns the texture ID by texture table name.
   GLuint getTextureByName(const base::Identifier* name);


   // ---
   // Viewports and projections for the display's main viewport (or window)
   // and are usually set before the window is created.
   // ---

   // Gets the viewport  parameters.
   void getViewport(GLint* x, GLint* y, GLsizei* vpWidth, GLsizei* vpHeight) const;

   // Sets the viewport parameters.
   virtual void setViewport(const GLint x, const GLint y, const GLsizei vpWidth, const GLsizei vpHeight);

   // Gets the viewport's origin.
   void getViewportOrigin(GLsizei* x, GLsizei* y) const;

   // Sets the viewport's origin.
   virtual void setViewportOrigin(const GLsizei x, const GLsizei y);

   // Gets the viewport's size.
   void getViewportSize(GLsizei* vpWidth, GLsizei* vpHeight) const;

   // Sets the viewport's size.
   virtual void setViewportSize(const GLsizei vpWidth, const GLsizei vpHeight);

   // Returns the orthogonal parameters
   virtual void getOrtho(GLdouble &left, GLdouble &right, GLdouble &bottom, GLdouble &top, GLdouble &near, GLdouble &far) const;

   // Sets the orthogonal parameters.
   virtual void setOrtho(
         GLdouble oLeft, GLdouble oRight,
         GLdouble oBottom, GLdouble oTop,
         GLdouble oNear = 0.0,
         GLdouble oFar = 10.0
      );

   // Sets the orthogonal parameters and resizes the viewport using orthogonals.
   virtual void forceOrtho(
         GLdouble oLeft, GLdouble oRight,
         GLdouble oBottom, GLdouble oTop,
         GLdouble oNear = 0.0,
         GLdouble oFar = 10.0
      );


   // ---
   // Scissor and subscreen areas
   // ---

   // Sets up a scissor box and enables the scissor test
   virtual void setScissor(const GLdouble left, const GLdouble right, const GLdouble bottom, const GLdouble top);

   // Clears the scissor box and disable scissor test.
   virtual void clearScissor();

   // Sets up a temporary sub-viewport; the parameters are relative
   // to the current display's orthogonal projection.
   virtual void setSubscreen(const GLdouble left, const GLdouble right, const GLdouble bottom, const GLdouble top);

   // Clears the temporary sub-viewport
   virtual void clearSubscreen();


   // ---
   // Mouse, keyboard and event functions
   // ---

   // Gets the current mouse position.
   void getMouse(int* const x, int* const y) const;

   // Sets the current mouse position.
   virtual void setMouse(const int x, const int y, Display* const subdisplay = 0);

   // Returns the current 'focus'
   virtual Graphic* focus() const;

   // Sets and returns the current 'focus'
   virtual Graphic* focus(Graphic* s);

   // Keyboard event handlers -- passes keyboard events to the focus
   virtual void keyboardEvent(const int key);

   // Mouse event handler -- passes mouse events to the focus
   virtual void mouseEvent(const int button, const int state, const int x, const int y);

   // Button event handler -- passes button events to the focus
   virtual void buttonEvent(const int b);

   // Event handlers for when the mouse enters our display
   virtual void onMouseEnter();

   // Event handlers for when the mouse exits our display
   virtual void onMouseExit();


   // ---
   // Text and font functions
   // ---

   Font* getFont(const char* const name);                // Returns a font by name.
   const Font* getFont(const char* const name) const;    // const version

   Font* getFont(const base::Identifier* const name);   // Returns a font by name (using an Identifier)
   const Font* getFont(const base::Identifier* const name) const; // const version

   Font* getFont(const int index);              // Returns a font by its font table index.
   const Font* getFont(const int index) const;  // const version

   Font* getNormalFont();                       // Returns the normal text font
   const Font* getNormalFont() const;           // const version

   bool isDefaultFont() const;                  // Are we using the default font?
   bool isFontReversed() const;                 // Is the reversed video font selected?
   bool isFontUnderlined() const;               // Is the underline font selected?

   // Sets the normal text font
   bool setNormalFont(Font* const f);
   bool setNormalFont(const char* const fontName);
   bool setNormalFont(const base::Identifier* const fontName);

   // Sets the current font) based on the font mode flags.
   void selectFont(const bool reversed, const bool underlined, Font* newFont = 0);

   Font* getCurrentFont();                      // Returns a pointer to the current font
   void setFont(Font* newFont);                 // Sets the current font.

   const base::Color* getNormColor() const;          // Returns the normal text color
   void setNormColor(const base::Color* const nc);   // Sets the normal text color

   const base::Color* getHighlightColor() const;        // Returns the highlighted text color
   void setHighlightColor(const base::Color* const nc); // Sets the highlighted text color

   void drawLeftBracket(const int ln, const int cp);  // Draws the left bracket at ln, cp
   void drawRightBracket(const int ln, const int cp); // Draws the right bracket at ln, cp

   char getLeftBracketCharacter() const;        // Returns the left bracket character
   bool setLeftBracketCharacter(const char c);  // Sets the left bracket character

   char getRightBracketCharacter() const;       // Returns the right bracket character
   bool setRightBracketCharacter(const char c); // Sets the right bracket character

   bool getReverseVideoBrackets() const;        // Returns the reversed video bracket flag
   bool setReverseVideoBrackets(const bool f);  // Sets the reversed video bracket flag

   // Using current font, outputs 'n' characters of 'text' starting at
   // line, ln, and column, cp.  When 'vf' is true, text is drawn vertically
   void outputTextLC(const int ln, const int cp, const char* txt, const int n, const bool vf = false) const;

   // Using current font, output 'n' characters of 'text'  starting at
   // the current position.  When 'vf' is true, text is drawn vertically
   void outputText(const char* txt, const int n, const bool vf = false) const;

   // ---
   // Screen snapshot functions
   // ---

   // Returns the pixels to the frame buffer.
   // (currently GL_BGR_EXT format only with 3 bytes per pixel)
   virtual Image* readFrameBuffer(unsigned int* const width=0, unsigned int* const height=0);
   virtual Image* readFrameBuffer(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);

   // ---
   // Slot functions
   // ---
   bool setSlotLeftOrthoBound(const base::Number* const sloobj);
   bool setSlotRightOrthoBound(const base::Number* const sroobj);
   bool setSlotBottomOrthoBound(const base::Number* const sbobobj);
   bool setSlotTopOrthoBound(const base::Number* const stobobj);
   bool setSlotNearOrthoBound(const base::Number* const snobobj);
   bool setSlotFarOrthoBound(const base::Number* const sfobobj);
   bool setSlotViewportXOrigin(const base::Number* const svxoobj);
   bool setSlotViewportYOrigin(const base::Number* const svyoobj);
   bool setSlotViewportWidth(const base::Number* const svwobj);
   bool setSlotViewportHeight(const base::Number* const svhobj);
   bool setSlotSubdisplayStream(base::PairStream* const dsobj);
   bool setSlotSubdisplaySingle(Display* const dobj);
   bool setSlotStdLineWidth(const base::Number* const svhobj);
   bool setSlotTexturesStream(base::PairStream* const obj);
   bool setSlotTexturesSingle(Texture* const obj);
   bool setSlotClearColor(const base::Color* const msg);
   bool setSlotLeftBracketCharacter(const base::Number* const svhob);
   bool setSlotLeftBracketCharacter(const base::String* const svhob);
   bool setSlotRightBracketCharacter(const base::Number* const svhob);
   bool setSlotRightBracketCharacter(const base::String* const svhob);
   bool setSlotReverseVideoBrackets(const base::Number* const svhobj);
   bool setFontList(base::PairStream* const dsobj);
   bool setSlotClearDepth(const base::Number* const msg);
   bool setSlotDisplayOrientation(const base::String* const msg);
   bool setSlotMaterials(base::PairStream* const msg);
   bool setSlotMaterials(Material* const msg);
   bool setSlotAntialias(const base::Number* const msg);

   // sync double buffer swapping flag
   // (just in case we don't want to swap buffers every time, we can wait)
   virtual bool isOkToSwap() const;
   virtual void setOkToSwap(const bool x);

   void updateTC(const double dt = 0.0) override;
   void reset() override;

protected:
   // Configures the display's GL modes
   virtual void configure();

   base::PairStream* getTextures();
   base::PairStream* subDisplays();
   void setSubdisplayFlag(const bool flg);

private:
    void initData();

    bool processSubdisplays();
    bool processTextures();
    bool processMaterials();

    base::String* name;               // Display Name
    base::PairStream* subdisplays;    // Sub-displays

    Graphic* focusPtr;                 // Input focus
    base::PairStream* materials;      // list of material objects

    base::PairStream* textures;       // List of textures

    GLsizei  vpX, vpY, vpWidth, vpHeight;                   // viewport size
    GLdouble oLeft, oRight, oBottom, oTop, oNear, oFar;     // Ortho parameters

    GLfloat linewidth;                 // Current Linewidth (last glLineWidth())
    GLfloat stdLinewidth;              // Standard linewidth

    bool subdisplayFlg;                // We're a sub-display
    bool antialias;                    // Anti-alias flag  (default off)
    int mx, my;                        // Mouse x, y

    Orientation orientation;           // Display orientation

    GLclampd  clearDepth;              // Display clear depth
    base::PairStream* colorTable;     // Color table
    osg::Vec4 color;                   // Current Color
    osg::Vec4 clearColor;              // Clear (background) color
    base::Identifier* colorName;      // Current color name
    const base::Color* normColor;     // Color of a normal text field
    const base::Color* hiColor;       // Color of a high lighted text field.

    base::PairStream* fontList;       // List of fonts
    Font* currentFont;                 // Current font
    Font* normalFont;                  // Normal font
    base::Identifier* normalFontName; // Normal font name
    bool    reversedFlg;               // Current font setting
    bool    underlinedFlg;             // Current font setting

    bool    rvBrackets;                 // Reverse video brackets flag
    char    leftBracketChar;            // Left bracket character
    char    rightBracketChar;           // right bracket character

    bool okToSwap;                     // just in case we don't want to swap buffers every time, we can wait.
};

//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------

inline const char* Display::getName() const                 { return *name; }
inline bool Display::isMainDisplay() const                  { return !subdisplayFlg; }
inline bool Display::isSubdisplay() const                   { return subdisplayFlg; }
inline Display::Orientation Display::getDisplayOrientation() const   { return orientation; }
inline bool Display::isDisplayOrientation(const Orientation o) const { return (o == getDisplayOrientation()); }
inline bool Display::isAntialiasing() const                 { return antialias; }
inline const osg::Vec4& Display::getClearColor() const      { return clearColor; }
inline GLclampd Display::getClearDepth() const              { return clearDepth; }
inline void Display::setClearDepth(const GLclampd depth)    { clearDepth = depth; }

inline void Display::setDisplayOrientation(const Orientation o) { orientation = o; }

inline GLfloat Display::getLinewidth() const                { return linewidth; }
inline GLfloat Display::getStdLineWidth() const             { return stdLinewidth; }
inline const osg::Vec4& Display::getCurrentColor() const    { return color; }

inline void Display::getMouse(int* const x, int* const y) const { *x = mx; *y = my; }

inline Font* Display::getCurrentFont()                      { return currentFont; }
inline bool Display::isFontReversed() const                 { return reversedFlg; }
inline bool Display::isFontUnderlined() const               { return underlinedFlg; }
inline bool Display::isDefaultFont() const                  { return currentFont == 0; }
inline const base::Color* Display::getNormColor() const    { return normColor; }
inline const base::Color* Display::getHighlightColor() const { return hiColor; }
inline char Display::getLeftBracketCharacter() const        { return leftBracketChar; }
inline bool Display::setLeftBracketCharacter(const char c)  { leftBracketChar = c; return true; }
inline char Display::getRightBracketCharacter() const       { return rightBracketChar; }
inline bool Display::setRightBracketCharacter(const char c) { rightBracketChar = c; return true; }
inline bool Display::getReverseVideoBrackets() const        { return rvBrackets; }
inline bool Display::setReverseVideoBrackets(const bool f)  { rvBrackets = f; return true; }

inline base::PairStream* Display::getTextures()            { return textures; }
inline const base::PairStream* Display::getTextures() const { return textures; }

inline base::PairStream* Display::subDisplays()            { return subdisplays; }
inline void Display::setSubdisplayFlag(const bool flg)      { subdisplayFlg = flg; }
inline bool Display::isOkToSwap() const                     { return okToSwap; }
inline void Display::setOkToSwap(const bool x)              { okToSwap = x; }

} // End graphics namespace
} // End oe namespace

#endif
