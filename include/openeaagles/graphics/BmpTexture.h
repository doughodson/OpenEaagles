//------------------------------------------------------------------------------
// Class: BmpTexture
//------------------------------------------------------------------------------
#ifndef __oe_graphics_BmpTexture_H__
#define __oe_graphics_BmpTexture_H__

#include "openeaagles/graphics/Texture.h"
#include <GL/gl.h>

namespace oe {
    namespace base { class String; }

namespace graphics {

//------------------------------------------------------------------------------
// Class: BmpTexture
//
// Description: Read/Write a BMP file (use loadTexture() to read)
//
// Factory name:
// Slots:
//    path    <String>     ! Path to the texture directory (default: 0)
//    file    <String>     ! BmpTexture file name (default: 0)
//
//------------------------------------------------------------------------------
class BmpTexture : public Texture {
   DECLARE_SUBCLASS(BmpTexture, Texture)

public:
   BmpTexture();

   // The filename of the current texture.
   const char* getTextureFileName() const;
   bool setTextureFileName(const char* const fileName);

   // The path to the current textures directory.
   const char* getTexturePath() const;
   bool setTexturePath(const char* const path);

   // Read the BMP file
   bool readFile();

   // Write the BMP file
   bool writeFile();

   void loadTexture() override;

protected:
   bool setSlotTexturePath(base::String* const s);
   bool setSlotTextureFileName(base::String* const s);

private:
   void initData();

   char texPath[MAX_PATH_LEN+1];       // Path to texture directory
   char texFile[MAX_FILENAME_LEN+1];   // BmpTexture filename
};

//------------------------------------------------------------------------------
// Inline routines
//------------------------------------------------------------------------------
inline const char* BmpTexture::getTexturePath() const       { return texPath; }
inline const char* BmpTexture::getTextureFileName() const   { return texFile; }

} // End graphics namespace
} // End oe namespace

#endif
