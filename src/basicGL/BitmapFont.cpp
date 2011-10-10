//------------------------------------------------------------------------------
// Bitmap Font
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/BitmapFont.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Number.h"
#include <stdio.h>

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(BitmapFont,"BitmapFont")

// Default font size
const int defaultFontWidth = 10;
const int defaultFontHeight = 15;

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
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void BitmapFont::deleteData()
{
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
    if (isLoaded()) return;
    
    int i;

    setBase( glGenLists(256) );

    // Loop through the font map
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (i=0; i < numFonts; i++)
    {
        GLubyte* bitmap = loadTypeFace(i, reverse);
        if (bitmap == 0) continue;

        GLfloat xmove = GLfloat(getBitmapWidth());
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
    if (rnumber != 0)  reverse = rnumber->getBoolean();
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
    using namespace std;

    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFormName() << endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "reverse: " << reverse << endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << endl;
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
   "ampersand.fon",     // '@'
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
};

const int BitmapFont::defaultNumFonts = sizeof(defaultMap) / sizeof(const char*);
const char** BitmapFont::defaultFontMap = &defaultMap[0];

// Reverse the order of the bits
GLubyte BitmapFont::reverseByteOrder(GLubyte byte)
{
   int i;
   GLubyte n = 0;

   for (i = 0; i < 8; i++)
      n |= ((byte >> i) & 0x01) << (7 - i);

   return n;
}

// Reverse the order of the bytes
void BitmapFont::reverseBitmapOrder(GLubyte* bitmap, int numBitmapBytes, int numBytesWide)
{
   int i;
   GLubyte* temp = new GLubyte[numBytesWide];

   for (i = 0; i < numBitmapBytes/2; i += numBytesWide)
   {
      int j;

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
   if (fontMap[index] == 0) return 0;

   // Create the font file name
   const size_t FONTPATHNAME_LENGTH = 256;
   char fontPathname[FONTPATHNAME_LENGTH];
   if (fontDirectory() != 0) lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
   else lcStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
   lcStrcat(fontPathname, FONTPATHNAME_LENGTH, fontMap[index]);

   // Open the font file
   FILE* fp = 0;
   if( (fp = fopen(fontPathname, "r")) ==0 ) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "BitmapFont::loadTypeFace: unable to open font file: " << fontPathname << std::endl;
      }
      return 0;
   }

   // used to store the num of input items successfully matched and assigned
   // by fscanf function
   int nItemsMatched;

   // Calculate the size of the font
   int width1;
   nItemsMatched = fscanf(fp, "%d\n", &width1);

   int height1;
   nItemsMatched = fscanf(fp, "%d\n", &height1);

   int numBytesWide = int(ceil(double(width1) / 8.0));
   int numFileBytes = numBytesWide * height1;
   int numFontBytes = numBytesWide * getBitmapHeight();

   GLubyte* bitmap = new GLubyte[numFontBytes];

   unsigned int i;  // index

   // Pad rest of the height
   int diff = numFontBytes - numFileBytes;
   for (i = 0; i < diff; i++)
   {
      bitmap[i] = reverse ? 255 : 0;
   }

   // Read in the bitmap bytes
   for (; i < numFontBytes; i++)
   {
      int value;
      nItemsMatched = fscanf(fp, "0x%x\n", &value);
      bitmap[i] = reverse ? GLubyte(~value) : GLubyte(value);
   }

   fclose(fp);

   // Reverse the bitmap
   reverseBitmapOrder(bitmap, numFontBytes, numBytesWide);

   return bitmap;
}

} // End BasicGL namespace
} // End Eaagles namespace

