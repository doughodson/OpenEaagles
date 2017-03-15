
#include "openeaagles/maps/rpf/CadrgFrame.hpp"

#include "openeaagles/maps/rpf/CadrgFrameEntry.hpp"
#include "openeaagles/base/String.hpp"

#include <cstring>

namespace oe {
namespace rpf {

IMPLEMENT_SUBCLASS(CadrgFrame, "CadrgFrame")
EMPTY_SLOTTABLE(CadrgFrame)
EMPTY_SERIALIZER(CadrgFrame)

CadrgFrame::CadrgFrame()
{
    STANDARD_CONSTRUCTOR()
}


void CadrgFrame::copyData(const CadrgFrame& org, const bool)
{
    BaseClass::copyData(org);

    if (org.frameEntry != nullptr) {
        if (frameEntry != nullptr) frameEntry->unref();
        frameEntry = org.frameEntry;
        frameEntry->ref();
    }
    nitfHdrLength = org.nitfHdrLength;
}

void CadrgFrame::deleteData()
{
    if (frameEntry != nullptr) frameEntry->unref();
    frameEntry = nullptr;
}

// -------------------------------------------------------------------------------------
// load() - Load a frame's data.
// -------------------------------------------------------------------------------------
void CadrgFrame::load(CadrgFrameEntry* entry)
{
    // Header, Location - from CadrgSupport.h
    Header head;
    // Physical locations
    Location loc[6];
    // Compression
    Compression compression;
    // Lookup table
    LookupTable lut[4];
    // Image structure
    Image image;
    //char filename[80];

    // All subframes present indicator
    bool allSubframes = false;
    // Lookup offset table offset
    uint lookupOffTblOff = 0;
    // Lookup table offset record length
    ushort lookupTblOffRecLen = 0;
    // Subframe mask table offset
    uint subframeMskTblOff = 0;
    // Subframe offset (mask section)
    uint subframeOff[6][6];
    // Temp table to transfer Lookup Table to
    unsigned char table[4][4096][4];

    if (entry != nullptr) {
        if (frameEntry != nullptr) frameEntry->unref();
        frameEntry = entry;
        frameEntry->ref();
    }

    base::String* string = nullptr;
    if (frameEntry != nullptr) {
        string = new base::String(frameEntry->getDirectory());
        string->catStr(frameEntry->getFileName());
    }
    if (string == nullptr) {
       return;
    }

    #if defined(WIN32)
        std::ifstream   fin(*string, std::ios::in | std::ios::binary);
    #else
        std::ifstream   fin(*string, std::ios::in);
    #endif

    if (fin.fail()) {
        std::cout << "CadrgFrame::load() : No filename " << *string << ", or directory found!  Error in reading the frame!" << std::endl;
        fin.close();
        return;
    }

    string->unref();

    // National Imagery Transmission Format (NITF)  header check - skipped for now (SLS),
    // assume no NITF hdr
    nitfHdrLength = NITF_HDR_NONE;

    // Read in our header buffer
    char buf[1024];

    fin.seekg(0,std::ios::beg);
    fin.read(buf,1024);

    // Find the end of the header
    char* ptr = std::strstr(buf,"RPFHDR");

    // Now calculate the length
    if (ptr) {
        int dist = static_cast<int>(ptr - &buf[0]);
        nitfHdrLength = dist + 11;
    }
    // Position properly
    fin.seekg(nitfHdrLength, std::ios::beg);

    // Read header
    unsigned char byte(0);
    fin.read(reinterpret_cast<char*>(&byte), sizeof(byte));
    head.endian = (byte != 0);
    fin.read(reinterpret_cast<char*>(&head.hdrSectionLength), sizeof(head.hdrSectionLength));
    if (!head.endian) swap(reinterpret_cast<unsigned char*>(&head.hdrSectionLength), sizeof(head.hdrSectionLength));
    fin.read(reinterpret_cast<char*>(head.filename), sizeof(head.filename));
    fin.read(reinterpret_cast<char*>(&head.nruInd), sizeof(head.nruInd));
    fin.read(reinterpret_cast<char*>(head.govSpecNum), sizeof(head.govSpecNum));
    fin.read(reinterpret_cast<char*>(head.govSpecdate), sizeof(head.govSpecdate));
    fin.read(reinterpret_cast<char*>(&head.secClass), sizeof(head.secClass));
    fin.read(reinterpret_cast<char*>(head.secCountryCode), sizeof(head.secCountryCode));
    fin.read(reinterpret_cast<char*>(head.secRelease), sizeof(head.secRelease));
    fin.read(reinterpret_cast<char*>(&head.locSecLoc), sizeof(head.locSecLoc));
    if (!head.endian) swap(reinterpret_cast<unsigned char*>(&head.locSecLoc), sizeof(head.locSecLoc));

    // Position to start of location section: 2 choices:
    fin.seekg(head.locSecLoc, std::ios::beg);

    // Locate the sections we need
    loc[0].componentId = LOC_COMPRESSION_SECTION;
    loc[1].componentId = LOC_IMAGE_DESCR_SUBHEADER;
    loc[2].componentId = LOC_COMPRESSION_LOOKUP_SUBSECTION;
    loc[3].componentId = LOC_SPATIAL_DATA_SUBSECTION;
    loc[4].componentId = LOC_IMAGE_DISPLAY_PARAM_SUBHEADER;
    loc[5].componentId = LOC_MASK_SUBSECTION;

    // Parse our locations
    parseLocations(fin, loc, 6);

    // From index to physicalIdx
    if (loc[0].physicalIdx == ~0 || loc[1].physicalIdx == ~0) {
        std::cout << "CadrgFrame::load() : Can't find section in frame!" << std::endl;
        return;
    }

    // Read the compression tables
    fin.seekg(loc[0].physicalIdx, std::ios::beg);
    fin.read(reinterpret_cast<char*>(&compression), sizeof(compression));
    swap(reinterpret_cast<unsigned char*>(&compression.algorithm), sizeof(compression.algorithm));
    swap(reinterpret_cast<unsigned char*>(&compression.nOffRecs), sizeof(compression.nOffRecs));
    swap(reinterpret_cast<unsigned char*>(&compression.nParmOffRecs), sizeof(compression.nParmOffRecs));

    if (loc[2].physicalIdx == ~0) {
        std::cout << "CadrgFrame::load() : Warning: Can't find compression lookup subsection in FrameFile, using alternate computation!" << std::endl;
        // Length of compr. sect. subhdr = 10
        fin.seekg(loc[0].physicalIdx + 10, std::ios::beg);
    }
    else {
        // DKS: Position at start of compression lookup table offset record
        fin.seekg(loc[2].physicalIdx, std::ios::beg);
    }

    // Read lookup offset table offset and record length
    fin.read(reinterpret_cast<char*>(&lookupOffTblOff), sizeof(lookupOffTblOff));
    swap(reinterpret_cast<unsigned char*>(&lookupOffTblOff), sizeof(lookupOffTblOff));
    fin.read(reinterpret_cast<char*>(&lookupTblOffRecLen), sizeof(lookupTblOffRecLen));
    swap(reinterpret_cast<unsigned char*>(&lookupTblOffRecLen), sizeof(lookupTblOffRecLen));

    // For each compression table
    for (int i = 0; i < 4; i++) {
        fin.read(reinterpret_cast<char*>(&lut[i].id), sizeof(lut[i].id));
        fin.read(reinterpret_cast<char*>(&lut[i].records), sizeof(lut[i].records));
        fin.read(reinterpret_cast<char*>(&lut[i].values), sizeof(lut[i].values));
        fin.read(reinterpret_cast<char*>(&lut[i].bitLength), sizeof(lut[i].bitLength));
        fin.read(reinterpret_cast<char*>(&lut[i].physOffset), sizeof(lut[i].physOffset));

        // Used for sanity checking
        swap(reinterpret_cast<unsigned char*>(&lut[i].id), sizeof(lut[i].id));
        swap(reinterpret_cast<unsigned char*>(&lut[i].records), sizeof(lut[i].records));
        swap(reinterpret_cast<unsigned char*>(&lut[i].values), sizeof(lut[i].values));
        swap(reinterpret_cast<unsigned char*>(&lut[i].bitLength), sizeof(lut[i].bitLength));
        swap(reinterpret_cast<unsigned char*>(&lut[i].physOffset), sizeof(lut[i].physOffset));
        if (lut[i].records != 4096 || lut[i].values != 4 || lut[i].bitLength != 8) {
            std::cout << "CadrgFrame::load() : Bad VQ info in compression record!" << std::endl;
            return;
        }
    }

    // Read compression lookup table
    for (int i = 0; i < 4; i++) {
        // Compression lookup subsection: loc[2]
        fin.seekg(loc[2].physicalIdx + lut[i].physOffset, std::ios::beg);
        fin.read(reinterpret_cast<char*>(table[i]), 4096 * 4);
    }

    // for i = 1 to 4 (# compression tables, 1 for each pixel row)
    for (int j = 0; j < 4096; j++) {
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < 4; i++) {
                lookupTable[j][k][i] = table[i][j][k];
            }
        }
    }

    fin.seekg(loc[1].physicalIdx, std::ios::beg);

    fin.read(reinterpret_cast<char*>(&image.spectralGroups), sizeof(image.spectralGroups));
    fin.read(reinterpret_cast<char*>(&image.subframeTables), sizeof(image.subframeTables));
    fin.read(reinterpret_cast<char*>(&image.spectralTables), sizeof(image.spectralTables));
    fin.read(reinterpret_cast<char*>(&image.spectralLines), sizeof(image.spectralLines));
    fin.read(reinterpret_cast<char*>(&image.horizSubframes), sizeof(image.horizSubframes));
    fin.read(reinterpret_cast<char*>(&image.vertSubframes), sizeof(image.vertSubframes));
    fin.read(reinterpret_cast<char*>(&image.outputCols), sizeof(image.outputCols));
    fin.read(reinterpret_cast<char*>(&image.outputRows), sizeof(image.outputRows));
    // NULL (FF) if no subfr mask table
    fin.read(reinterpret_cast<char*>(&subframeMskTblOff), sizeof(subframeMskTblOff));

    swap(reinterpret_cast<unsigned char*>(&image.spectralGroups), sizeof(image.spectralGroups));
    swap(reinterpret_cast<unsigned char*>(&image.subframeTables), sizeof(image.subframeTables));
    swap(reinterpret_cast<unsigned char*>(&image.spectralTables), sizeof(image.spectralTables));
    swap(reinterpret_cast<unsigned char*>(&image.spectralLines), sizeof(image.spectralLines));
    swap(reinterpret_cast<unsigned char*>(&image.horizSubframes), sizeof(image.horizSubframes));
    swap(reinterpret_cast<unsigned char*>(&image.vertSubframes), sizeof(image.vertSubframes));
    swap(reinterpret_cast<unsigned char*>(&image.outputCols), sizeof(image.outputCols));
    swap(reinterpret_cast<unsigned char*>(&image.outputRows), sizeof(image.outputRows));
    swap(reinterpret_cast<unsigned char*>(&subframeMskTblOff), sizeof(subframeMskTblOff));

    // ERROR Check
    if (subframeMskTblOff == 0) {
        std::cout << "CadrgFrame::load() : EROR in frame loading, sub frame mask table offset == 0.  Using old format frame file.  Run old-new converter?" << std::endl;
        return;
    }
    if (subframeMskTblOff == 0xFFFFFFFF) allSubframes = true;
    else allSubframes = false;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            // Subframe is not masked
            masked[i][j] = false;
        }
    }

    // Read mask data
    if (!allSubframes) {
        // fseek to LOC_MASK_SUBSECTION, ID=138
        if (loc[5].physicalIdx == ~0) {
            std::cout << "CadrgFrame::load() : Can't find MASK_SUBSECTION in the frame file!" << std::endl;
            return;
        }
        fin.seekg(loc[5].physicalIdx, std::ios::beg);
        // Go to offset: skip header
        fin.seekg(subframeMskTblOff, std::ios::cur);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                // Read subframe offset
                fin.read(reinterpret_cast<char*>(&subframeOff[i][j]), 4);
                swap(reinterpret_cast<unsigned char*>(&subframeOff[i][j]), 4);
                if (subframeOff[i][j] == 0xFFFFFFFF) masked[i][j] = true;
            }
        }
    }

    // must be 6 x 6 frame, or it will be masked.
    // Determine row bytes
    //int rowBytes = 256 / 4 * 3 / 2;

    // fseek to LOC_IMAGE_DISPLAY_PARAM_SUBHEADER, ID=137
    if (loc[4].physicalIdx == ~0) {
        std::cout << "CadrgFrame::load() : Can't find IMAGE_DISPLAY_PARAM_SUBHEADER section in the frame file!" << std::endl;
        return;
    }

    // Image Display Parameters Subheader
    fin.seekg(loc[4].physicalIdx, std::ios::beg);

    // No image parameters needed
    // Go to start of image spatial data subsection
    if (loc[3].physicalIdx == ~0) {
        std::cout << "CadrgFrame::load() : WARNING: Can't find Image spatial data subsection in FrameFile, using alternate computation!" << std::endl;
        // Skip 14 bytes of image display parameters subheader instead
        fin.seekg(14, std::ios::cur);
    }
    else {
        // Position at start of image spatial data subsection
        fin.seekg(loc[3].physicalIdx, std::ios::beg);
    }

    // Read subframes from top left, row-wise
    // Row
    //ushort indices[6][6];
    for (int i = 0; i < 6; i++) {
        // Column
        for (int j = 0; j < 6; j++) {
            //indices[i][j] = (ushort)(i * 6 + j);
            if (!masked[i][j]) {
                // (256/4)=64.  64*64 * 12bits / 8bits = 6144 bytes
                fin.read(reinterpret_cast<char*>(subFrameTable[i][j]), frameSize);
            }
        }
    }

    fin.close();
}


// -------------------------------------------------------------------------------------
// decompressSubframe() - Take our frame and decompress the subframe image
// -------------------------------------------------------------------------------------
int CadrgFrame::decompressSubframe(int x, int y, Subframe& subFrame)
{
    unsigned char* ptr = nullptr;
    unsigned int   val = 0;
    unsigned long  blackpixel = 255;

    const int tx = x % 6;
    const int ty = y % 6;
    x = ty;
    y = tx;

    // This should never occur since all subFrames should be present,
    // but if it does occur, just put up black pixels on the screen.
    if (((ptr = subFrameTable[y][x]) == nullptr) || masked[y][x]) {
        for (int i = 0; i < 256; i ++) {
            for (int j = 0; j < 256; j ++) {
                subFrame.image[i][j] = static_cast<unsigned char>(blackpixel);
            }
        }
    }
    else {
        for (int i = 0; i < 256; i += 4) {
            for (int j = 0; j < 256; j += 8, ptr += 3) {
                unsigned int vals = ptr[0] << 16 | ptr[1] << 8 | ptr[2];
                // Get first 12-bit value as index into VQ table
                unsigned char* imagePtr = &subFrame.image[j][i];
                val = (vals >> 12) & 0xfff;
                unsigned char* lutPtr = &lookupTable[val][0][0];
                for (int e = 0; e < 4; e++) {
                    for (int t = 0; t < 4; t++) {
                        *imagePtr = *lutPtr;
                        imagePtr++;
                        lutPtr++;
                    }
                    // 256 - 4
                    imagePtr += 252;
                }

                // Get second 12-bit value as index
                imagePtr = &subFrame.image[j + 4][i];
                val = vals & 0xfff;
                lutPtr = &lookupTable[val][0][0];
                for (int e = 0; e < 4; e++) {
                    for (int t = 0; t < 4; t++) {
                        *imagePtr = *lutPtr;
                        imagePtr++;
                        lutPtr++;
                    }
                    // 256 - 4
                    imagePtr += 252;
                }
            }
        }
    }
    return 1;
}

}
}

