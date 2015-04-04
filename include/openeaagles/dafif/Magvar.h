//
// NOT USED
//

#ifndef __DsMagvar_H__
#define __DsMagvar_H__

#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class: DsMagvar
// Base class: <This is a base class>
//
// Description: Returns the mag variance for a given location.
//
//
// Public member functions:
//
//    DsMagvar()
//    DsMagvar(const char* file)
//        Constructors; 'file' is the location of the WMM data file.
//
//
//    float magvar(double latitude, double longitude, float altitude, time_t time)
//        Returns the magnetic variance for latitude, longitude, altitude,
//        and time.
//
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <float.h>
#include <cmath>
//#include "openeaagles/dafifc.h"

class DsMagvar {
public:
   DsMagvar(const char* file = "/magnavar/geomagu/wmm-95.dat");
   virtual ~DsMagvar();

   virtual float magvar(const double lat, const double lon, const float alt, const time_t tim);


private:
   void  geomag(const int maxdeg, const char* file);
   float geomag(const double lat, const double lon,
                const float alt, const double tim);

   int maxord;

   double c[13][13];
   double cd[13][13];
   double tc[13][13];
   double dp[13][13];
   double snorm[169];
   double sp[13];
   double cp[13];
   double fn[13];
   double fm[13];
   double pp[13];
   double k[13][13];

   double epoc;

   double ra;
   double ct, st;
   double ca, sa;

   double otim;
   double oalt;
   double olat;
   double olon;

   struct tm* gtim;
   double dtim;

   int haveData;
};

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
inline DsMagvar::DsMagvar(const char* file)
{
   if (file != 0) {
      geomag(12,file);
      haveData = true;
   }
   else {
      haveData = 0;
   }
}

#endif
#endif
