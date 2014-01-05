//------------------------------------------------------------------------------
// Class: Locus
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Locus_H__
#define __Eaagles_Basic_Locus_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Locus
// Base class: Object -> Locus
// Factory name: Locus
//
// Description: A line of evenly spaced data points in a specific direction and
//              range from a reference point.  Or according to Webster, "Set of
//              all points whose location is determined by stated conditions"
//------------------------------------------------------------------------------
class Locus : public Object {
    DECLARE_SUBCLASS(Locus,Object)

public:
    static const unsigned int MAX_SIZE = 2048;     // Max size of the data array

public:
    Locus();
    Locus(
       const LCreal* const data,    // Array of data points
       const unsigned int n,        // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const LCreal direction,      // True direction (heading) angle of the data (degs)
       const LCreal minRng,         // Range to first data point (meters)
       const LCreal maxRng          // Range to last data point (meters)
       );

    bool isDataValid() const           { return data != 0; }   // True if we have a valid data array
    LCreal getData(const unsigned int idx) const;              // The idx'th data point, [ idx: 0 ... (getNumPts()-1) ]
    LCreal getRange(const unsigned int idx) const;             // The range to the idx'th point (meters), [ idx: 0 ... (getNumPts()-1) ]

    unsigned int getNumPts() const     { return np; }          // Number of data points
    double getRefLatitude() const      { return refLat; }      // Reference latitude (degs)
    double getRefLongitude() const     { return refLon; }      // Reference longitude (degs)
    LCreal getDirection() const        { return angle; }       // The 'true heading' angle of the data array (degs)
    LCreal getMinRng() const           { return minRng; }      // Range to the first data point (meters)
    LCreal getMaxRng() const           { return maxRng; }      // Range to the last data point (meters)
    LCreal getDeltaRange() const       { return deltaRng; }    // Distance between the data points (meters)
    const LCreal* getDataArray() const { return data; }        // The array of data points

    // Sets the data array and its stated conditions
    virtual void setData(
       const LCreal* const data,    // Array of data points
       const unsigned int n,        // Number of points in the data array
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const LCreal direction,      // True direction (heading) angle of the data (degs)
       const LCreal minRng,         // Range to first data point (meters)
       const LCreal maxRng          // Range to last data point (meters)
       );

    // Clears the data array
    virtual void clearData();

private:
    double  refLat;     // Reference latitude (degs)
    double  refLon;     // Reference longitude (degs)
    LCreal* data;       // Data array
    LCreal  angle;      // True direction (heading) angle of the data (degs)
    LCreal  minRng;     // Range to first data point (meters)
    LCreal  maxRng;     // Range to last data point  (meters)
    LCreal  deltaRng;   // Range between data points (meters)
    unsigned int np;    // Number of data points
};


} // End Basic namespace
} // End Eaagles namespace

#endif
