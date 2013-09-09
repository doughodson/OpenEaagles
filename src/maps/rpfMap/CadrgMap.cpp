// ---------------------------------------------------------------------------------
// Class: CadrgMap
// ---------------------------------------------------------------------------------

#include "openeaagles/maps/rpfMap/CadrgMap.h"
#include "openeaagles/maps/rpfMap/CadrgFile.h"
#include "openeaagles/maps/rpfMap/CadrgFrame.h"
#include "openeaagles/maps/rpfMap/CadrgTocEntry.h"
#include "openeaagles/maps/rpfMap/TexturePager.h"
#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/basicGL/Texture.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

IMPLEMENT_SUBCLASS(CadrgMap, "CadrgMap")
EMPTY_SERIALIZER(CadrgMap)

BEGIN_SLOTTABLE(CadrgMap)
    "pathNames",        // Path names to our TOC file
    "maxTableSize",     // Max table size to set up
    "mapLevel",         // Map level we are going to set (if it exists)
END_SLOTTABLE(CadrgMap)       

BEGIN_SLOT_MAP(CadrgMap)
    ON_SLOT(1, setSlotPathnames, Basic::PairStream)
    ON_SLOT(2, setSlotMaxTableSize, Basic::Number)
    ON_SLOT(3, setSlotMapLevel, Basic::String)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor() 
//------------------------------------------------------------------------------
CadrgMap::CadrgMap()
{
    STANDARD_CONSTRUCTOR()

    stack = 0;
    pathNames = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        cadrgFiles[i] = 0;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        mergedCadrgFiles[i] = 0;
    }
    numFiles = 0;
    curCadrgFile = 0;
    setMaxTableSize(3);
    mapLevel = 0;
    initLevelLoaded = false;
}


//------------------------------------------------------------------------------
// copyData() 
//------------------------------------------------------------------------------
void CadrgMap::copyData(const CadrgMap& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) {
        curCadrgFile = 0;
        stack = 0;
        mapLevel = 0;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (cadrgFiles[i] != 0) cadrgFiles[i]->unref();
        cadrgFiles[i] = org.cadrgFiles[i]->clone();
    }   
    for (int i = 0; i < MAX_FILES; i++) {
        if (mergedCadrgFiles[i] != 0) mergedCadrgFiles[i]->unref();
        mergedCadrgFiles[i] = org.mergedCadrgFiles[i]->clone();
    }   

    if (org.curCadrgFile!= 0) {
        if (curCadrgFile != 0) curCadrgFile->unref();
        curCadrgFile = org.curCadrgFile->clone();
    }

    if (org.stack != 0) {
        if (stack != 0) stack->unref();
        stack = org.stack;
        stack->ref();
    }
    if (org.mapLevel != 0) setMapLevel(org.mapLevel->getString());

    maxTableSize = org.maxTableSize;      
    numFiles = org.numFiles;
    initLevelLoaded = org.initLevelLoaded;
}

//------------------------------------------------------------------------------
// deleteData() 
//------------------------------------------------------------------------------
void CadrgMap::deleteData()
{
    for (int i = 0; i < MAX_FILES; i++) {
        if (cadrgFiles[i] != 0) cadrgFiles[i]->unref();
        cadrgFiles[i] = 0;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (mergedCadrgFiles[i] != 0) mergedCadrgFiles[i]->unref();
        mergedCadrgFiles[i] = 0;
    }
    if (curCadrgFile != 0) curCadrgFile->unref();
    curCadrgFile = 0;

    if (stack != 0) stack->unref();
    stack = 0;
}

//------------------------------------------------------------------------------
// getNumberOfCadrgFiles() - Return total number of all files.
//------------------------------------------------------------------------------
int CadrgMap::getNumberOfCadrgFiles() {
    int num = 0;
    for (int i = 0; i < MAX_FILES; i++) if (mergedCadrgFiles[i] != 0) num++;
    return num;
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotPathnames() - Pathnames to the CADRG A.toc files.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotPathnames(const Basic::PairStream* const x)
{
    bool ok = false;
    int count = 0;
    if (x != 0) {
        // Go through and set up our files based on the path names given
        Basic::List::Item* item = (Basic::List::Item*)x->getFirstItem();
        while (item != 0 && count < MAX_FILES) {
            Basic::Pair* p = (Basic::Pair*)item->getValue();
            if (p != 0) {
                Basic::String* text = dynamic_cast<Basic::String*>(p->object());
                if (text != 0) {
                    ok = setPathName(text->getString());
                }
            }
            item = item->getNext();
            count++;
        }
    }

    sortMaps(count);

    
    return ok;
}

//------------------------------------------------------------------------------
// this goes through and sorts our maps, after we have counted how many we have 
// in.
//------------------------------------------------------------------------------
void CadrgMap::sortMaps(const int count)
{
    // Now we have created all of our CadrgFile, we need to sort them by levels!
    if (count > 0) {
        std::cout << "CadrgMap - loading map files..." << std::endl;
        // Go through and see if we have matching scales
        // list of possible scales
        Basic::String* scales = new Basic::String[MAX_FILES];
        int sCount = 0;
        // We are going to have to create a list of which scale to add and how many to add
        int num2Add[MAX_FILES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < MAX_FILES; i++) {
            // We are going to step through all of the files, and figure out which
            // file has the same levels.  As we do this, we will group all of the entries with
            // the same scale into one File, and then it will include all of the files with the same scale (level)
            if (cadrgFiles[i] != 0) {
                // First file, get the first entry, and figure out the scale
                int nb = cadrgFiles[i]->getNumBoundaries();
                for (int j = 0; j < nb; j++) {
                    CadrgTocEntry* toc  = cadrgFiles[i]->entry(j);
                    if (toc != 0) {
                        if (sCount == 0) {
                            // We are going to put the first scale into the list
                            scales[sCount].setStr(toc->getScale());
                            num2Add[sCount]++;
                            sCount++;
                        }
                        else {
                            // We have to compare our scales with the new TOC scale, to see if we need a new category
                            bool match = false;
                            for (int k = 0; k < sCount; k++) {
                                if (scales[k] != 0) {
                                    if (strcmp(scales[k].getString(), toc->getScale()) == 0) {
                                        match = true;
                                        num2Add[k]++;
                                    }
                                }
                            }
                            // If we don't match it, we create a new scale
                            if (!match) {
                                num2Add[sCount]++;
                                scales[sCount++].setStr(toc->getScale());
                            }
                        }
                    }
                }
            }
        }
        int mFile = 0;
        // Clear our our merged files if we haven't done that
        for (int i = 0; i < MAX_FILES; i++) {
            if (mergedCadrgFiles[i] != 0) mergedCadrgFiles[i]->unref();
            mergedCadrgFiles[i] = 0;
        }
        // Now we know the number to add to the scount, so we can create our new files
        for (int y = 0; y < sCount; y++) {
            mergedCadrgFiles[mFile] = new CadrgFile();
            // Now go through our files, find the one that matches, and add it
            int tocIndex = 0;
            for (int i = 0; i < MAX_FILES; i++) {
                if (cadrgFiles[i] != 0) {
                    int nb = cadrgFiles[i]->getNumBoundaries();
                    for (int j = 0; j < nb; j++) {
                        CadrgTocEntry* toc = cadrgFiles[i]->entry(j);
                        if (toc != 0) {
                            if (strcmp(toc->getScale(), scales[y].getString()) == 0) {
                                mergedCadrgFiles[mFile]->addTocEntry(toc, tocIndex++);
                            }
                        }
                    }
                }
            }
            mFile++;
        }
    }
    //// check our maps now
    //for (int i = 0; i < MAX_FILES; i++) {
    //    if (mergedCadrgFiles[i] != 0) {
    //        std::cout << "FILE # " << i << " : " << std::endl;
    //        int nb = mergedCadrgFiles[i]->getNumBoundaries();
    //        for (int j = 0; j < nb; j++) {
    //            Maps::Rpf::CadrgTocEntry* toc = mergedCadrgFiles[i]->entry(j);
    //            if (toc != 0) {
    //                std::cout << "Boundary # && Scale = " << j << ", " << toc->getScale() << std::endl;
    //            }
    //        }
    //    }
    //}

}

//------------------------------------------------------------------------------
// setSlotMaxTableSize() - Sets our max table size and array up.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotMaxTableSize(const Basic::Number* const x)
{
    bool ok = false;
    if (x != 0) ok = setMaxTableSize(x->getInt());
    return ok;
    
}

//------------------------------------------------------------------------------
// setPathName() - Set our path name, which will also initialize our cadrg file.
//------------------------------------------------------------------------------
bool CadrgMap::setPathName(const char* aGenPathName)
{
    bool ok = false;
    if (aGenPathName != 0) {
        if (cadrgFiles[numFiles] == 0) cadrgFiles[numFiles] = new CadrgFile();
        cadrgFiles[numFiles]->initialize(aGenPathName);
        numFiles++;
        ok = true;
    }
    return ok;
}

// ------------------------------------------------------------------------
// loadFrameToTexture() - Bring in a texture object, a color array, and the texture
// pager, and from that we will setup the texture parameters and load the
// texture object.
// ------------------------------------------------------------------------
void CadrgMap::loadFrameToTexture(BasicGL::Texture* tex, void* pixels)
{
    if (tex != 0) {
        tex->setPixels((GLubyte*)pixels);
        tex->setWrapS(GL_CLAMP);
        tex->setWrapT(GL_CLAMP);
        tex->setWidth(256);
        tex->setHeight(256);
        tex->setFormat(GL_RGB);
        tex->setMagFilter(GL_LINEAR);
        tex->setMinFilter(GL_LINEAR);
        tex->setNumComponents(3);
        tex->loadTexture();
    }
}

//------------------------------------------------------------------------------
// setMaxTableSize() - Sets up our Frame array.
//------------------------------------------------------------------------------
bool CadrgMap::setMaxTableSize(const int x)
{
    maxTableSize = x;
    int size = maxTableSize * 2;
    // Reset our list stack
    if (stack != 0) stack->unref();
    stack = new Basic::List();
    for (int i = 0; i < size; i++) {
        CadrgFrame* t = new CadrgFrame();
        stack->addHead(t);
        t->unref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setZone - Set the current zone we are in.
//------------------------------------------------------------------------------
void CadrgMap::setZone(const int num, TexturePager* tp)
{
    if (curCadrgFile != 0) {
        int numBoundaries = curCadrgFile->getNumBoundaries();
        // Make sure we are within the zone boundaries, and make sure we aren't already using that TOC entry
        if (tp != 0) {
            // If we are a valid boundary, set our TOC, else clear us out.
            if (num != -1 && num < numBoundaries) tp->setToc(curCadrgFile->entry(num));
            else tp->setToc(0);
        }
    }
}

// ------------------------------------------------------------------------
// zoomInMapLevel() - Zooms in to our next map level, if we have one.
// ------------------------------------------------------------------------
bool CadrgMap::zoomInMapLevel()
{
    bool ok = false;
    int index = 0;        
    if (mapLevel != 0) {
        // Early out check, we have zoomed in as far as we can
        if (strcmp(mapLevel->getString(), "5M") == 0) return false;

        Basic::String* newLevel = new Basic::String(); 
        //Zoom in if we can    
        if (strcmp(mapLevel->getString(), "10M") == 0) { 
            newLevel->setStr("5M"); 
            ok = true; 
            index = 1;
        }
        else if (strcmp(mapLevel->getString(), "1:250K") == 0) { 
            newLevel->setStr("10M"); 
            ok = true; 
            index = 2;
        }
        else if (strcmp(mapLevel->getString(), "1:500K") == 0) { 
            newLevel->setStr("1:250K"); 
            ok = true; 
            index = 3;
        }
        else if (strcmp(mapLevel->getString(), "1:1M") == 0) { 
            newLevel->setStr("1:500K"); 
            ok = true; 
            index = 4;
        }
        else if (strcmp(mapLevel->getString(), "1:2M") == 0) { 
            newLevel->setStr("1:1M"); 
            ok = true; 
            index = 5;
        }
        else if (strcmp(mapLevel->getString(), "1:5M") == 0) { 
            newLevel->setStr("1:2M"); 
            ok = true; 
            index = 6;
        }
        if (ok) {
            ok = setMapLevel(newLevel->getString());
            if (!ok) {
                while (!ok && index > 0) {
                    index--;
                    if (index == 5) newLevel->setStr("1:1M");
                    if (index == 4) newLevel->setStr("1:500K");
                    if (index == 3) newLevel->setStr("1:250K");
                    if (index == 2) newLevel->setStr("10M");
                    if (index == 1) newLevel->setStr("5M");
                    ok = setMapLevel(newLevel->getString());
                }
            }
        }
        newLevel->unref();
    }
    return ok;
}

// ------------------------------------------------------------------------
// zoomOutMapLevel() - Zoom out to the next map level, if we have any.
// ------------------------------------------------------------------------
bool CadrgMap::zoomOutMapLevel()
{
    bool ok = false;
    int index = 0;
    if (mapLevel != 0) {
        // Early out check, we have zoomed out as far as we can
        if (strcmp(mapLevel->getString(), "1:5M") == 0) return false;

        Basic::String* newLevel = new Basic::String(); 
        //Zoom in if we can    
        if (strcmp(mapLevel->getString(), "5M") == 0) { 
            newLevel->setStr("10M"); 
            ok = true; 
            index = 6;
        }
        else if (strcmp(mapLevel->getString(), "10M") == 0) { 
            newLevel->setStr("1:250K"); 
            ok = true; 
            index = 5;
        }
        else if (strcmp(mapLevel->getString(), "1:250K") == 0) { 
            newLevel->setStr("1:500K"); 
            ok = true; 
            index = 4;
        }
        else if (strcmp(mapLevel->getString(), "1:500K") == 0) { 
            newLevel->setStr("1:1M"); 
            ok = true; 
            index = 3;
        }
        else if (strcmp(mapLevel->getString(), "1:1M") == 0) { 
            newLevel->setStr("1:2M"); 
            ok = true; 
            index = 2;
        }
        else if (strcmp(mapLevel->getString(), "1:2M") == 0) { 
            newLevel->setStr("1:5M"); 
            ok = true; 
            index = 1;
        }

        if (ok) {
            ok = setMapLevel(newLevel->getString());
            if (!ok) {
                while (!ok && index > 0) {
                    index--;
                    if (index == 5) newLevel->setStr("1:250K");
                    if (index == 4) newLevel->setStr("1:500K");
                    if (index == 3) newLevel->setStr("1:1M");
                    if (index == 2) newLevel->setStr("1:2M");
                    if (index == 1) newLevel->setStr("1:5M");
                    ok = setMapLevel(newLevel->getString());
                }
            }
        }
        // Now set our new level
        newLevel->unref();
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMapLevel() - Initially sets our map resolution level.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotMapLevel(Basic::String* x)
{
    bool ok = false;
    if (mapLevel != 0) {
        mapLevel->unref();
        mapLevel = 0;
    }
    if (x != 0) {
        mapLevel = x;
        mapLevel->ref();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setMapLevel() - See if the given resolution level exists in our files, and if it does,
// set it as the current cadrg file.
//------------------------------------------------------------------------------
bool CadrgMap::setMapLevel(const char* x)
{
    bool found = false;
    int num = getNumberOfCadrgFiles();
    for (int i = 0; i < num; i++) {
        if (mergedCadrgFiles[i] != 0) {
            int nb = mergedCadrgFiles[i]->getNumBoundaries();
            for (int j = 0; j < nb; j++) {
                CadrgTocEntry* toc = mergedCadrgFiles[i]->entry(j);
                if (toc != 0) { 
                    // If we find the right scale, and our current file isn't = to our last file, we set it.
                    if (strcmp(toc->getScale(), x) == 0) {
                        if (curCadrgFile != mergedCadrgFiles[i]) {
                            // This file has our current level on it
                            if (curCadrgFile != 0) curCadrgFile->unref();
                            curCadrgFile = mergedCadrgFiles[i];
                            curCadrgFile->ref();
                            // Now set our map level
                            if (mapLevel != 0) mapLevel->setStr(x);
                            else mapLevel = new Basic::String(x);
                        }
                        found = true;
                    }
                }
            } 
        }    
    }
    return found;
}

//------------------------------------------------------------------------------
// findBestZone() - Find the best zone based on the given lat/lon.
//------------------------------------------------------------------------------
int CadrgMap::findBestZone(const double lat, const double lon)
{
   int t = -1;
   int nb = 0;
    
   if (curCadrgFile != 0) { 
      // Number of boundaries is actually the number of zones in this file!
      nb = curCadrgFile->getNumBoundaries();
      for (int i = 0; i < nb; i++) {
         CadrgTocEntry* toc = curCadrgFile->entry(i);
         if (toc != 0 && toc->isMapImage()) {
            if (toc->isInZone(lat, lon)) return i;
         }
      }
   }
   return t;
}

//------------------------------------------------------------------------------
// getMapImage - Return our moving map image.
//------------------------------------------------------------------------------
MapDrawer* CadrgMap::getMapImage()
{
    MapDrawer* image = 0;
    Basic::Pair* pair = findByType(typeid(MapDrawer));
    if (pair != 0) image = dynamic_cast<MapDrawer*>(pair->object());
    return image;
}


//------------------------------------------------------------------------------
// getMapImage - Return our moving map image.
//------------------------------------------------------------------------------
const MapDrawer* CadrgMap::getMapImage() const
{
    MapDrawer* image = 0;
    Basic::Pair* pair = (Basic::Pair*)findByType(typeid(MapDrawer));
    if (pair != 0) image = dynamic_cast<MapDrawer*>(pair->object());
    return image;
}

// ------------------------------------------------------------------------
// isValidFrame() - Is the row column specified within our current TOCS
// boundary rectangle of frames?
// ------------------------------------------------------------------------
bool CadrgMap::isValidFrame(const int row, const int column, TexturePager* tp)
{
   int vFrames = 0;
   int hFrames = 0;
   bool ok = false;
   if (tp != 0) {
      CadrgTocEntry* currentToc = tp->getToc();
      if (currentToc != 0) {
         vFrames = currentToc->getVertFrames();
         hFrames = currentToc->getHorizFrames();
      }
   }

   // The rows and columns we specified must fall in our frames and subframes
   if (row >= 0 && row < (vFrames * 6) && column >= 0 && column < (hFrames * 6)) ok =  true;

   return ok;
}

//------------------------------------------------------------------------------
// latLonToTileRowColumn() - Takes in a given lat/lon, and based on that, 
// finds the closest tile in the map file to that lat/lon.  It also sets our
// origin row (in (float) pixels) so we know the exact location of the lat/lon,
// for calculating pixel offset later.  This also sets our intial origin row
// and origin column.
//------------------------------------------------------------------------------
void CadrgMap::latLonToTileRowColumn(const double lat, const double lon, float &originRow, float &originCol, int &tileRow, int &tileCol, float &pixelRow, float &pixelCol, TexturePager* tp)
{
    float row = 0, col = 0;

    latLonToPixelRowColumn(lat, lon, row, col, tp);

    // 256 pixels per tile, so we can figure out which tile we are actually on.
    int ppt = 256;

    // Determine our tile location(s) + pixel offset, and then set our original column, which is the aggregate of both
    // Tile row is the int result of the total row offset / pixels per tile (256)
    tileRow = int(row) / ppt;
    // The remainder is the pixel offset of that tile
    pixelRow = row - (tileRow * ppt);
    // Original row 
    originRow = row;

    // Same here, only columns
    tileCol = int(col) / ppt;
    pixelCol = col - (tileCol*ppt);
    originCol = col;
}

// ------------------------------------------------------------------------
// latLonToPixelRowColumn() - This gets the aggregate pixel position of 
// of specified lat/lon, and then sets the origin row and column.
// ------------------------------------------------------------------------
void CadrgMap::latLonToPixelRowColumn(const double lat, const double lon, float &originRow, float &originCol, TexturePager* tp)
{
    double nwLat = 0, nwLon = 0, vInt = 0, hInt = 0;
    if (tp != 0) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != 0) {
            nwLat = currentToc->getNWLat();
            nwLon = currentToc->getNWLon();
            // Interval is the spacing (nominal), in decimal degrees, between each pixel in the NS and EW direction.
            vInt = currentToc->getVertInterval();
            hInt = currentToc->getHorizInterval();
        }
    }

    double deltaLat = nwLat - lat;
    double deltaLon = lon - nwLon;

    if (vInt != 0) originRow = (float)(deltaLat / vInt);
    if (hInt != 0) originCol = (float)(deltaLon / hInt);
}

// ------------------------------------------------------------------------
// getTile() - Gets our pixels.
// ------------------------------------------------------------------------
void* CadrgMap::getPixels(const int row, const int column, TexturePager* tp)
{
    if (tp != 0) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != 0) {
            bool ok = isValidFrame(row, column, tp);
            if (!ok) {
                int vFrames = currentToc->getVertFrames();
                int hFrames = currentToc->getHorizFrames();
                std::cout << "Bad row,column " << row << "," << column << "   " << vFrames * 6 << "," << hFrames * 6 << std::endl;
                return 0;
            }
            

            int frameRow = row / 6;
            int frameCol = column / 6;
            CadrgFrameEntry* frameEntry = currentToc->getFrameEntry(frameRow, frameCol);
            if (frameEntry != 0) {
                frameEntry->loadClut();
                CadrgFrame* frame = frameEntry->getFrame();
                // If we don't have an entry, let's pull one from the stack
                if (frame == 0) {
                    Basic::List::Item* item = stack->getFirstItem();
                    if (item != 0) {               
                        CadrgFrame* x = (CadrgFrame*)(item->getValue());
                        if (x != 0) {
                            stack->removeHead();
                            // Tell it about the frame entry that owns it
                            x->load(frameEntry);
                            // Now on the other side, tell our frame entry about its child
                            frameEntry->setFrame(x);
                        }
                    }
                }
                // Get our frame again, because it now has been loaded
                frame = frameEntry->getFrame();
                if (frame != 0) {
                    // Setup our subframe for decompression
                    Subframe subframe;  
                    // Decompress our subframe
                    frame->decompressSubframe(row, column, subframe);
                    // Set our color based on subframe image
                    for (int i = 0; i < 256; i++) {
                        for (int j = 0; j < 256; j++) {
                            CadrgClut::Rgb rgb = frameEntry->getClut().getColor(subframe.image[j][255-i]);
                            outTile.texel[i][j].red = rgb.red;
                            outTile.texel[i][j].green = rgb.green;
                            outTile.texel[i][j].blue = rgb.blue;
                        }
                    }  
                }
            }
        }
    }

    return (void *) &outTile;
}

// ------------------------------------------------------------------------
// releaseFrame() - Release the current frame within the frame entry 
// at the specific row and column, if it exists.  This frees us space
// and is more efficient if the frame is not being used.
// ------------------------------------------------------------------------
void CadrgMap::releaseFrame(const int row, const int column, TexturePager* tp)
{
    if (tp != 0) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != 0) {
            bool ok = isValidFrame(row, column, tp);
            if (!ok) {
                std::cout << "Bad row,column " << row << "," << column << "   " << currentToc->getVertFrames() * 6 << "," << currentToc->getHorizFrames() * 6 << std::endl;
                return;
            }

            int frameRow = row / 6;
            int frameCol = column / 6;

            CadrgFrameEntry* frameEntry = currentToc->getFrameEntry(frameRow, frameCol);
            if (frameEntry != 0) {
            CadrgFrame* frame = frameEntry->getFrame();
                if (frame != 0) {
                    stack->addHead(frame);
                    frame->unref();
                    frame = 0;
                }
                frameEntry->setFrame(0);
            }
        }
    }
}


// ------------------------------------------------------------------------
// getLevel() - Return our resolution level.
// ------------------------------------------------------------------------
const char* CadrgMap::getLevel()
{
    if (mapLevel != 0) return mapLevel->getString();
    return 0;
}

//------------------------------------------------------------------------------
// updateData() - Update map data.
//------------------------------------------------------------------------------
void CadrgMap::updateData(LCreal dt)
{
    BaseClass::updateData(dt);

    if (!initLevelLoaded && mapLevel != 0 && !mapLevel->isEmpty()) {
        setMapLevel(mapLevel->getString());
        initLevelLoaded = true;
    }
}

//------------------------------------------------------------------------------
// getSlotByIndex() - Get the slot data.
//------------------------------------------------------------------------------
Basic::Object* CadrgMap::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Rpf namespace
} // End Maps namespace
} // End Eaagles namespace

