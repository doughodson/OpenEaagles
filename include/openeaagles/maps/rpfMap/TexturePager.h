// ------------------------------------------------------------------------------
// Class:  Basic::Object -> TexturePager
// 
// Description: holds the texture tables and gets "paged" through the textures, freeing,
// re-using, and loading new textures in the table as needed.  This is driven
// by the MapDrawer class.

// Subroutines:
// setSize() - Sets the size of our table.
//      void TexturePager::setSize(int tableSize)

// setMap() - Sets our parent map.
//      void TexturePager::setMap(CadrgMap* newMap)

// updateTextures() - This takes in the current row and column that our
// reference lat/lon is at, and if we have moved, we adjust our textures
// accordingly, freeing any textures that are now out of the boundary, re-using
// any that are still in and adding any new ones.
//      void TexturePager::updateTextures(const int tRow, const int tCol)

// setToc() - Sets the toc entry we are associated to.
//      void TexturePager::setToc(CadrgTocEntry* x)

// freeTextures() - These are the textures that were used before, but are now
// out of the boundary from our center, so we release them back to the stack.
//      void TexturePager::freeTextures()

// reuseTextures() - If we have moved our row and column, but still have texture
// objects that fall within the boundaries of the new position, then we will
// just update their position, instead of releasing them and adding new ones.
//      void TexturePager::reuseTextures()

// loadNewTextures() - Create new texture objects for our table positions that
// don't have one (not reused).
//      void TexturePager::loadNewTextures()

// flushTextures() - Clear out the textures and put them back on the stack. 
//      void TexturePager::flushTextures()

// ------------------------------------------------------------------------------

#ifndef __EAAGLES_MAPS_RPF_TEXTUREPAGER_H__
#define __EAAGLES_MAPS_RPF_TEXTUREPAGER_H__

#include "openeaagles/basic/Object.h"
#include "TextureTable.h"

namespace Eaagles {
namespace Basic { class List; }
namespace Maps {
namespace Rpf {

class CadrgMap;
class CadrgTocEntry;

class TexturePager : public Basic::Object {
    DECLARE_SUBCLASS(TexturePager, Basic::Object)
	
public:
    TexturePager();

    // Set functions
    virtual void setSize(int tableSize = 9);
    virtual void setMap(CadrgMap* newMap);
    virtual void setToc(CadrgTocEntry* x);

    // Get functions
    TextureTable& getTable() { return table; }    
    CadrgTocEntry* getToc()     { return toc; }
    const CadrgTocEntry* getToc() const { return toc; }

    // Texture table operations
    void updateTextures(const int tRow, const int tCol);
    void flushTextures();

private:
    void freeTextures();
    void reuseTextures();
    void loadNewTextures();

	Basic::List* stack;

    TextureTable table;
    int maxTableSize;
    CadrgMap* map;
    int row;
    int col;
    int diffRow;
    int diffCol;
    CadrgTocEntry* toc;
};

};  // End Rpf namespace
};  // End Maps namespace
};  // End Eaagles namespace

#endif // __EAAGLES_MAPS_RPF_TEXTUREPAGER_H__

