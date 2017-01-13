//------------------------------------------------------------------------------
// Class: CadrgClut
//------------------------------------------------------------------------------

#include "openeaagles/maps/rpf/CadrgClut.hpp"
#include "openeaagles/maps/rpf/CadrgFrameEntry.hpp"
#include "openeaagles/maps/rpf/support.hpp"
#include "openeaagles/base/String.hpp"

#include <cstring>

namespace oe {
namespace rpf {

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...

#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

IMPLEMENT_SUBCLASS(CadrgClut, "CadrgClut")
EMPTY_SLOTTABLE(CadrgClut)
EMPTY_SERIALIZER(CadrgClut)

// -------------------------------------------------------------------------------------
// Constructor()
// -------------------------------------------------------------------------------------
CadrgClut::CadrgClut()
{
    STANDARD_CONSTRUCTOR()
}

// -------------------------------------------------------------------------------------
// copyData() && deleteData()
// -------------------------------------------------------------------------------------
EMPTY_COPYDATA(CadrgClut)
EMPTY_DELETEDATA(CadrgClut)

// -------------------------------------------------------------------------------------
// load() - Load our color table from the frame entry file.
// -------------------------------------------------------------------------------------
void CadrgClut::load(CadrgFrameEntry& frame, int cib, ColorTableSizes clutSize)
{
    // Local variables
    // Header length
    int headerLength = 0;
    // Filename
    //char filename[80];
    // Location records
    Location loc[3];
    // Holds our colors locally
    Rgb rgb[CADRG_COLORS];
    // Location section physical location
    uint locSecPhysLoc = 0;
    // # of color/gray offset records
    uchar numOffsetRecs = 0;
    // # of color converter offset records
    uchar numCCOffRecs = 0;
    // Offset record length
    ushort offsetRecLen = 17;
    // Color/Gray offset record fields
    ushort tableId[MAXOFFSETRECS];
    // # Color records
    uint numColRecs[MAXOFFSETRECS];
    // Color element length
    uchar colorElemLen[MAXOFFSETRECS];
    // Histogram record length
    ushort histRecLen[MAXOFFSETRECS];
    // Color table offset
    uint colorTblOff[MAXOFFSETRECS];
    // Histogram table offset
    uint histTblOff[MAXOFFSETRECS];
    // Colormap offset table offset
    uint colorMapOffTblOff = 0;
    // Color converter subsection hdr
    // Color conversion offset table offset
    uint ccOffTblOff = 0;
    // Color converter offset recl
    ushort ccOffRect = 0;
    // cc Rectangle
    ushort ccRect = 0;
    // Color converter offset record fields
    // Color conversion table ID
    ushort cctId[MAXCCRECS];
    // Color conversion # records
    uint cctNumRecs[MAXCCRECS];
    // cc table offset
    uint ccTblOff[MAXCCRECS];
    // cc src color/gray offset table offset
    uint ccTblSrc[MAXCCRECS];
    // cc tgt color/gray offset table offset
    uint ccTblTgt[MAXCCRECS];
    // Found lookup table flag
    bool foundLut = false;
    // Temporary filename
    auto string = new base::String(frame.getDirectory());
    string->catStr(frame.getFileName());

    #if defined(WIN32)
        std::ifstream   fin(*string, std::ios::in | std::ios::binary);
    #else
        std::ifstream   fin(*string, std::ios::in);
    #endif
    // Color lookup table filename doesn't exist or failed to open
    if (fin.fail()) {
        std::cout << "CLUT FILENAME " << *string << " does not exist, or failed to open!" << std::endl;
        return;
    }

    string->unref();

    // National Imagery Transmission Format (NITF) file check - right now assumes to NITF header
    // 1st assume no NITF hdr
    headerLength = NITF_HDR_NONE;

    // Buffer to hold our header
    char buf[1024];

    // Go to beginning of file
    fin.seekg(0,std::ios::beg);
    fin.read(buf,1024);

    // Search for end of header
    char* ptr = std::strstr(buf,"RPFHDR");

    // Have valid pointer, find header length
    if (ptr) {
        int dist = static_cast<int>(ptr - &buf[0]);
        headerLength = dist + 11;
    }

    // Position properly
    fin.seekg(headerLength, std::ios::beg);

    // Skip header up to location of location section: 48-4=44 */
    fin.seekg(48 - 4, std::ios::cur);

    // Read our locations
    fin.read(reinterpret_cast<char*>(&locSecPhysLoc), sizeof(locSecPhysLoc));
    swap(reinterpret_cast<unsigned char*>(&locSecPhysLoc), sizeof(locSecPhysLoc));

    // Go to location section
    fin.seekg(locSecPhysLoc, std::ios::beg);

    // Locate the proper section
    loc[0].componentId = LOC_COLORGRAY_SECTION_SUBHEADER;
    loc[1].componentId = LOC_COLORMAP_SUBSECTION;
    loc[2].componentId = LOC_COLOR_CONVERTER_SUBSECTION;
    // Parse our locations, just like the file.
    parseLocations(fin, loc, 3);

    // If we don't get a valid index, return
    if (loc[0].physicalIdx == ~0) {
        std::cout << "CadrgClut::load() : Can't find the color/gray section subheader (ID=134) location!" << std::endl;
        return;
    }

    // Go find the color table
    fin.seekg(loc[0].physicalIdx, std::ios::beg);

    // Read section subheader
    // Number of offset records: 2
    fin.read(reinterpret_cast<char*>(&numOffsetRecs), sizeof(numOffsetRecs));
    // Number of color convertor offset records: 3
    fin.read(reinterpret_cast<char*>(&numCCOffRecs), sizeof(numCCOffRecs));

    // Check for colormap subsection: id = 135
    if (loc[1].physicalIdx == ~0) {
        std::cout << "CadrgClut::load() : Can't find colormap subsection location ID = 135!" << std::endl;
        return;
    }
    // Read color/gray offset records (colormap subsection)
    fin.seekg(loc[1].physicalIdx, std::ios::beg);

    // Colormap offset table offset: length 4
    fin.read(reinterpret_cast<char*>(&colorMapOffTblOff), sizeof(colorMapOffTblOff));
    swap(reinterpret_cast<unsigned char*>(&colorMapOffTblOff), sizeof(colorMapOffTblOff));

    // Offset record length:17? length 2
    fin.read(reinterpret_cast<char*>(&offsetRecLen), sizeof(offsetRecLen));
    swap(reinterpret_cast<unsigned char*>(&offsetRecLen), sizeof(offsetRecLen));

    // 216 or 217 colors desired.  No cct reading needed
    if (clutSize == CLUT_216) {
        // Read colormap offset table
        for (int i = 0; i < numOffsetRecs && !foundLut; i++) {
            fin.read(reinterpret_cast<char*>(&tableId[i]), sizeof(tableId[i]));
            swap(reinterpret_cast<unsigned char*>(&tableId[i]), sizeof(tableId[i]));

            fin.read(reinterpret_cast<char*>(&numColRecs[i]), sizeof(numColRecs[i]));
            swap(reinterpret_cast<unsigned char*>(&numColRecs[i]), sizeof(numColRecs[i]));

            fin.read(reinterpret_cast<char*>(&colorElemLen[i]), sizeof(colorElemLen[i]));

            fin.read(reinterpret_cast<char*>(&histRecLen[i]), sizeof(histRecLen[i]));
            swap(reinterpret_cast<unsigned char*>(&histRecLen[i]), sizeof(histRecLen[i]));

            // Color table offset
            fin.read(reinterpret_cast<char*>(&colorTblOff[i]), sizeof(colorTblOff[i]));
            swap(reinterpret_cast<unsigned char*>(&colorTblOff[i]), sizeof(colorTblOff[i]));

            // hist. table offset
            fin.read(reinterpret_cast<char*>(&histTblOff[i]), sizeof(histTblOff[i]));
            swap(reinterpret_cast<unsigned char*>(&histTblOff[i]), sizeof(histTblOff[i]));

            // Look for numColRecs[i] == 216 or 217
            if ((numColRecs[i] == 216) || (numColRecs[i] == 217)) foundLut = true;
            else foundLut = false;

            // Found a look up table, read our colors.
            if (foundLut) {
                // Read the color/gray records: 216 or 217 (transp) color table.
                // loc[1] is colormap subsection
                fin.seekg(loc[1].physicalIdx + colorTblOff[i], std::ios::beg);

                // 216 or 217
                for (int j = 0; j < static_cast<int>(numColRecs[i]); j++) {
                    // Image base reads mono byte value
                    if (cib) {
                        // Read mono byte value
                        fin.read(reinterpret_cast<char*>(&rgb[j].red), 1);
                        rgb[j].green = rgb[j].red;
                        rgb[j].blue = rgb[j].red;
                    }
                    else {
                        // rgb is rgba, size = 4
                        fin.read(reinterpret_cast<char*>(&rgb[j]), sizeof(rgb[j]));

                        // Transparency exists
                        if (numColRecs[i] == 217) {
                            // White, not black
                            rgb[numColRecs[i] - 1].red = 255;
                            rgb[numColRecs[i] - 1].green = 255;
                            rgb[numColRecs[i] - 1].blue = 255;
                        }
                    }
                }
            }
        }
    }
    // If clutSize != CLUT_216, a color convertor table is needed
    else {

        // Read Color Conversion Table first to find which color table we need to read
        // Check for Color converter subsection: id = 139
        if (loc[2].physicalIdx == ~0) {
            std::cout << "CadrgClut::load() : Can't find Color converter subsection location ID = 139!" << std::endl;
            return;
        }
        // Read cct records
        fin.seekg(loc[2].physicalIdx, std::ios::beg);

        fin.read(reinterpret_cast<char*>(&ccOffTblOff), sizeof(ccOffTblOff));
        swap(reinterpret_cast<unsigned char*>(&ccOffTblOff), sizeof(ccOffTblOff));

        fin.read(reinterpret_cast<char*>(&ccOffRect), sizeof(ccOffRect));
        swap(reinterpret_cast<unsigned char*>(&ccOffRect), sizeof(ccOffRect));

        fin.read(reinterpret_cast<char*>(&ccRect), sizeof(ccRect));
        swap(reinterpret_cast<unsigned char*>(&ccRect), sizeof(ccRect));

        // Color Converter offset table
        // 2 cct records
        for (int i = 0; i < numCCOffRecs; i++) {
            fin.read(reinterpret_cast<char*>(&cctId[i]), sizeof(cctId[i]));
            swap(reinterpret_cast<unsigned char*>(&cctId[i]), sizeof(cctId[i]));

            fin.read(reinterpret_cast<char*>(&cctNumRecs[i]), sizeof(cctNumRecs[i]));
            swap(reinterpret_cast<unsigned char*>(&cctNumRecs[i]), sizeof(cctNumRecs[i]));

            fin.read(reinterpret_cast<char*>(&ccTblOff[i]), sizeof(ccTblOff[i]));
            swap(reinterpret_cast<unsigned char*>(&ccTblOff[i]), sizeof(ccTblOff[i]));

            fin.read(reinterpret_cast<char*>(&ccTblSrc[i]), sizeof(ccTblSrc[i]));
            swap(reinterpret_cast<unsigned char*>(&ccTblSrc[i]), sizeof(ccTblSrc[i]));

            fin.read(reinterpret_cast<char*>(&ccTblTgt[i]), sizeof(ccTblTgt[i]));
            swap(reinterpret_cast<unsigned char*>(&ccTblTgt[i]), sizeof(ccTblTgt[i]));
        }

        // Go back through and read color map subsections
        for (int i = 0; i < numCCOffRecs && !foundLut; i++) {
            // Read colormap subsection for this target table: find # color/gray recs.
            fin.seekg(loc[1].physicalIdx + ccTblTgt[i], std::ios::beg);

            // We're at start of a colormap offset table
            fin.read(reinterpret_cast<char*>(&tableId[0]), sizeof(tableId[0]));
            swap(reinterpret_cast<unsigned char*>(&tableId[0]), sizeof(tableId[0]));

            fin.read(reinterpret_cast<char*>(&numColRecs[0]), sizeof(numColRecs[0]));
            swap(reinterpret_cast<unsigned char*>(&numColRecs[0]), sizeof(numColRecs[0]));

            // numColRecs[0] can't be 216 for a cct
            // Read, use 32 or 33 clrs OR read, use 16 or 17 clrs
            if ((((numColRecs[0] == 32) || (numColRecs[0] == 33)) && (clutSize == CLUT_32)) || (((numColRecs[0] == 16) || (numColRecs[0] == 17)) && (clutSize == CLUT_16))) foundLut = true;
            else foundLut = false;

            // Continue reading colormap subsection
            if (foundLut) {
                fin.read(reinterpret_cast<char*>(&colorElemLen[0]), sizeof(colorElemLen[0]));

                fin.read(reinterpret_cast<char*>(&histRecLen[0]), sizeof(histRecLen[0]));
                swap(reinterpret_cast<unsigned char*>(&histRecLen[0]), sizeof(histRecLen[0]));

                // Color table offset
                fin.read(reinterpret_cast<char*>(&colorTblOff[0]), sizeof(colorTblOff[0]));
                swap(reinterpret_cast<unsigned char*>(&colorTblOff[0]), sizeof(colorTblOff[0]));

                // hist. table offset
                fin.read(reinterpret_cast<char*>(&histTblOff[0]), sizeof(histTblOff[0]));
                swap(reinterpret_cast<unsigned char*>(&histTblOff[0]), sizeof(histTblOff[0]));

                // loc[1] is colormap subsection.  Seek to color/gray table.
                fin.seekg(loc[1].physicalIdx + colorTblOff[0], std::ios::beg);

                // Read the color/gray records:  32 or 33, or 16 or 17 color tables
                for (int j = 0; j < static_cast<int>(numColRecs[0]); j++) {
                    // rgb is rgba, size = 4
                    fin.read(reinterpret_cast<char*>(&rgb[j]), sizeof(rgb[j]));

                    // Transparency exists
                    if (numColRecs[0] == 217) {
                        // White, not black
                        rgb[numColRecs[0] - 1].red = 255;
                        rgb[numColRecs[0] - 1].green = 255;
                        rgb[numColRecs[0] - 1].blue = 255;
                    }
                }

                // Go to start of color converter table
                // loc[2] is color converter subsection
                fin.seekg(loc[2].physicalIdx + ccTblOff[i], std::ios::beg);

                // Read color conversion table: NOW 4 bytes, NOT 1
                fin.read(reinterpret_cast<char*>(cct), 4 * cctNumRecs[i]);
            }
        }
    }

    // 216 colors chosen
    if (clutSize == CLUT_216) {
        for (int j = 0; j < CADRG_COLORS; j++) cct[j] = j;
        //  cct added here instead of load_frame
        for (int j = 0; j < CADRG_COLORS; j++)  colorTable[j] = rgb[cct[j]];
    }
    // Close our input stream
    fin.close();
}

// ------------------------------------------------------------------------------------------------
// getColor() - Retrieve our color value for a specific index
// ------------------------------------------------------------------------------------------------
const CadrgClut::Rgb& CadrgClut::getColor(unsigned int index) const {
    if (index < 256) return colorTable[index];
    else {
        std::cout << "CadrgClut::getColor() - Index > 256, no colortable associated!" << std::endl;
        return colorTable[0];
    }
}

}
}

