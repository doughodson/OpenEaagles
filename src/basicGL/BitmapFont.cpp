//------------------------------------------------------------------------------
// Bitmap Font
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/BitmapFont.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Number.h"
#include <cstdio>
#include <cmath>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(BitmapFont,"BitmapFont")
EMPTY_DELETEDATA(BitmapFont)

// Default font size
const unsigned int defaultFontWidth = 10;
const unsigned int defaultFontHeight = 15;

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(BitmapFont)
    "reverse",      // Invert the bitmap's bits (reverse video)
END_SLOTTABLE(BitmapFont)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(BitmapFont)
    ON_SLOT(1, setReverse, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
BitmapFont::BitmapFont()
{
    STANDARD_CONSTRUCTOR()

    fontMap = defaultFontMap;
    numFonts = defaultNumFonts;
    reverse = false;

    setFontWidth( defaultFontWidth );
    setFontHeight( defaultFontHeight );

    setBitmapWidth( defaultFontWidth );
    setBitmapHeight( defaultFontHeight );
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void BitmapFont::copyData(const BitmapFont& org, const bool)
{
    BaseClass::copyData(org);
    fontMap = org.fontMap;
    numFonts = org.numFonts;
    reverse = org.reverse;
}

//------------------------------------------------------------------------------
// outputText() -- Text output routines
//------------------------------------------------------------------------------
void BitmapFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
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
    if (vf) {
        // Vertical text
        for (int i = 0; i < nn; i++) {
            glRasterPos2d( x, (y - float(i)*getLineSpacing()) );
            glCallLists(1, GL_UNSIGNED_BYTE, &cbuf[i]);
        }
    }
    else {
        // Normal text
        glRasterPos2d(x,y);
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
}

void BitmapFont::outputText(const char* txt, const int n, const bool vf, const bool)
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
    if (vf) {
        // Vertical text
        GLdouble rpos[4] = { 0, 0, 0, 0 };
        for (int i = 0; i < nn; i++) {
            glRasterPos2d( rpos[0], (rpos[1] - float(i)*getLineSpacing()) );
            glCallLists(1, GL_UNSIGNED_BYTE, &cbuf[i]);
        }
    }
    else {
        glRasterPos2d(0,0);
        // Normal text
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
}


//------------------------------------------------------------------------------
// Font loader -- loads the font
//------------------------------------------------------------------------------
void BitmapFont::loadFont()
{
    if (isLoaded())
        return;

    setBase( glGenLists(256) );

    // Loop through the font map
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned int i=0; i < numFonts; i++)
    {
        GLubyte* bitmap = loadTypeFace(i, reverse);
        if (bitmap == 0) continue;

        GLfloat xmove = static_cast<GLfloat>(getBitmapWidth());
        GLfloat ymove = 0.0;

        glNewList(getBase()+i, GL_COMPILE);
        glBitmap(getBitmapWidth(), getBitmapHeight(), 0.0, 0.0, xmove, ymove, bitmap);
        glEndList();

        delete[] bitmap;
    }
    setFontLoaded();
}

//------------------------------------------------------------------------------
// setReverse() -- sets text in reverse type
//------------------------------------------------------------------------------
bool BitmapFont::setReverse(const Basic::Number* const rnumber)
{
    if (rnumber != nullptr)
        reverse = rnumber->getBoolean();
    return true;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
Basic::Object* BitmapFont::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& BitmapFont::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    unsigned int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "reverse: " << reverse << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


//------------------------------------------------------------------------------
// Default font ----
//------------------------------------------------------------------------------

// Default Font map: straight from the ASCII table given by; 'man ascii'.  It allows
// the ASCII character to map to a font file.
static const char* defaultMap[] =
{
   0,                   // null
   0,                   // soh
   0,                   // stx
   0,                   // etx
   0,                   // eot
   0,                   // enq
   0,                   // ack
   0,                   // bell
   0,                   // bs
   0,                   // ht
   0,                   // nl
   0,                   // vt
   0,                   // np
   0,                   // cr
   0,                   // so
   0,                   // si
   0,                   // dle
   0,                   // dc1
   0,                   // dc2
   0,                   // dc3
   0,                   // dc4
   0,                   // nak
   0,                   // syn
   0,                   // etb
   0,                   // can
   0,                   // em
   0,                   // sub
   0,                   // esc
   0,                   // fs
   0,                   // gs
   0,                   // rs
   0,                   // us
   "space.fon",         // ' '
   "exclamation.fon",   // '!'
   "doublequote.fon",   // '"'
   "poundsign.fon",     // '#'
   "dollarsign.fon",    // '$'
   "percent.fon",       // '%'
   "andsign.fon",       // '&'
   "rightquote.fon",    // '''
   "leftparen.fon",     // '('
   "rightparen.fon",    // ')'
   "star.fon",          // '*'
   "plussign.fon",      // '+'

   "comma.fon",         // ','
   "minussign.fon",     // '-'
   "period.fon",        // '.'
   "forwardslash.fon",  // '/'
   "0.fon",             // '0'
   "1.fon",             // '1'
   "2.fon",             // '2'
   "3.fon",             // '3'
   "4.fon",             // '4'
   "5.fon",             // '5'
   "6.fon",             // '6'
   "7.fon",             // '7'
   "8.fon",             // '8'
   "9.fon",             // '9'
   "colon.fon",         // ':'
   "semicolon.fon",     // ';'
   "leftcaret.fon",     // '<'
   "equalsign.fon",     // '='
   "rightcaret.fon",    // '>'
   "questionmark.fon",  // '?'
   "atsign.fon",        // '@'
   "u_a.fon",           // 'A'
   "u_b.fon",           // 'B'
   "u_c.fon",           // 'C'
   "u_d.fon",           // 'D'
   "u_e.fon",           // 'E'
   "u_f.fon",           // 'F'
   "u_g.fon",           // 'G'
   "u_h.fon",           // 'H'
   "u_i.fon",           // 'I'
   "u_j.fon",           // 'J'
   "u_k.fon",           // 'K'
   "u_l.fon",           // 'L'
   "u_m.fon",           // 'M'
   "u_n.fon",           // 'N'
   "u_o.fon",           // 'O'
   "u_p.fon",           // 'P'
   "u_q.fon",           // 'Q'
   "u_r.fon",           // 'R'
   "u_s.fon",           // 'S'
   "u_t.fon",           // 'T'
   "u_u.fon",           // 'U'
   "u_v.fon",           // 'V'
   "u_w.fon",           // 'W'
   "u_x.fon",           // 'X'
   "u_y.fon",           // 'Y'
   "u_z.fon",           // 'Z'
   "leftbracket.fon",   // '['
   "backslash.fon",     // '\'
   "rightbracket.fon",  // ']'
   "caret.fon",         // '^'
   "underline.fon",     // '_'
   "leftquote.fon",     // '`'
   "l_a.fon",           // 'a'
   "l_b.fon",           // 'b'
   "l_c.fon",           // 'c'
   "l_d.fon",           // 'd'
   "l_e.fon",           // 'e'
   "l_f.fon",           // 'f'
   "l_g.fon",           // 'g'
   "l_h.fon",           // 'h'
   "l_i.fon",           // 'i'
   "l_j.fon",           // 'j'
   "l_k.fon",           // 'k'
   "l_l.fon",           // 'l'
   "l_m.fon",           // 'm'
   "l_n.fon",           // 'n'
   "l_o.fon",           // 'o'
   "l_p.fon",           // 'p'
   "l_q.fon",           // 'q'
   "l_r.fon",           // 'r'
   "l_s.fon",           // 's'
   "l_t.fon",           // 't'
   "l_u.fon",           // 'u'
   "l_v.fon",           // 'v'
   "l_w.fon",           // 'w'
   "l_x.fon",           // 'x'
   "l_y.fon",           // 'y'
   "l_z.fon",           // 'z'
   "leftcurly.fon",     // '{'
   "verticalbar.fon",   // '|'
   "rightcurly.fon",    // '}'
   "tilda.fon",         // '~'
   0,                   // del
   0,                   // 0x80
   0,                   // 0x81
   0,                   // 0x82
   0,                   // 0x83
   0,                   // 0x84
   0,                   // 0x85
   0,                   // 0x86
   0,                   // 0x87
   0,                   // 0x88
   0,                   // 0x89
   0,                   // 0x8A
   0,                   // 0x8B
   0,                   // 0x8C
   0,                   // 0x8D
   0,                   // 0x8E
   0,                   // 0x8F
   0,                   // 0x90
   0,                   // 0x91
   0,                   // 0x92
   0,                   // 0x93
   0,                   // 0x94
   0,                   // 0x95
   0,                   // 0x96
   0,                   // 0x97
   0,                   // 0x98
   0,                   // 0x99
   0,                   // 0x9A
   0,                   // 0x9B
   0,                   // 0x9C
   0,                   // 0x9D
   0,                   // 0x9E
   0,                   // 0x9F
   0,                   // 0xA0
   0,                   // 0xA1
   0,                   // 0xA2
   0,                   // 0xA3
   0,                   // 0xA4
   0,                   // 0xA5
   0,                   // 0xA6
   0,                   // 0xA7
   0,                   // 0xA8
   0,                   // 0xA9
   0,                   // 0xAA
   0,                   // 0xAB
   0,                   // 0xAC
   0,                   // 0xAD
   0,                   // 0xAE
   0,                   // 0xAF
   "degreesymbol.fon",  // 0xB0
   0,                   // 0xB1
   0,                   // 0xB2
   0,                   // 0xB3
   0,                   // 0xB4
   0,                   // 0xB5
   0,                   // 0xB6
   0,                   // 0xB7
   0,                   // 0xB8
   0,                   // 0xB9
   0,                   // 0xBA
   0,                   // 0xBB
   0,                   // 0xBC
   0,                   // 0xBD
   0,                   // 0xBE
   0,                   // 0xBF
   0,                   // 0xC0
   0,                   // 0xC1
   0,                   // 0xC2
   0,                   // 0xC3
   0,                   // 0xC4
   0,                   // 0xC5
   0,                   // 0xC6
   0,                   // 0xC7
   0,                   // 0xC8
   0,                   // 0xC9
   0,                   // 0xCA
   0,                   // 0xCB
   0,                   // 0xCC
   0,                   // 0xCD
   0,                   // 0xCE
   0,                   // 0xCF
   0,                   // 0xD0
   0,                   // 0xD1
   0,                   // 0xD2
   0,                   // 0xD3
   0,                   // 0xD4
   0,                   // 0xD5
   0,                   // 0xD6
   0,                   // 0xD7
   0,                   // 0xD8
   0,                   // 0xD9
   0,                   // 0xDA
   0,                   // 0xDB
   0,                   // 0xDC
   0,                   // 0xDD
   0,                   // 0xDE
   0,                   // 0xDF
   0,                   // 0xE0
   0,                   // 0xE1
   0,                   // 0xE2
   0,                   // 0xE3
   0,                   // 0xE4
   0,                   // 0xE5
   0,                   // 0xE6
   0,                   // 0xE7
   0,                   // 0xE8
   0,                   // 0xE9
   0,                   // 0xEA
   0,                   // 0xEB
   0,                   // 0xEC
   0,                   // 0xED
   0,                   // 0xEE
   0,                   // 0xEF
   0,                   // 0xF0
   0,                   // 0xF1
   0,                   // 0xF2
   0,                   // 0xF3
   0,                   // 0xF4
   0,                   // 0xF5
   0,                   // 0xF6
   0,                   // 0xF7
   0,                   // 0xF8
   0,                   // 0xF9
   0,                   // 0xFA
   0,                   // 0xFB
   0,                   // 0xFC
   0,                   // 0xFD
   0,                   // 0xFE
   0                    // 0xFF
};

const unsigned int BitmapFont::defaultNumFonts = sizeof(defaultMap) / sizeof(const char*);
const char** BitmapFont::defaultFontMap = &defaultMap[0];

// Reverse the order of the bits
GLubyte BitmapFont::reverseByteOrder(GLubyte byte)
{
   GLubyte n = 0;

   for (unsigned int i = 0; i < 8; i++)
      n |= ((byte >> i) & 0x01) << (7 - i);

   return n;
}

// Reverse the order of the bytes
void BitmapFont::reverseBitmapOrder(GLubyte* bitmap, unsigned int numBitmapBytes, unsigned int numBytesWide)
{
   GLubyte* temp = new GLubyte[numBytesWide];

   for (unsigned int i = 0; i < numBitmapBytes/2; i += numBytesWide) {

      unsigned int j = 0;

      for (j = 0; j < numBytesWide; j++)
         temp[j] = reverseByteOrder(bitmap[i+j]);

      for (j = 0; j < numBytesWide; j++)
         bitmap[i+j] = reverseByteOrder(bitmap[numBitmapBytes-i-numBytesWide+j]);

      for (j = 0; j < numBytesWide; j++)
         bitmap[numBitmapBytes-i-numBytesWide+j] = temp[j];
   }

   delete [] temp;
}

// Load the font for one character
GLubyte* BitmapFont::loadTypeFace(const GLint index, const GLenum reverse)
{
   // If no font to load, return
   if (fontMap[index] == nullptr)
      return 0;

   // Create the font file name
   const size_t FONTPATHNAME_LENGTH = 256;
   char fontPathname[FONTPATHNAME_LENGTH];
   if (fontDirectory() != 0)
      lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
   else
      lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
   lcStrcat(fontPathname, FONTPATHNAME_LENGTH, fontMap[index]);

   // Open the font file
   FILE* fp = nullptr;
   if( (fp = std::fopen(fontPathname, "r")) ==0 ) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "BitmapFont::loadTypeFace: unable to open font file: " << fontPathname << std::endl;
      }
      return 0;
   }

   // used to store the num of input items successfully matched and assigned
   // by fscanf function
   int nItemsMatched(0);

   // Calculate the size of the font
   unsigned int width1(0);
   nItemsMatched = std::fscanf(fp, "%u\n", &width1);
   unsigned int height1(0);
   nItemsMatched = std::fscanf(fp, "%u\n", &height1);

   unsigned int numBytesWide = int(std::ceil(double(width1) / 8.0));
   unsigned int numFileBytes = numBytesWide * height1;
   unsigned int numFontBytes = numBytesWide * getBitmapHeight();

   GLubyte* bitmap = new GLubyte[numFontBytes];

   unsigned int i;  // index

   // Pad rest of the height
   unsigned int diff = numFontBytes - numFileBytes;
   for (i = 0; i < diff; i++) {
      bitmap[i] = reverse ? 255 : 0;
   }

   // Read in the bitmap bytes
   for (; i < numFontBytes; i++) {
      int value;
      nItemsMatched = std::fscanf(fp, "0x%x\n", &value);
      bitmap[i] = reverse ? GLubyte(~value) : GLubyte(value);
   }

   std::fclose(fp);

   // Reverse the bitmap
   reverseBitmapOrder(bitmap, numFontBytes, numBytesWide);

   return bitmap;
}

} // End BasicGL namespace
} // End Eaagles namespace
