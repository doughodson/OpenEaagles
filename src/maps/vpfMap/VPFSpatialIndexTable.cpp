// --------------------------------------------------------------
// Class: VPFSpatialIndexTable
// --------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VPFSpatialIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFDirectory.h"

#include "openeaagles/basic/String.h"
// for opening and closing files
#include <fstream>

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VPFSpatialIndexTable,"VPFSpatialIndexTable")
EMPTY_SERIALIZER(VPFSpatialIndexTable)

EMPTY_COPYDATA(VPFSpatialIndexTable)

VPFSpatialIndexTable::VPFSpatialIndexTable()
{
    STANDARD_CONSTRUCTOR()
    type = 1;
    numPrims = 0;
    loaded = false;
    mbrX1 = 0;
    mbrX2 = 0;
    mbrY1 = 0;
    mbrY2 = 0;
    numNodes = 0;
    headerByteOffset = 24;
    binSize = 0;

    fullPath = 0;
}

void VPFSpatialIndexTable::deleteData()
{
    if (fullPath != 0) {
        fullPath->unref();
        fullPath = 0;
    }
}

void VPFSpatialIndexTable::loadIndexTableFromFile(const char* pathname, const char* filename, const int t)
{
#if defined DEBUG_OUTPUT
    std::cout << "SPACIAL INDEX PATH AND FILE = " << pathname << filename << std::endl;
#endif
    type = t;
    fullPath = new Basic::String(pathname);
    fullPath->catStr(filename);
    std::ifstream inStream;
    inStream.open(fullPath->getString(), std::ios::in | std::ios::binary);
    if (inStream.fail()) std::cerr << "VPFSpatialIndexTable::loadIndexFromFile(), failed to open file " << filename << std::endl;
    else {
        inStream.seekg(0, std::ios::beg);
        // ok, our first bit should be our number of primitives
        inStream.read((char*)&numPrims, sizeof(numPrims));
        inStream.read((char*)&mbrX1, sizeof(mbrX1));
        inStream.read((char*)&mbrY1, sizeof(mbrY1));
        inStream.read((char*)&mbrX2, sizeof(mbrX2));
        inStream.read((char*)&mbrY2, sizeof(mbrY2));
        inStream.read((char*)&numNodes, sizeof(numNodes));
#if defined DEBUG_OUTPUT
        std::cout << "NUMBER OF PRIMITIVES = " << numPrims << std::endl;
        std::cout << "MBR X1 = " << mbrX1 << std::endl;
        std::cout << "MBR Y1 = " << mbrY1 << std::endl;
        std::cout << "MBR X2 = " << mbrX2 << std::endl;
        std::cout << "MBR Y2 = " << mbrY2 << std::endl;
        std::cout << "NUMBER OF NODES = " << numNodes << std::endl;
#endif

        headerByteOffset = 24;
        binSize = numNodes * 8;

        loaded = true;
        inStream.close();
    }
#if defined DEBUG_OUTPUT
    std::cout << "CLOSE FILE = " << filename << std::endl;
#endif
}

int VPFSpatialIndexTable::findPrimitivesBySpatialQuery(const float lat, const float lon, int primIds[], const float width, const float height)
{
    //std::cout << "LAT / LON = " << lat << " / " << lon << std::endl;
    int primCount = 0; 
    // first, check our minimum bounding rectangle just to see if we contain this lat/lon point
    if ((lon >= mbrX1 && lon <= mbrX2) && (lat >= mbrY1 && lat <= mbrY2)) {
        //std::cout << "VALID LAT / LON convert to spatial coordinates!" << std::endl;
        int x = 0, y = 0;
        int left = 0, right = 0, top = 0, bottom = 0;
        // flag to tell our checker to check for a space query, not just a point query
        bool spaceQuery = false;
        // we are going to convert our latitude to spatial coordinates, but we are also going to create spacial coordinates for our width and height
        if (width != 0 && height != 0) {
            spaceQuery = true;
            float widthDeg = (width / 60) / 2;
            float heightDeg = (height / 60) / 2;
            convertDegsToSpatialPoint(lat + heightDeg, lon + widthDeg, top, right);
            convertDegsToSpatialPoint(lat - heightDeg, lon - widthDeg, bottom, left);
        }

        // convert our reference lat / lon to spatial coordinates
        convertDegsToSpatialPoint(lat, lon, y, x);

#if defined DEBUG_OUTPUT
        std::cout << "Lat / Lon Spatial point = " << y << ", " << x << std::endl;
        std::cout << "TOP RIGHT Spatial point = " << top << ", " << right << std::endl;
        std::cout << "BOTTOM LEFT Spatial point = " << bottom << ", " << left << std::endl;
#endif
        // ok, we have the spatial point, now let's start reading the records that are in this coordinate
        std::fstream stream;
        stream.open(fullPath->getString(), std::ios::in | std::ios::binary);
        if (stream.fail()) std::cout << "COULD NOT OPEN FILE = " << fullPath->getString() << std::endl;
        else {
            // now get our first cell, and go from there
            for (int i = 0; i < numNodes; i++) {
                stream.seekg(headerByteOffset + (i * 8), std::ios::beg);
                int offset = 0, count = 0;
                stream.read((char*)&offset, sizeof(offset));
                stream.read((char*)&count, sizeof(count));
                if (count == 0 && offset == 0) {
                    //std::cout << "NO PRIMITIVES FOR CELL #" << i+1 << std::endl;
                }
                else {
                    //std::cout << "CELL #" << i+1 << " OFFSET/COUNT = " << offset << " / " << count << std::endl;
                    stream.seekg(headerByteOffset + binSize + offset, std::ios::beg);
                    // now read our primitive information
                    for (int i = 0; i < count; i++) {
                        int minLon = 0;
                        int minLat = 0;
                        int maxLon = 0;
                        int maxLat = 0;
                        stream.read((char*)&minLon, 1);
                        stream.read((char*)&minLat, 1);
                        stream.read((char*)&maxLon, 1);
                        stream.read((char*)&maxLat, 1);
                        int primId = 0;
                        stream.read((char*)&primId, sizeof(primId));
                        if (spaceQuery) {
                            if ((left <= minLon && right >= maxLon) && (top >= maxLat && bottom <= minLat)) {
                                //std::cout << "PRIMITIVE ID OF PRIMITIVE THAT FITS QUERY = " << primId << std::endl;
                                //std::cout << "Min LONGITUDE = " << minLon << std::endl;
                                //std::cout << "Max LONGITUDE = " << maxLon << std::endl;
                                //std::cout << "Min LATITUDE = " << minLat << std::endl;
                                //std::cout << "Max LATITUDE = " << maxLat << std::endl;
                                primIds[primCount++] = primId;
                            }
                        }
                        else {
                            if ((x >= minLon && x <= maxLon) && (y >= minLat && y <= maxLat)) {
                                //std::cout << "PRIMITIVE ID OF PRIMITIVE THAT FITS QUERY = " << primId << std::endl;
                                //std::cout << "Min LONGITUDE = " << minLon << std::endl;
                                //std::cout << "Max LONGITUDE = " << maxLon << std::endl;
                                //std::cout << "Min LATITUDE = " << minLat << std::endl;
                                //std::cout << "Max LATITUDE = " << maxLat << std::endl;
                                primIds[primCount++] = primId;
                            }
                        }
                    }
                }
            }
            stream.close();       
        }
    }
    return primCount;

}

void VPFSpatialIndexTable::convertDegsToSpatialPoint(const float lat, const float lon, int& y, int& x)
{
    // we should already have checked the mbr to make sure these lat/lons are valid
    if ((mbrY2 - mbrY1) != 0) y = int(255 * (lat - mbrY1) / (mbrY2 - mbrY1));
    else y = 0;
    if ((mbrX2 - mbrX1) != 0) x = int(255 * (lon - mbrX1) / (mbrX2 - mbrX1));
    else x = 0;
    if (y > 255) y = 255;
    else if (y < 0) y = 0;
    if (x > 255) x = 255;
    else if (x < 0) x = 0;
}



} // end Vpf namespace
} // end Maps namespace 
} // end Eaagles namespace

