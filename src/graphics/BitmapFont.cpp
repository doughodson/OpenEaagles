//------------------------------------------------------------------------------
// Bitmap Font
//------------------------------------------------------------------------------
#include "openeaagles/graphics/BitmapFont.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/util/string.h"

#include <cstdio>
#include <cmath>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace graphics {

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
    ON_SLOT(1, setReverse, base::Number)
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
            glRasterPos2d( x, (y - static_cast<float>(i)*getLineSpacing()) );
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
            glRasterPos2d( rpos[0], (rpos[1] - static_cast<float>(i)*getLineSpacing()) );
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
        if (bitmap == nullptr) continue;

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
bool BitmapFont::setReverse(const base::Number* const rnumber)
{
    if (rnumber != nullptr)
        reverse = rnumber->getBoolean();
    return true;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
base::Object* BitmapFont::getSlotByIndex(const int si)
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
   nullptr,                   // null
   nullptr,                   // soh
   nullptr,                   // stx
   nullptr,                   // etx
   nullptr,                   // eot
   nullptr,                   // enq
   nullptr,                   // ack
   nullptr,                   // bell
   nullptr,                   // bs
   nullptr,                   // ht
   nullptr,                   // nl
   nullptr,                   // vt
   nullptr,                   // np
   nullptr,                   // cr
   nullptr,                   // so
   nullptr,                   // si
   nullptr,                   // dle
   nullptr,                   // dc1
   nullptr,                   // dc2
   nullptr,                   // dc3
   nullptr,                   // dc4
   nullptr,                   // nak
   nullptr,                   // syn
   nullptr,                   // etb
   nullptr,                   // can
   nullptr,                   // em
   nullptr,                   // sub
   nullptr,                   // esc
   nullptr,                   // fs
   nullptr,                   // gs
   nullptr,                   // rs
   nullptr,                   // us
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
   nullptr,                   // del
   nullptr,                   // 0x80
   nullptr,                   // 0x81
   nullptr,                   // 0x82
   nullptr,                   // 0x83
   nullptr,                   // 0x84
   nullptr,                   // 0x85
   nullptr,                   // 0x86
   nullptr,                   // 0x87
   nullptr,                   // 0x88
   nullptr,                   // 0x89
   nullptr,                   // 0x8A
   nullptr,                   // 0x8B
   nullptr,                   // 0x8C
   nullptr,                   // 0x8D
   nullptr,                   // 0x8E
   nullptr,                   // 0x8F
   nullptr,                   // 0x90
   nullptr,                   // 0x91
   nullptr,                   // 0x92
   nullptr,                   // 0x93
   nullptr,                   // 0x94
   nullptr,                   // 0x95
   nullptr,                   // 0x96
   nullptr,                   // 0x97
   nullptr,                   // 0x98
   nullptr,                   // 0x99
   nullptr,                   // 0x9A
   nullptr,                   // 0x9B
   nullptr,                   // 0x9C
   nullptr,                   // 0x9D
   nullptr,                   // 0x9E
   nullptr,                   // 0x9F
   nullptr,                   // 0xA0
   nullptr,                   // 0xA1
   nullptr,                   // 0xA2
   nullptr,                   // 0xA3
   nullptr,                   // 0xA4
   nullptr,                   // 0xA5
   nullptr,                   // 0xA6
   nullptr,                   // 0xA7
   nullptr,                   // 0xA8
   nullptr,                   // 0xA9
   nullptr,                   // 0xAA
   nullptr,                   // 0xAB
   nullptr,                   // 0xAC
   nullptr,                   // 0xAD
   nullptr,                   // 0xAE
   nullptr,                   // 0xAF
   "degreesymbol.fon",  // 0xB0
   nullptr,                   // 0xB1
   nullptr,                   // 0xB2
   nullptr,                   // 0xB3
   nullptr,                   // 0xB4
   nullptr,                   // 0xB5
   nullptr,                   // 0xB6
   nullptr,                   // 0xB7
   nullptr,                   // 0xB8
   nullptr,                   // 0xB9
   nullptr,                   // 0xBA
   nullptr,                   // 0xBB
   nullptr,                   // 0xBC
   nullptr,                   // 0xBD
   nullptr,                   // 0xBE
   nullptr,                   // 0xBF
   nullptr,                   // 0xC0
   nullptr,                   // 0xC1
   nullptr,                   // 0xC2
   nullptr,                   // 0xC3
   nullptr,                   // 0xC4
   nullptr,                   // 0xC5
   nullptr,                   // 0xC6
   nullptr,                   // 0xC7
   nullptr,                   // 0xC8
   nullptr,                   // 0xC9
   nullptr,                   // 0xCA
   nullptr,                   // 0xCB
   nullptr,                   // 0xCC
   nullptr,                   // 0xCD
   nullptr,                   // 0xCE
   nullptr,                   // 0xCF
   nullptr,                   // 0xD0
   nullptr,                   // 0xD1
   nullptr,                   // 0xD2
   nullptr,                   // 0xD3
   nullptr,                   // 0xD4
   nullptr,                   // 0xD5
   nullptr,                   // 0xD6
   nullptr,                   // 0xD7
   nullptr,                   // 0xD8
   nullptr,                   // 0xD9
   nullptr,                   // 0xDA
   nullptr,                   // 0xDB
   nullptr,                   // 0xDC
   nullptr,                   // 0xDD
   nullptr,                   // 0xDE
   nullptr,                   // 0xDF
   nullptr,                   // 0xE0
   nullptr,                   // 0xE1
   nullptr,                   // 0xE2
   nullptr,                   // 0xE3
   nullptr,                   // 0xE4
   nullptr,                   // 0xE5
   nullptr,                   // 0xE6
   nullptr,                   // 0xE7
   nullptr,                   // 0xE8
   nullptr,                   // 0xE9
   nullptr,                   // 0xEA
   nullptr,                   // 0xEB
   nullptr,                   // 0xEC
   nullptr,                   // 0xED
   nullptr,                   // 0xEE
   nullptr,                   // 0xEF
   nullptr,                   // 0xF0
   nullptr,                   // 0xF1
   nullptr,                   // 0xF2
   nullptr,                   // 0xF3
   nullptr,                   // 0xF4
   nullptr,                   // 0xF5
   nullptr,                   // 0xF6
   nullptr,                   // 0xF7
   nullptr,                   // 0xF8
   nullptr,                   // 0xF9
   nullptr,                   // 0xFA
   nullptr,                   // 0xFB
   nullptr,                   // 0xFC
   nullptr,                   // 0xFD
   nullptr,                   // 0xFE
   nullptr                    // 0xFF
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
      return nullptr;

   // Create the font file name
   const size_t FONTPATHNAME_LENGTH = 256;
   char fontPathname[FONTPATHNAME_LENGTH];
   if (fontDirectory() != nullptr)
      base::lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
   else
      base::lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");

   base::lcStrcat(fontPathname, FONTPATHNAME_LENGTH, fontMap[index]);

   // Open the font file
   FILE* fp = nullptr;
   if( (fp = std::fopen(fontPathname, "r")) ==nullptr ) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "BitmapFont::loadTypeFace: unable to open font file: " << fontPathname << std::endl;
      }
      return nullptr;
   }

   // used to store the num of input items successfully matched and assigned
   // by fscanf function
   int nItemsMatched(0);

   // Calculate the size of the font
   unsigned int width1(0);
   nItemsMatched = std::fscanf(fp, "%u\n", &width1);
   unsigned int height1(0);
   nItemsMatched = std::fscanf(fp, "%u\n", &height1);

   unsigned int numBytesWide = static_cast<int>(std::ceil(static_cast<double>(width1) / 8.0));
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

} // End graphics namespace
} // End oe namespace
