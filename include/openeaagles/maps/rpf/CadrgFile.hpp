
#ifndef __oe_maps_rpf_CadrgFile_H__
#define __oe_maps_rpf_CadrgFile_H__

#include "openeaagles/base/Object.hpp"
#include <array>

namespace oe {
namespace base { class String; }
namespace rpf {
class CadrgTocEntry;

//------------------------------------------------------------------------------
// Class: CadrgFile
//
// Description:
// This is the data file that is created by CadrgMap when the CADRG files are '
// initially loaded.  These files contain all the TocEntries that are created,
// and can be accessed by the MapDrawer to draw easily.
//
// Subroutines:
// entry() - Returns a Toc Entry at the boundary given, if it's within
// our boundaries.
//     int getNumBoundaries() { return numBoundaries; }
//
// entry() - Returns a CONST Toc Entry at the boundary given, if it's within
// our boundaries.
//     const CadrgTocEntry* entry(int boundaryIndex) const;
//
// entry() - Returns a Toc Entry at the boundary given, if it's within
// our boundaries.
//     CadrgTocEntry* entry(int boundaryIndex);
//
// setEntries() - Sets a whole new list of Toc Entries.
//     virtual void setEntries(CadrgTocEntry* newEntries[MAX_TOC_ENTRIES]);
//
// addTocEntry() - Adds a Toc entry
//     virtual void addTocEntry(CadrgTocEntry* newEntry, const int idx);
//
// removeTocEntry() - Removes a Toc entry
//     virtual void removeTocEntry(const int idx);
//
// initialize our file to create entries and such
//     virtual void initialize(const char* dir);
//
// check for map data in a particular directory
//      virtual bool checkForMap(const char* dir)
//
// Note - when creating an instance of this, make sure to initialize it or the
// file will never get loaded.
//
//------------------------------------------------------------------------------
class CadrgFile : public base::Object
{
    DECLARE_SUBCLASS(CadrgFile, base::Object)

public:
    CadrgFile();

    // Max number of Toc entries we can have.
    static const int MAX_TOC_ENTRIES = 50;

    // Get / Set / Add / Initialize
    int getNumBoundaries() { return numBoundaries; }

    const CadrgTocEntry* entry(int boundaryIndex) const;
    CadrgTocEntry* entry(int boundaryIndex);

    virtual void setEntries(CadrgTocEntry* newEntries[MAX_TOC_ENTRIES]);
    virtual void addTocEntry(CadrgTocEntry* newEntry, const int idx);
    virtual void removeTocEntry(const int idx);

    // Initialize our file to create entries and such
    virtual bool initialize(const char* dir);

    // check for map data in a certain directory
    static bool checkForMap(const char* dir);

    // get our original directory
    const char* getDirectory();
    void setDirectory(const char* x);

private:
    void initData();
    int numBoundaries {};                       // How many boundaries are there?
    std::array<CadrgTocEntry*, MAX_TOC_ENTRIES> entries {};  // Holds our table of contents entries
    bool cib {};                                // CIB flag
    base::String* originalDir {};               // directory of the file we are associated with
};

}
}

#endif
