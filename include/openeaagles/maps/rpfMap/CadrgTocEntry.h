//------------------------------------------------------------------------------
// Class: CadrgTocEntry
//
// This is a Table of Contents Boundary Rectangle Entry, which describes the
// boundary of a given set of frames, including the scale and size of the
// rectangle.  

// Subroutines:
// getFrameEntry() - Return the frame at the given row, column.
//      CadrgFrameEntry* CadrgTocEntry::getFrameEntry(const int v, const int h)

// generateItems() - Simply generate the name of the file we are using for our image.
//      void CadrgTocEntry::generateItems(void)

// setType() - Sets our entry type (ie... "CADRG")
//      void CadrgTocEntry::setType(const char* x, const int size) 

// setZone() - Sets the zone this entry lies in
//      void CadrgTocEntry::setZone(const char* x, const int size) 

// setScale() - Sets the scale of this entry (ie... "1:500K")
//      void CadrgTocEntry::setScale(const char* x, const int size) 

// isInZone() - Takes in a lat lon and returns true if that point falls in our entry
//      bool CadrgTocEntry::isInZone(double lat, double lon)
//------------------------------------------------------------------------------

#ifndef __Eaagles_Maps_Rpf_CadrgTocEntry_H__
#define __Eaagles_Maps_Rpf_CadrgTocEntry_H__

#include "openeaagles/maps/rpfMap/CadrgFrameEntry.h"
#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

class CadrgTocEntry : public Basic::Object {
    DECLARE_SUBCLASS(CadrgTocEntry, Basic::Object)	

public:
    CadrgTocEntry();

    // Get functions
    double getNWLat()           { return nwLat; }
    double getNWLon()           { return nwLon; }
    double getSWLat()           { return swLat; }
    double getSWLon()           { return swLon; }
    double getNELat()           { return neLat; }
    double getNELon()           { return neLon; }
    double getSELat()           { return seLat; }
    double getSELon()           { return seLon; }
    double getHorizInterval()   { return horizInterval; }
    double getVertInterval()    { return vertInterval; }
    int getHorizFrames()        { return horizFrames; }
    int getVertFrames()         { return vertFrames; }
    int isMapImage()            { return mapImage; }
    int getMapIndex()           { return mapIndex; }
    bool isInZone(double lat, double lon);
    double getVertResolution()  { return vertResolution; }
    double getHorizResolution() { return horizResolution; }
    const char* getType()       { return type; }
    const char* getZone()       { return zone; }
    const char*  getScale ()    { return scale; }
    const char* getTitle()      { return title; }
    CadrgFrameEntry*  getFrameEntry(const int v, const int h);
    CadrgFrameEntry** getFrames()   { return frames; }

    // Set functions
    virtual void setNWLat(const double x) { nwLat = x; }
    virtual void setNWLon(const double x) { nwLon = x; }
    virtual void setSWLat(const double x) { swLat = x; }
    virtual void setSWLon(const double x) { swLon = x; }
    virtual void setNELat(const double x) { neLat = x; }
    virtual void setNELon(const double x) { neLon = x; }
    virtual void setSELat(const double x) { seLat = x; }
    virtual void setSELon(const double x) { seLon = x; }
    virtual void setHorizInterval(const double x) { horizInterval = x; }
    virtual void setVertInterval(const double x) { vertInterval = x; }
    virtual void setVertFrames(const int x) { vertFrames = x; }
    virtual void setHorizFrames(const int x) { horizFrames = x; }
    virtual void setVertResolution(const double x) { vertResolution = x; }
    virtual void setHorizResolution(const double x) { horizResolution = x; }
    virtual void setType(const char* x, const int size);
    virtual void setZone(const char* x, const int size); 
    virtual void setScale(const char* x, const int size); 
    virtual void setEntries(CadrgFrameEntry** x) { frames = x; }
    virtual void setMapIndex(const int x)   { mapIndex = x; }

    // Generate our frames
    virtual void generateItems();

private:
    char type[5];               // Type of map entry we are (ie.. CADRG)
    char  scale[12];            // Scale of this entry (1:500K, etc.)
    char  zone[1];              // Zone this entry falls in 
    double nwLat;               // Northwest latitude of entry
    double nwLon;               // Northwest longitude of entry
    double seLat;               // Southeast latitude of entry
    double seLon;               // Southest longitude of entry
    double swLat;               // Southwest latitude of entry
    double swLon;               // Southwest longitude of entry
    double neLat;               // Northeast latitude of entry
    double neLon;               // Northeast longitude of entry
    double vertInterval;        // Distance (deg) between each pixel in this entry NS
    double horizInterval;       // Distance (deg) between each pixel in the entry EW
    double vertResolution;      // Resolution (pixels) NS
    double horizResolution;     // Resolution (pixels) EW
    int horizFrames;            // Number of horizontal frames (rows)
    int vertFrames;             // Number of vertical frames (cols)
    char title[100];            // Full name of file we represent
    int   mapImage;             // are we a map image?
    int mapIndex;               // index to keep track of which map file we belong to   
    CadrgFrameEntry** frames;   // Our array of frames that make up this entry.
};

};  // End Rpf namespace
};  // End Maps namespace
};  // End Eaagles namespace

#endif
