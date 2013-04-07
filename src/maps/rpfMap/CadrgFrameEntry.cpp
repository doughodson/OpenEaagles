#include "openeaagles/maps/rpfMap/CadrgFrameEntry.h"
#include "openeaagles/maps/rpfMap/CadrgFrame.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

    
IMPLEMENT_SUBCLASS(CadrgFrameEntry, "CadrgFrameEntry")
EMPTY_SLOTTABLE(CadrgFrameEntry)
EMPTY_SERIALIZER(CadrgFrameEntry)

//--------------------------------------------------------------------------
// constructor()
//--------------------------------------------------------------------------
CadrgFrameEntry::CadrgFrameEntry()
{
    STANDARD_CONSTRUCTOR()
    exists = false;
    loaded = false;
    directory = 0;
    frame = 0;
	filename[0] = '\0';
	cib = false;
}

//--------------------------------------------------------------------------
// copyData() 
//--------------------------------------------------------------------------
void CadrgFrameEntry::copyData(const CadrgFrameEntry& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) frame = 0;

    setFrame(org.frame);

    exists = org.exists;
    loaded = org.loaded;
    directory = org.directory;
    cib = org.cib;
}

//--------------------------------------------------------------------------
// deleteData() 
//--------------------------------------------------------------------------
void CadrgFrameEntry::deleteData()
{
    if (frame != 0) frame->unref();
    frame = 0;
    delete [] &directory;
}

//--------------------------------------------------------------------------
// setPathName() - Sets the a.toc files directory and filename (a.toc)
//--------------------------------------------------------------------------
void CadrgFrameEntry::setPathName(const char* dir, const char* file)
{
	exists = true;
	directory = new char[strlen(dir) + 1];
	strncpy(directory, dir, strlen(dir));
	directory[strlen(dir)] = '\0';
	strncpy(filename, file, 15);
	filename[15] = '\0';
}

//--------------------------------------------------------------------------
// setCib() - Set our cib flag
//--------------------------------------------------------------------------
void CadrgFrameEntry::setCib(const bool x)
{
    cib = x; 
}

//--------------------------------------------------------------------------
// loadClut() - Load our color lookup table, if it isn't already loaded.
//--------------------------------------------------------------------------
void CadrgFrameEntry::loadClut() 
{
	if (loaded)	return;

	clut.load(*this, cib);

	loaded = 1;
}

//--------------------------------------------------------------------------
// setFrame() - Sets the child frame
//--------------------------------------------------------------------------
void CadrgFrameEntry::setFrame(CadrgFrame* newFrame)
{
    if (newFrame != 0) {
        if (frame != 0) frame->unref();
        frame = newFrame;
        frame->ref();
    }

    else {
        if (frame != 0) frame->unref();
        frame = 0;
    }
}

};  // End Rpf namespace
};  // End Maps namespace
};  // End Eaagles namespace
