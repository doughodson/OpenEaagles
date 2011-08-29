//------------------------------------------------------------------------------
// Original Author: Chris Buell
// Base class: Basic::Object -> CadrgFrameEntry
// 
// Description: Basic Table of Contents Frame entry that holds a frame and the
// Color Look up table associated with that frame.
//
// setPathName() - Sets the a.toc files directory and filename (a.toc)
//    void CadrgFrameEntry::setPathName(const char* dir, const char* file)
//
// setCib() - Set our cib flag
//    void CadrgFrameEntry::setCib(const bool x)
//
// loadClut() - Load our color lookup table, if it isn't already loaded.
//    void CadrgFrameEntry::loadClut() 
//
// setFrame() - Sets the child frame
//    void CadrgFrameEntry::setFrame(CadrgFrame* newFrame)
//
//------------------------------------------------------------------------------
#ifndef __EAAGLES_MAPS_RPF_CADRGFRAMEENTRY_H__
#define __EAAGLES_MAPS_RPF_CADRGFRAMEENTRY_H__

#include "openeaagles/maps/rpfMap/CadrgClut.h"
#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

class CadrgFrame;

class CadrgFrameEntry : public Basic::Object {
    DECLARE_SUBCLASS(CadrgFrameEntry, Basic::Object)	

public:
    CadrgFrameEntry();

    // Get 
    bool doIExist() { return exists; }
    const char* getDirectory() { return directory; }
    const char* getFileName() { return filename; }
    CadrgFrame* getFrame() { return frame; }
    const CadrgClut& getClut() { return clut; }

    // Set
    virtual void setPathName(const char* dir, const char* file);
    virtual void setCib(const bool x);
    virtual void loadClut();
    virtual void setFrame(CadrgFrame* newFrame);

private:
    bool exists;        // Does this entry exist (has a path name been set?)
    char* directory;    // Directory
    char filename[16];  // Filename
    CadrgClut clut;     // Color lookup table
    CadrgFrame* frame;  // Pointer to our child frame
    bool loaded;        // Loaded flag
    bool cib;           // Are we a CIB or not?
};

};  // End Maps namespace
};  // End Rpf namespace 
};  // End Eaagles namespace

#endif // __EAAGLES_MAPS_RPF_CADRGFRAMEENTRY_H__
