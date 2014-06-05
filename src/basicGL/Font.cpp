#include "openeaagles/basicGL/Font.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_PARTIAL_SUBCLASS(Font,"Font")

// Default font size
const double defaultFontWidth = 1;
const double defaultFontHeight = 1;

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Font)
        "fontWidth",
        "fontHeight",
        "fontPosition",         // Upper/Left origin of the text field
        "bitmapWidth",
        "bitmapHeight",
        "path",                 // Path to the font directory
        "file",                 // FTGL Font file name - will be altered in each instance of FTGLFont
        "lut",                  // Lookup Table
        "characterSpacing",     // spacing for each character
        "lineSpacing",          // spacing for each line
END_SLOTTABLE(Font)


//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Font)
    ON_SLOT(1,setSlotFontWidth,Basic::Number)
    ON_SLOT(2,setSlotFontHeight,Basic::Number)
    ON_SLOT(3,setSlotFontPosition,Basic::List)
    ON_SLOT(4,setSlotBitmapWidth,Basic::Number)
    ON_SLOT(5,setSlotBitmapHeight,Basic::Number)
    ON_SLOT(6,setSlotFontPath,Basic::String)
    ON_SLOT(7,setSlotFTGLFontFileName,Basic::String)   
    ON_SLOT(8,setSlotLookupTable,Basic::List)
    ON_SLOT(9,setSlotCharacterSpacing,Basic::Number);
    ON_SLOT(10,setSlotLineSpacing,Basic::Number);
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Font::Font()
{
    STANDARD_CONSTRUCTOR()
    
    leftSide = 0;
    topSide = 0;
    b = 0;
    pLUT = 0;
    pFTGL = 0;
    fontPath = 0;
    fontFile = 0;
    loaded = false;
    charSpacing = 0;
    lineSpacing = 0;
    
    setFontWidth( defaultFontWidth );
    setFontHeight( defaultFontHeight );

    setBitmapWidth( int(defaultFontWidth) );
    setBitmapHeight( int(defaultFontHeight) );
}

Font::Font(const Font& org)
{ 
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

Font::~Font()
{
   STANDARD_DESTRUCTOR()
}

Font& Font::operator=(const Font& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Font* Font::clone() const
{
    return 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Font::copyData(const Font& org, const bool cc)
{
    BaseClass::copyData(org);
    leftSide = org.leftSide;
    topSide = org.topSide;
    b = org.b;
    pLUT = org.pLUT;
    pFTGL = org.pFTGL;
    charSpacing = org.charSpacing;
    lineSpacing = org.lineSpacing;
    if (!cc) {
        if (fontPath != 0) delete[] fontPath;
        if (fontFile != 0) delete[] fontFile;
    }
    fontPath = 0;
    fontFile = 0;
    if (org.fontPath != 0) {
        size_t len = strlen(org.fontPath);
        fontPath = new char[len+1];
        lcStrcpy(fontPath,len+1,org.fontPath);
    }
    if (org.fontFile != 0) {
        size_t len = strlen(org.fontFile);
        fontFile = new char[len+1];
        lcStrcpy(fontFile,len+1,org.fontFile);
    }
    loaded = org.loaded;

    fWidth = org.fWidth;
    fHeight = org.fHeight;
    bWidth = org.bWidth;
    bHeight = org.bHeight;

}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Font::deleteData()
{
}

//------------------------------------------------------------------------------
// position() -- computes the position of the (ln, cp) pair
//------------------------------------------------------------------------------
void Font::position(const int ln, const int cp, GLdouble& px, GLdouble& py) const
{
   // Compute X position based on the one based character position (column)
   px = leftSide + charSpacing*(cp-1);

   // Compute Y position based on the one based line number (row)
   py = topSide - lineSpacing*ln;
}


//------------------------------------------------------------------------------
// setTextOrigin() -- sets the upper left corner
//------------------------------------------------------------------------------
void Font::setTextOrigin(const GLdouble x, const GLdouble y)
{
    leftSide = x;
    topSide = y;
}


//------------------------------------------------------------------------------
// xferChars() -- transfer characters
//------------------------------------------------------------------------------
int Font::xferChars(char* const outp, const size_t BUF_SIZE, const char* const inp, const unsigned int n) const
{
    if (outp == 0 || inp == 0) return 0;
    if (n >= BUF_SIZE) return 0;
    
   // Buffer the string to translate the characters with the LUT,
   // as required, and to ensure that the string is terminated.
   if (pLUT != 0) {
      for (unsigned int i = 0; i < n; i++) {
          unsigned char idx = static_cast<unsigned char>(inp[i]);
          outp[i] = (idx < LUT_SIZE ? static_cast<char>(pLUT[idx]) : inp[i]);
      }
   }
   else {
      lcStrncpy(outp,BUF_SIZE,inp,n);
   }
   outp[n] = '\0';
   return int(strlen(outp));
}



//------------------------------------------------------------------------------
// getSlotByIndex() for Font
//------------------------------------------------------------------------------
Basic::Object* Font::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Font::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (getFontWidth() > 0) {
        indent(sout,i+j);
        sout << "fontWidth: " << getFontWidth() << std::endl;
    }

    if (getFontHeight() > 0) {
        indent(sout,i+j);
        sout << "fontHeight: " << getFontHeight() << std::endl;
    }

    if (leftSide != 0 || topSide != 0) {
        indent(sout,i+j);
        sout << "fontPosition: [ " << leftSide << " " << topSide << " ]" << std::endl;
    }

    if (getBitmapWidth() > 0) {
        indent(sout,i+j);
        sout << "bitmapWidth: " << getBitmapWidth() << std::endl;
    }

    if (getBitmapHeight() > 0) {
        indent(sout,i+j);
        sout << "bitmapHeight: " << getBitmapHeight() << std::endl;
    }

    if (fontDirectory() != 0) {
        indent(sout,i+j);
        sout << "path: " << fontDirectory() << std::endl;
    }

    if (filename() != 0) {
        indent(sout,i+j);
        sout << "file: " << filename() << std::endl;
    }

    if (lut() != 0) {
        indent(sout,i+j);
        sout << "lut: [" << std::endl;
        indent(sout,i+j+10);
        int iiCount = 0;
        for (int ii = 0; ii < LUT_SIZE; ii++) {
            int vLUT = 0x000000FF & int(pLUT[ii]);
            sout << std::hex << "0x" << vLUT << std::dec;
            iiCount++;
            if (iiCount == 256 && LUT_SIZE == 256) break;    // last one
            else if (iiCount % 64 == 0) {
                sout << std::endl << std::endl;
                indent(sout,i+j+10);
            }
            else if (iiCount % 16 == 0) {
                sout << std::endl;
                indent(sout,i+j+10);
            }
            else if (iiCount % 4 == 0) sout << "  ";
            else sout << " ";
        }
        sout << std::endl;
        indent(sout,i+j+5);
        sout << "]" << std::endl;
    }


    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// setSlotFontWidth () -- sets the font width
//------------------------------------------------------------------------------
bool Font::setSlotFontWidth(const Basic::Number* const sfwobj)
{
    if (sfwobj != 0) setFontWidth( sfwobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontHeight() - sets the font height
//------------------------------------------------------------------------------
bool Font::setSlotFontHeight (const Basic::Number* const sfhobj)
{
    if (sfhobj != 0) setFontHeight( sfhobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPosition() - sets the font position
//------------------------------------------------------------------------------
bool Font::setSlotFontPosition (const Basic::List* const sfpobj)
{
    bool ok = true;
    if (sfpobj != 0) {
        LCreal values[2];
        int n = sfpobj->getNumberList(values, 2);
        ok = (n == 2);
        if (ok) setTextOrigin(values[0],values[1]);
        else {
              if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Font::setFontPosition: Values are invalid or missing" << std::endl;
              }
            ok = false;
        }   
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotBitmapWidth() - sets the bitmap width 
//------------------------------------------------------------------------------
bool Font::setSlotBitmapWidth(const Basic::Number* const sbwobj)
{
    if (sbwobj != 0) setBitmapWidth( sbwobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setBitmapHeight() - sets the bitmap height 
//------------------------------------------------------------------------------
bool Font::setSlotBitmapHeight(const Basic::Number* const sbhobj)
{
    if (sbhobj != 0) setBitmapHeight( sbhobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPath() - sets the path to the font directory 
//------------------------------------------------------------------------------
bool Font::setSlotFontPath(const Basic::String* const sfpobj)
{
    bool ok = true;
    if (sfpobj != 0) {
        size_t j = sfpobj->len();
        if (j > 0) {
            fontPath = new char[j+1];
            lcStrcpy(fontPath,j+1,*sfpobj);
        }
        else {
              if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Font::setFontPath: Invalid font path value" << std::endl;
              }
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotFTGLFontFileName() - sets the FTGL Font File Name
//------------------------------------------------------------------------------
bool Font::setSlotFTGLFontFileName(const Basic::String* const sgffnobj)
{
    bool ok = true;
    if (sgffnobj != 0) {
        size_t j = sgffnobj->len();
        if (j > 0) {
            fontFile = new char[j+1];
            lcStrcpy(fontFile,j+1,*sgffnobj);
        }
        else {
           if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Font::setFTGLFontFileName: File name invalid or missing" << std::endl;
           }
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotLookupTable() - sets the lookup table
//------------------------------------------------------------------------------
bool Font::setSlotLookupTable(const Basic::List* const sltobj)
{
    bool ok = true;
    if (sltobj != 0) {
        // Load the LUT
        if (pLUT == 0) pLUT = new unsigned char[LUT_SIZE];
        int values[LUT_SIZE];
        int nn = sltobj->getNumberList(values, LUT_SIZE);
        int ii = 0;
        // Transfer the table
        while (ii < nn) {
            pLUT[ii] = ( (values[ii] >= 0 && values[ii] <= 255) ? char(values[ii]) : char(ii) );
            ii++;
        }
        // Pad the end of the table
        while (ii < LUT_SIZE) {
            pLUT[ii] = char(ii);
            ii++;
        }
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "Font::setLookupTable: Nothing in the lookup table." << std::endl;
          }
        ok = false;
    }
    return ok;
}

bool Font::setSlotCharacterSpacing(const Basic::Number* const newCharSpacing)
{
    // set our character spacing
    if (newCharSpacing != 0) setCharacterSpacing( newCharSpacing->getFloat() );
    return true;
}

bool Font::setSlotLineSpacing(const Basic::Number* const newLineSpacing)
{
    // set our line spacing
    if (newLineSpacing != 0) setLineSpacing( newLineSpacing->getFloat() );
    return true;
}

} // End BasicGL namespace
} // End Eaagles namespace
