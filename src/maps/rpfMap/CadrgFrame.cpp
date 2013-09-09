//------------------------------------------------------------------------------
// Class: CadrgFrame
//------------------------------------------------------------------------------

#include "openeaagles/maps/rpfMap/CadrgFrame.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/maps/rpfMap/CadrgFrameEntry.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

IMPLEMENT_SUBCLASS(CadrgFrame, "CadrgFrame")
EMPTY_SLOTTABLE(CadrgFrame)
EMPTY_SERIALIZER(CadrgFrame)

CadrgFrame::CadrgFrame()
{
    STANDARD_CONSTRUCTOR()
    frameEntry = 0;
    nitfHdrLength = 0;
}


// -------------------------------------------------------------------------------------
// copyData() 
// -------------------------------------------------------------------------------------
void CadrgFrame::copyData(const CadrgFrame& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) frameEntry = 0;

    if (org.frameEntry != 0) {
        if (frameEntry != 0) frameEntry->unref();
        frameEntry = org.frameEntry;
        frameEntry->ref();
    }
    nitfHdrLength = org.nitfHdrLength;    
}

// -------------------------------------------------------------------------------------
// deleteData()
// -------------------------------------------------------------------------------------
void CadrgFrame::deleteData()
{
    if (frameEntry != 0) frameEntry->unref();
    frameEntry = 0;
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
    ushort indices[6][6];
    int rowBytes = 0;

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

    if (entry != 0) {
        if (frameEntry != 0) frameEntry->unref();
        frameEntry = entry;
        frameEntry->ref();
    }

    Basic::String* string = 0;
    if (frameEntry != 0) {
        string = new Basic::String(frameEntry->getDirectory());
        string->catStr(frameEntry->getFileName());
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

    if (string != 0) string->unref();

    // National Imagery Transmission Format (NITF)  header check - skipped for now (SLS),
    // assume no NITF hdr 
    nitfHdrLength = NITF_HDR_NONE;

    // Read in our header buffer
    char buf[1024];

    fin.seekg(0,std::ios::beg);
    fin.read(buf,1024);

    // Find the end of the header
    char *ptr = strstr(buf,"RPFHDR");

    // Now calculate the length
    if (ptr) {
        int dist = int(ptr - &buf[0]);
        nitfHdrLength = dist + 11;
    }
    // Position properly 
    fin.seekg(nitfHdrLength, std::ios::beg);

    // Read header 
    unsigned char byte;
    fin.read((char *) &byte, sizeof(byte));
    head.endian = (byte != 0);
    fin.read((char *) &head.hdrSectionLength, sizeof(head.hdrSectionLength));
    if (!head.endian) swap((unsigned char *) &head.hdrSectionLength, sizeof(head.hdrSectionLength));
    fin.read((char *) head.filename, sizeof(head.filename));
    fin.read((char *) &head.nruInd, sizeof(head.nruInd));
    fin.read((char *) head.govSpecNum, sizeof(head.govSpecNum));
    fin.read((char *) head.govSpecdate, sizeof(head.govSpecdate));
    fin.read((char *) &head.secClass, sizeof(head.secClass));
    fin.read((char *) head.secCountryCode, sizeof(head.secCountryCode));
    fin.read((char *) head.secRelease, sizeof(head.secRelease));
    fin.read((char *) &head.locSecLoc, sizeof(head.locSecLoc));
    if (!head.endian) swap((unsigned char *) &head.locSecLoc, sizeof(head.locSecLoc));

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
    fin.read((char *) &compression, sizeof(compression));
    swap((unsigned char *) &compression.algorithm, sizeof(compression.algorithm));
    swap((unsigned char *) &compression.nOffRecs, sizeof(compression.nOffRecs));
    swap((unsigned char *) &compression.nParmOffRecs, sizeof(compression.nParmOffRecs));

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
    fin.read((char *) &lookupOffTblOff, sizeof(lookupOffTblOff));
    swap((unsigned char *) &lookupOffTblOff, sizeof(lookupOffTblOff));
    fin.read((char *) &lookupTblOffRecLen, sizeof(lookupTblOffRecLen));
    swap((unsigned char *) &lookupTblOffRecLen, sizeof(lookupTblOffRecLen));

    // For each compression table 
    for (int i = 0; i < 4; i++) {
        fin.read((char *) &lut[i].id, sizeof(lut[i].id));
        fin.read((char *) &lut[i].records, sizeof(lut[i].records));
        fin.read((char *) &lut[i].values, sizeof(lut[i].values));
        fin.read((char *) &lut[i].bitLength, sizeof(lut[i].bitLength));
        fin.read((char *) &lut[i].physOffset, sizeof(lut[i].physOffset));

        // Used for sanity checking 
        swap((unsigned char *) &lut[i].id, sizeof(lut[i].id));
        swap((unsigned char *) &lut[i].records, sizeof(lut[i].records));
        swap((unsigned char *) &lut[i].values, sizeof(lut[i].values));
        swap((unsigned char *) &lut[i].bitLength, sizeof(lut[i].bitLength));
        swap((unsigned char *) &lut[i].physOffset, sizeof(lut[i].physOffset));
        if (lut[i].records != 4096 || lut[i].values != 4 || lut[i].bitLength != 8) {
            std::cout << "CadrgFrame::load() : Bad VQ info in compression record!" << std::endl;
            return;
        }
    }

    // Read compression lookup table 
    for (int i = 0; i < 4; i++) {
        // Compression lookup subsection: loc[2] 
        fin.seekg(loc[2].physicalIdx + lut[i].physOffset, std::ios::beg);
        fin.read((char *) table[i], 4096 * 4);
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

    fin.read((char *) &image.spectralGroups, sizeof(image.spectralGroups));
    fin.read((char *) &image.subframeTables, sizeof(image.subframeTables));
    fin.read((char *) &image.spectralTables, sizeof(image.spectralTables));
    fin.read((char *) &image.spectralLines, sizeof(image.spectralLines));
    fin.read((char *) &image.horizSubframes, sizeof(image.horizSubframes));
    fin.read((char *) &image.vertSubframes, sizeof(image.vertSubframes));
    fin.read((char *) &image.outputCols, sizeof(image.outputCols));
    fin.read((char *) &image.outputRows, sizeof(image.outputRows));
    // NULL (FF) if no subfr mask table 
    fin.read((char *) &subframeMskTblOff, sizeof(subframeMskTblOff));

    swap((unsigned char *) &image.spectralGroups, sizeof(image.spectralGroups));
    swap((unsigned char *) &image.subframeTables, sizeof(image.subframeTables));
    swap((unsigned char *) &image.spectralTables, sizeof(image.spectralTables));
    swap((unsigned char *) &image.spectralLines, sizeof(image.spectralLines));
    swap((unsigned char *) &image.horizSubframes, sizeof(image.horizSubframes));
    swap((unsigned char *) &image.vertSubframes, sizeof(image.vertSubframes));
    swap((unsigned char *) &image.outputCols, sizeof(image.outputCols));
    swap((unsigned char *) &image.outputRows, sizeof(image.outputRows));
    swap((unsigned char *) &subframeMskTblOff, sizeof(subframeMskTblOff));

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
                fin.read((char *) &subframeOff[i][j], 4);
                swap((unsigned char *) &subframeOff[i][j], 4);
                if (subframeOff[i][j] == 0xFFFFFFFF) masked[i][j] = true;
            }
        }
    }

    // must be 6 x 6 frame, or it will be masked.
    // Determine row bytes 
    rowBytes = 256 / 4 * 3 / 2;

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
    for (int i = 0; i < 6; i++) {
        // Column
        for (int j = 0; j < 6; j++) {
            indices[i][j] = (ushort)(i * 6 + j);
            if (!masked[i][j]) {
                // (256/4)=64.  64*64 * 12bits / 8bits = 6144 bytes 
                fin.read((char *) subFrameTable[i][j], frameSize);
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
    unsigned char*  ptr;
    unsigned int    val = 0;
    unsigned long   blackpixel = 255;

    int tx = x % 6;
    int ty = y % 6;
    x = ty;
    y = tx;

    // This should never occur since all subFrames should be present, 
    // but if it does occur, just put up black pixels on the screen.  
    if (((ptr = subFrameTable[y][x]) == 0) || masked[y][x]) {
        for (int i = 0; i < 256; i ++) {
            for (int j = 0; j < 256; j ++) {
                subFrame.image[i][j] = (unsigned char)blackpixel;
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

}  // End Rpf namespace
}  // End Maps namespace
}  // End Eaagles namespace

