// -------------------------------------------------------------------------------
// Class: MapDrawer
//
// Description:
// This is the actual class that does the drawing of the map.  It cycles through
// it's texture pagers and tells them where to reference, and how to draw.  Then
// texture pages draws the tiles.
//
// Subroutines:
//
// setSlotMapIntensity() -
//      bool MapDrawer::setSlotMapIntensity(const Basic::Number* const x)
//
// setSlotDrawGridMode() -
//      bool MapDrawer::setSlotDrawGridMode(const Basic::Number* const x)
//
// setSlotShowMap() -
//      bool MapDrawer::setSlotShowMap(const Basic::Number* const x)
//
// setGridSize() - does our initial setup.
//      bool MapDrawer::setGridSize(const int aGridSize)
//
// setShowMap() - show our map or not?
//      bool MapDrawer::setShowMap(const bool x)
//
// setMap() - sets our parent map.
//      void MapDrawer::setMap(CadrgMap* map)
//
// updateZone() - update our zone in accordance with our texture pager.
//      void MapDrawer::updateZone(int curZone, int &oldZone, const int idx)
//
// drawFunc() - Draw the map.
//      void MapDrawer::drawFunc()
//
// determineScaling() - determine our reference scaling for each zone.
//      void MapDrawer::determineScaling(const int idx)
//
// drawMap() - called from draw fun, it tells our specific map to draw.
//      void MapDrawer::drawMap(const int zone, const int idx)
//
// drawTexture() - draws a tile at a specific position.
//      void MapDrawer::drawTexture(const int row, const int column, const int idx)
//
// getSlotByIndex() 
//      Basic::Object* MapDrawer::getSlotByIndex(const int si)
//
// -------------------------------------------------------------------------------

#ifndef __Eaagles_Maps_Rpf_MapDrawer_H__
#define __Eaagles_Maps_Rpf_MapDrawer_H__

#include "openeaagles/basicGL/MapPage.h"

namespace Eaagles {
namespace Basic { class String; }
namespace Maps {
namespace Rpf {

class TexturePager;
class CadrgMap;

class MapDrawer : public BasicGL::MapPage
{
    DECLARE_SUBCLASS(MapDrawer, BasicGL::MapPage)

public:
    MapDrawer();

    // Set functions
    virtual bool setDrawGridMode(const bool dg)             { drawGrid = dg; return true; }
    virtual bool setMapIntensity(const LCreal intensity)    { mapIntensity = intensity; return true; }
    virtual bool setShowMap(const bool x);
    virtual void setMap(CadrgMap* map);
    // this function initializes our size
    virtual bool setGridSize(const int aGridSize);

    // Get functions
    LCreal getMapIntensity() { return mapIntensity; }
    virtual void updateZone(int zone, int& selected, const int idx);
            
    // BasicGL::Graphic interface
    virtual void drawFunc();
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.000000);
                  
protected:
    bool setSlotMapIntensity(const Basic::Number* const x);
    bool setSlotDrawGridMode(const Basic::Number* const x);
    bool setSlotShowMap(const Basic::Number* const x);

private:
    static const int MAX_PAGERS = 2;

    enum { CENTER_PAGER, TOP_PAGER };   // Names of our pagers
    void drawTexture(const int row, const int column, const int idx);
    void goDrawGrid(const int row, const int column, const int idx);
    void drawMap(const int zone, const int idx);
    // Function to determine how to scale our non-center zones to line up with the center zone
    void determineScaling(const int idx);        
    CadrgMap* myMap;                  // The map 
    LCreal pixPerTile;                // Number of pixels per tile
    int gridSize;                     // Size of the map grid 1x1 or 3x3 or NxN                
    bool drawGrid;                    // Will the grid be drawn?
    LCreal mapIntensity;              // Map brightness factor
   
    LCreal sinAng;                    // SIN of heading
    LCreal cosAng;                    // COS of heading

    bool showMap;                       // Flag used to command the drawing of actual textures or not

    LCreal vpWL;                        // Viewport width and height
    LCreal vpHL;                        // Viewport width and height


    TexturePager* pagers[MAX_PAGERS];   // List of texture pagers


    float scalingNorth[MAX_PAGERS];     // Scaling of our north size from center pager
    float scalingEast[MAX_PAGERS];      // Scaling of our east size from center pager
    int zones[MAX_PAGERS];              // Holds our zones
    int textureRow[MAX_PAGERS];         // Holds our row numbers of the textures to draw
    int textureCol[MAX_PAGERS];         // Holds our col numbers of the textures to draw
    float pixelRow[MAX_PAGERS];         // Pixel row position of the textures to draw
    float pixelCol[MAX_PAGERS];         // Pixel col position of the textures to draw
    float originRow[MAX_PAGERS];        // Pixel + texture row of the textures to draw
    float originCol[MAX_PAGERS];        // Pixel + texture col of the textures to draw
};

} // End Rpf namespace
} // End Maps namespace
} // End Eaagles namespace

#endif

