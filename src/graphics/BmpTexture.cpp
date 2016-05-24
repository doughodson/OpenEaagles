
#include "openeaagles/graphics/BmpTexture.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/Identifier.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/util/str_utils.h"

#include <cstdio>

#if defined(__MINGW32__)
   #include <GL/glext.h>
#endif


namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(BmpTexture,"BitmapTexture")
EMPTY_SERIALIZER(BmpTexture)
EMPTY_DELETEDATA(BmpTexture)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(BmpTexture)
    "path",         // 1) Path to the texture directory
    "file",         // 2) BmpTexture file name
END_SLOTTABLE(BmpTexture)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(BmpTexture)
    ON_SLOT(1, setSlotTexturePath, base::String)
    ON_SLOT(2, setSlotTextureFileName, base::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
BmpTexture::BmpTexture()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void BmpTexture::initData()
{
   texPath[0] = '\0';
   texFile[0] = '\0';
}

//------------------------------------------------------------------------------
// copyData() -
//------------------------------------------------------------------------------
void BmpTexture::copyData(const BmpTexture& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   base::utStrcpy(texPath, sizeof(texPath), org.texPath);
   base::utStrcpy(texFile, sizeof(texFile), org.texFile);
}

//------------------------------------------------------------------------------
// Set routines
//------------------------------------------------------------------------------
bool BmpTexture::setTextureFileName(const char* const fileName)
{
   if (fileName != nullptr) {
      base::utStrcpy(texFile, sizeof(texFile), fileName);
   }
   else {
      texFile[0] = '\0';
   }
   return true;
}

bool BmpTexture::setTexturePath(const char* const path)
{
   if (path != nullptr) {
      base::utStrcpy(texPath, sizeof(texPath), path);
   }
   else {
      texPath[0] = '\0';
   }
   return true;
}

//------------------------------------------------------------------------------
// loadTexture() -- load the texture
//------------------------------------------------------------------------------
void BmpTexture::loadTexture()
{
   // If needed, read the BMP file
   if (getPixels() == nullptr) readFile();

   // Load the texture
   if (getPixels() != nullptr) BaseClass::loadTexture();
}

//------------------------------------------------------------------------------
// Read the BMP file
//------------------------------------------------------------------------------
bool BmpTexture::readFile()
{
   return readFileBMP(getTextureFileName(), getTexturePath());
}

//------------------------------------------------------------------------------
// Write the BMP file
//------------------------------------------------------------------------------
bool BmpTexture::writeFile()
{
   return writeFileBMP(getTextureFileName(), getTexturePath());
    }

//------------------------------------------------------------------------------
// SLOT functions
//------------------------------------------------------------------------------

// setSlotTexturePath() -- sets the path to the textures directory
bool BmpTexture::setSlotTexturePath(base::String* const s)
{
   if (s != nullptr) setTexturePath(*s);
   else setTexturePath(nullptr);
   return true;
}

// setSlotTextureFileName() -- sets the texture file name
bool BmpTexture::setSlotTextureFileName(base::String* const s)
{
   if (s != nullptr) setTextureFileName(*s);
   else setTextureFileName(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
base::Object* BmpTexture::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End graphics namespace
}
