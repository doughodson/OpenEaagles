#include "eaagles/basic/Number.h"
#include "eaagles/basic/Support.h"
#include "eaagles/basic1/Field.h"
#include <fstream.h>
#include <math.h>
#include "DigiBitmap.h"
#include "MapDisplay.h"
#include "Digital.h"
#include "formFunc.h"
#include "dib.h"

SUBCLASS_B(DigiBitmap,MapPage)
SUBFORM_B(DigiBitmap,MapPage,"DigiBitmap")

const char* DigiBitmap::slots[] = {
	"bitmap",	// Bitmap filename
	"latitude",	// southwest latitude (deg)
	"longitude",	// southwest longitude (deg)
        "northSize",    // Size of bitmap: North (deg)
        "eastSize",     // Size of bitmap: East (deg)
        "texMapSize",   // Size of the texture map
};
const int DigiBitmap::nslots = (sizeof(DigiBitmap::slots)/sizeof(char*));
const SlotTable DigiBitmap::slottable(DigiBitmap::slots,
             DigiBitmap::nslots, MapPage::slottable);


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DigiBitmap::DigiBitmap()
{
   SET_SLOTTABLE
   map = NULL;
   haveBitmap = FALSE;
   hDibScaled = NULL;
   hDibOriginal = NULL;
   hWnd = NULL;
   pBits = NULL;
   texName = -1;
   bitmapFilename("/b1/data/map/map1.bmp");
   lat = 0.0;
   lon = 0.0;
   ns = 1.0;
   es = 1.0;
   scaledSize = 256;
   // Default Texture Coordinates
   texCoord[0].vec[PF_X] = 0.0f;
   texCoord[0].vec[PF_Y] = 0.0f;
   texCoord[1].vec[PF_X] = 1.0f;
   texCoord[1].vec[PF_Y] = 0.0f;
   texCoord[2].vec[PF_X] = 1.0f;
   texCoord[2].vec[PF_Y] = 1.0f;
   texCoord[3].vec[PF_X] = 0.0f;
   texCoord[3].vec[PF_Y] = 1.0f;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DigiBitmap::copyData(const DigiBitmap& org)
{
   MapPage::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void DigiBitmap::deleteData()
{
   MapPage::deleteData();
}

//------------------------------------------------------------------------------
// updateData --
//------------------------------------------------------------------------------
void DigiBitmap::updateData()
{
   // Update data in base class
   MapPage::updateData();

   // We need a pointer to the main MapDisplay object
   if (map == NULL) {
      map = (MapDisplay*) findContainerByType(MapDisplay::getType());
   }
   if (map == NULL) return;

   // ---
   // Convert corner Latitiude/Longitude to Vertices
   // ---
   if (vertices == NULL)  vertices = new pfVec3[4];

   double latOffset[4] = { 0.0,        0.0, northSize(), northSize() };
   double lonOffset[4] = { 0.0, eastSize(),  eastSize(),         0.0 };

   for (int nv = 0; nv < 4; nv++) {
      // Get the lat/long of the vertex
      double lat = latitude()  + latOffset[nv]; 
      double lon = longitude() + lonOffset[nv];

      // Calculate North & East Distance (NM)
      double north = 0.0;
      double east  = 0.0;
      map->ll2ne( lat, lon, north, east );

      // Calculate Screen Coordinates (inches)
      double XCoord = 0.0;
      double YCoord = 0.0;
      map->ne2xy( north, east, XCoord, YCoord );

      // X, Y, Z Coordinates
      vertices[nv][PF_X] = XCoord;
      vertices[nv][PF_Y] = YCoord;
      vertices[nv][PF_Z] = 0.0;
   }
}

//------------------------------------------------------------------------------
// drawFunc() --
//------------------------------------------------------------------------------
void DigiBitmap::drawFunc()
{
   if (!haveBitmap) return;

   // Setup required mode
   glBlendFunc(GL_ONE,  GL_ZERO);
   glDisable(GL_BLEND);
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);

   // Draw Texture
   glBegin(GL_QUADS);
   for (int i = 0; i < 4; i++) {
      glTexCoord2dv(texCoord[i].vec);
      glVertex3dv( vertices[i].vec );
   }
   glEnd();

   // Return modes
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//------------------------------------------------------------------------------
// readBitmap() -- read the in the digital maps
//------------------------------------------------------------------------------
void DigiBitmap::readBitmap()
{
   hWnd = FindWindow(NULL, "MAP");

   glPixelTransferf(GL_RED_SCALE, 0.66f);
   glPixelTransferf(GL_RED_BIAS, 0.0f);
   glPixelTransferf(GL_GREEN_SCALE, 0.66f);
   glPixelTransferf(GL_GREEN_BIAS, 0.0f);
   glPixelTransferf(GL_BLUE_SCALE, 0.66f);
   glPixelTransferf(GL_BLUE_BIAS, 0.0f);

   cout << "DigiBitmap: Loading file: " << bitmapFile << endl;
   hDibOriginal = OpenDIB(bitmapFile);
   if (hDibOriginal)
   {
      // Convert DIB to a square 32bpp DIB... 
      if (scaleDIB()) {

         // Create the texture map
         glGenTextures(1, &texName);
         glBindTexture(GL_TEXTURE_2D, texName);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
         glTexImage2D(GL_TEXTURE_2D, 0, 3, scaledSize, scaledSize,
                              0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBits);

         // Free the scaled DIB
         if (hDibScaled)
         {
            DeleteObject(hDibScaled);
            hDibScaled = NULL;
            pBits = NULL;
         }
      }

      // Free the original DIB
      GlobalFreeDIB(hDibOriginal);
      hDibOriginal = NULL;

      // Set flag
      haveBitmap = TRUE;
   }
   else {
      haveBitmap = FALSE;
   }
}

//------------------------------------------------------------------------------
// ScaleDIB - Converts the loaded DIB (hDibOriginal) into a
// square 32-bpp DIB Section (hDibScaled)...
//------------------------------------------------------------------------------
bool DigiBitmap::scaleDIB()
{
   // Get stats for original dib...
   if (!hDibOriginal) return FALSE;

   LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER) hDibOriginal;

   // Allocate memory for new 32bpp DIB section. No color table is needed...
   BYTE *abBitmapInfo[sizeof(BITMAPINFOHEADER)];
   BITMAPINFOHEADER *pBMIH = (BITMAPINFOHEADER*)abBitmapInfo;
   memset(pBMIH, 0, sizeof(BITMAPINFOHEADER));

   // Fill in the header info...
   pBMIH->biSize         = sizeof(BITMAPINFOHEADER);
   pBMIH->biWidth        = scaledSize;
   pBMIH->biHeight       = scaledSize;
   pBMIH->biPlanes       = 1;
   pBMIH->biBitCount     = 32;
   pBMIH->biCompression  = BI_RGB;

   // Create the new 32-bpp DIB section...
   HDC hDC = GetDC(hWnd);
   HDC hMemDC = CreateCompatibleDC(hDC);
   ReleaseDC(hWnd, hDC);
   hDibScaled = CreateDIBSection(hMemDC,
                                 (BITMAPINFO*)pBMIH,
                                 DIB_RGB_COLORS,
                                 (VOID **) &pBits,
                                 NULL,
                                 0);

   // Select new DIB Section into DC...
   HBITMAP hbmOld = (struct HBITMAP__ *) SelectObject(hMemDC, hDibScaled);

   // Blt the loaded x-bpp DIB into our new 32bpp DIB...
   //SetStretchBltMode(hMemDC, COLORONCOLOR);
   SetStretchBltMode(hMemDC, HALFTONE);
   StretchDIBBlt(hMemDC,
                  0,
                  0,
                  scaledSize,
                  scaledSize,
                  hDibOriginal,
                  0,
                  0,
                  lpbi->biWidth,
                  lpbi->biHeight,
                  SRCCOPY);

   // Deselect DIB Section and destroy its memory DC...
   if (hbmOld) SelectObject(hMemDC, hbmOld);
   DeleteDC(hMemDC);

   return TRUE;
}

//------------------------------------------------------------------------------
// setSlotByIndex() -- process one GUF pair
//------------------------------------------------------------------------------
void DigiBitmap::setSlotByIndex(const int slotindex, Object* obj)
{
    const int n = MapPage::slottable.n();

    // bitmap: Bitmap filename
    if (slotindex == (n + 1)) {
	if (obj->isClassType(String::getType())) {
           const String* sobj = (String*) obj;
           const char* str = *sobj;
           bitmapFilename(str);
	}
    }

    // latitude: southwest latitude (deg)
    if (slotindex == (n + 2)) {
	if (obj->isClassType(Number::getType())) {
	   const Number* n = (const Number*) obj;
	   latitude(n->getFloat());
	}
    }

    // longitude: southwest longitude (deg)
    if (slotindex == (n + 3)) {
	if (obj->isClassType(Number::getType())) {
	   const Number* n = (const Number*) obj;
	   longitude(n->getFloat());
	}
    }

    // northSize: Size of bitmap: North (deg)
    if (slotindex == (n + 4)) {
	if (obj->isClassType(Number::getType())) {
	   const Number* n = (const Number*) obj;
	   northSize(n->getFloat());
	}
    }

    // eastSize: Size of bitmap: East (deg)
    if (slotindex == (n + 5)) {
	if (obj->isClassType(Number::getType())) {
	   const Number* n = (const Number*) obj;
	   eastSize(n->getFloat());
	}
    }

    // texMapSize: Size of the texture map
    if (slotindex == (n + 6)) {
	if (obj->isClassType(Number::getType())) {
	   const Number* n = (const Number*) obj;
	   int i = n->getInt();
           if (i >= 1024)     i = 1024;
           else if (i >= 512) i = 512;
           else if (i >= 256) i = 256;
           else if (i >= 128) i = 128;
           else if (i >=  64) i =  64;
           else if (i >=  32) i =  32;
           else i = 16;
           scaledSize = i;
	}
    }

    else {
	MapPage::setSlotByIndex(slotindex,obj);
    }
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
ostream& DigiBitmap::serialize(ostream& sout, const int i, const int slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
    	indent(sout,i);
    	sout << "( " << form << endl;
	j = 4;
    }

    if (strlen(bitmapFile) > 0) {
       indent(sout,i+j);
       sout << "bitmap: " << "\"" << bitmapFile << "\"" << endl;
    }

    indent(sout,i+j);
    sout << "latitude: " << latitude() << endl;

    indent(sout,i+j);
    sout << "longitude: " << longitude() << endl;

    indent(sout,i+j);
    sout << "northSize: " << northSize() << endl;

    indent(sout,i+j);
    sout << "eastSize: " << eastSize() << endl;

    MapPage::serialize(sout,i+j,TRUE);

    if ( !slotsOnly ) {
    	indent(sout,i);
    	sout << ")" << endl;
    }

    return sout;
}
