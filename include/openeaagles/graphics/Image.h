//------------------------------------------------------------------------------
// Class: Image
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Image_H__
#define __oe_graphics_Image_H__

#include "openeaagles/base/Object.h"

#include "openeaagles/base/util/platform_api.h"
#include <GL/gl.h>

namespace oe {
   namespace base { class Number; }

namespace graphics {
   struct BITMAPINFOHEADER_X;

//------------------------------------------------------------------------------
// Class: Image
// Description: Base class for images and textures
//------------------------------------------------------------------------------
class Image : public base::Object {
   DECLARE_SUBCLASS(Image,base::Object)

public:
   // Max length of file name or directory path
   static const unsigned int MAX_PATH_LEN = 256;
   static const unsigned int MAX_FILENAME_LEN = 64;

   // pixel size in bytes { RGB }
   static const int PIXEL_SIZE = 3;

public:
   Image();

   Image(
      const unsigned int w,   // Width of the image
      const unsigned int h,   // Height of the image
      const unsigned int nc,  // Number of components
      const GLenum format,    // format: GL_RGB, GL_BGRA_EXT, etc
      const GLubyte* const image // pixels to copy
   );

   unsigned int getWidth() const;         // width in pixels
   unsigned int getHeight() const;        // height in pixels
   unsigned int getNumComponents() const; // Number of color components (3 or 4)
   GLenum getFormat() const;              // format: GL_RGB, GL_BGRA_EXT, etc

   const GLubyte* getPixels() const;      // Pointer to our image pixels

   // Resolution (pixels per meter)
   unsigned int getXResolutionPPM() const;
   unsigned int getYResolutionPPM() const;

   // set functions
   virtual bool setWidth(const unsigned int w);
   virtual bool setHeight(const unsigned int h);
   virtual bool setFormat(const GLenum f);
   virtual bool setNumComponents(const unsigned int n);
   virtual bool setXResolutionPPM(const unsigned int ppm);
   virtual bool setYResolutionPPM(const unsigned int ppm);

   // Sets our image to a copy of this image
   virtual bool setPixels(const GLubyte* const image);

   // Read/write a bitmap (BMP) format file
   virtual bool readFileBMP(const char* const filename, const char* const path=0);
   virtual bool writeFileBMP(const char* const filename, const char* const path=0);

private:
   void initData();
   GLubyte* readRgbValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const;
   GLubyte* readColorValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const;

   unsigned int width;           // width in pixels
   unsigned int height;          // height in pixels
   unsigned int numComponents;   // Number of color components (3 or 4)
   GLenum format;                // GL_RGB, GL_BGRA_EXT, etc.

   GLubyte* pixels;              // Pointer to our pixels; number of pixel bytes must
                                 // be equal to (width*height*numComponents)

   unsigned int xPixPerMeter;   // X pixels per meter
   unsigned int yPixPerMeter;   // Y pixels per meter
};

inline unsigned int Image::getWidth() const           { return width; }
inline unsigned int Image::getHeight() const          { return height; }
inline unsigned int Image::getNumComponents() const   { return numComponents; }
inline GLenum Image::getFormat() const                { return format; }
inline const GLubyte* Image::getPixels() const        { return pixels; }

inline unsigned int Image::getXResolutionPPM() const  { return xPixPerMeter; }
inline unsigned int Image::getYResolutionPPM() const  { return yPixPerMeter; }

} // End graphics namespace
}

#endif
