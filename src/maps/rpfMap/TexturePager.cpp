// ------------------------------------------------------------------------------
// Class: TexturePager
// ------------------------------------------------------------------------------

#include "openeaagles/maps/rpfMap/TexturePager.h"
#include "openeaagles/maps/rpfMap/CadrgTocEntry.h"
#include "openeaagles/graphics/Texture.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/List.h"

namespace oe {
namespace maps  {
namespace rpf {

IMPLEMENT_SUBCLASS(TexturePager, "TexturePager")
EMPTY_SLOTTABLE(TexturePager)
EMPTY_SERIALIZER(TexturePager)

//------------------------------------------------------------------------------
// Constructor()
//------------------------------------------------------------------------------
TexturePager::TexturePager()
{
    STANDARD_CONSTRUCTOR()
    maxTableSize = 0;
    map = nullptr;
    row = 0;
    col = 0;
    diffRow = 0;
    diffCol = 0;
    stack = nullptr;
    toc = nullptr;
}

//------------------------------------------------------------------------------
// copyData()
//------------------------------------------------------------------------------
void TexturePager::copyData(const TexturePager& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) {
        map = nullptr;
        stack = nullptr;
        toc = nullptr;
    }

    table = org.table;

    maxTableSize = org.maxTableSize;
    if (org.map != nullptr ){
        if (map != nullptr) map->unref();
        map = org.map;
        map->ref();
    }
    if (org.stack != nullptr) {
        if (stack != nullptr) stack->unref();
        stack = org.stack;
        stack->ref();
    }
    setToc(org.toc);

    row = org.row;
    col = org.col;
    diffRow = org.diffRow;
    diffCol = org.diffCol;
}

//------------------------------------------------------------------------------
// deleteData()
//------------------------------------------------------------------------------
void TexturePager::deleteData()
{
    if (map != nullptr) map->unref();
    map = nullptr;
    if (stack != nullptr) stack->unref();
    stack = nullptr;
    if (toc != nullptr) toc->unref();
    toc = nullptr;
}

//------------------------------------------------------------------------------
// setSize() - Sets the size of our table.
//------------------------------------------------------------------------------
void TexturePager::setSize(int tableSize)
{

    table.setSize(tableSize);
    maxTableSize = tableSize;
    int size = maxTableSize * maxTableSize;
    if (stack != nullptr) {
        stack->clear();
    }
    else stack = new basic::List();

    for (int i = 0; i < size; i++) {
        BasicGL::Texture* t = new BasicGL::Texture();
        stack->addHead(t);
     }
}

//------------------------------------------------------------------------------
// setMap() - Sets our parent map.
//------------------------------------------------------------------------------
void TexturePager::setMap(CadrgMap* newMap)
{
    if (newMap != nullptr) {
        if (map != nullptr) map->unref();
        map = newMap;
        map->ref();
    }
}

// -------------------------------------------------------------------------
// updateTextures() - This takes in the current row and column that our
// reference lat/lon is at, and if we have moved, we adjust our textures
// accordingly, freeing any textures that are now out of the boundary, re-using
// any that are still in and adding any new ones.
// -------------------------------------------------------------------------
void TexturePager::updateTextures(const int tRow, const int tCol)
{

    if (map == nullptr) {
        std::cerr << "TexturePager::UpdateTextures invalid map" << std::endl;
        return;
    }

    row = tRow;
    col = tCol;

    int cRow = table.centerRowTexture();
    int cCol = table.centerColumnTexture();

    if (row != cRow || col != cCol) {
        // Position has moved enough to cause a change in the textures displayed
        diffRow = row - cRow;
        diffCol = col - cCol;
        freeTextures();
        reuseTextures();
    }
    loadNewTextures();
}

//------------------------------------------------------------------------------
// setToc() - Sets the toc entry we are associated to.
//------------------------------------------------------------------------------
void TexturePager::setToc(CadrgTocEntry* x)
{
    if (toc != nullptr) toc->unref();
    toc = x;
    if (toc != nullptr) toc->ref();
}

// -------------------------------------------------------------------------
// freeTextures() - These are the textures that were used before, but are now
// out of the boundary from our center, so we release them back to the stack.
// -------------------------------------------------------------------------
void TexturePager::freeTextures()
{
    if (map != nullptr) {

        // Get our starting position, which is the bottom and top of our table
        int lb = table.getLowerBoundIndex();
        int ub = table.getUpperBoundIndex();
        // Traverse through the table from bottom left to top right
        for (int i = lb; i <= ub; i++) {
            for (int j = lb; j <= ub; j++) {
                int r = i - diffRow;
                int c = j - diffCol;
                // Is our new row and column still in the boundary
                bool ok = table.isInBounds(r, c);
                // If we aren't in the boundary any more, clear us out and let
                // the stack know we have another Object open.
                if (!ok) {
                    // Get our texture object at the no longer visible row,column
                    BasicGL::Texture* textureIndex = table.getTexture(i, j);
                    if (textureIndex != nullptr) {
                        // Add the object back to the stack
                        stack->addHead(textureIndex);
                        // Tell our table to clear its object out
                        table.setTextureObject(i, j, nullptr);
                        // Tell our map to release its frame entry's frames, so we aren't storing those unecessarily
                        map->releaseFrame(r + row, c + col, this);
                    }
                }
            }
        }
    }
}

// -------------------------------------------------------------------------
// reuseTextures() - If we have moved our row and column, but still have texture
// objects that fall within the boundaries of the new position, then we will
// just update their position, instead of releasing them and adding new ones.
// -------------------------------------------------------------------------
void TexturePager::reuseTextures()
{
    // Temporary table to store new data
    TextureTable table2;
    table2.setSize(maxTableSize);
    // Set our new center row and column
    table2.setCenterRowTexture(row);
    table2.setCenterColumnTexture(col);
    int lb = table.getLowerBoundIndex();
    int ub = table.getUpperBoundIndex();
    for (int i = lb; i <= ub; i++) {
        for (int j = lb; j <= ub; j++) {
            int r = i + diffRow;
            int c = j + diffCol;
            if (table.isInBounds(r, c)) {
                // Table is in bounds, set the new texture object
                BasicGL::Texture* obj = table.getTexture(r, c);
                table2.setTextureObject(i, j, obj);
            }
        }
    }
    // when we are done, set our table to the new table, with the reused ones still in!
    // clear our old table
#if 0
    for (int i = lb; i <= ub; i++) {
        for (int j = lb; j <= ub; j++) {
            table.setTextureObject(i, j, 0);
        }
    }
#endif
    table = table2;
}

// -------------------------------------------------------------------------
// loadNewTextures() - Create new texture objects for our table positions that
// don't have one (not reused).
// -------------------------------------------------------------------------
void TexturePager::loadNewTextures()
{
    int offset[4] = { 1, 0, 0, -1 };
    int rowChange[4] = { 0, -1, 0, 1 };
    int colChange[4] = { -1, 0, 1, 0 };

    // The max table size should be odd that way there is always a middle table position
    // to process all textures in a spiral from inside to out
    int maxSize = table.getMaxTableSize();
    if (maxSize % 2) {
        for (int level = 0; level < maxSize; level += 2) {
            int r = level >> 1;
            int c = r + 1;
            for (int dir = 0; dir < 4; dir++) {
                for (int j = 0; j < level + offset[dir]; j++) {
                    r += rowChange[dir];
                    c += colChange[dir];
                    // Do we have a valid texture at this position in our table?  If we dont we are going
                    // to add one, but only if the row and column + our center row and column position fall
                    // within our valid frames.
                    BasicGL::Texture* texObj = table.getTexture(r, c);
                    if (texObj == nullptr && map->isValidFrame(r + row, c + col, this)) {
                        if (stack != nullptr) {
                            basic::List::Item* item = stack->getFirstItem();
                            if (item != nullptr) {
                                BasicGL::Texture* obj = dynamic_cast<BasicGL::Texture*>(item->getValue());
                                if (obj != nullptr) {
                                    // Set our new texture object there, and remove it from our stack.
                                    table.setTextureObject(r, c, obj);
                                    stack->removeHead();
                                    // Now load the frame data onto our new texture.
                                    void* pixels = map->getPixels(r + row, c + col, this);
                                    map->loadFrameToTexture(obj, pixels);
                                    // The return is here because we only want to load one texture at a time.
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else std::cout << "TexturePager::loadNewTextures() - could not process new textures because grid size is even or 0!" << std::endl;

    return;
}

//------------------------------------------------------------------------------
// flushTextures() - Clear out the textures and put them back on the stack.
//------------------------------------------------------------------------------
void TexturePager::flushTextures()
{
    // Return textures indices that are no longer used
    if (map != nullptr) {
        int lb = table.getLowerBoundIndex();
        int ub = table.getUpperBoundIndex();
        for (int i = lb; i <= ub; i++) {
            for (int j = lb; j <= ub; j++) {
                // Find all of our texture objects and release them back to the stack
                BasicGL::Texture* textureIndex = table.getTexture(i, j);
                if (textureIndex != nullptr) {
                    stack->addHead(textureIndex);
                    table.setTextureObject(i, j, nullptr);
                    map->releaseFrame(i + row, j + col, this);
                }
            }
        }
    }
}

} // End rpf namespace
} // End Maps namespace
} // End oe namespace

