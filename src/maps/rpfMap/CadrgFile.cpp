//------------------------------------------------------------------------------
// Class: CadrgFile
//------------------------------------------------------------------------------

#include "openeaagles/maps/rpfMap/CadrgFile.h"
#include "openeaagles/maps/rpfMap/CadrgTocEntry.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/maps/rpfMap/Support.h"
#include <cstring>

//#define PRINT_MAP_LOCATIONS

namespace Eaagles {
namespace Maps {
namespace Rpf {

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

IMPLEMENT_SUBCLASS(CadrgFile, "CadrgFile")
EMPTY_SLOTTABLE(CadrgFile)
EMPTY_SERIALIZER(CadrgFile)

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
CadrgFile::CadrgFile()
{
    STANDARD_CONSTRUCTOR()
    numBoundaries = 0;
    cib = false;
    for (int i = 0; i < MAX_TOC_ENTRIES; i++) entries[i] = nullptr;
    originalDir = nullptr;
}

//--------------------------------------------------------------------------
// copyData()
//--------------------------------------------------------------------------
void CadrgFile::copyData(const CadrgFile& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) {
        for (int i = 0; i < MAX_TOC_ENTRIES; i++) entries[i] = nullptr;
        originalDir = nullptr;
    }

    numBoundaries = org.numBoundaries;

    for (int i = 0; i < MAX_TOC_ENTRIES; i++) {
        if (entries[i] != nullptr) {
            entries[i]->unref();
            entries[i] = nullptr;
        }
        if (org.entries[i] != nullptr) entries[i] = org.entries[i]->clone();
    }
    cib = org.cib;
    if (originalDir != nullptr) {
        originalDir->unref();
        originalDir = nullptr;
    }
    if (org.originalDir != nullptr) {
        originalDir = org.originalDir;
        originalDir->ref();
    }
}

//--------------------------------------------------------------------------
// deleteData()
//--------------------------------------------------------------------------
void CadrgFile::deleteData()
{
    for (int i = 0; i < MAX_TOC_ENTRIES; i++) {
        if (entries[i] != nullptr) entries[i]->unref();
        entries[i] = nullptr;
    }
    if (originalDir != nullptr) {
        originalDir->unref();
        originalDir = nullptr;
    }
}

//--------------------------------------------------------------------------
// entry() - Returns a CONST Toc Entry at the boundary given, if it's within
// our boundaries.
//--------------------------------------------------------------------------
const CadrgTocEntry* CadrgFile::entry(int boundaryIndex) const
{
    if (boundaryIndex >= 0 && boundaryIndex < numBoundaries) return entries[boundaryIndex];
    else return nullptr;
}

//--------------------------------------------------------------------------
// entry() - Returns a Toc Entry at the boundary given, if it's within
// our boundaries.
//--------------------------------------------------------------------------
CadrgTocEntry* CadrgFile::entry(int boundaryIndex)
{
    if (boundaryIndex >= 0 && entries[boundaryIndex] != nullptr) return entries[boundaryIndex];
    else return nullptr;
}

//--------------------------------------------------------------------------
// setEntries() - Sets a whole new list of Toc Entries.
//--------------------------------------------------------------------------
void CadrgFile::setEntries(CadrgTocEntry* newEntries[MAX_TOC_ENTRIES])
{
    // Reset our boundaries
    numBoundaries = 0;
    // Clear our old entries
    for (int i = 0; i < MAX_TOC_ENTRIES; i++) {
        if (entries[i] != nullptr) {
            entries[i]->unref();
            entries[i] = nullptr;
        }
        // Now determine if we have an entry
        if (newEntries[i] != nullptr) {
            entries[i] = newEntries[i];
            entries[i]->ref();
            numBoundaries++;
        }
    }
}

//--------------------------------------------------------------------------
// setDirectory() - sets our original directory
//--------------------------------------------------------------------------
void CadrgFile::setDirectory(const char* x)
{
    if (originalDir != nullptr) originalDir->setStr(x);
    else originalDir = new Basic::String(x);
}

//--------------------------------------------------------------------------
// addTocEntry() - Adds a Toc entry
//--------------------------------------------------------------------------
void CadrgFile::addTocEntry(CadrgTocEntry* newEntry, const int idx)
{
    // First, make sure we have an entry there
    if (idx >= 0 && newEntry != nullptr) {
        if (entries[idx] != nullptr) {
            // We don't increment our numBoudaries, because we are replacing the Toc entry
            entries[idx]->unref();
            entries[idx] = newEntry;
            entries[idx]->ref();
        }
        else {
            // New boundary, increment our count
            numBoundaries++;
            entries[idx] = newEntry;
            entries[idx]->ref();
        }
    }
}

//--------------------------------------------------------------------------
// removeTocEntry() - removes a Toc entry at the given index
//--------------------------------------------------------------------------
void CadrgFile::removeTocEntry(const int idx)
{
    // First, make sure we have an entry there
    if (entries[idx] != nullptr) {
        entries[idx]->unref();
        entries[idx] = nullptr;
        numBoundaries--;
    }
}

//--------------------------------------------------------------------------
// checkForMap() - given a directory, do we have valid map data there that
// we can draw?  If so, return true.
//--------------------------------------------------------------------------
bool CadrgFile::checkForMap(const char* dir)
{
    bool ok = false;
    // Try to find and open our a.toc file
    std::ifstream toc;
    Basic::String* string = new Basic::String(dir);
    string->catStr("A.TOC");

    #if defined(WIN32)
        toc.open(*string, std::ios::in | std::ios::binary);
    #else
        toc.open(*string, std::ios::in);
    #endif

    // We didn't make it, so either we have a bad location, or the filename is lower case.  Let's try lowercase.
    if (toc.fail()) {
        // Clear out our input stream
        toc.clear();
        string->empty();
        string->setStr(dir);
        string->catStr("a.toc");
        #if defined(WIN32)
            toc.open(*string, std::ios::in | std::ios::binary);
        #else
            toc.open(*string, std::ios::in);
        #endif
        // if we succeed, return true
        if (!toc.fail()) ok = true;
    }
    else ok = true;

    toc.close();

    return ok;
}

//--------------------------------------------------------------------------
// initialize() - Checks to see we have valid data, then goes through and
// opens all the files that are required (from the input language), and then
// creates Toc Entries for each one.  It then fills each Toc with its frames.
//--------------------------------------------------------------------------
bool CadrgFile::initialize(const char* dir)
{
    numBoundaries = 0;

    // Remove all of our previous entries, if any
    for (int yy = 0; yy < MAX_TOC_ENTRIES; yy++) entries[yy] = nullptr;

    // Our header
    Header          head;
    // Physical locations of our boundary rectangles
    Location        locations[4];
    // Number of boundary records
    ushort          numBndryRecords = 0;
    ushort          ffPathLength = 0;
    int i = 0;
    int j = 0;
    std::streamoff currTocPos = 0;
    //char            filename[80];
    // Boundary record number
    ushort          boundaryRecNum = 0;
    // Frame row number
    ushort          frameRow = 0;
    // Frame column number
    ushort          frameCol = 0;
    // # of frame file index records
    uint            numFFIdxRec = 0;
    // Offset of frame file pathname

    uint            ffPathOff = 0;
    // Boundary record length
    ushort          bndryRecLength = 0;
    // # frame file pathname records
    ushort          numPathNameRecs = 0;
    // Frame file index record length
    ushort          idxRecLength = 0;
    // TOC NITF header length, if there
    uint            nitfHdrLength = 410;
    // Boundary rectangle table offset
    uint            bndryRecTableOffset = 0;
    // Frame file index table offset (not used)
    uint            ffIdxTableOff = 0;

    // Try to find and open our a.toc file
    std::ifstream toc;
    Basic::String* string = new Basic::String(dir);
    if (originalDir != nullptr) originalDir->setStr(dir);
    else originalDir = new Basic::String(dir);
    string->catStr("A.TOC");

    #if defined(WIN32)
        toc.open(*string, std::ios::in | std::ios::binary);
    #else
        toc.open(*string, std::ios::in);
    #endif

    // We didn't make it, so either we have a bad location, or the filename is lower case.  Let's try lowercase.
    if (toc.fail()) {
        // Clear out our input stream
        toc.clear();
        string->empty();
        string->setStr(dir);
        string->catStr("a.toc");
        #if defined(WIN32)
            toc.open(*string, std::ios::in | std::ios::binary);
        #else
            toc.open(*string, std::ios::in);
        #endif
        // If we still failed again, we know it's the directory that is misspelled or doesn't exist, so we print an error
        // and return.
        if (toc.fail()) {
            std::cout << "Could not open the A.TOC or a.toc file in location " << dir << std::endl;
            return false;
        }
    }
    else std::cout << "file = " << dir << std::endl;

    string->unref();

    // Let's read the header section length.
    // We seek right to position 31, to determine our governing spec date, which may or may not be there.  We
    // are going to see if it is there, because if it is, then we don't have an NITF message to worry about.
    toc.seekg(31, std::ios::beg);
    toc.read(head.govSpecdate, sizeof(head.govSpecdate));

    // Now move back to the beginning
    toc.seekg(0, std::ios::beg);

    // If we don't have a standard date of 199... then we know
    // that we have an National Imagery Transmission Format (NITF) message, so we should skip over the message and search
    // for the Raster Product Format Header (RPFHDR).
    if (std::strncmp(head.govSpecdate, "199", 3) != 0) {
        // Make a large set of characters to read from
        char buf[1024];
        // Read in some of the file
        toc.read(buf,1024);

        // Look for the RPFHDR indicator
        char* ptr = std::strstr(buf, "RPFHDR");

        // Found it!
        if (ptr) {
            // Distance from where we found RPFHDR to the beginning of the file (sizeof the NITF message, basically)
            int dist = static_cast<int>(ptr - &buf[0]);
            // Padding 11 for some reason.
            nitfHdrLength = dist + 11;
        }
        // Move to where our header starts!
        toc.seekg(nitfHdrLength, std::ios::beg);
    }

    // Ok, we are now at the header portion of the A.TOC file.
    // The Header file is setup like this (according to MIL-PRF-89038 6 OCTOBER 1994)
    // Name                             Type    Size (Bytes)    Position in file
    // <little/big endian indicator>    bool:    1                  1
    // <header section length>          uint:    2                  2
    // <filename>                       ascii:  12                  4
    // <replacement/update indicator>   uint:    1                  15
    // <governing specification num>    ascii:  15                  16
    // <governing spec. date>           ascii:   8                  31
    // <security classification>        ascii:   1                  39
    // <country/intntl code>            ascii:   2                  41
    // <security release marking>       ascii:   2                  43
    // <location section location>      uint:    4                  45

    // endian indicator
    unsigned char byte(0);
    toc.read(reinterpret_cast<char*>(&byte), sizeof(byte));
    head.endian = (byte != 0);

    toc.read(reinterpret_cast<char*>(&head.hdrSectionLength), sizeof(head.hdrSectionLength));
    // If we are using big endian encoding, we swap our bytes on uints and ushorts
    if (!head.endian) swap(reinterpret_cast<unsigned char*>(&head.hdrSectionLength), sizeof(head.hdrSectionLength));
    toc.read(reinterpret_cast<char*>(head.filename), sizeof(head.filename));
    toc.read(reinterpret_cast<char*>(&head.nruInd), sizeof(head.nruInd));
    toc.read(reinterpret_cast<char*>(head.govSpecNum), sizeof(head.govSpecNum));
    toc.read(reinterpret_cast<char*>(head.govSpecdate), sizeof(head.govSpecdate));
    toc.read(reinterpret_cast<char*>(&head.secClass), sizeof(head.secClass));
    toc.read(reinterpret_cast<char*>(head.secCountryCode), sizeof(head.secCountryCode));
    toc.read(reinterpret_cast<char*>(head.secRelease), sizeof(head.secRelease));
    toc.read(reinterpret_cast<char*>(&head.locSecLoc), sizeof(head.locSecLoc));
    if (!head.endian) swap(reinterpret_cast<unsigned char*>(&head.locSecLoc), sizeof(head.locSecLoc));

    // seek to start of location section:
    toc.seekg(head.locSecLoc, std::ios::beg);

    // We need four different sections to get all of our boundary, frame data and
    // The location section boundary rectangle sub header.
    locations[0].componentId = LOC_BOUNDARY_SECTION_SUBHEADER;     // 148
    // The actual boundary rectangle table
    locations[1].componentId = LOC_BOUNDARY_RECTANGLE_TABLE;       // 149
    // The frame file index section subheader
    locations[2].componentId = LOC_FRAME_FILE_INDEX_SUBHEADER;     // 150
    // The frame file index subsection, which holds the frame index table and records.
    locations[3].componentId = LOC_FRAME_FILE_INDEX_SUBSECTION;    // 151

    // GO through and find the physical locations in the file of each one of our locations we need (see above).
    parseLocations(toc, locations, 4);

    if (locations[0].physicalIdx == ~0) {
        std::cout << "Can't find the LOC_BOUNDARY_SECTION_SUBHEADER in the TOC!" << std::endl;
        return false;
    }
    else if (locations[1].physicalIdx == ~0) {
        std::cout << "Can't find the LOC_BOUNDARY_RECTANGLE_TABLE in the TOC!" << std::endl;
        return false;
    }
    else if (locations[2].physicalIdx == ~0) {
        std::cout << "Can't find the LOC_FRAME_FILE_INDEX_SUBHEADER in the TOC!" << std::endl;
        return false;
    }
    else if (locations[3].physicalIdx == ~0) {
        std::cout << "Can't find the LOC_FRAME_FILE_INDEX_SUBSECTION in the TOC!" << std::endl;
        return false;
    }

    // Our first location is the boundary rectangle section, which lays out the coverage area
    // Seek to the physical offset of the boundary rectangle section subheader
    toc.seekg(locations[0].physicalIdx, std::ios::beg);

    // Skip our boundary table record offset
    toc.read(reinterpret_cast<char*>(&bndryRecTableOffset), sizeof(bndryRecTableOffset));
    swap(reinterpret_cast<unsigned char*>(&bndryRecTableOffset), 4);

    // Read in the number of boundary rectangle records!
    toc.read(reinterpret_cast<char*>(&numBndryRecords), sizeof(numBndryRecords));
    swap(reinterpret_cast<unsigned char*>(&numBndryRecords), sizeof(numBndryRecords));

    // Skip the boundary record length
    toc.read(reinterpret_cast<char*>(&bndryRecLength), sizeof(bndryRecLength));
    swap(reinterpret_cast<unsigned char*>(&bndryRecLength), sizeof(bndryRecLength));

    // Now we have parsed all the header information from the boundary rectangle section subheader.
    // Let's seek to the actual position of the boundary rectangle table and read the data in.
    toc.seekg(locations[1].physicalIdx, std::ios::beg);

    // ------------------------------------------------------------------------------------
    // Our boundaries are setup like this (MIL-STD-2411, pg 45,46)
    // Name                             Type    Size (Bytes)
    // <product data type>              char     5
    // <compression ratio>              char     5
    // <scale>                          char    12
    // <zone>                           char     1
    // <producer>                       char     5
    // <NW/upper left lat>              real     8
    // <NW/upper left lon>              real     8
    // <SW/lower left lat>              real     8
    // <SW/lower left lon>              real     8
    // <NE/upper right lat>             real     8
    // <NE/upper right lon>             real     8
    // <SE/lower right lat>             real     8
    // <SE/lower right lon>             real     8
    // <vertical resolution>            real     8
    // <horizontal resolution>          real     8
    // <vertical interval>              real     8
    // <horizontal interval>            real     8
    // <# of frames vertically>         uint     4
    // <# of frames horizontally>       uint     4
    // ------------------------------------------------------------------------------------

    // Set the number of boundary records
    numBoundaries = numBndryRecords;
    for (i = 0; i < numBndryRecords; i++) {
        // Create a new entry
        entries[i] = new CadrgTocEntry();

        char type[5];
        //strncpy(type, entries[i]->getType(), 5);
        // Read the type in
        toc.read(type, 5);

        if (std::strncmp(type, "CADRG", 5) == 0)
            cib = false;
        else
            cib = true;

        // Skip the compression ratio
        toc.seekg(5, std::ios::cur);
        // Scale of our map (1:250k, etc...)
        char scale[12];
        std::strncpy(scale, entries[i]->getScale(), 12);
        toc.read(scale, 12);

        // Read the zone we are in.
        char zone[2];
        std::strncpy(zone, entries[i]->getZone(), 1);
        toc.read(zone, 1);

        // Skip producer (5 chars)
        toc.seekg(5, std::ios::cur);

        double nwLat = 0, nwLon = 0, swLat = 0, swLon = 0, neLat = 0, neLon = 0, seLat = 0, seLon = 0;
        double vertResolution = 0, horzResolution = 0;
        double vertInterval = 0, horzInterval = 0;
        int vertFrames = 0, horzFrames = 0;

        // Read all of our geodetic data.
        toc.read(reinterpret_cast<char*>(&nwLat), sizeof(double));
        toc.read(reinterpret_cast<char*>(&nwLon), sizeof(double));
        toc.read(reinterpret_cast<char*>(&swLat), sizeof(double));
        toc.read(reinterpret_cast<char*>(&swLon), sizeof(double));
        toc.read(reinterpret_cast<char*>(&neLat), sizeof(double));
        toc.read(reinterpret_cast<char*>(&neLon), sizeof(double));
        toc.read(reinterpret_cast<char*>(&seLat), sizeof(double));
        toc.read(reinterpret_cast<char*>(&seLon), sizeof(double));
        toc.read(reinterpret_cast<char*>(&vertResolution), sizeof(double));
        toc.read(reinterpret_cast<char*>(&horzResolution), sizeof(double));
        toc.read(reinterpret_cast<char*>(&vertInterval), sizeof(double));
        toc.read(reinterpret_cast<char*>(&horzInterval), sizeof(double));
        toc.read(reinterpret_cast<char*>(&vertFrames), sizeof(uint));
        toc.read(reinterpret_cast<char*>(&horzFrames), sizeof(uint));

        swap(reinterpret_cast<unsigned char*>(&nwLat), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&nwLon), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&swLat), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&swLon), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&neLat), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&neLon), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&seLat), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&seLon), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&vertResolution), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&horzResolution), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&vertInterval), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&horzInterval), sizeof(double));
        swap(reinterpret_cast<unsigned char*>(&vertFrames), sizeof(uint));
        swap(reinterpret_cast<unsigned char*>(&horzFrames), sizeof(uint));

        // Set our values back now
        entries[i]->setNWLat(nwLat);
        entries[i]->setNWLon(nwLon);
        entries[i]->setSWLat(swLat);
        entries[i]->setSWLon(swLon);
        entries[i]->setNELat(neLat);
        entries[i]->setNELon(neLon);
        entries[i]->setSELat(seLat);
        entries[i]->setSELon(seLon);
        entries[i]->setVertResolution(vertResolution);
        entries[i]->setHorizResolution(horzResolution);
        entries[i]->setVertInterval(vertInterval);
        entries[i]->setHorizInterval(horzInterval);
        entries[i]->setVertFrames(vertFrames);
        entries[i]->setHorizFrames(horzFrames);

        if (cib) entries[i]->setType(type, 3);
        else entries[i]->setType(type, 5);
        entries[i]->setScale(scale, 12);
        entries[i]->setZone(zone, 1);

        #if defined (PRINT_MAP_LOCATIONS)
            scale[8] = '\0';
            zone[1] = '\0';
            std::cout << "ZONE #" << zone << ", SCALE = " << scale << ", LATS = " << swLat << ", " << neLat << ", LONS = " << swLon << ", " <<  neLon << std::endl;
        #endif
        vertFrames = entries[i]->getVertFrames();
        horzFrames = entries[i]->getHorizFrames();

        CadrgFrameEntry** frames = new CadrgFrameEntry*[vertFrames];

        for (j = 0; j < vertFrames; j++) {
            frames[j] = new CadrgFrameEntry[horzFrames];
        }
        entries[i]->generateItems();
        entries[i]->setEntries(frames);
    }

    // Read # of frame file index records
    // Skip 1 byte highest security classification
    // locations[2] + 1 is the physical location of frame file index section subheader + we skip one byte
    // for the security classification
    toc.seekg(locations[2].physicalIdx + 1, std::ios::beg);

    // The frame file index table offset
    toc.read(reinterpret_cast<char*>(&ffIdxTableOff), sizeof(ffIdxTableOff));
    swap(reinterpret_cast<unsigned char*>(&ffIdxTableOff), 4);

    // Number of frame file index records
    toc.read(reinterpret_cast<char*>(&numFFIdxRec), sizeof(numFFIdxRec));
    swap(reinterpret_cast<unsigned char*>(&numFFIdxRec), sizeof(numFFIdxRec));

    // Number of pathname records
    toc.read(reinterpret_cast<char*>(&numPathNameRecs), sizeof(numPathNameRecs));
    swap(reinterpret_cast<unsigned char*>(&numPathNameRecs), sizeof(numPathNameRecs));

    // Frame file index record length
    toc.read(reinterpret_cast<char*>(&idxRecLength), sizeof(idxRecLength));
    swap(reinterpret_cast<unsigned char*>(&idxRecLength), sizeof(idxRecLength));

    // Here is a temp pointer to our TOC entries
    CadrgTocEntry* entry = nullptr;

    // Temp point to our TOC entries frame file
    CadrgFrameEntry* frame = nullptr;

    // Read through all of our frame files.
    for (i = 0; i < static_cast<int>(numFFIdxRec); i++) {

        // Locations[3] is frame file index table subsection * the record we are on.
        toc.seekg(locations[3].physicalIdx + idxRecLength * i, std::ios::beg);

        toc.read(reinterpret_cast<char*>(&boundaryRecNum), sizeof(boundaryRecNum));
        swap(reinterpret_cast<unsigned char*>(&boundaryRecNum), sizeof(boundaryRecNum));

        // If we are outside of our boundaries, print an error!
        if (entries[boundaryRecNum] != nullptr && boundaryRecNum <= numBoundaries - 1) {
            // Get our toc entry from our list
            entry = entries[boundaryRecNum];
            if (entry != nullptr) {
                entry->ref();

                // Read in the starting frame row and column of this frame
                toc.read(reinterpret_cast<char*>(&frameRow), sizeof(frameRow));
                toc.read(reinterpret_cast<char*>(&frameCol), sizeof(frameCol));
                swap(reinterpret_cast<unsigned char*>(&frameRow), sizeof(frameRow));
                swap(reinterpret_cast<unsigned char*>(&frameCol), sizeof(frameCol));

                // We already know how many vertical and horizontal frames we have
                int vertFrames = entry->getVertFrames();
                int horzFrames = entry->getHorizFrames();

                // If our row is greater than the # of frame we have, print out an error and return!
                if (frameRow > vertFrames - 1) {
                    std::cout << "Frame Row #" << frameRow << " is greater than the # of vertical frames, which = " << vertFrames << std::endl;
                    return false;
                }

                // If our frame column is greater than the # of horizontal frames we have, print out error and return.
                if (frameCol > horzFrames - 1) {
                    std::cout << "Frame Col #" << frameCol << " is greater than the # of horizontal frames, which = " << horzFrames << std::endl;
                    return false;
                }

                // Get our frames
                CadrgFrameEntry** frames = entry->getFrames();

                // Set a local frame to manipulate
                if (frames != nullptr) frame = &frames[(vertFrames - 1) - frameRow][frameCol];
                if (frame != nullptr && !frame->doIExist()) {
                    // Get our path name byte offset
                    toc.read(reinterpret_cast<char*>(&ffPathOff), sizeof(ffPathOff));
                    swap(reinterpret_cast<unsigned char*>(&ffPathOff), sizeof(ffPathOff));

                    // Save file position for later
                    currTocPos = toc.tellg();

                    // Go to start of pathname record, which is our LOC_FRAME_FILE_INDEX_SUBSECTION + the path offset
                    toc.seekg(locations[3].physicalIdx + ffPathOff, std::ios::beg);

                    toc.read(reinterpret_cast<char*>(&ffPathLength), sizeof(ffPathLength));
                    swap(reinterpret_cast<unsigned char*>(&ffPathLength), sizeof(ffPathLength));

                    // Allocate our frame file directory path, which is our path name length + 1 + string length of the directory name passed in

                    size_t size = ffPathLength + 1 + std::strlen(dir);
                    #if defined(WIN32)
                        char* directory = (char *) malloc(size);
                    #else
                        char* directory = (char *) alloca(size);
                    #endif

                    // 1st part of directory name is passed as our initial parameter "projects/data/maps/gncjncn/RPF/"
                    lcStrcpy(directory, size, dir);

                    // Read rest of directory name from Toc
                    // Skip 1st 2 chars, because they are the root characters (./), and are ignored since we are creating
                    // our own directory string
                    toc.read(&directory[std::strlen(dir)], 2);

                    // We read the rest, minus the first 2 characters
                    toc.read(&directory[std::strlen(dir)], ffPathLength - 2);
                    directory[ffPathLength - 2 + std::strlen(dir)] = '\0';

                    // Go back to get filename tail
                    toc.seekg(currTocPos, std::ios::beg);

                    // Get the actual frame file name!
                    char frameFilename[16];
                    toc.read(frameFilename, 12);
                    frameFilename[12] = '\0';

                    // Set our final path name, and file name!
                    frame->setPathName(directory, frameFilename);
                    frame->setCib(cib);
                }
                else {
                    std::cout << "Frame File #" << i << " is a duplicate!" << std::endl;
                    return false;
                }
                // Get rid of our entry and reset
                entry->unref();
                entry = nullptr;
            }
            else std::cout << "No TOC entry available at position " << boundaryRecNum << std::endl;
        }
        else {
            std::cout << "Bad boundary id in frame file index record " << std::endl;
            return false;
        }
    }
    // Clear out our entries
    toc.close();

    return true;
}

//--------------------------------------------------------------------------
// getDirectory() - returns the directory we created our file from
//--------------------------------------------------------------------------
const char* CadrgFile::getDirectory()
{
    if (originalDir != nullptr) return originalDir->getString();
    else return "";
}


} // End Rpf namespace
} // End Maps namespace
} // End Eaagles namespace

