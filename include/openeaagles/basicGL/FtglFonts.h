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
// Class:  FtglFonts, FtglBitmapFont, FtglExtrdFont, FtglOutlineFont, FtglPixmapFont, FtglPolygonFont, FtglTextureFont
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
//	  -Gets the face size of the font.
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
//Comment sections last updated: 2004.10.14 by MJK 
//---------------------------------------------------------------------------------
class FtglFonts : public Font {
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

#endif



#ifndef __FtglBitmapFont_96B63687_1D77_4bad_B038_12C99B145F08
#define __FtglBitmapFont_96B63687_1D77_4bad_B038_12C99B145F08
//------------------------------------------------------------------------------
// Class:	FtglBitmapFont
//
// Description: Bitmap Font
// Form name: FTGLBitmapFonts
//
// Public member functions:
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//	outputText(char* Txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglBitmapFont : public FtglFonts {
    DECLARE_SUBCLASS(FtglBitmapFont,FtglFonts)

public:
    FtglBitmapFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

#endif


#ifndef __FtglExtrdFont_CCD52653_E5A8_4811_8BD4_C397A6B46F0B
#define __FtglExtrdFont_CCD52653_E5A8_4811_8BD4_C397A6B46F0B
//------------------------------------------------------------------------------
// Class:	FtglExtrdFont
// Description: Creates a Freetype Extruded Font type
// Form name: FTGLExtrdFonts
// Slots:
//   depth         <Number>    ! Depth (default: 5.0f)
//
// public member functions:
// 	setDepth(const Basic::Number* const newDepth) 
//        sets the depth of the extruded font
//
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//	outputText(char* Txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglExtrdFont : public FtglFonts {
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
    float depth;        // depth of the extruded font (for 3D puposes)
};

#endif      /* __FtglExtrdFont_CCD52653_E5A8_4811_8BD4_C397A6B46F0B */


#ifndef __FtglOutlineFont_906E6C23_2189_4cba_821E_DBFF19341B69
#define __FtglOutlineFont_906E6C23_2189_4cba_821E_DBFF19341B69
//------------------------------------------------------------------------------
// Class:	FtglOutlineFont
//
// Description: Outlined Font
// Form name: FTGLOutlineFonts
//
// Public member functions:
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//	outputText(char* Txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglOutlineFont : public FtglFonts {
    DECLARE_SUBCLASS(FtglOutlineFont,FtglFonts)

public:
    FtglOutlineFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

#endif      /* __FtglOutlineFont_906E6C23_2189_4cba_821E_DBFF19341B69 */



#ifndef __FtglPixmapFont_BD694E43_E34E_408b_A0BE_82A8F5C61680
#define __FtglPixmapFont_BD694E43_E34E_408b_A0BE_82A8F5C61680
//------------------------------------------------------------------------------
// Class:	FtglPixmapFont
//
// Description: Pixmapped Font
// Form name: FTGLPixmapFonts
//
// Public member functions:
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//	outputText(char* Txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglPixmapFont : public FtglFonts {
    DECLARE_SUBCLASS(FtglPixmapFont,FtglFonts)

public:
    FtglPixmapFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

#endif      /* __FtglPixmapFont_BD694E43_E34E_408b_A0BE_82A8F5C61680 */



#ifndef __FtglPolygonFont_D19D09FA_6B53_44d7_AFC2_6CEFF67C9BC7
#define __FtglPolygonFont_D19D09FA_6B53_44d7_AFC2_6CEFF67C9BC7
//------------------------------------------------------------------------------
// Class:	FtglPolygonFont
//
// Description: Polygon Font
// Form name: FTGLPolygonFonts
//
// Public member functions:
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//	outputText(char* Txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglPolygonFont : public FtglFonts {
    DECLARE_SUBCLASS(FtglPolygonFont,FtglFonts)

public:
    FtglPolygonFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};

#endif      /* __FtglPolygonFont_D19D09FA_6B53_44d7_AFC2_6CEFF67C9BC7 */


#ifndef __FtglHaloFont_CF6B6560_5E86_4ef6_8E3C_A5FFECEAA227
#define __FtglHaloFont_CF6B6560_5E86_4ef6_8E3C_A5FFECEAA227
//------------------------------------------------------------------------------
// Class:	FtglHaloFont
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

class FtglHaloFont : public FtglFonts {
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

#endif      /* __FtglHaloFont_CF6B6560_5E86_4ef6_8E3C_A5FFECEAA227 */

#ifndef __FtglTextureFont_4789D5E6_5916_4539_89B4_636EC9DBB1F0
#define __FtglTextureFont_4789D5E6_5916_4539_89B4_636EC9DBB1F0
//------------------------------------------------------------------------------
// Class:	FtglTextureFont
//
// Description: Texture Font
// Form name: FTGLTextureFonts
//
// Public member functions:
//	outputText(double x, double y, char* txt, int n, bool vf)
//	outputText(double x, double y, char* txt, int n)
//		Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//	outputText(char* txt, int n, bool vf)
//    outputText(char* txt, int n)
//		Outputs n characters of txt at the current position. Vertically if vf == true.
//
//	loadFont()
//		Loads the font.
//------------------------------------------------------------------------------
class FtglTextureFont : public FtglFonts {
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
