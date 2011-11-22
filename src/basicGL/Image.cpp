// Image
#include "openeaagles/basicGL/Image.h"
#include <fstream>

// if OpenGL extension is not defined by glu.h, try loading glext.h
#ifndef GL_BGR_EXT
#include <GL/glext.h>
#endif

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

//------------------------------------------------------------------------------
// Define the BITMAP File for non-Windows systems
//------------------------------------------------------------------------------
#if !defined(WIN32)

struct BITMAPINFOHEADER{
    unsigned int  biSize;
    int           biWidth;
    int           biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int  biCompression;
    unsigned int  biSizeImage;
    int           biXPelsPerMeter;
    int           biYPelsPerMeter;
    unsigned int  biClrUsed;
    unsigned int  biClrImportant;
};

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

#endif

namespace Eaagles {
namespace BasicGL {

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
      const GLenum f,         // format: GL_RGB, GL_BGRA_EXT, etc
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
   pixels = 0;

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
   setPixels(0);

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
   if (pixels != 0) {
      // delete the old pixels
      delete[] pixels;
      pixels = 0;
   }

   // copy the new image
   unsigned int size = width * height * numComponents;
   if (newPixels != 0 && size > 0) {
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
   if (p1 != 0 && strlen(p1) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p1);
      lcStrcat(bitmapFile, sizeof(bitmapFile), "/");
   }

   // append file name
   const char* p2 = filename;
   if (p2 != 0 && strlen(p2) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p2);
   }

   // Do we have a full path name?
   if (strlen(bitmapFile) <= 1) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(): invalid file name: " << bitmapFile << std::endl;
      }
      return false;
   }

   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "Image: Loading file: " << filename << std::endl;
   }

   FILE* fp = fopen(bitmapFile,"rb");
   if (fp == 0) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP: unable to open bitmap file: " << bitmapFile << std::endl;
      }
      return false;
   }

   // Read the bitmap file header (BITMAPFILEHEADER)
   unsigned short bfType(0);
   unsigned int  bfSize(0);
   unsigned short bfReserved1(0);
   unsigned short bfReserved2(0);
   unsigned int  bfOffBits(0);

   unsigned int bitmapFileHdrSize =
      sizeof(bfType) + sizeof(bfSize) + sizeof(bfReserved1) + sizeof(bfReserved2) + sizeof(bfOffBits);

   size_t nItemsRead;
   nItemsRead = fread(&bfType, sizeof(bfType), 1, fp);
   nItemsRead = fread(&bfSize, sizeof(bfSize), 1, fp);
   nItemsRead = fread(&bfReserved1, sizeof(bfReserved1), 1, fp);
   nItemsRead = fread(&bfReserved2, sizeof(bfReserved2), 1, fp);
   nItemsRead = fread(&bfOffBits, sizeof(bfOffBits), 1, fp);

   if (bfType != 0x4D42) {
      // Not a bitmap file
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(1): invalid bitmap file: " << bitmapFile << std::endl;
      }
      return false;
   }

   // Read the bitmap file info
   BITMAPINFOHEADER bmfi;
   nItemsRead = fread(&bmfi, sizeof(BITMAPINFOHEADER), 1, fp);

   if (bmfi.biSize != sizeof(BITMAPINFOHEADER) || bmfi.biPlanes != 1) {
      // Not a bitmap file
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Image::readFileBMP(2): invalid bitmap file: " << bitmapFile << std::endl;
      }
      return false;
   }

   // set width & height (truncates to powers of two)
   setWidth(bmfi.biWidth);
   setHeight(bmfi.biHeight);

   // set resolution
   setXResolutionPPM(bmfi.biXPelsPerMeter);
   setYResolutionPPM(bmfi.biYPelsPerMeter);

   // Read the colors
   GLubyte* bmap = 0;
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

   if (bmap == 0) {
      if (isMessageEnabled(MSG_WARNING)) {
         std::cerr << "Image::readFileBMP(3): invalid bitmap file: " << bitmapFile << std::endl;
      }
   }

   // close the file
   fclose(fp);

   // Set the pixel bit map
   if (bmap != 0) setPixels(bmap);

   return (bmap != 0);
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
   if (p1 != 0 && strlen(path) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p1);
      lcStrcat(bitmapFile, sizeof(bitmapFile), "/");
   }

   // append file name
   const char* p2 = filename;
   if (p2 != 0 && strlen(p2) > 0) {
      lcStrcat(bitmapFile, sizeof(bitmapFile), p2);
   }

   // Do we have a full path name?
   if (strlen(bitmapFile) <= 1) {
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
   unsigned int  bfSize(0);
   unsigned short bfReserved1(0);
   unsigned short bfReserved2(0);
   unsigned int  bfOffBits(0);

   unsigned int bitmapFileHdrSize =
      sizeof(bfType) + sizeof(bfSize) + sizeof(bfReserved1) + sizeof(bfReserved2) + sizeof(bfOffBits);

   // Number of bytes per row of pizels
   size_t widthBytes = getWidth() * getNumComponents();

   // Offset to bitmap data
   unsigned int offset = bitmapFileHdrSize + sizeof(BITMAPINFOHEADER);

   // Image size
   unsigned int isize = getHeight() * (unsigned int) widthBytes;

   // File size (active bytes)
   unsigned int size = isize + offset;

   // File size (4 byte words)
   unsigned int sizew = (size + 3)/4;

   // Total file size (with padding)
   unsigned int tsize = sizew*4;

   // Number of padding bytes at the end if the file to give a even word boundery
   unsigned int filePadding = tsize - size;

   // ---
   // Write the bitmap file header (BITMAPFILEHEADER)
   // ---
   bfType = 0x4D42;
   bfSize = tsize;
   bfReserved1 = 0;
   bfReserved2 = 0;
   bfOffBits = offset;

   fout->write( (char*) &bfType, sizeof(bfType));
   fout->write( (char*) &bfSize, sizeof(bfSize));
   fout->write( (char*) &bfReserved1, sizeof(bfReserved1));
   fout->write( (char*) &bfReserved2, sizeof(bfReserved2));
   fout->write( (char*) &bfOffBits, sizeof(bfOffBits));

   // ---
   // Write the bitmap file info
   // ---
   BITMAPINFOHEADER bmfi;
   bmfi.biSize = sizeof(BITMAPINFOHEADER);
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
   fout->write( (char*) &bmfi, sizeof(bmfi));

   // ---
   // Write the pixel bit map
   // ---
   {
      const GLubyte* bmap = getPixels();
      for (unsigned int i = 0; i < getHeight(); i++) {
         const GLubyte* p = bmap + (i * widthBytes);
         fout->write( (char* ) p, width* PIXEL_SIZE);
      }
      if (filePadding > 0) {
         unsigned int padding = 0;
         fout->write( (char* ) &padding, filePadding);
      }
   }

   // close the file
   fout->close();
   delete fout;
   fout = 0;

   return true;
}

//------------------------------------------------------------------------------
// readRgbValues() -- read the color components from the file & return the bit map
//------------------------------------------------------------------------------
GLubyte* Image::readRgbValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER* const bmfi) const
{
    // Alloate the texture memory bits
    unsigned int bmSize = getWidth() * getHeight() * getNumComponents();
    GLubyte* bmap = new GLubyte[bmSize];

    // Position to start of bitmap
   fseek(fp, offset, SEEK_SET);

    // Read the bitmap
    size_t widthBytes = (getWidth() * getNumComponents());          // Number of bytes we want per row
    size_t origWidthBytes = (bmfi->biWidth * getNumComponents());   // Original number of bytes per row
    origWidthBytes = (((origWidthBytes + 3) / 4 ) * 4);             // round up to 4 byte boundary
    size_t seekBytes = (origWidthBytes - widthBytes);               // Number of bytes to skip
    for (unsigned int i = 0; i < getHeight(); i++) {
        GLubyte* p = bmap + (i * widthBytes);
        size_t n = fread(p, widthBytes, 1, fp);
        if (n == 0) {
            delete[] bmap;
            return 0;
        }
        if (seekBytes > 0) {
            fseek(fp, int(seekBytes), SEEK_CUR);
        }
    }

    return bmap;
}


//------------------------------------------------------------------------------
// readColorValues() -- read the color table & color index values; returns the bit map
//------------------------------------------------------------------------------
GLubyte* Image::readColorValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER* const bmfi) const
{
    // Alloate the texture memory bits
    unsigned int bmSize = getWidth() * getHeight() * getNumComponents();
    GLubyte* bmap = new GLubyte[bmSize];

    // Read the color table
    size_t ctSize = 256;
    if (bmfi->biClrUsed > 0) ctSize = bmfi->biClrUsed;
    GLubyte* colorTable = new GLubyte[ctSize*4];
    size_t nItemsRead = fread(colorTable, 4, ctSize, fp);

    // Position to start of colors
   fseek(fp, offset, SEEK_SET);

    // Read the bitmap
    unsigned int nbytes = (((bmfi->biWidth + 3) / 4 ) * 4); // round up to 4 byte boundary
    GLubyte* tbuf = new GLubyte[nbytes];
    for (unsigned int i = 0; bmap != 0 && i < getHeight(); i++) {
        size_t n = fread(tbuf, nbytes, 1, fp);
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
            bmap = 0;
        }
    }

    delete[] colorTable;
    delete[] tbuf;
    return bmap;
}

#if 0
//------------------------------------------------------------------------------
// Write a targa(TGA) format file
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
   std::ofstream* f = 0;

   // Create the output stream
   f = new std::ofstream();

   // Open the output file
   f->open(filename, std::ios::out | std::ios::binary);

   if (f->is_open()) {
      // Write the header values
      f->write( (char*) &TargaHead, sizeof(TargaHead));

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
   f = 0;

   return true;
}
#endif

} // End BasicGL namespace
} // End Eaagles namespace
