
#include "openeaagles/basicGL/BmpTexture.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/String.h"

#include <cstdio>

#if defined(__MINGW32__)
   #include <GL/glext.h>
#endif


namespace Eaagles {
namespace BasicGL {

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
    ON_SLOT(1, setSlotTexturePath, Basic::String)
    ON_SLOT(2, setSlotTextureFileName, Basic::String)
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

   lcStrcpy(texPath, sizeof(texPath), org.texPath);
   lcStrcpy(texFile, sizeof(texFile), org.texFile);
}

//------------------------------------------------------------------------------
// Set routines
//------------------------------------------------------------------------------
bool BmpTexture::setTextureFileName(const char* const fileName)
{
   if (fileName != nullptr) {
      Eaagles::lcStrcpy(texFile, sizeof(texFile), fileName);
   }
   else {
      texFile[0] = '\0';
   }
   return true;
}

bool BmpTexture::setTexturePath(const char* const path)
{
   if (path != nullptr) {
      Eaagles::lcStrcpy(texPath, sizeof(texPath), path);
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
bool BmpTexture::setSlotTexturePath(Basic::String* const s)
{
   if (s != nullptr) setTexturePath(*s);
   else setTexturePath(nullptr);
   return true;
}

// setSlotTextureFileName() -- sets the texture file name
bool BmpTexture::setSlotTextureFileName(Basic::String* const s)
{
   if (s != nullptr) setTextureFileName(*s);
   else setTextureFileName(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
Basic::Object* BmpTexture::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End BasicGL namespace
} // End Eaagles namespace
