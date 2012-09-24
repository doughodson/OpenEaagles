//------------------------------------------------------------------------------
// Class: Font
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Font_H__
#define __Eaagles_BasicGL_Font_H__

#include "openeaagles/basic/Object.h"
#include <GL/gl.h>

namespace Eaagles {
   
   namespace Basic {
      class Number;
      class List;
      class String;
   }

namespace BasicGL {

//------------------------------------------------------------------------------
// Class:  Font
// Base class:  Object -> Font
//
// Description:  Abstract font class
//
// Form name: Font
// Slots:
//      fontWidth      <Number> ! Font width (default: 1)
//      fontHeight     <Number> ! Font height (default: 1)
//      fontPosition   <List>   ! Upper/Left origin of the text field (default: 0, 0)
//      bitmapWidth    <Number> ! Width of bit map font (default: 1)
//      bitmapHeight   <Number> ! Height of bit map font (default: 1)
//      path           <String> ! Path to the font directory (default: 0)
//      file           <String> ! FTGL Font file name - will be altered in each instance of FTGLFont (default: 0)
//      lut            <List>   ! Lookup Table (default: 0)
//      characterSpacing <Number>  ! spacing for each character (default: 0)
//      lineSpacing      <Number>  ! spacing for each line (default: 0)
//
// public methods (member functions):
//
//      outputText(double x, double y, char* txt, int n)
//      outputText(double x, double y, char* txt, int n, bool vf)
//          Outputs the text at position (x, y).
//          When 'vf' is true, text is drawn vertically
//
//      outputText(char* txt, int n)
//      outputText(char* txt, int n, bool vf)
//          Outputs the text with the (ln, cp) pair (line, column). (Current Position)
//          When 'vf' is true, text is drawn vertically
//
//      position(const int ln, const int cp, GLdouble& px, GLdouble& py) const
//          Computes the (X, Y) position of the (ln, cp) pair (line, column).
//
//      loadFont()
//          Loads the font map.
//
//      bool isLoaded()
//          Returns true if the font is loaded, else false.
//
//      bool isNotLoaded()
//          Returns true if the font is not loaded, else false.
//
//      setTextOrigin(const GLdouble x, const GLdouble y)
//          Sets the upper left corner.
//
//	  LCreal getCharacterSpacing()
//		Returns the character spacing.
//
//	  setCharacterSpacing(LCreal v)
//		Sets the character spacing to v.
//
//	  LCreal getLineSpacing()
//		Returns the line spacing.
//
//	  setLineSpacing(LCreal v)
//		Sets the line spacing to v.
//
//      GLdouble getFontWidth()
//          Returns the font width.
//
//      setFontWidth(const GLdouble v)
//          Sets the font width.
//
//      GLdouble getFontHeight()
//          Returns the font height.
//
//      setFontHeight(const GLdouble v)
//          Sets the font height.
//
//      GLuint getBase()
//          Returns the base.
//
//      setBase(const GLuint nb)
//          Sets the base.
//
//      GLuint getBitmapWidth()
//          Returns the bitmap width.
//
//      setBitmapWidth(const GLuint v)
//          Sets the bitmap width.
//
//      GLuint getBitmapHeight()
//          Returns the bitmap height.
//
//      setBitmapHeight(const GLuint v)
//          Sets the the bitmap height.
//
//      const unsigned char* lut()
//          Returns pLUT (string pointer to the Look-Up-Table <character translations>).
//
//      bool isFTGL()
//          Returns true if the font is an OpenGL TrueType Font (FTGL), else false.
//
//      void* FTGL()
//          Returns a pointer to an OpenGL Freetype2 TrueType Font (FTGL).
//
//      const char* fontDirectory()
//          Returns a string pointer to the font path directory.
//
//      const char* filename()
//          Returns a string pointer to a font file name.
//
// Exceptions:
//      ExpInvalidFont:
//          Thrown by method outputText() when the font map is not valid and
//          couldn't be loaded.
//
//
// Protected functions:
//	  int xferChars(char* outp, size_t outpSize, char* inp, int n)
//		Transfers n characters of inp to outp and returns the size of outp.
//
//	  ftgl(void* p)
//		Sets the pFTGL font pointer to p.
//
//Comment section last updated: 2004.10.13 by MJK
//------------------------------------------------------------------------------
class Font : public Basic::Object {
    DECLARE_SUBCLASS(Font,Basic::Object)

public:
    static const size_t MAX_MESSAGE_LENGTH = 256; // Max length of character buffers 

public:
    Font();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) =0;
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) =0;
    virtual void position(const int ln, const int cp, GLdouble& px, GLdouble& py) const;
    
    virtual void setTextOrigin(const GLdouble x, const GLdouble y);

    LCreal getCharacterSpacing() const                  { return charSpacing; }    
    void setCharacterSpacing(const LCreal v)            { charSpacing = v; }

    LCreal getLineSpacing() const                       { return lineSpacing; }
    void setLineSpacing(const LCreal v)                 { lineSpacing = v; }

    GLdouble getFontWidth() const                       { return fWidth; }
    void setFontWidth(const GLdouble v)                 { fWidth = v; }
    
    GLdouble getFontHeight() const                      { return fHeight; }
    void setFontHeight(const GLdouble v)                { fHeight = v; }
    
    GLuint getBase() const                              { return b; }
    void setBase(const GLuint nb)                       { b = nb; }

    GLuint getBitmapWidth() const                       { return bWidth; }
    void setBitmapWidth(const GLuint v)                 { bWidth = v; }
    
    GLuint getBitmapHeight() const                      { return bHeight; }
    void setBitmapHeight(const GLuint v)                { bHeight = v; }

    virtual void loadFont() =0;
    bool isLoaded() const                               { return loaded; }
    bool isNotLoaded() const                            { return !loaded; }

    const unsigned char* lut() const                    { return pLUT; }

    bool isFTGL() const                                 { return (pFTGL != 0); }
    void* FTGL()                                        { return pFTGL; }

    const char* fontDirectory() const                   { return fontPath; }
    const char* filename() const                        { return fontFile; }
    
public: // Exceptions
    class ExpInvalidFont : public Basic::Object::Exception {
        public:
            ExpInvalidFont() : Exception() {}
            virtual const char* getDescription() const     { return "font is invalid"; }
    };

protected:
    // Slot functions
    bool setSlotFontWidth(const Basic::Number* const sfwobj);
    bool setSlotFontHeight (const Basic::Number* const sfhobj);
    bool setSlotFontPosition (const Basic::List* const sfpobj);
    bool setSlotBitmapWidth(const Basic::Number* const sbwobj);
    bool setSlotBitmapHeight(const Basic::Number* const sbhobj);
    bool setSlotFontPath(const Basic::String* const sfpobj);
    bool setSlotFTGLFontFileName(const Basic::String* const sgffnobj);
    bool setSlotLookupTable(const Basic::List* const sltobj);
    bool setSlotCharacterSpacing(const Basic::Number* const newCharSpacing);
    bool setSlotLineSpacing(const Basic::Number* const newLineSpacing);


    static const size_t MSG_BUF_LEN = (MAX_MESSAGE_LENGTH+1); // Max length of character buffers 
    int xferChars(char* const outp, const size_t BUF_SIZE, const char* const inp, const unsigned int n) const;

    GLdouble leftSide,topSide;          // Origin: upper left corner of ln=1, cp=1
    void ftgl(void* p)                                  { pFTGL = p; }
    void setFontLoaded()                                { loaded = true; }
    
private:
    static const int LUT_SIZE = 256;
    GLdouble  fWidth, fHeight;          // Font size
    GLuint    bWidth, bHeight;          // Bitmap size
    GLuint    b;                        // Font call list base
    unsigned char* pLUT;                // Look-Up-Table (character translations)
    void*     pFTGL;                    // OpenGL TrueType Font (FTGL)
    char*     fontPath;                 // Path to font directory (FTGL & Bitmap fonts)
    char*     fontFile;                 // Font filename (FTGL)
    bool      loaded;                   // Font has been loaded
    LCreal charSpacing;                 // holds our character spacing
    LCreal lineSpacing;                 // holds our line spacing
    
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
