#include "openeaagles/basicGL/FtglFonts.h"
#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Color.h"

#ifdef FTGL_2_1_2
#include <FTGL/FTFace.h>
#include <FTGL/FTFont.h>
#include <FTGL/FTGLBitmapFont.h>
#include <FTGL/FTGLExtrdFont.h>
#include <FTGL/FTGLOutlineFont.h>
#include <FTGL/FTGLPixmapFont.h>
#include <FTGL/FTGLPolygonFont.h>
#include <FTGL/FTGLTextureFont.h>
#endif

namespace Eaagles {
namespace BasicGL {


//------------------------------------------------------------------------------
// static class members
//------------------------------------------------------------------------------
const int FtglFonts::DEFAULT_FACE_SIZE = 1;     // one unit by unit (in this case, inches)
const float FtglExtrdFont::DEFAULT_DEPTH = 5.0f;

//------------------------------------------------------------------------------
// Setting up classes
//------------------------------------------------------------------------------
IMPLEMENT_PARTIAL_SUBCLASS(FtglFonts,"FTGLFonts")
EMPTY_SERIALIZER(FtglFonts)
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FtglBitmapFont,"FTGLBitmapFonts")
EMPTY_SERIALIZER(FtglBitmapFont)
IMPLEMENT_SUBCLASS(FtglExtrdFont,"FTGLExtrdFonts")
EMPTY_SERIALIZER(FtglExtrdFont)
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FtglOutlineFont,"FTGLOutlineFonts")
EMPTY_SERIALIZER(FtglOutlineFont)
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FtglPixmapFont,"FTGLPixmapFonts")
EMPTY_SERIALIZER(FtglPixmapFont)
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FtglPolygonFont,"FTGLPolygonFonts")
EMPTY_SERIALIZER(FtglPolygonFont)
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FtglTextureFont,"FTGLTextureFonts")
EMPTY_SERIALIZER(FtglTextureFont)
IMPLEMENT_SUBCLASS(FtglHaloFont,"FTGLHaloFonts")
EMPTY_SERIALIZER(FtglHaloFont)


//------------------------------------------------------------------------------
// Slot tables 
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(FtglFonts)
    "faceSize",
END_SLOTTABLE(FtglFonts)
BEGIN_SLOTTABLE(FtglExtrdFont)
    "depth",
END_SLOTTABLE(FtglExtrdFont)
BEGIN_SLOTTABLE(FtglHaloFont)
    "halocolor",
    "linewidth",
END_SLOTTABLE(FtglHaloFont)

//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(FtglFonts)
    ON_SLOT(1,setFaceSize,Basic::Number)
END_SLOT_MAP()
BEGIN_SLOT_MAP(FtglExtrdFont)
    ON_SLOT(1, setDepth, Basic::Number)
END_SLOT_MAP()
BEGIN_SLOT_MAP(FtglHaloFont)
    ON_SLOT(1, setHaloColor, Basic::Color)
    ON_SLOT(2, setLinewidth, Basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s) - FtglFonts
//------------------------------------------------------------------------------
FtglFonts::FtglFonts()
{
    STANDARD_CONSTRUCTOR() 
    fSize = DEFAULT_FACE_SIZE;     // default face size
}
FtglFonts::FtglFonts(const FtglFonts& org)
{ 
    STANDARD_CONSTRUCTOR()
    fSize = org.fSize;
    copyData(org,true);
}
FtglFonts::~FtglFonts()
{
   STANDARD_DESTRUCTOR()
}
FtglFonts& FtglFonts::operator=(const FtglFonts& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}
FtglFonts* FtglFonts::clone() const
{
    return 0;
}
FtglBitmapFont::FtglBitmapFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglExtrdFont::FtglExtrdFont()
{
    STANDARD_CONSTRUCTOR();
    depth = DEFAULT_DEPTH;
}
FtglOutlineFont::FtglOutlineFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglPixmapFont::FtglPixmapFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglPolygonFont::FtglPolygonFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglTextureFont::FtglTextureFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglHaloFont::FtglHaloFont()
{
    STANDARD_CONSTRUCTOR();
    outline = 0;
    haloColor = 0;
    linewidth = 1;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for FtglFonts and FtglExtrdFonts
//------------------------------------------------------------------------------
Basic::Object* FtglFonts::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}
Basic::Object* FtglExtrdFont::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}
Basic::Object* FtglHaloFont::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void FtglFonts::copyData(const FtglFonts& org, const bool)
{
    BaseClass::copyData(org);
    fSize = org.fSize;
}

void FtglBitmapFont::copyData(const FtglBitmapFont& org, const bool)
{
    BaseClass::copyData(org);
}

void FtglExtrdFont::copyData(const FtglExtrdFont& org, const bool)
{
    BaseClass::copyData(org);
    depth = org.depth;
}

void FtglOutlineFont::copyData(const FtglOutlineFont& org, const bool)
{
    BaseClass::copyData(org);
}

void FtglPixmapFont::copyData(const FtglPixmapFont& org, const bool)
{
    BaseClass::copyData(org);
}

void FtglPolygonFont::copyData(const FtglPolygonFont& org, const bool)
{
    BaseClass::copyData(org);
}

void FtglTextureFont::copyData(const FtglTextureFont& org, const bool)
{
    BaseClass::copyData(org);
}

void FtglHaloFont::copyData(const FtglHaloFont& org, const bool)
{
    BaseClass::copyData(org);
    if (haloColor != 0) haloColor->unref();
    haloColor = 0;
    linewidth = org.linewidth;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void FtglFonts::deleteData()
{
}

void FtglBitmapFont::deleteData()
{
}

void FtglExtrdFont::deleteData()
{
}

void FtglOutlineFont::deleteData()
{
}

void FtglPixmapFont::deleteData()
{
}

void FtglPolygonFont::deleteData()
{
}

void FtglTextureFont::deleteData()
{
}

void FtglHaloFont::deleteData()
{
    if (haloColor != 0) haloColor->unref();
    haloColor = 0;
    if (outline != 0) delete outline;
    outline = 0;
}


//------------------------------------------------------------------------------
// slot table functions FtglFonts and FtglExtrdFonts
//------------------------------------------------------------------------------
bool FtglFonts::setFaceSize(const Basic::Number* const faceSize)
{
    // get our new face size, else we return to a default face size
    if (faceSize != 0) fSize = faceSize->getInt();
    else fSize = DEFAULT_FACE_SIZE;
    return true;
};

bool FtglExtrdFont::setDepth(const Basic::Number* const newDepth)
{
    // set our depth
    if (newDepth != 0) depth = newDepth->getFloat();
    else depth = DEFAULT_DEPTH;
    return true;
}

bool FtglHaloFont::setHaloColor(Basic::Color* x)
{
    // set our halo color
    if (haloColor != 0) haloColor->unref();
    haloColor = 0;
    if (x != 0) {
        haloColor = x;
        haloColor->ref();
    }
    return true;
}
bool FtglHaloFont::setLinewidth(const Basic::Number* const x )
{
    bool ok = false;
    if (x != 0) {
        linewidth = x->getFloat();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// BitmapFonts functions
// FtglBitmapFont::outputText() -- FTGLBitmapFont output routines
//------------------------------------------------------------------------------
void FtglBitmapFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // output the text 
    FTGLBitmapFont* ftgl1 = (FTGLBitmapFont*)FTGL();   
    if (ftgl1 != 0) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( x, (y - float(i)*getLineSpacing()) );
                ftgl1->Render(cc);    
            }
        }
        else {
            // Normal text
            glRasterPos2d(x,y);
            ftgl1->Render(cbuf);    
        }
    }
    else if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglBitmapFont::outputText() - no Bitmap font available" << std::endl;
    }

}

void FtglBitmapFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // output the text
    FTGLBitmapFont* ftgl1 = (FTGLBitmapFont*)FTGL();   
    if (ftgl1 != 0) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            GLdouble rpos[4];
            glGetDoublev(GL_CURRENT_RASTER_POSITION, rpos);
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( rpos[0], (rpos[1] - float(i)*getLineSpacing()) );
                ftgl1->Render(cc);    
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglBitmapFont::outputText() - no Bitmap font available" << std::endl;
          }
    }
}

// Load FtglBitmapFont
void FtglBitmapFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    
    if( filename() == 0 ) {
       if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
       }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLBitmapFont* ftglFont = new FTGLBitmapFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());  
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglBitmapFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}

//------------------------------------------------------------------------------
// Extruded font functions
// FtglExtrdFont::outputText() -- FTGL output routines
//------------------------------------------------------------------------------
void FtglExtrdFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{    
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // Output the text
    FTGLExtrdFont* ftgl1 = (FTGLExtrdFont*)FTGL();
    if (ftgl1 != 0) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglExtrdFont::outputText() - no extrude font available" << std::endl;
          }
    }
}

void FtglExtrdFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // Output the text
    FTGLExtrdFont* ftgl1 = (FTGLExtrdFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }   
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FTGLExtrdFont::outputText() - no extruded font available" << std::endl;
          }
    }
}
    
// Load FtglExtrdFont
void FtglExtrdFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters  
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLExtrdFont* ftglFont = new FTGLExtrdFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());  
        ftgl(ftglFont);
        // we'll put this here for now
        ftglFont->Depth(10);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglExtrdFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}

//------------------------------------------------------------------------------
// Outline Font functions
// FtglOutlineFont::outputText() -- FTGLOutlineFont output routines
//------------------------------------------------------------------------------
void FtglOutlineFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // Output the text
    FTGLOutlineFont* ftgl1 = (FTGLOutlineFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    } 
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglOutlineFont::outputText() - no outline font available" << std::endl;
          }
    }
}

void FtglOutlineFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // Output the text
    FTGLOutlineFont* ftgl1 = (FTGLOutlineFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    } 
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglOutlineFont::outputText() - no outline font available" << std::endl;
          }
    }
}

// Load FtglOutlineFont
void FtglOutlineFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLOutlineFont* ftglFont = new FTGLOutlineFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglOutlineFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}

//------------------------------------------------------------------------------
// Pixmap Font functions
// FtglPixmapFont::outputText() -- FTGLPixmapFont output routines
//------------------------------------------------------------------------------
void FtglPixmapFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // output the text 
    FTGLPixmapFont* ftgl1 = (FTGLPixmapFont*)FTGL();   
    if (ftgl1 != 0) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( x, (y - float(i)*getLineSpacing()) );
                ftgl1->Render(cc);    
            }
        }
        else {
            // Normal text
            glRasterPos2d(x,y);
            ftgl1->Render(cbuf);    
        }
    }  
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FTGLPixmapFont::outputText() - no Pixmap font available" << std::endl;
          }
    }
}

void FtglPixmapFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // output the text
    FTGLPixmapFont* ftgl1 = (FTGLPixmapFont*)FTGL();   
    if (ftgl1 != 0) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            GLdouble rpos[4];
            glGetDoublev(GL_CURRENT_RASTER_POSITION, rpos);
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( rpos[0], (rpos[1] - float(i)*getLineSpacing()) );
                ftgl1->Render(cc);    
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglPixmapFont::outputText() - no pixmap font available" << std::endl;
          }
    }
}

// Load FtglPixmapFont
void FtglPixmapFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLPixmapFont* ftglFont = new FTGLPixmapFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());  
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglPixmapFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}


//------------------------------------------------------------------------------
// Polygon Font functions
// FtglPolygonFont::outputText() -- FTGLPolygonFont output routines
//------------------------------------------------------------------------------
void FtglPolygonFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // output the text
    FTGLPolygonFont* ftgl1 = (FTGLPolygonFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }   
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglPolygonFont::outputText() - no Polygon font available" << std::endl;
          }
    }
}

void FtglPolygonFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // output the text
    FTGLPolygonFont* ftgl1 = (FTGLPolygonFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }   
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglPolygonFont::outputText() - no Polygon font available" << std::endl;
          }
    }
}

// Load FtglPolygonFont
void FtglPolygonFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLPolygonFont* ftglFont = new FTGLPolygonFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());  
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglPolygonFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}

//------------------------------------------------------------------------------
// Halo Font functions
// FtglHaloFont::outputText() -- FtglHaloFont output routines
//------------------------------------------------------------------------------
void FtglHaloFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool rf)
{
    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);        

    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    if (haloColor != 0) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue()); 
    // default to black if we have no color specified
    else glColor3f(0,0,0);

    // now render the outline font over it! 
    if (!rf) {
        if (outline != 0) {
            glPushMatrix();
            glTranslated(x, y, -0.01f);
            // get our current linewidth
            GLfloat lw = 0;
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
            if (vf) {
                // Vertical text
                GLdouble dy = getLineSpacing();
                if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
                char cc[2];
                cc[1] = '\0';
                for (int i = 0; i < nn; i++) {
                    cc[0] = cbuf[i];
                    outline->Render(cc);    
                    glTranslated(0.0, -dy, 0.0);
                }
            }
            else {
                // Normal text
                outline->Render(cbuf);    
            }
            glLineWidth(lw);
            glPopMatrix();
        } 
        else {
              if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "FtglHaloFont::outputText() - no outline font available" << std::endl;
              }
        }
    }

    glColor4fv(ocolor);

    // output the text
    FTGLPolygonFont* ftgl1 = (FTGLPolygonFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }   
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglPolygonFont::outputText() - no Polygon font available" << std::endl;
          }
    }
}

void FtglHaloFont::outputText(const char* txt, const int n, const bool vf, const bool rf)
{

    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);        

    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }
    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    if (haloColor != 0) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue()); 
    // default to black if we have no color specified
    else glColor3f(0,0,0);
    
    if (!rf) {
        // Output the outline in black
        if (outline != 0) {
            glPushMatrix();
            // get our current linewidth
            GLfloat lw = 0;
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glTranslatef(0,0,-0.01f);
            glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
            if (vf) {
                // Vertical text
                GLdouble dy = getLineSpacing();
                if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
                char cc[2];
                cc[1] = '\0';
                for (int i = 0; i < nn; i++) {
                    cc[0] = cbuf[i];
                    outline->Render(cc);    
                    glTranslated(0.0, -dy, 0.0);
                }
            }
            else {
                outline->Render(cbuf);    
            }
            glLineWidth(lw);
            glPopMatrix();
        } 
        else {
              if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "FtglHaloFont::outputText() - no outline font available" << std::endl;
              }
        }
    }

    glColor4fv(ocolor);

    // output the text
    FTGLPolygonFont* ftgl1 = (FTGLPolygonFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }   
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglHaloFont::outputText() - no Polygon font available" << std::endl;
          }
    }
}

// Load FtglPolygonFont
void FtglHaloFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLPolygonFont* ftglFont = new FTGLPolygonFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());  
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglHaloFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }

    // now create the outline font over it
    outline = new FTGLOutlineFont(fontPathname);  
    if (outline != 0 && !outline->Error()) {
        // set the face size 
        outline->FaceSize(getFaceSize());
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglHaloFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }

}


//------------------------------------------------------------------------------
// Texture Font functions
// FtglTextureFont::outputText() -- FTGLTextureFont output routines
//------------------------------------------------------------------------------
void FtglTextureFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }
    
    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // output the text
    FTGLTextureFont* ftgl1 = (FTGLTextureFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    } 
    else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglTextureFont::outputText() - no Texture font available" << std::endl;
          }
    }
}

void FtglTextureFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }
    
    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;
    
    // output the text
    FTGLTextureFont* ftgl1 = (FTGLTextureFont*)FTGL();   
    if (ftgl1 != 0) {
        glPushMatrix();
        glScalef(GLfloat(getFontWidth()), GLfloat(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy = getLineSpacing();
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);    
                glTranslated(0.0, -dy, 0.0);
            }
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);    
        }
        glPopMatrix();
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "FtglTextureFont::outputText() - no Texture font available" << std::endl;
        }
    }
}

// Load FtglTextureFont
void FtglTextureFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    
    if( filename() == 0 ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }
    
    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    lcStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());
    
    FTGLTextureFont* ftglFont = new FTGLTextureFont(fontPathname);  
    if (ftglFont != 0 && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglTextureFont::loadFont() - font did not load correctly: file: \"";
        std::cerr << fontPathname << "\"";
        std::cerr << std::endl;
          }
        exit(1);
    }
}

} // End BasicGL namespace
} // End Eaagles namespace
