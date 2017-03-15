
#include "openeaagles/maps/rpf/CadrgFrameEntry.hpp"
#include "openeaagles/maps/rpf/CadrgFrame.hpp"
#include <cstring>

namespace oe {
namespace rpf {

IMPLEMENT_SUBCLASS(CadrgFrameEntry, "CadrgFrameEntry")
EMPTY_SLOTTABLE(CadrgFrameEntry)
EMPTY_SERIALIZER(CadrgFrameEntry)

CadrgFrameEntry::CadrgFrameEntry()
{
    STANDARD_CONSTRUCTOR()
}

void CadrgFrameEntry::copyData(const CadrgFrameEntry& org, const bool)
{
    BaseClass::copyData(org);

    setFrame(org.frame);

    exists = org.exists;
    loaded = org.loaded;
    directory = org.directory;
    cib = org.cib;
}

void CadrgFrameEntry::deleteData()
{
    if (frame != nullptr) frame->unref();
    frame = nullptr;
    delete[] directory;
}

//--------------------------------------------------------------------------
// setPathName() - Sets the a.toc files directory and filename (a.toc)
//--------------------------------------------------------------------------
void CadrgFrameEntry::setPathName(const char* dir, const char* file)
{
    exists = true;
    directory = new char[std::strlen(dir) + 1];
    std::strncpy(directory, dir, std::strlen(dir));
    directory[std::strlen(dir)] = '\0';
    std::strncpy(filename, file, 15);
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
    if (loaded) return;

    clut.load(*this, cib);

    loaded = true;
}

//--------------------------------------------------------------------------
// setFrame() - Sets the child frame
//--------------------------------------------------------------------------
void CadrgFrameEntry::setFrame(CadrgFrame* newFrame)
{
    if (newFrame != nullptr) {
        if (frame != nullptr) frame->unref();
        frame = newFrame;
        frame->ref();
    }

    else {
        if (frame != nullptr) frame->unref();
        frame = nullptr;
    }
}

}
}
