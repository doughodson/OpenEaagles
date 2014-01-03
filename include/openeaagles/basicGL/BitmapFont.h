//------------------------------------------------------------------------------
// Class: BitmapFont
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_BitmapFont_H__
#define __Eaagles_BasicGL_BitmapFont_H__

#include "Font.h"

namespace Eaagles {

namespace Basic { class Number; }

namespace BasicGL {

//------------------------------------------------------------------------------
// Class:  BitmapFont
// Base class:  Object -> Font -> BitmapFont
//
// Description:  Class to create and load bit map fonts
//
// Factory name: BitmapFont
// Slots:
//   reverse   <Number>    ! Reverse video (default: false)
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//      (See font.h for descriptions of those functions.)
//
//   outputText(double x, double y, char* txt, int n, bool vf)
//   -- Output n characters of txt at (x,y). Output vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   -- Output n characters of txt at the current position. Output vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//
//   setReverse(Number* rnumber)
//   -- Sets the font to reverse if rnumber != 0.
//
//------------------------------------------------------------------------------
class BitmapFont : public Font {
    DECLARE_SUBCLASS(BitmapFont,Font)

public:
    BitmapFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
    virtual bool setReverse(const Basic::Number* const rnumber);

private:
    // Loader support functions
    GLubyte* loadTypeFace(const GLint index, const GLenum reverse);  
    static void reverseBitmapOrder(GLubyte* bitmap, unsigned int numBitmapBytes, unsigned int numBytesWide);
    static GLubyte reverseByteOrder(GLubyte byte);
    
    bool reverse;                   // Reverse the font
    const char** fontMap;           // Font map (ASCII code to file name mapping)
    unsigned int numFonts;          // Number of fonts in the map
    
    // Default fontMap
    static const char** defaultFontMap;
    static const unsigned int defaultNumFonts;
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
