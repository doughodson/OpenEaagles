//----------------------------------------------------------------------------------------------------------------------------------
// Class: FtglFonts, FtglBitmapFont, FtglExtrdFont, FtglOutlineFont, FtglPixmapFont, FtglPolygonFont, FtglTextureFont
//----------------------------------------------------------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_FtglFont_H__
#define __Eaagles_BasicGL_FtglFont_H__

#include "Font.h"

//#define FTGL_2_1_2  // version 2.1.2 or earlier

#ifdef FTGL_2_1_2
class FTGLOutlineFont;
#else
#include <FTGL/ftgl.h>
#endif

namespace Eaagles {
namespace Basic { class Color; } 
namespace BasicGL {

//----------------------------------------------------------------------------------------------------------------------------------
// Class: FtglFonts, FtglBitmapFont, FtglExtrdFont, FtglOutlineFont, FtglPixmapFont, FtglPolygonFont, FtglTextureFont
// Base class:  Object -> Font -> FtglFonts -> (all other fonts are derived from FtglFonts)
// 
// Description: all the Freetype font classes 
//---------------------------------------------------------------------------------
// Class: FtglFonts
// Description:  Abstract font class for the Freetype2 fonts.  This uses freetype 2 
// and Open GL (FTGL) to draw the fonts.
//
// Form name: FTGLFonts
// Slots:
//  faceSize      <Number>    ! font face size (default: 1)
//
// public methods (member functions):
//
//  setFaceSize(Number* faceSize) - sets the face size in points (1/72 of an inch per point)
//      - this is different than setting character width and height, as it 
//      - expands and retracts the font proportionally.
//
//  unsigned int getFaceSize()
//  -- Gets the face size of the font.
// 
// All font files NEED the following info to work correctly:
//      - path to the font  "path"  ie.. "C:WINNT\FONTS"
//      - font name         "file"  ie.. "arial.ttf"
// all other slots are optional
//
// The fontWidth and fontHeight will SCALE the individual width and height, however
// faceSize will SET the actual font width and height symmetrically (ie.. X by X instead
// of X by Y).  You can both set the fontWidth and Height and faceSize, and that
// will scale and then draw a font of certain facesize. 
//
//---------------------------------------------------------------------------------
class FtglFonts : public Font
{
    DECLARE_SUBCLASS(FtglFonts,Font)

public:
    FtglFonts();  
    
    // Slot table Set Functions 
    virtual bool setFaceSize(const Basic::Number* const faceSize);
    
    // Get Functions
    unsigned int getFaceSize()      { return fSize; }; 
    
private:
    static const int DEFAULT_FACE_SIZE;
    int fSize;      // face size 
};

//------------------------------------------------------------------------------
// Class: FtglBitmapFont
//
// Description: Bitmap Font
// Form name: FTGLBitmapFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglBitmapFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglBitmapFont,FtglFonts)

public:
    FtglBitmapFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

//------------------------------------------------------------------------------
// Class: FtglExtrdFont
// Description: Creates a Freetype Extruded Font type
// Form name: FTGLExtrdFonts
// Slots:
//   depth         <Number>    ! Depth (default: 5.0f)
//
// public member functions:
//   setDepth(const Basic::Number* const newDepth) 
//   -- sets the depth of the extruded font
//
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglExtrdFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglExtrdFont,FtglFonts)

public:
    FtglExtrdFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
    
    // slot table functions
    bool setDepth(const Basic::Number* const newDepth);

private:
    static const float DEFAULT_DEPTH;
    float depth;        // depth of the extruded font (for 3D purposes)
};

//------------------------------------------------------------------------------
// Class:  FtglOutlineFont
//
// Description: Outlined Font
// Form name: FTGLOutlineFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglOutlineFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglOutlineFont,FtglFonts)

public:
    FtglOutlineFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

//------------------------------------------------------------------------------
// Class: FtglPixmapFont
//
// Description: Pixmapped Font
// Form name: FTGLPixmapFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglPixmapFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglPixmapFont,FtglFonts)

public:
    FtglPixmapFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

//------------------------------------------------------------------------------
// Class: FtglPolygonFont
//
// Description: Polygon Font
// Form name: FTGLPolygonFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglPolygonFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglPolygonFont,FtglFonts)

public:
    FtglPolygonFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

//------------------------------------------------------------------------------
// Class: FtglHaloFont
//
// Description: Polygon font that also draws a halo around the font
// Form name: FTGLHaloFonts
// Slots:
//   halocolor      <Color>    ! Font halo color (default = 0)
//   linewidth      <Number>   ! Font line width (default = 1)
//
// Public member functions:
//    outputText(double x, double y, char* txt, int n, bool vf)
//    outputText(double x, double y, char* txt, int n)
//       Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//    outputText(char* txt, int n, bool vf)
//    outputText(char* txt, int n)
//       Outputs n characters of txt at the current position. Vertically if vf == true.
//
// loadFont()
//    Loads the font.
//------------------------------------------------------------------------------
class FtglHaloFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglHaloFont,FtglFonts)

public:
    FtglHaloFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
    
    Basic::Color* getHaloColor()    { return haloColor; }
    float getHaloLineWidth()        { return linewidth; }
    FTGLOutlineFont* getOutline()    { return outline; }

protected:
    bool setHaloColor(Basic::Color* x);
    bool setLinewidth(const Basic::Number* const x);

private:
    FTGLOutlineFont* outline;
    Basic::Color* haloColor;
    float linewidth;
};

//------------------------------------------------------------------------------
// Class: FtglTextureFont
//
// Description: Texture Font
// Form name: FTGLTextureFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglTextureFont : public FtglFonts
{
    DECLARE_SUBCLASS(FtglTextureFont,FtglFonts)

public:
    FtglTextureFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
