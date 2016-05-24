#include "openeaagles/instruments/eadi3D/Eadi3DFont.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Eadi3DFont,"Eadi3DFont")
EMPTY_SLOTTABLE(Eadi3DFont)
EMPTY_SERIALIZER(Eadi3DFont)

// font scaled so six stroke font points (units used below) is 1 screen unit
#define XSCALE (1.0/6.0)
#define YSCALE (1.0/6.0)

// Default font sizes
const double defaultFontWidth = static_cast<double>(6.0f * XSCALE);
const double defaultFontHeight = static_cast<double>(7.0f * YSCALE);

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Eadi3DFont::Eadi3DFont()
{
    STANDARD_CONSTRUCTOR()

    setFontWidth( defaultFontWidth );
    setFontHeight( defaultFontHeight );
    setCharacterSpacing( defaultFontWidth );
    setLineSpacing( defaultFontHeight );
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(Eadi3DFont)

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Eadi3DFont)

//------------------------------------------------------------------------------
// outputText() -- Text output routines
//------------------------------------------------------------------------------
void Eadi3DFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
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

    // Set the base
    glListBase(getBase());

    // output the text
    glPushMatrix();
    glTranslated(x,y,0.0);
    glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
    if (vf) {
        // Vertical text
        char cc[2];
        cc[1] = '\0';
        GLdouble dy = getLineSpacing();
        if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
        for (int i = 0; i < nn; i++) {
            cc[0] = cbuf[i];
            glPushMatrix();
            glTranslated(0.0, -(dy * i), 0.0);
            glCallLists(1, GL_UNSIGNED_BYTE, cc);
            glPopMatrix();
        }
    }
    else {
        // Normal text
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
    glPopMatrix();
}

//------------------------------------------------------------------------------
// outputText() - text output, different parameters
//------------------------------------------------------------------------------
void Eadi3DFont::outputText(const char* txt, const int n, const bool vf, const bool)
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

    // Set the base
    glListBase(getBase());

    // output and scale the text
    glPushMatrix();
    glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
    if (vf) {
        // Vertical text
        char cc[2];
        cc[1] = '\0';
        GLdouble dy = getLineSpacing();
        if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
        for (int i = 0; i < nn; i++) {
            cc[0] = cbuf[i];
            glPushMatrix();
            glTranslated(0.0, -(dy * i), 0.0);
            glCallLists(1, GL_UNSIGNED_BYTE, cc);
            glPopMatrix();
        }
    }
    else {
        // Normal text
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
    glPopMatrix();
}

//------------------------------------------------------------------------------
// loadFont() --
//------------------------------------------------------------------------------
void Eadi3DFont::loadFont()
{
    if (isLoaded()) return;

    // create the stroke font
    setBase( glGenLists(256) );
    createEadi3DFont( getBase() );
    setFontLoaded();
}


//------------------------------------------------------------------------------
// Default stroke font ---
//------------------------------------------------------------------------------
enum {
    FONT_BEGIN = 1,
    FONT_NEXT,
    FONT_END,
    FONT_ADVANCE
};
#define MAX_STROKES 256
#define END_OF_LIST 256

// Displays so two (2) points on Y is our origin
#define XOFFSET 0
#define YOFFSET 0

static float strokeFont[][1+MAX_STROKES*3] = {
    {
        45,
        FONT_BEGIN, 0.000000f, 3.500000f,
        FONT_END, 4.000005f, 3.500000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        46,
        FONT_BEGIN, 1.500016f, 1.000011f,
        FONT_NEXT, 1.999984f, 1.999984f,
        FONT_NEXT, 2.499989f, 1.000011f,
        FONT_END, 1.500016f, 1.000011f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        47,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        48,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 0.000000f, 1.500016f,
        FONT_NEXT, 0.000000f, 5.499984f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_END, 0.500005f, 0.500005f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        49,
        FONT_BEGIN, 1.000011f, 0.000000f,
        FONT_END, 2.999995f, 0.000000f,
        FONT_BEGIN, 1.999984f, 0.000000f,
        FONT_NEXT, 1.999984f, 7.000000f,
        FONT_END, 1.000011f, 5.000016f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        50,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_END, 0.000000f, 5.499984f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        51,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 2.499989f,
        FONT_NEXT, 3.500000f, 3.500000f,
        FONT_NEXT, 2.499989f, 4.000005f,
        FONT_NEXT, 1.999984f, 4.000005f,
        FONT_NEXT, 4.000005f, 7.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        52,
        FONT_BEGIN, 3.500000f, 0.000000f,
        FONT_NEXT, 3.500000f, 7.000000f,
        FONT_NEXT, 2.499989f, 6.499995f,
        FONT_NEXT, 0.000000f, 2.499989f,
        FONT_END, 4.000005f, 2.499989f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        53,
        FONT_BEGIN, 0.000000f, 1.000011f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 2.999995f,
        FONT_NEXT, 2.999995f, 4.000005f,
        FONT_NEXT, 1.000011f, 4.000005f,
        FONT_NEXT, 0.000000f, 2.999995f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        54,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 1.999984f,
        FONT_NEXT, 3.500000f, 2.999995f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_NEXT, 1.500016f, 3.500000f,
        FONT_NEXT, 0.500005f, 2.999995f,
        FONT_END, 0.000000f, 1.999984f,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.000000f, 3.500000f,
        FONT_NEXT, 1.000011f, 5.999989f,
        FONT_END, 2.499989f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        55,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 4.000005f, 7.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        56,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 1.999984f,
        FONT_NEXT, 3.500000f, 2.999995f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_NEXT, 1.500016f, 3.500000f,
        FONT_NEXT, 0.500005f, 4.000005f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.000000f, 5.499984f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_END, 2.499989f, 3.500000f,
        FONT_BEGIN, 1.500016f, 3.500000f,
        FONT_NEXT, 0.500005f, 2.999995f,
        FONT_NEXT, 0.000000f, 1.999984f,
        FONT_NEXT, 0.000000f, 1.500016f,
        FONT_END, 0.500005f, 0.500005f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        57,
        FONT_BEGIN, 1.500016f, 0.000000f,
        FONT_NEXT, 2.999995f, 1.000011f,
        FONT_NEXT, 4.000005f, 3.500000f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 0.000000f, 5.499984f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.500005f, 4.000005f,
        FONT_NEXT, 1.500016f, 3.500000f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_END, 4.000005f, 5.000016f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        65,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 4.500011f,
        FONT_NEXT, 1.000011f, 6.499995f,
        FONT_NEXT, 1.999984f, 7.000000f,
        FONT_NEXT, 2.999995f, 6.499995f,
        FONT_NEXT, 4.000005f, 4.500011f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_BEGIN, 4.000005f, 3.500000f,
        FONT_END, 0.000000f, 3.500000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        66,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 2.999995f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 1.999984f,
        FONT_NEXT, 3.500000f, 2.999995f,
        FONT_NEXT, 2.999995f, 3.500000f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.999995f, 7.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 0.500005f, 7.000000f,
        FONT_END, 0.500005f, 0.000000f,
        FONT_BEGIN, 0.500005f, 3.500000f,
        FONT_END, 2.999995f, 3.500000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        67,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_END, 4.000005f, 1.500016f,
        FONT_BEGIN, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.000000f, 1.999984f,
        FONT_END, 0.500005f, 0.500005f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        68,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 0.500005f, 7.000000f,
        FONT_END, 0.500005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        69,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_BEGIN, 2.999995f, 3.500000f,
        FONT_END, 0.000000f, 3.500000f,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        70,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_BEGIN, 2.499989f, 4.000005f,
        FONT_END, 0.000000f, 4.000005f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        71,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 2.999995f,
        FONT_END, 1.999984f, 2.999995f,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 0.000000f, 1.999984f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_END, 4.000005f, 5.499984f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        72,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 0.000000f, 3.500000f,
        FONT_END, 4.000005f, 3.500000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        73,
        FONT_BEGIN, 1.000011f, 0.000000f,
        FONT_END, 2.999995f, 0.000000f,
        FONT_BEGIN, 1.999984f, 0.000000f,
        FONT_END, 1.999984f, 7.000000f,
        FONT_BEGIN, 1.000011f, 7.000000f,
        FONT_END, 2.999995f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        74,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.500005f,
        FONT_NEXT, 2.999995f, 1.500016f,
        FONT_END, 2.999995f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        75,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_NEXT, 0.500005f, 3.500000f,
        FONT_END, 0.000000f, 3.500000f,
        FONT_BEGIN, 0.500005f, 3.500000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        76,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        77,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_NEXT, 1.999984f, 4.000005f,
        FONT_NEXT, 4.000005f, 7.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        78,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_NEXT, 4.000005f, 0.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        79,
        FONT_BEGIN, 1.000011f, 0.000000f,
        FONT_NEXT, 2.999995f, 0.000000f,
        FONT_NEXT, 4.000005f, 1.999984f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 2.999995f, 7.000000f,
        FONT_NEXT, 1.000011f, 7.000000f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.000000f, 1.999984f,
        FONT_END, 1.000011f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        80,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_END, 0.000000f, 3.500000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        81,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 0.000000f, 5.499984f,
        FONT_END, 0.000000f, 1.500016f,
        FONT_BEGIN, 1.999984f, 1.999984f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        82,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 0.000000f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_NEXT, 4.000005f, 5.499984f,
        FONT_NEXT, 4.000005f, 5.000016f,
        FONT_NEXT, 3.500000f, 4.000005f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_END, 0.000000f, 3.500000f,
        FONT_BEGIN, 1.999984f, 3.500000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        83,
        FONT_BEGIN, 0.000000f, 1.500016f,
        FONT_NEXT, 0.500005f, 0.500005f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 4.000005f, 1.999984f,
        FONT_NEXT, 3.500000f, 2.999995f,
        FONT_NEXT, 2.499989f, 3.500000f,
        FONT_NEXT, 1.500016f, 3.500000f,
        FONT_NEXT, 0.500005f, 4.000005f,
        FONT_NEXT, 0.000000f, 5.000016f,
        FONT_NEXT, 0.000000f, 5.499984f,
        FONT_NEXT, 0.500005f, 6.499995f,
        FONT_NEXT, 1.500016f, 7.000000f,
        FONT_NEXT, 2.499989f, 7.000000f,
        FONT_NEXT, 3.500000f, 6.499995f,
        FONT_END, 4.000005f, 5.499984f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        84,
        FONT_BEGIN, 1.999984f, 0.000000f,
        FONT_END, 1.999984f, 7.000000f,
        FONT_BEGIN, 0.000000f, 7.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        85,
        FONT_BEGIN, 0.500005f, 0.500005f,
        FONT_NEXT, 0.000000f, 1.500016f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_NEXT, 4.000005f, 1.500016f,
        FONT_NEXT, 3.500000f, 0.500005f,
        FONT_NEXT, 2.499989f, 0.000000f,
        FONT_NEXT, 1.500016f, 0.000000f,
        FONT_END, 0.500005f, 0.500005f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        86,
        FONT_BEGIN, 1.999984f, 0.000000f,
        FONT_NEXT, 0.000000f, 4.500011f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_NEXT, 4.000005f, 4.500011f,
        FONT_END, 1.999984f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        87,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_NEXT, 4.000005f, 0.000000f,
        FONT_NEXT, 1.999984f, 3.500000f,
        FONT_END, 0.000000f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        88,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_END, 4.000005f, 7.000000f,
        FONT_BEGIN, 0.000000f, 7.000000f,
        FONT_END, 4.000005f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        89,
        FONT_BEGIN, 1.999984f, 0.000000f,
        FONT_NEXT, 1.999984f, 3.500000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 4.000005f, 7.000000f,
        FONT_END, 1.999984f, 3.500000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        90,
        FONT_BEGIN, 0.000000f, 0.000000f,
        FONT_NEXT, 4.000005f, 7.000000f,
        FONT_END, 0.000000f, 7.000000f,
        FONT_BEGIN, 0.500005f, 3.500000f,
        FONT_END, 3.500000f, 3.500000f,
        FONT_BEGIN, 4.000005f, 0.000000f,
        FONT_END, 0.000000f, 0.000000f,
        FONT_ADVANCE, 6.0f, 0.0f
    },

    {
        END_OF_LIST
    }
};


//------------------------------------------------------------------------------
// createEadi3DFont() - create our font
//------------------------------------------------------------------------------
GLenum Eadi3DFont::createEadi3DFont(GLuint fontBase)
{
    for (GLint i = 0; strokeFont[i][0] != END_OF_LIST; i++) {
        glNewList(fontBase+static_cast<unsigned int>(strokeFont[i][0]), GL_COMPILE);
        for (GLint j = 1; strokeFont[i][j]; j += 3) {
            GLint mode = (GLint) strokeFont[i][j];
            if (mode == FONT_BEGIN) {
                glBegin(GL_LINE_STRIP);
                glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                      static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
            }
            else if (mode == FONT_NEXT) {
                glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                      static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
            }
            else if (mode == FONT_END) {
                glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                      static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
                glEnd();
            }
            else if (mode == FONT_ADVANCE) {
                glTranslated(static_cast<double>(strokeFont[i][j+1])*XSCALE,
                      static_cast<double>(strokeFont[i][j+2])*YSCALE, 0.0);
                break;
            }
        }
        glEndList();
    }
    return GL_TRUE;
}

}
}

