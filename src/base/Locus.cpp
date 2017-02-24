
#include "openeaagles/base/Locus.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Locus, "Locus")
EMPTY_SLOTTABLE(Locus)
EMPTY_SERIALIZER(Locus)

Locus::Locus()
{
   STANDARD_CONSTRUCTOR()
}

Locus::Locus(
       const double* const newData, // Array of data points
       const unsigned int n,        // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double ang,            // True direction (heading) angle of the data (degs)
       const double mn,             // Range to first data point
       const double mx              // Range to last data point
       )
{
   STANDARD_CONSTRUCTOR()
   setData(newData, n, lat, lon, ang, mn, mx);
}

void Locus::copyData(const Locus& org, const bool)
{
   BaseClass::copyData(org);

   // Let setData() copy the data
   setData(
         org.getDataArray(),
         org.getNumPts(),
         org.getRefLatitude(),
         org.getRefLongitude(),
         org.getDirection(),
         org.getMinRng(),
         org.getMaxRng()
      );
}

void Locus::deleteData()
{
   clearData();
}

//------------------------------------------------------------------------------
// Access (get) functions
//------------------------------------------------------------------------------
double Locus::getRange(const unsigned int idx) const
{
   double rng = 0;
   if (idx < np && isDataValid()) {
      rng = minRng + deltaRng * static_cast<double>(idx);
   }
   return rng;
}

double Locus::getData(const unsigned int idx) const
{
   double value = 0;
   if (idx < np && isDataValid()) {
      value = data[idx];
   }
   return value;
}

//------------------------------------------------------------------------------
// Sets new array data
//------------------------------------------------------------------------------
void Locus::setData(
       const double* const newData, // Array of data points
       const unsigned int n,        // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double ang,            // True direction (heading) angle of the data (degs)
       const double mn,             // Range to first data point
       const double mx              // Range to last data point
       )
{
   // Remove any old data
   if (data != nullptr) {
      delete[] data;
      data = nullptr;
      np = 0;
   }

   // Copy the new data
   if (n < MAX_SIZE && newData != nullptr) {
      np = n;
      data = new double[np];
      double* p = data;
      const double* q = newData;
      for (unsigned int i = 0; i < np; i++) {
         *p++ = *q++;
      }
   }

   // copy the rest of the parameters
   refLat = lat;
   refLon = lon;
   angle = ang;
   minRng = mn;
   maxRng = mx;

   // Compute the delta range
   deltaRng = 0;
   if (n > 1) {
      deltaRng = (maxRng - minRng) / (n - 1);
   }
}

//------------------------------------------------------------------------------
// Clears out the data array
//------------------------------------------------------------------------------
void Locus::clearData()
{
   // Let setData() clear the data
   setData(nullptr, 0, 0, 0, 0, 0, 0);
}

}
}
