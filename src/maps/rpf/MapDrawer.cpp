// -------------------------------------------------------------------------------
// Class: MapDrawer
// -------------------------------------------------------------------------------

#include "openeaagles/maps/rpf/MapDrawer.h"
#include "openeaagles/maps/rpf/CadrgMap.h"
#include "openeaagles/maps/rpf/TexturePager.h"
#include "openeaagles/maps/rpf/CadrgTocEntry.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/graphics/Texture.h"

#include <cmath>

namespace oe {
namespace maps {
namespace rpf {

IMPLEMENT_SUBCLASS(MapDrawer,"MapDrawer")
EMPTY_SERIALIZER(MapDrawer)

BEGIN_SLOTTABLE(MapDrawer)
    "mapIntensity",     // 1) Map intensity
    "drawGridMode",     // 2) Draw the outline grid?
    "showMap",          // 3) Show our map data or not (if not, we still calculate zones)
END_SLOTTABLE(MapDrawer)

BEGIN_SLOT_MAP(MapDrawer)
    ON_SLOT(1, setSlotMapIntensity, base::Number)
    ON_SLOT(2, setSlotDrawGridMode, base::Number)
    ON_SLOT(3, setSlotShowMap, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
//  Constructor()
//------------------------------------------------------------------------------
MapDrawer::MapDrawer()
{
    STANDARD_CONSTRUCTOR()

    // Default our values
    gridSize = 0;

    // Initialize our pagers, zones, and texture info
    for (int i = 0; i < MAX_PAGERS; i++) {
        pagers[i] = nullptr;
        zones[i] = -1;
        textureRow[i] = 0;
        textureCol[i] = 0;
        pixelRow[i] = 0.0;
        pixelCol[i] = 0.0;
        scalingNorth[i] = 1;
        scalingEast[i] = 1;
    }

    // The cadrg map
    myMap = nullptr;

    pixPerTile = 256.0;

    drawGrid = false;
    mapIntensity = 1.0f;

    sinAng = 0.0;
    cosAng = 0.0;

    showMap = true;
    vpWL = 0.0;
    vpHL = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- Copy member data.
//------------------------------------------------------------------------------
void MapDrawer::copyData(const MapDrawer& org, const bool cc)
{
    // Copy baseclass stuff first
    BaseClass::copyData(org);

    if (cc) {
        myMap = nullptr;
        for (int i = 0; i < MAX_PAGERS; i++) pagers[i] = nullptr;
    }

    if (org.myMap != nullptr) {
        if (myMap != nullptr) myMap->unref();
        myMap = org.myMap->clone();
        myMap->ref();
    }

    for (int i =0 ; i < MAX_PAGERS; i++) {
        if (pagers[i] != nullptr) {
            pagers[i]->unref();
            pagers[i] = nullptr;
        }
        if (org.pagers[i] != nullptr) {
            pagers[i] = org.pagers[i]->clone();
            pagers[i]->ref();
        }
        scalingNorth[i] = org.scalingNorth[i];
        scalingEast[i] = org.scalingEast[i];
        zones[i] = org.zones[i];
        textureRow[i] = org.textureRow[i];
        textureCol[i] = org.textureCol[i];
        pixelRow[i] = org.pixelRow[i];
        pixelCol[i] = org.pixelCol[i];
        originRow[i] = org.originRow[i];
        originCol[i] = org.originCol[i];
    }

    gridSize = org.gridSize;
    drawGrid = org.drawGrid;
    mapIntensity = org.mapIntensity;
    sinAng = org.sinAng;
    cosAng = org.cosAng;
    pixPerTile = org.pixPerTile;
}

//------------------------------------------------------------------------------
// deleteData() -
//------------------------------------------------------------------------------
void MapDrawer::deleteData()
{
    if (myMap != nullptr) {
        myMap->unref();
        myMap = nullptr;
    }

    for (int i = 0; i < MAX_PAGERS; i++) {
        if (pagers[i] != nullptr) {
            pagers[i]->unref();
            pagers[i] = nullptr;
        }
    }
}

// SLOT function(s)
//------------------------------------------------------------------------------
// setSlotMapIntensity() -
//------------------------------------------------------------------------------
bool MapDrawer::setSlotMapIntensity(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setMapIntensity(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotDrawGridMode() -
//------------------------------------------------------------------------------
bool MapDrawer::setSlotDrawGridMode(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setDrawGridMode(x->getBoolean());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotShowMap() -
//------------------------------------------------------------------------------
bool MapDrawer::setSlotShowMap(const base::Number* const x)
{
   bool ok = false;
   if (x != nullptr) ok = setShowMap(x->getBoolean());
   return ok;
}

//------------------------------------------------------------------------------
// setGridSize() - Does our initial setup.
//------------------------------------------------------------------------------
bool MapDrawer::setGridSize(const int aGridSize)
{
    bool ok = false;
    int rem = aGridSize % 2;
    // Create a new tile pager with the new values
    if (aGridSize > 0 && rem) {
        gridSize = aGridSize;
        for (int i = 0; i < MAX_PAGERS; i++) {
            if (pagers[i]!= nullptr) pagers[i]->unref();
            pagers[i] = new TexturePager();
            pagers[i]->setSize(gridSize);
        }
        ok = true;
    }
    else {
        std::cout << "MapDrawer::setGridSize() - grid size MUST be greater than 0 and an odd number!" << std::endl;
        return ok;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setShowMap() - Show our map or not?
//------------------------------------------------------------------------------
bool MapDrawer::setShowMap(const bool x)
{
   showMap = x;
   return true;
}

//------------------------------------------------------------------------------
// setMap() - sets our parent map.
//------------------------------------------------------------------------------
void MapDrawer::setMap(CadrgMap* map)
{
    bool done = false;

    if (myMap != nullptr && myMap != map) {
        for (int i = 0; i < MAX_PAGERS; i++) {
            if (pagers[i] != nullptr) pagers[i]->flushTextures();
            myMap->unref();
        }
        myMap = map;
        myMap->ref();
        done = true;
    }
    else if (myMap == nullptr && map != nullptr) {
        myMap = map;
        myMap->ref();
        done = true;
    }


    if (done) {
        setGridSize(myMap->getMaxTableSize());
        for (int i = 0; i < MAX_PAGERS; i++) {
            if (pagers[i] != nullptr) pagers[i]->setMap(myMap);
        }
    }
}

//------------------------------------------------------------------------------
// updateZone() - Update our zone in accordance with our texture pager.
//------------------------------------------------------------------------------
void MapDrawer::updateZone(int curZone, int &oldZone, const int idx)
{
    if (myMap != nullptr) {
        if (pagers[idx] != nullptr) {
            // Quick check for flushing tiles.
            if (curZone == -1) pagers[idx]->flushTextures();
            else if (curZone != oldZone) {
                pagers[idx]->flushTextures();
                oldZone = curZone;
            }
        }
        myMap->setZone(curZone, pagers[idx]);
    }
}

//------------------------------------------------------------------------------
// drawFunc() - Draw the map.
//------------------------------------------------------------------------------
void MapDrawer::drawFunc()
{
    GLdouble ocolor[4];
    // Get our old color
    glGetDoublev(GL_CURRENT_COLOR, &ocolor[0]);

    GLdouble dLeft = 0, dRight = 0, dBottom = 0, dTop = 0, dNear = 0, dFar = 0;
    //double lat = 0, lon = 0;

    // Determine our rotation, if needed.
    if (getDisplay() != nullptr) getDisplay()->getOrtho(dLeft, dRight, dBottom, dTop, dNear, dFar);

    if (myMap != nullptr) {
        const double rLat = myMap->getReferenceLatDeg();
        const double rLon = myMap->getReferenceLonDeg();
        const int refZone = myMap->findBestZone(rLat, rLon);
        if (refZone != -1) {
            // Determine our CENTER tile and pixel position
            myMap->latLonToTileRowColumn(rLat, rLon, originRow[CENTER_PAGER], originCol[CENTER_PAGER], textureRow[CENTER_PAGER], textureCol[CENTER_PAGER], pixelRow[CENTER_PAGER], \
                pixelCol[CENTER_PAGER], pagers[CENTER_PAGER]);
            // Take the distance in nautical miles, and then convert to degrees
            const double quickRange = getRange();
            const double disDegN = quickRange / 60.0f;
            const double disDegE = static_cast<double>(quickRange / (60.0 * getCosRefLat()));
            // Get the space (in latitude degrees) between each pixel
            CadrgTocEntry* te = pagers[CENTER_PAGER]->getToc();
            double n = 1, e = 1;
            if (te != nullptr) {
                n = static_cast<double>(te->getVertInterval());
                e = static_cast<double>(te->getHorizInterval());
            }

            // OK, so we know how far we want to look out (disDeg, and how far it is per pixel, so we can find
            // the total amount of pixels by dividing disDeg / x.
            vpHL = disDegN / n;
            vpWL = disDegE / e;

            // Scale our viewport by the ratio of our map page to our actual ortho, to make our background map fit into our range circle
            const double rad = getOuterRadius();
            const double radRatio = static_cast<double>(dTop / rad);
            vpHL *= radRatio;
            vpWL *= radRatio;

            // Now stretch and shrink our ortho based on if we are track up
            const double newVPHL = vpHL;
            const double newVPWL = vpWL;

            // Force our ortho before drawing
            getDisplay()->forceOrtho(-newVPWL, newVPWL,  -newVPHL, newVPHL, -2, 2);

            // Update our current reference zone
            updateZone(refZone, zones[CENTER_PAGER], CENTER_PAGER);

            // Tell our center pager to draw the map
            drawMap(zones[CENTER_PAGER], CENTER_PAGER);

            // Check to see if we need to have zones around us (depending on the range)
            const double rngDeg = (quickRange * 2) / 60.0f;
            const int tZone = myMap->findBestZone(rLat + rngDeg, rLon);

            // Now determine if there are other zones to draw
            if (tZone != zones[CENTER_PAGER]) {
                updateZone(tZone, zones[TOP_PAGER], TOP_PAGER);
                myMap->latLonToTileRowColumn(rLat, rLon, originRow[TOP_PAGER], originCol[TOP_PAGER], textureRow[TOP_PAGER], textureCol[TOP_PAGER], pixelRow[TOP_PAGER], \
                pixelCol[TOP_PAGER], pagers[TOP_PAGER]);
                // Draw the map
                drawMap(zones[TOP_PAGER], TOP_PAGER);
            }
            else pagers[TOP_PAGER]->flushTextures();

            // Set our reference zone.
            myMap->setZone(zones[CENTER_PAGER], pagers[CENTER_PAGER]);
        }
    }

    // Set our ortho and our color back to its original state after we draw.
    getDisplay()->forceOrtho(dLeft, dRight, dBottom, dTop, dNear, dFar);
    glColor4dv(ocolor);
}

// -----------------------------------------------------------------------
// determineScaling() - Determine our reference scaling for each zone.
// -----------------------------------------------------------------------
void MapDrawer::determineScaling(const int idx)
{
    // Scaling issues here, because one zone
    if (pagers[CENTER_PAGER] != nullptr && pagers[idx] != nullptr && pagers[idx] != pagers[CENTER_PAGER]) {
        CadrgTocEntry* toc = pagers[CENTER_PAGER]->getToc();
        double centerHi = 0, centerVi = 0, currHi = 0, currVi = 0;
        if (toc != nullptr) {
            centerHi = toc->getHorizInterval();
            centerVi = toc->getVertInterval();
        }
        toc = pagers[idx]->getToc();
        if (toc != nullptr) {
            currHi = toc->getHorizInterval();
            currVi = toc->getVertInterval();
        }

        if (centerHi != 0 && centerVi != 0) {
            scalingEast[idx] = static_cast<float>(currHi / centerHi);
            scalingNorth[idx] = static_cast<float>(currVi / centerVi);
        }
    }
}


// -----------------------------------------------------------------------
// drawMap() - Called from draw fun, it tells our specific map to draw.
// -----------------------------------------------------------------------
void MapDrawer::drawMap(const int zone, const int idx)
{
    if (myMap != nullptr && pagers[idx] != nullptr && showMap && getDisplay() != nullptr){
        // Update the tiles for the pager
        pagers[idx]->updateTextures(textureRow[idx], textureCol[idx]);
        // Set up for drawing
        lcColor3(mapIntensity, mapIntensity, mapIntensity);
        glPushMatrix();
            // Not centered, move the whole map down the displacement value.
            if (!getCentered()) {
                const double dis = getOuterRadius();
                //double scale = getScale();
                const double myScale = vpHL / dis;
                glTranslatef(0, static_cast<GLfloat>(getDisplacement() * myScale), 0);
            }
            glTranslatef(0, 0, -0.1f);
            sinAng = 0.0f;
            cosAng = 1.0f;

            // Set the scale, if not the CENTER_PAGER
            if (idx != CENTER_PAGER) determineScaling(idx);

            bool nu = getNorthUp();
            if (!nu) {
                const GLfloat hdg = static_cast<GLfloat>(getHeadingDeg());
                glRotatef(hdg, 0.0f, 0.0f, 1.0f);
                sinAng = static_cast<double>(std::sin(hdg * static_cast<double>(base::Angle::D2RCC)));
                cosAng = static_cast<double>(std::cos(hdg * static_cast<double>(base::Angle::D2RCC)));
            }

            // Translate down the pixels first
            const float transPixelX = -pixelCol[idx] * scalingEast[idx];
            const float transPixelY =  pixelRow[idx] * scalingNorth[idx];

            // Translate to the next tile
            glTranslatef(transPixelX, transPixelY, 0.0f);
            TextureTable& tbl = pagers[idx]->getTable();
            int si = tbl.getLowerBoundIndex();

            int i1 = si;
            int i = 0;
            int lb = 0, ub = 0;

            // Enable texturing
            glEnable(GL_TEXTURE_2D);
            lb = tbl.getLowerBoundIndex();
            ub = tbl.getUpperBoundIndex();
            for (i = lb; i <= ub; i++) {
                int j1 = si;
                for (int j = lb; j <= ub; j++) {
                    drawTexture(i1, j1, idx);
                    j1++;
                }
                i1++;
            }
            glDisable(GL_TEXTURE_2D);

            // Done drawing tiles, now draw grid, if selected to draw.

            if (drawGrid) {
                i1 = si;
                for (i = lb; i <= ub; i++) {
                    int j1 = si;
                    for (int j = lb; j <= ub; j++) {
                        goDrawGrid(i1, j1, idx);
                        j1++;
                    }
                    i1++;
                }
            }
        glPopMatrix();
    }
}

// -------------------------------------------------------------------------------------------------------
// drawTexture() - Draws a tile at a specific position.
// -------------------------------------------------------------------------------------------------------
void MapDrawer::drawTexture(const int row, const int column, const int idx)
{
    if (pagers[idx] != nullptr && myMap != nullptr && getDisplay() != nullptr) {
        TextureTable& tbl = pagers[idx]->getTable();
        graphics::Texture* newTex = dynamic_cast<graphics::Texture*>(tbl.getTexture(row, column));
        if (newTex != nullptr) {
            // Bind our texture and set up our modulation
            glBindTexture(GL_TEXTURE_2D, newTex->getTexture());
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            const double transX = static_cast<double>(column * pixPerTile * scalingEast[idx]);
            const double transY = static_cast<double>(-row * pixPerTile * scalingNorth[idx]);
            glPushMatrix();
                glTranslatef(static_cast<GLfloat>(transX), static_cast<GLfloat>(transY), 0.0f);
                glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); lcVertex2(0.0f, -pixPerTile * scalingNorth[idx]);
                    glTexCoord2f(1.0f, 0.0f); lcVertex2(pixPerTile * scalingEast[idx], -pixPerTile * scalingNorth[idx]);
                    glTexCoord2f(1.0f, 1.0f); lcVertex2(pixPerTile * scalingEast[idx], 0.0f);
                    glTexCoord2f(0.0f, 1.0f); lcVertex2(0.0f, 0.0f);
                glEnd();
            glPopMatrix();
        }
    }
}

//------------------------------------------------------------------------------
// goDrawDGrid() - Draw the grid, if needed
//------------------------------------------------------------------------------
void MapDrawer::goDrawGrid(const int row, const int column, const int idx)
{
    glPushMatrix();
        glTranslatef(static_cast<GLfloat>(column * pixPerTile), static_cast<GLfloat>(-row * pixPerTile), 0.5f);
        glColor3f(1,0,0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            lcVertex2(0.0f, -pixPerTile * scalingNorth[idx]);
            lcVertex2(pixPerTile * scalingEast[idx], -pixPerTile * scalingNorth[idx]);
            lcVertex2(pixPerTile * scalingEast[idx], 0.0f);
            lcVertex2(0.0f, 0.0f);
        glEnd();
    glPopMatrix();
}

//------------------------------------------------------------------------------
// Update data
//------------------------------------------------------------------------------
void MapDrawer::updateData(const double dt)
{
    // Update our baseclass
    BaseClass::updateData(dt);

    // Set our map if we don't have one
    if (myMap == nullptr) {
        CadrgMap* map = dynamic_cast<CadrgMap*>(findContainerByType(typeid(CadrgMap)));
        if (map != nullptr) setMap(map);
    }
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* MapDrawer::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End rpf namespace
} // End maps namespace
} // End oe namespace

