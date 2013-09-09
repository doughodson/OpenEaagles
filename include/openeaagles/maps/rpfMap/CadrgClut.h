//------------------------------------------------------------------------------
// Class: CadrgClut
//
// Description: Used by CadrgFile, this is a Color Lookup Table (Clut), which loads a
// CadrgFrameEntry with it's proper color table
//
// Load our color table
//     void load(CadrgFrameEntry& frame, int cib, ColorTableSizes clutSize = CLUT_216);
// Get a color from the color table at a particular index
//     const CadrgClut::Rgb& getColor(unsigned int index) const;
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Rpf_CadrgClut_H__
#define __Eaagles_Maps_Rpf_CadrgClut_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Maps {
namespace Rpf {

class CadrgFrameEntry;

class CadrgClut: public Basic::Object
{
    DECLARE_SUBCLASS(CadrgClut, Basic::Object)

public:
    CadrgClut();

    // Structure to hold our RGB values.
    struct Rgb {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    };
    // Color lookup table - 216, 32 or 16 colors
    enum ColorTableSizes { CLUT_216, CLUT_32, CLUT_16 };

    // Load our color lookup table
    void load(CadrgFrameEntry& frame, int cib, ColorTableSizes clutSize = CLUT_216);
    // Get a color at a particular index from the lookup table
    const CadrgClut::Rgb& getColor(unsigned int index) const;

private:
    enum { CADRG_COLORS = 217 };            // Max CADRG colors
    static const int MAXRECLEN = 500;       // Max record length
    static const int MAXOFFSETRECS = 10;    // Max # of color/gray offset records: usually 3
    static const int MAXCCRECS = 5;         // Max # of color converter records: usually 2

    unsigned int cct[CADRG_COLORS];         // Holds the list of RGB colors

    Rgb colorTable[255];                    // Holds our individual colors
};

} // end Rpf namespace
} // end Maps namespace
} // end Eaagles namespace

#endif
