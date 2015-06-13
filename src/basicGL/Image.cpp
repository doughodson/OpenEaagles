// Image
#include "openeaagles/basicGL/Image.h"
#include <fstream>
#include <cstring>
#include <cstdio>

// if OpenGL extension is not defined by glu.h, try loading glext.h
#ifndef GL_BGR_EXT
#include <GL/glext.h>
#endif

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Define the BITMAP File for non-Windows systems
//------------------------------------------------------------------------------
//#if !defined(WIN32)

struct BITMAPINFOHEADER_X {
   uint32_t biSize;
   int32_t  biWidth;
   int32_t  biHeight;
   uint16_t biPlanes;
   uint16_t biBitCount;
   uint32_t biCompression;
   uint32_t biSizeImage;
   int32_t  biXPelsPerMeter;
   int32_t  biYPelsPerMeter;
   uint32_t biClrUsed;
   uint32_t biClrImportant;
};

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L
//
//#endif

// Byte swap check
// -- returns true if this computer's byte order is big endian.
static bool checkSwap()
{
  uint16_t w16 = 1;
  uint8_t* p8 = reinterpret_cast<uint8_t*>(&w16);
  return (*p8 == 0);  // msb is zero; big endian
}

// Swap signed 32 bit word
static int32_t convertInt32(const int32_t v)
{
   union Udata {
      int32_t i32;
      int8_t   byte[4];
   };

   Udata value0;
   value0.i32 = v;

   Udata value1;
   value1.byte[3] = value0.byte[0];
   value1.byte[2] = value0.byte[1];
   value1.byte[1] = value0.byte[2];
   value1.byte[0] = value0.byte[3];

   return value1.i32;
}

// Swap unsigned 32 bit word
static uint32_t convertUInt32(const uint32_t v)
{
   union Udata {
      uint32_t ui32;
      int8_t   byte[4];
   };

   Udata value0;
   value0.ui32 = v;

   Udata value1;
   value1.byte[3] = value0.byte[0];
   value1.byte[2] = value0.byte[1];
   value1.byte[1] = value0.byte[2];
   value1.byte[0] = value0.byte[3];

   return value1.ui32;
}

// Swap 16 bit word
static uint16_t convertUInt16(const uint16_t v)
{
   union Udata {
      uint16_t ui16;
      int8_t   byte[2];
   };

   Udata value0;
   value0.ui16 = v;

   Udata value1;
   value1.byte[1] = value0.byte[0];
   value1.byte[0] = value0.byte[1];

   return value1.ui16;
}

//==============================================================================
// Class: Image
//==============================================================================
IMPLEMENT_SUBCLASS(Image,"Image")
EMPTY_SERIALIZER(Image)
EMPTY_SLOTTABLE(Image)

// Constructors
Image::Image()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

Image::Image(
      const unsigned int w,   // Width of the image
      const unsigned int h,   // Height of the image
      const unsigned int nc,  // Number of components
      const GLenum f,         // format: GL_RGB, GL_BGRA_EXT, etc.
      const GLubyte* const image // image pixels (will be copied)
   )
{
   STANDARD_CONSTRUCTOR()

   initData();

   setWidth(w);
   setHeight(h);
   setFormat(f);
   setNumComponents(nc);
   setPixels(image);
}

void Image::initData()
{
   width = 0;
   height = 0;
   numComponents = 0;
   format = GL_RGB;
   pixels = nullptr;

   xPixPerMeter = 3937;  // Default: about 100 pixels per inch
   yPixPerMeter = 3937;
}

// Copy data
void Image::copyData(const Image& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   width = org.width;
   height = org.height;
   numComponents = org.numComponents;
   format = org.format;

   setPixels(org.pixels);

   xPixPerMeter = org.xPixPerMeter;
   yPixPerMeter = org.yPixPerMeter;
}

// deleteData() -
void Image::deleteData()
{
   setPixels(nullptr);

   width = 0;
   height = 0;
   numComponents = 0;
}

//------------------------------------------------------------------------------
// set functions
//------------------------------------------------------------------------------

bool Image::setWidth(const unsigned int w)
{
   width = w;
   return true;
}

bool Image::setHeight(const unsigned int h)
{
   height = h;
   return true;
}

bool Image::setFormat(const GLenum f)
{
   format = f;
   return true;
}

bool Image::setNumComponents(const unsigned int n)
{
   numComponents = n;
   return true;
}

bool Image::setPixels(const GLubyte* const newPixels)
{
   if (pixels != nullptr) {
      // delete the old pixels
      delete[] pixels;
      pixels = nullptr;
   }

   // copy the new image
   unsigned int size = width * height * numComponents;
   if (newPixels != nullptr && size > 0) {
      pixels = new GLubyte[size];
      for (unsigned int i = 0; i < size; i++) { pixels[i] = newPixels[i]; }
   }

   return true;
}

bool Image::setXResolutionPPM(const unsigned int ppm)
{
   xPixPerMeter = ppm;
   return true;
}

bool Image::setYResolutionPPM(const unsigned int ppm)
{
   yPixPerMeter = ppm;
   return true;
}

//------------------------------------------------------------------------------
// Read a bitmap (BMP) format file
//------------------------------------------------------------------------------
bool Image::readFileBMP(const char* const filename, const char* const path)
{
   static const unsigned int BITMAPFILE_SIZE = (MAX_PATH_LEN+MAX_FILENAME_LEN);
   char bitmapFile[BITMAPFILE_SIZE];
   bitmapFile[0] = '\0';

   // append path name
   const char* p1 = path;
   if (p1 != nullptr && std::strlen(p1) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p1);
      lcStrcat(bitmapFile, sizeof(bitmapFile), "/");
   }

   // append file name
   const char* p2 = filename;
   if (p2 != nullptr && std::strlen(p2) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p2);
   }

   // Do we have a full path name?
   if (std::strlen(bitmapFile) <= 1) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(): invalid file name: " << bitmapFile << std::endl;
      }
      return false;
   }

   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "Image: Loading file: " << filename << std::endl;
   }

   FILE* fp = std::fopen(bitmapFile,"rb");
   if (fp == nullptr) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP: unable to open bitmap file: " << bitmapFile << std::endl;
      }
      return false;
   }

   // Big or little ending?  Swap if we're a big endian arch
   bool swap = checkSwap();

   // Read the bitmap file header (BITMAPFILEHEADER)
   char bfType[2];
   uint32_t  bfSize(0);
   uint16_t  bfReserved1(0);
   uint16_t  bfReserved2(0);
   uint32_t  bfOffBits(0);

   //unsigned int bitmapFileHdrSize =
   //   sizeof(bfType) + sizeof(bfSize) + sizeof(bfReserved1) + sizeof(bfReserved2) + sizeof(bfOffBits);

   size_t nItemsRead(0);
   nItemsRead = std::fread(&bfType, sizeof(char), 2, fp);

   nItemsRead = std::fread(&bfSize, sizeof(bfSize), 1, fp);
   if (swap) bfSize = convertUInt32(bfSize);

   nItemsRead = std::fread(&bfReserved1, sizeof(bfReserved1), 1, fp);
   if (swap) bfReserved1 = convertUInt16(bfReserved1);

   nItemsRead = std::fread(&bfReserved2, sizeof(bfReserved2), 1, fp);
   if (swap) bfReserved2 = convertUInt16(bfReserved2);

   nItemsRead = std::fread(&bfOffBits, sizeof(bfOffBits), 1, fp);
   if (swap) bfOffBits = convertUInt32(bfOffBits);

   if (bfType[0] != 'B' || bfType[1] != 'M') {
      // Not a bitmap file
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(1): invalid bitmap file: " << bitmapFile << std::endl;
      }
      std::fclose(fp);
      return false;
   }

   // Read the bitmap file info
   BITMAPINFOHEADER_X bmfi;
   nItemsRead = std::fread(&bmfi, sizeof(BITMAPINFOHEADER_X), 1, fp);

   if (swap) {
      bmfi.biSize = convertUInt32(bmfi.biSize);
      bmfi.biWidth = convertInt32(bmfi.biWidth);
      bmfi.biHeight = convertInt32(bmfi.biHeight);
      bmfi.biPlanes = convertUInt16(bmfi.biPlanes);
      bmfi.biBitCount = convertUInt16(bmfi.biBitCount);
      bmfi.biCompression = convertUInt32(bmfi.biCompression);
      bmfi.biSizeImage = convertUInt32(bmfi.biSizeImage);
      bmfi.biXPelsPerMeter = convertInt32(bmfi.biXPelsPerMeter);
      bmfi.biYPelsPerMeter = convertInt32(bmfi.biYPelsPerMeter);
      bmfi.biClrUsed = convertUInt32(bmfi.biClrUsed);
      bmfi.biClrImportant = convertUInt32(bmfi.biClrImportant);
   }

   if (bmfi.biSize != sizeof(BITMAPINFOHEADER_X) || bmfi.biPlanes != 1) {
      // Not a bitmap file
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(2): invalid bitmap file: " << bitmapFile << std::endl;
      }
      std::fclose(fp);
      return false;
   }

   // set width & height (truncates to powers of two)
   setWidth(bmfi.biWidth);
   setHeight(bmfi.biHeight);

   // set resolution
   setXResolutionPPM(bmfi.biXPelsPerMeter);
   setYResolutionPPM(bmfi.biYPelsPerMeter);

   // Read the colors
   GLubyte* bmap = nullptr;
   if (bmfi.biBitCount == 24) {
      setNumComponents(3);
      setFormat(GL_BGR_EXT);
      bmap = readRgbValuesBMP(fp, bfOffBits, &bmfi);
   }
   else if (bmfi.biBitCount == 8){
      setNumComponents(3);
      setFormat(GL_BGR_EXT);
      bmap = readColorValuesBMP(fp, bfOffBits, &bmfi);
   }
   else if (bmfi.biBitCount == 4){
      if (isMessageEnabled(MSG_WARNING)) {
         std::cerr << "Image::readFileBMP: can not load a 16 color bitmap!" << std::endl;
      }
   }

   if (bmap == nullptr) {
      if (isMessageEnabled(MSG_WARNING)) {
         std::cerr << "Image::readFileBMP(3): invalid bitmap file: " << bitmapFile << std::endl;
      }
   }

   // close the file
   std::fclose(fp);

   // Set the pixel bit map
   if (bmap != nullptr) setPixels(bmap);

   return (bmap != nullptr);
}

//------------------------------------------------------------------------------
// Write a bitmap (BMP) format file
//------------------------------------------------------------------------------
bool Image::writeFileBMP(const char* const filename, const char* const path)
{
   static const unsigned int BITMAPFILE_SIZE = (MAX_PATH_LEN+MAX_FILENAME_LEN);
   char bitmapFile[BITMAPFILE_SIZE];
   bitmapFile[0] = '\0';

   // append path name
   const char* p1 = path;
   if (p1 != nullptr && std::strlen(path) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p1);
      lcStrcat(bitmapFile, sizeof(bitmapFile), "/");
   }

   // append file name
   const char* p2 = filename;
   if (p2 != nullptr && std::strlen(p2) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p2);
   }

   // Do we have a full path name?
   if (std::strlen(bitmapFile) <= 1) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::writeFileBMP(): invalid file name: " << bitmapFile << std::endl;
      }
      return false;
   }

   // Create the output stream
   std::ofstream* fout = new std::ofstream();

   // Open the output file
   fout->open(bitmapFile, std::ios::out | std::ios::binary);
   if ( !(fout->is_open()) ) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::writeFileBMP(): unable to open bitmap file: " << bitmapFile << std::endl;
      }
      return false;
   }


   // BITMAPFILEHEADER
   unsigned short bfType(0);
   unsigned int   bfSize(0);
   unsigned short bfReserved1(0);
   unsigned short bfReserved2(0);
   unsigned int   bfOffBits(0);

   unsigned int bitmapFileHdrSize =
      sizeof(bfType) + sizeof(bfSize) + sizeof(bfReserved1) + sizeof(bfReserved2) + sizeof(bfOffBits);

   // Number of bytes per row of pixels
   size_t widthBytes = getWidth() * getNumComponents();

   // Offset to bitmap data
   unsigned int offset = bitmapFileHdrSize + sizeof(BITMAPINFOHEADER_X);

   // Image size
   unsigned int isize = getHeight() * static_cast<unsigned int>(widthBytes);

   // File size (active bytes)
   unsigned int size = isize + offset;

   // File size (4 byte words)
   unsigned int sizew = (size + 3)/4;

   // Total file size (with padding)
   unsigned int tsize = sizew*4;

   // Number of padding bytes at the end if the file to give a even word boundary
   unsigned int filePadding = tsize - size;

   // ---
   // Write the bitmap file header (BITMAPFILEHEADER)
   // ---
   bfType = 0x4D42;
   bfSize = tsize;
   bfReserved1 = 0;
   bfReserved2 = 0;
   bfOffBits = offset;

   fout->write(reinterpret_cast<char*>(&bfType),      sizeof(bfType));
   fout->write(reinterpret_cast<char*>(&bfSize),      sizeof(bfSize));
   fout->write(reinterpret_cast<char*>(&bfReserved1), sizeof(bfReserved1));
   fout->write(reinterpret_cast<char*>(&bfReserved2), sizeof(bfReserved2));
   fout->write(reinterpret_cast<char*>(&bfOffBits),   sizeof(bfOffBits));

   // ---
   // Write the bitmap file info
   // ---
   BITMAPINFOHEADER_X bmfi;
   bmfi.biSize = sizeof(BITMAPINFOHEADER_X);
   bmfi.biWidth = getWidth();
   bmfi.biHeight = getHeight();
   bmfi.biPlanes = 1;
   bmfi.biBitCount = getNumComponents() * 8;
   bmfi.biCompression = BI_RGB;
   bmfi.biSizeImage = 0;
   bmfi.biXPelsPerMeter = getXResolutionPPM();
   bmfi.biYPelsPerMeter = getYResolutionPPM();
   bmfi.biClrUsed = 0;
   bmfi.biClrImportant = 0;
   fout->write(reinterpret_cast<char*>(&bmfi), sizeof(bmfi));

   // ---
   // Write the pixel bit map
   // ---
   {
      const GLubyte* bmap = getPixels();
      for (unsigned int i = 0; i < getHeight(); i++) {
         const GLubyte* p = bmap + (i * widthBytes);
         fout->write(reinterpret_cast<const char*>(p), width* PIXEL_SIZE);
      }
      if (filePadding > 0) {
         unsigned int padding = 0;
         fout->write(reinterpret_cast<char*>(&padding), filePadding);
      }
   }

   // close the file
   fout->close();
   delete fout;
   fout = nullptr;

   return true;
}

//------------------------------------------------------------------------------
// readRgbValues() -- read the color components from the file & return the bit map
//------------------------------------------------------------------------------
GLubyte* Image::readRgbValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const
{
    // Allocate the texture memory bits
    unsigned int bmSize = getWidth() * getHeight() * getNumComponents();
    GLubyte* bmap = new GLubyte[bmSize];

    // Position to start of bitmap
    std::fseek(fp, offset, SEEK_SET);

    // Read the bitmap
    size_t widthBytes = (getWidth() * getNumComponents());          // Number of bytes we want per row
    size_t origWidthBytes = (bmfi->biWidth * getNumComponents());   // Original number of bytes per row
    origWidthBytes = (((origWidthBytes + 3) / 4 ) * 4);             // round up to 4 byte boundary
    size_t seekBytes = (origWidthBytes - widthBytes);               // Number of bytes to skip
    for (unsigned int i = 0; i < getHeight(); i++) {
        GLubyte* p = bmap + (i * widthBytes);
        size_t n = std::fread(p, widthBytes, 1, fp);
        if (n == 0) {
            delete[] bmap;
            return nullptr;
        }
        if (seekBytes > 0) {
            std::fseek(fp, static_cast<int>(seekBytes), SEEK_CUR);
        }
    }

    return bmap;
}


//------------------------------------------------------------------------------
// readColorValues() -- read the color table & color index values; returns the bit map
//------------------------------------------------------------------------------
GLubyte* Image::readColorValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const
{
    // Allocate the texture memory bits
    unsigned int bmSize = getWidth() * getHeight() * getNumComponents();
    GLubyte* bmap = new GLubyte[bmSize];

    // Read the color table
    size_t ctSize = 256;
    if (bmfi->biClrUsed > 0) ctSize = bmfi->biClrUsed;
    GLubyte* colorTable = new GLubyte[ctSize*4];
    size_t nItemRead = std::fread(colorTable, 4, ctSize, fp);

    // Position to start of colors
    std::fseek(fp, offset, SEEK_SET);

    // Read the bitmap
    unsigned int nbytes = (((bmfi->biWidth + 3) / 4 ) * 4); // round up to 4 byte boundary
    GLubyte* tbuf = new GLubyte[nbytes];
    for (unsigned int i = 0; bmap != nullptr && i < getHeight(); i++) {
        size_t n = std::fread(tbuf, nbytes, 1, fp);
        if (n > 0) {
            // transfer the colors
            GLubyte* p = bmap + (i * getWidth() * getNumComponents());
            for (unsigned int j = 0; j < getWidth(); j++) {
                GLubyte* q = colorTable + (tbuf[j] * 4);
                *p++ = *q++;
                *p++ = *q++;
                *p++ = *q++;
            }
        }
        else {
            delete[] bmap;
            bmap = nullptr;
        }
    }

    delete[] colorTable;
    delete[] tbuf;
    return bmap;
}

#if 0
//------------------------------------------------------------------------------
// Write a Targa (TGA) format file
// ### not working yet ###
//------------------------------------------------------------------------------
bool Image::writeFileTGA(const char* const filename)
{
   static const unsigned int PIXEL_SIZE = 3; // pixel size in bytes { GL_RGB }

   // error checking:
   if ((width < 1) || (height < 1) || (pixels == 0) || (filename == 0)) {
      std::cout << "Input error in Display::writeTarga()." << std::endl;
      return false;
   }

   // Targa File Header
   struct {
      unsigned char IdLength;
      unsigned char colorMapType;
      unsigned char imageType;
      unsigned short cMapStart;
      unsigned short cMapLength;
      unsigned char cMapDepth;
      unsigned short xOffset;
      unsigned short yOffset;
      unsigned short width;
      unsigned short height;
      unsigned char pixelDepth;
      unsigned char imageDescriptor;
   } TargaHead;

   TargaHead.IdLength = 0;
   TargaHead.colorMapType = 0;
   TargaHead.imageType = 2;
   TargaHead.cMapStart = 0;
   TargaHead.cMapLength = 0;
   TargaHead.cMapDepth = 0;
   TargaHead.xOffset = 0;
   TargaHead.yOffset = 0;
   TargaHead.width = width;
   TargaHead.height = height;
   TargaHead.pixelDepth = 8*PIXEL_SIZE;
   TargaHead.imageDescriptor = 0;

   // Create the output file
   std::ofstream* f = nullptr;

   // Create the output stream
   f = new std::ofstream();

   // Open the output file
   f->open(filename, std::ios::out | std::ios::binary);

   if (f->is_open()) {
      // Write the header values
      f->write( reinterpret_cast<char*>(&TargaHead), sizeof(TargaHead));

      // Write the pixel data to the file
      for (unsigned int j = 0; j < height; j++) {
         const GLubyte* p = pixels + (j * width* PIXEL_SIZE);
         f->write( (char* ) p, width* PIXEL_SIZE);
      }

      // close the file
      f->close();
   }

   // delete the pointer
   delete f;
   f = nullptr;

   return true;
}
#endif

} // End BasicGL namespace
} // End Eaagles namespace
