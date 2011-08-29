//------------------------------------------------------------------------------
// Class:	DigiBitmap
//------------------------------------------------------------------------------
#ifndef __DigiBitmap_H__
#define __DigiBitmap_H__

class MapDisplay;
class DigitalMap;
#include "MapPage.h"

//------------------------------------------------------------------------------
// Class: DigiBitmap
// Base class:	Object > Graphic > DigiBitmap
//
// Description:	Digital Map
//
// Form name: DigiBitmap
// Slots:
//  bitmap      <String>    ! Bitmap filename (default: "/b1/data/map/map1.bmp")
//  latitude    <Number>    ! southwest latitude (deg) (default: 0)
//  longitude   <Number>    ! southwest longitude (deg) (default: 0)
//  northSize   <Number>    ! Size of bitmap: North (deg) (default: 1)
//  eastSize    <Number>    ! Size of bitmap: East (deg) (default: 1)
//  texMapSize  <Number>    ! Size of the texture map (default: 256)
//
// Public methods (member functions):
//	char* bitmapFilename()
//		Returns the bitmap file name as a char*.
//
//	bitmapFilename(char* str)
//		Sets the bitmap file name to str.
//
//	double latitude()
//		Returns the latitude.
//
//	double longitude()
//		Returns the longitude.
//
//	double northSize()
//		Returns north size.
//
//	double eastSize()
//		Returns east size.
//
//	northSize(double v)
//		Sets north size to v.
//
//	eastSize(double v)
//		Sets east size to v.
//
//	updateData()
//		Function to update non time critical stuff.
//
//	drawFunc()
//		Function to draw the bitmap.
//
//	readBitmap()
//		Reads the Bitmap.
//
//------------------------------------------------------------------------------
class DigiBitmap : public MapPage {
   DECLARE_SUBCLASS(DigiBitmap,MapPage)
   SUBFORM(MapPage)
   PRINT

public:
   DigiBitmap();

   const char* bitmapFilename() const     { return bitmapFile; }
   void bitmapFilename(const char* str)   { lcStrcpy(bitmapFile,BITMAP_FILE_SIZE,str); }

   double latitude() const                { return lat; }
   double longitude() const               { return lon; }

   void latitude(const double v)          { lat = v; }
   void longitude(const double v)         { lon = v; }

   double northSize() const               { return ns; }
   double eastSize() const                { return es; }

   void northSize(const double v)         { ns = v; }
   void eastSize(const double v)          { es = v; }

   virtual void updateData();
   virtual void drawFunc();
   void readBitmap();

private:
   bool scaleDIB();

   static const unsigned int BITMAP_FILE_SIZE = 128;
   char        bitmapFile[BITMAP_FILE_SIZE];  // Bitmap file name

   MapDisplay* map;           // Pointer to main MapDisplay object
   int         haveBitmap;    // True if we have the bitmap
   pfVec2      texCoord[4];   // Texture Coord
   GLuint      texName;       // Texture
   BYTE*       pBits;         // Pointer to scaled 32bpp Texture DIB bits
   HWND        hWnd;          // Handle to OpenGL window
   HANDLE      hDibOriginal;  // Handle to original DIB
   HBITMAP     hDibScaled;    // Handle to scaled 32bpp Texture DIB
   double      lat,lon;       // Southwest corner laitutde/longitude (deg)
   double      ns, es;        // Size of bitmap north & east (deg)
   int         scaledSize;    // Square Textures x and y dimensions
};

#endif	/* __DigiBitmap_H__ */

