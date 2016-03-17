
#include "openeaagles/graphics/Font.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/util/str_utils.h"

#include <cstring>

namespace oe {
namespace graphics {

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
    ON_SLOT(1,setSlotFontWidth,base::Number)
    ON_SLOT(2,setSlotFontHeight,base::Number)
    ON_SLOT(3,setSlotFontPosition,base::List)
    ON_SLOT(4,setSlotBitmapWidth,base::Number)
    ON_SLOT(5,setSlotBitmapHeight,base::Number)
    ON_SLOT(6,setSlotFontPath,base::String)
    ON_SLOT(7,setSlotFTGLFontFileName,base::String)
    ON_SLOT(8,setSlotLookupTable,base::List)
    ON_SLOT(9,setSlotCharacterSpacing,base::Number);
    ON_SLOT(10,setSlotLineSpacing,base::Number);
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Font::Font()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Font::initData()
{
   leftSide = 0;
   topSide = 0;
   b = 0;
   pLUT = nullptr;
   pFTGL = nullptr;
   fontPath = nullptr;
   fontFile = nullptr;
   loaded = false;
   charSpacing = 0;
   lineSpacing = 0;

   setFontWidth( defaultFontWidth );
   setFontHeight( defaultFontHeight );

   setBitmapWidth( static_cast<int>(defaultFontWidth) );
   setBitmapHeight( static_cast<int>(defaultFontHeight) );
}

Font::Font(const Font& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
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
   return nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Font::copyData(const Font& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   leftSide = org.leftSide;
   topSide = org.topSide;
   b = org.b;

   if (org.pLUT != nullptr) {
      if (pLUT == nullptr) {
         pLUT = new unsigned char[LUT_SIZE];
      }
      for (unsigned int i = 0; i < LUT_SIZE; i++) {
         pLUT[i] = org.pLUT[i];
      }
   }
   else {
      if (pLUT != nullptr) delete[] pLUT;
      pLUT = nullptr;
   }

   pFTGL = org.pFTGL;

   if (fontPath != nullptr) delete[] fontPath;
   if (fontFile != nullptr) delete[] fontFile;
   fontPath = nullptr;
   fontFile = nullptr;
   if (org.fontPath != nullptr) {
      size_t len = std::strlen(org.fontPath);
      fontPath = new char[len+1];
      base::utStrcpy(fontPath,len+1,org.fontPath);
   }
   if (org.fontFile != nullptr) {
      size_t len = std::strlen(org.fontFile);
      fontFile = new char[len+1];
      base::utStrcpy(fontFile,len+1,org.fontFile);
   }
   loaded = org.loaded;

   charSpacing = org.charSpacing;
   lineSpacing = org.lineSpacing;

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
   if (pLUT != nullptr) delete[] pLUT;
   pLUT = nullptr;

   if (fontPath != nullptr) delete[] fontPath;
   fontPath = nullptr;

   if (fontFile != nullptr) delete[] fontFile;
   fontFile = nullptr;
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
    if (outp == nullptr || inp == nullptr) return 0;
    if (n >= BUF_SIZE) return 0;

   // Buffer the string to translate the characters with the LUT,
   // as required, and to ensure that the string is terminated.
   if (pLUT != nullptr) {
      for (unsigned int i = 0; i < n; i++) {
          unsigned char idx = static_cast<unsigned char>(inp[i]);
          outp[i] = static_cast<char>(pLUT[idx]);
      }
   }
   else {
      base::utStrncpy(outp,BUF_SIZE,inp,n);
   }
   outp[n] = '\0';
   return static_cast<int>(std::strlen(outp));
}



//------------------------------------------------------------------------------
// getSlotByIndex() for Font
//------------------------------------------------------------------------------
base::Object* Font::getSlotByIndex(const int si)
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

    if (fontDirectory() != nullptr) {
        indent(sout,i+j);
        sout << "path: " << fontDirectory() << std::endl;
    }

    if (filename() != nullptr) {
        indent(sout,i+j);
        sout << "file: " << filename() << std::endl;
    }

    if (lut() != nullptr) {
        indent(sout,i+j);
        sout << "lut: [" << std::endl;
        indent(sout,i+j+10);
        int iiCount = 0;
        for (int ii = 0; ii < LUT_SIZE; ii++) {
            int vLUT = 0x000000FF & static_cast<int>(pLUT[ii]);
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
bool Font::setSlotFontWidth(const base::Number* const sfwobj)
{
    if (sfwobj != nullptr) setFontWidth( sfwobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontHeight() - sets the font height
//------------------------------------------------------------------------------
bool Font::setSlotFontHeight (const base::Number* const sfhobj)
{
    if (sfhobj != nullptr) setFontHeight( sfhobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPosition() - sets the font position
//------------------------------------------------------------------------------
bool Font::setSlotFontPosition (const base::List* const sfpobj)
{
    bool ok = true;
    if (sfpobj != nullptr) {
        double values[2];
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
bool Font::setSlotBitmapWidth(const base::Number* const sbwobj)
{
    if (sbwobj != nullptr) setBitmapWidth( sbwobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setBitmapHeight() - sets the bitmap height
//------------------------------------------------------------------------------
bool Font::setSlotBitmapHeight(const base::Number* const sbhobj)
{
    if (sbhobj != nullptr) setBitmapHeight( sbhobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPath() - sets the path to the font directory
//------------------------------------------------------------------------------
bool Font::setSlotFontPath(const base::String* const sfpobj)
{
    bool ok = true;
    if (sfpobj != nullptr) {
        size_t j = sfpobj->len();
        if (j > 0) {
            fontPath = new char[j+1];
            utStrcpy(fontPath,j+1,*sfpobj);
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
bool Font::setSlotFTGLFontFileName(const base::String* const sgffnobj)
{
    bool ok = true;
    if (sgffnobj != nullptr) {
        size_t j = sgffnobj->len();
        if (j > 0) {
            fontFile = new char[j+1];
            utStrcpy(fontFile,j+1,*sgffnobj);
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
bool Font::setSlotLookupTable(const base::List* const sltobj)
{
    bool ok = true;
    if (sltobj != nullptr) {
        // Load the LUT
        if (pLUT == nullptr) pLUT = new unsigned char[LUT_SIZE];
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

bool Font::setSlotCharacterSpacing(const base::Number* const newCharSpacing)
{
    // set our character spacing
    if (newCharSpacing != nullptr) setCharacterSpacing( newCharSpacing->getFloat() );
    return true;
}

bool Font::setSlotLineSpacing(const base::Number* const newLineSpacing)
{
    // set our line spacing
    if (newLineSpacing != nullptr) setLineSpacing( newLineSpacing->getFloat() );
    return true;
}

} // End graphics namespace
} // End oe namespace
