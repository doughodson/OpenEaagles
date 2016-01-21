// ---------------------------------------------------------------------------------
// Class: CadrgMap
//
// Description: This is the main map drawing class, CadrgMap.  This class is derived
// from MapPage, so it inherently knows all the map calculations for range, displacement,
// lat/lon positions, and other data.  This class also has the ability to control and
// draw CADRG map data from RPF files.  To set this up properly, you will need valid
// RPF CADRG data (gnc/jnc, cib, etc..) files.  This page will then need the pathNames
//  to those files, and it will then setup a list of maps.  It then will use the
// MapDrawer class to actually draw the maps themselves, and it does this automatically.
// An example input file would look like this:
// ( Display
//    page: map
//    page:
//       map: ( CadrgMap
//              refLat: 40          // Map page derived slots
//              refLon: -117
//              range: 60
//              mapLevel: "1:5M"    // This tells us the CADRG map level to draw
//              pathNames: {
//                  // Where our gncjnc map data A.toc file is
//                  "C:/projects/data/maps/gncjnc/RPF/"
//                  "C:/some other directory/maps/somemaplevel/RPF"
//                  // You can use as many directories as you like
//                  // it will be sorted by this class.
//              }
//              components: {
//                  // Our CadrgMap will find this drawer if it is a component, and will
//                  // use it to draw the maps!
//                  ( MapDrawer )
//              }
//            )   // end of CadrgMap
// ) // end of display
//
// Subroutines:
// getNumberOfCadrgFiles() - Return total number of all files
//       int CadrgMap::getNumberOfCadrgFiles()
//
// setSlotPathnames() - Pathnames to the CADRG A.toc files.
//       bool CadrgMap::setSlotPathnames(const Basic::PairStream* const x)
//
// setSlotMaxTableSize() - Sets our max table size and array up
//       bool CadrgMap::setSlotMaxTableSize(const Basic::Number* const x)
//
// setPathName() - Set our path name, which will also initialize our cadrg file
//       bool CadrgMap::setPathName(const char* aGenPathName)
//
// loadFrameToTexture() - Bring in a texture object, a color array, and the texture
// pager, and from that we will setup the texture parameters and load the texture object.
//       void CadrgMap::loadFrameToTexture(BasicGL::Texture* tex, void* pixels)
//
// setMaxTableSize() - Sets up our Frame array
//       bool CadrgMap::setMaxTableSize(const int x)
//
// setZone - Set the current zone we are in
//       void CadrgMap::setZone(const int num, TexturePager* tp)
//
// zoomInMapLevel() - Zooms in to our next map resolution level, if we have one.
//       bool CadrgMap::zoomInMapLevel()
//
// zoomOutMapLevel() - Zoom out to the next map resolution level, if we have any
//       bool CadrgMap::zoomOutMapLevel()
//
// setSlotMapLevel() - Initially sets our map resolution level
//       bool CadrgMap::setSlotMapLevel(Basic::String* x)
//
// setMapLevel() - See if the given resolution level exists in our files, and if it does,
// set it as the current cadrg file.
//       bool CadrgMap::setMapLevel(const char* x)
//
// findBestZone() - Find the best geographical zone based on the given lat/lon
//      int CadrgMap::findBestZone(const double lat, const double lon)
//
// getMapImage - Return our moving map image
//      MapDrawer* CadrgMap::getMapImage()
//
// getMapImage - Return our moving map image (const version)
//      const MapDrawer* CadrgMap::getMapImage() const
//
// isValidFrame() - Is the row/column specified within our current TOCS
// boundary rectangle?
//      bool CadrgMap::isValidFrame(const int row, const int column, TexturePager* tp)
//
// latLonToTileRowColumn() - Takes in a given lat/lon, and based on that, finds
// the closest tile in the map file to that lat/lon.  It also sets our for origin
// row (in (float) pixels) so we know the exact location of the lat/lon, calculating
// pixel offset later.  This also sets our initial origin row and origin column.
//
//      void CadrgMap::latLonToTileRowColumn(const double lat, const double lon, float &originRow,
//                                           float &originCol, int &tileRow, int &tileCol, float &pixelRow,
//                                           float &pixelCol, TexturePager* tp)
//
// latLonToPixelRowColumn() - This gets the aggregate pixel position of of specified lat/lon,
// and then sets the origin row and column.
//      void CadrgMap::latLonToPixelRowColumn(const double lat, const double lon, float &originRow,
//                                            float &originCol, TexturePager* tp)
//
// getTile() - Gets our pixels.
//      void* CadrgMap::getPixels(const int row, const int column, TexturePager* tp)
//
// releaseFrame() - Release the current frame within the frame entry at the specific row and column,
// if it exists.  This frees us space and is more efficient if the frame is not being used.
//      void CadrgMap::releaseFrame(const int row, const int column, TexturePager* tp)
//
// getLevel() - Return our resolution level.
//      const char* CadrgMap::getLevel()
//
// updateData() - Update map data.
//      void CadrgMap::updateData(LCreal dt)
//
// getSlotByIndex() - Get the slot data.
//      Basic::Object* CadrgMap::getSlotByIndex(const int si)
//
// ---------------------------------------------------------------------------------
#ifndef __oe_Maps_Rpf_CadrgMap_H__
#define __oe_Maps_Rpf_CadrgMap_H__

#include "openeaagles/basicGL/MapPage.h"

namespace oe {
namespace Basic { class List; }
namespace BasicGL { class Texture; }
namespace Maps {
namespace Rpf {

class CadrgFile;
class TexturePager;
class MapDrawer;

class CadrgMap : public BasicGL::MapPage
{
    DECLARE_SUBCLASS(CadrgMap, BasicGL::MapPage)

public:
    CadrgMap();

    // A color structure to hold GLubytes color
    struct RGBColor {
        GLubyte red;
        GLubyte green;
        GLubyte blue;
    };

    // Quick color array for holding our RGB values
    struct ColorArray {
        // [row][column] 3 components per pixel
        RGBColor texel[256][256];
    };

    // Set functions - these functions must be set for it to work properly
    virtual bool setPathName(const char* aGenPathName);
    virtual bool setMaxTableSize(const int x);

    // More set functions
    virtual void setZone(const int num, TexturePager* tp);
    virtual int findBestZone(const double lat, const double lon);
    virtual bool isValidFrame(const int row, const int column, TexturePager* tp);

    // Zooms in / out of our map.
    virtual bool zoomInMapLevel();
    virtual bool zoomOutMapLevel();

    // Conversions
    virtual void latLonToTileRowColumn(const double lat, const double lon, float &originRow, float &originCol, int &tileRow, int &tileCol, float &pixelRow, float &pixelCol, TexturePager* tp);
    virtual void latLonToPixelRowColumn(const double lat, const double lon, float &originRow, float &originCol, TexturePager* tp);

    // Frame operations
    virtual void releaseFrame(const int row, const int column, TexturePager* tp);
    virtual void loadFrameToTexture(BasicGL::Texture* tex, void* ptr);

    // Get pixels
    virtual void* getPixels(const int row, const int column, TexturePager* tp);
    int getNumberOfCadrgFiles();
    const char* getLevel();

    // Finds a specific file by string
    bool setMapLevel(const char* x);

    MapDrawer* getMapImage();
    const MapDrawer* getMapImage() const;

    int getMaxTableSize()   { return maxTableSize; }

    void updateData(const LCreal dt = 0.0) override;

    virtual void sortMaps(const int count);             // simple function to sort our maps.

protected:
    // Slot functions
    bool setSlotPathnames(const Basic::PairStream* const x);
    bool setSlotMaxTableSize(const Basic::Number* const x);
    bool setSlotMapLevel(Basic::String* x);

private:
    static const int MAX_FILES = 10;            // Holds the maximum number of cadrg files we can hold
    CadrgFile* cadrgFiles[MAX_FILES];           // List of cadrg files
    CadrgFile* mergedCadrgFiles[MAX_FILES];     // Merged list of cadrg files from all paths
    int numFiles;                               // Number of CADRG files we are holding
    CadrgFile* curCadrgFile;                    // The current CADRG file we are using
    Basic::PairStream* pathNames;               // Holds our path names for the directories of our files
    int maxTableSize;                           // Our max table size
    Basic::List* stack;                         // Stack of unused frames
    ColorArray outTile;                         // Holds the tile color information
    Basic::String* mapLevel;                    // Our map "level" we are ("1:500K", etc..)
    bool initLevelLoaded;                       // Has our initial map level been loaded?
};

}  // End Rpf namespace
}  // End Maps namespace
}  // End oe namespace

#endif

