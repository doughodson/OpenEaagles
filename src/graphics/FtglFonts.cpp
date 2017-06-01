#include "openeaagles/graphics/FtglFonts.hpp"
#include "openeaagles/graphics/Graphic.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Color.hpp"
#include "openeaagles/base/util/str_utils.hpp"

#include <cstdlib>

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

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// static class members
//------------------------------------------------------------------------------
const int FtglFonts::DEFAULT_FACE_SIZE = 1;        // one unit by unit (in this case, inches)
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
    ON_SLOT(1,setFaceSize,base::Number)
END_SLOT_MAP()
BEGIN_SLOT_MAP(FtglExtrdFont)
    ON_SLOT(1, setDepth, base::Number)
END_SLOT_MAP()
BEGIN_SLOT_MAP(FtglHaloFont)
    ON_SLOT(1, setHaloColor, base::Color)
    ON_SLOT(2, setLinewidth, base::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s) - FtglFonts
//------------------------------------------------------------------------------
FtglFonts::FtglFonts()
{
    STANDARD_CONSTRUCTOR()
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
    return nullptr;
}
FtglBitmapFont::FtglBitmapFont()
{
    STANDARD_CONSTRUCTOR();
}
FtglExtrdFont::FtglExtrdFont()
{
    STANDARD_CONSTRUCTOR();
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
}

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
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    linewidth = org.linewidth;
}

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
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    if (outline != nullptr) delete outline;
    outline = nullptr;
}

//------------------------------------------------------------------------------
// slot table functions FtglFonts and FtglExtrdFonts
//------------------------------------------------------------------------------
bool FtglFonts::setFaceSize(const base::Number* const faceSize)
{
    // get our new face size, else we return to a default face size
    if (faceSize != nullptr) fSize = faceSize->getInt();
    else fSize = DEFAULT_FACE_SIZE;
    return true;
}

bool FtglExtrdFont::setDepth(const base::Number* const newDepth)
{
    // set our depth
    if (newDepth != nullptr) depth = newDepth->getFloat();
    else depth = DEFAULT_DEPTH;
    return true;
}

bool FtglHaloFont::setHaloColor(base::Color* x)
{
    // set our halo color
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    if (x != nullptr) {
        haloColor = x;
        haloColor->ref();
    }
    return true;
}
bool FtglHaloFont::setLinewidth(const base::Number* const x )
{
    bool ok = false;
    if (x != nullptr) {
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
    const auto ftgl1 = static_cast<FTGLBitmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( x, (y - static_cast<float>(i)*getLineSpacing()) );
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
    const auto ftgl1 = static_cast<FTGLBitmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            GLdouble rpos[4];
            glGetDoublev(GL_CURRENT_RASTER_POSITION, rpos);
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( rpos[0], (rpos[1] - static_cast<float>(i)*getLineSpacing()) );
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

    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLBitmapFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
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
    const auto ftgl1 = static_cast<FTGLExtrdFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLExtrdFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLExtrdFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
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
    const auto ftgl1 = static_cast<FTGLOutlineFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLOutlineFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLOutlineFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
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
    const auto ftgl1 = static_cast<FTGLPixmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( x, (y - static_cast<float>(i)*getLineSpacing()) );
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
    const auto ftgl1 = static_cast<FTGLPixmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2];
            cc[1] = '\0';
            GLdouble rpos[4];
            glGetDoublev(GL_CURRENT_RASTER_POSITION, rpos);
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( rpos[0], (rpos[1] - static_cast<float>(i)*getLineSpacing()) );
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

    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLPixmapFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
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
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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

    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLPolygonFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
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

    if (haloColor != nullptr) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue());
    // default to black if we have no color specified
    else glColor3f(0,0,0);

    // now render the outline font over it!
    if (!rf) {
        if (outline != nullptr) {
            glPushMatrix();
            glTranslated(x, y, -0.01f);
            // get our current linewidth
            GLfloat lw = 0;
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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

    if (haloColor != nullptr) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue());
    // default to black if we have no color specified
    else glColor3f(0,0,0);

    if (!rf) {
        // Output the outline in black
        if (outline != nullptr) {
            glPushMatrix();
            // get our current linewidth
            GLfloat lw = 0;
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glTranslatef(0,0,-0.01f);
            glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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

    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLPolygonFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
    }

    // now create the outline font over it
    outline = new FTGLOutlineFont(fontPathname);
    if (outline != nullptr && !outline->Error()) {
        // set the face size
        outline->FaceSize(getFaceSize());
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglHaloFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
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
    const auto ftgl1 = static_cast<FTGLTextureFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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
    const auto ftgl1 = static_cast<FTGLTextureFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
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

    if( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const size_t FONTPATHNAME_LENGTH = 256;
    char fontPathname[FONTPATHNAME_LENGTH];
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLTextureFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
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
        std::exit(1);
    }
}

}
}

