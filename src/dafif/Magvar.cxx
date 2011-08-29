
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Magvar.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsMagvar::~DsMagvar()
{
}


//------------------------------------------------------------------------------
// magvar() --
//------------------------------------------------------------------------------
float DsMagvar::magvar(const double lat, const double lon, const float alt,
		       const time_t tim)
{
   double mgvar;
   if (haveData) {
      gtim = gmtime((time_t*) &tim);
      dtim = double(gtim->tm_year) + double(gtim->tm_yday) / 365.0;
      mgvar = geomag(lat, lon, (float) (alt * Distance::FT2M/1000.0), dtim);
   }
   return (float) mgvar;
}


//------------------------------------------------------------------------------
// geomag() --
//------------------------------------------------------------------------------
void DsMagvar::geomag(const int maxdeg, const char* file)
{
   std::ifstream fin(file);

   maxord   = maxdeg;
   sp[0]    = 0.0;
   cp[0]    = 1.0;
   snorm[0] = 1.0;
   pp[0]    = 1.0;
   dp[0][0] = 0.0;

   c[0][0]  = 0.0;
   cd[0][0] = 0.0;

   double e;
   char   model[21];
   char   date[21];
   fin >> e >> model >> date;

   epoc = e - double(100 * int(e / 100.0));

   while (!fin.eof() && !fin.fail())
   {
      int    n, m;
      double gnm, hnm, dgnm, dhnm;
      fin >> n >> m >> gnm >> hnm >> dgnm >> dhnm;
      if (n > 100) break;

      if (m <= n)
      {
	 c[m][n]  = gnm;
	 cd[m][n] = dgnm;
	 if (m != 0)
	 {
	    c[n][m-1]  = hnm;
	    cd[n][m-1] = dhnm;
	 }
      }
   }

   *snorm = 1.0;
   for (int n=1; n <= maxord; n++) {
      snorm[n] = snorm[n-1] * double(2 * n - 1) / double(n);
      int j = 2;
      int m, D2;
      for (m=0, D2=n-m+1; D2 > 0; D2--, m++)
      {
	 k[m][n] = double((n - 1) * (n - 1) - m * m) / double((2 * n - 1) * (2 * n - 3));
         if (m > 0)
	 {
	    double flnmj = double((n - m + 1) * j) / double(n + m);
	    snorm[n+m*13] = snorm[n+(m-1)*13] * sqrt(flnmj);
	    j = 1;
	    c[n][m-1]  = snorm[n+m*13] * c[n][m-1];
	    cd[n][m-1] = snorm[n+m*13] * cd[n][m-1];
         }
         c[m][n]  = snorm[n+m*13] * c[m][n];
         cd[m][n] = snorm[n+m*13] * cd[m][n];
      }
      fn[n] = double(n + 1);
      fm[n] = double(n);
   }
   k[1][1] = 0.0;

   olat = -999.0;
   olon = -999.0;
   oalt = -999.0;
   otim = -999.0;

   fin.close();
}


float DsMagvar::geomag(const double lat, const double lon, const float alt, const double tim)
{
   double dt = tim - epoc;

   if (otim < 0.0 && (dt < 0.0 || dt > 5.0))
   {
      static int count = 0;

      if (count++ > 50)
      {
        count = 0;
        std::cerr << "---------- geomag ----------\n" << std::endl;
        std::cerr << "warning - time extends beyond model 5-year life span\n";
        std::cerr << "contact DMA for product updates\n";
        std::cerr << "\n";
        std::cerr << "   epoc: " << epoc << "\n";
        std::cerr << "   time: " << tim << "\n";
        std::cerr << "----------------------------\n";
        std::cerr << std::endl;
      }

      return 0.0;
   }

   double a2 = 6378.137 * 6378.137;
   double b2 = 6356.7523142 * 6356.7523142;
   double c2 = a2 - b2;
   double a4 = a2 * a2;
   double b4 = b2 * b2;
   double c4 = a4 - b4;

   double srlat  = sin(lat * Angle::D2RCC);
   double crlat  = cos(lat * Angle::D2RCC);
   double srlon  = sin(lon * Angle::D2RCC);
   double crlon  = cos(lon * Angle::D2RCC);
   double srlat2 = srlat * srlat;
   double crlat2 = crlat * crlat;

   sp[1] = srlon;
   cp[1] = crlon;

   if (alt != oalt || lat != olat)
   {
      double q  = sqrt(a2 - c2 * srlat2);
      double q1 = alt * q;
      double q2 = ((q1 + a2) / (q1 + b2)) * ((q1 + a2) / (q1 + b2));

      ct = srlat / sqrt(q2 * crlat2 + srlat2);
      st = sqrt(1.0 - ct * ct);

      double r2 = alt * alt + 2.0 * q1 + (a4 - c4 * srlat2) / (q * q);
      double d  = sqrt(a2 * crlat2 + b2 * srlat2);

      ra = sqrt(r2);
      ca = (alt + d) / ra;
      sa = c2 * crlat * srlat / (ra* d);
   }

   if (lon != olon)
   {
      for (int m=2; m <= maxord; m++)
      {
	 sp[m] = sp[1] * cp[m-1] + cp[1] * sp[m-1];
	 cp[m] = cp[1] * cp[m-1] - sp[1] * sp[m-1];
      }
   }

   double aor = 6371.2 / ra;
   double ar  = aor * aor;
   double br  = 0.0;
   double bt  = 0.0;
   double bp  = 0.0;
   double bpp = 0.0;

   for (int n=1; n <= maxord; n++)
   {
      ar *= aor;
      int m, D4;
      for (m=0, D4=n+m+1; D4 > 0; D4--, m++)
      {
	 if (alt != oalt || lat != olat)
	 {
	    if (n == m)
	    {
	       snorm[n+m*13] = st * snorm[n-1+(m-1)*13];
	       dp[m][n]      = st * dp[m-1][n-1] + ct * snorm[n-1+(m-1)*13];
	       goto S50;
	    }
	    if (n == 1 && m == 0)
	    {
	       snorm[n+m*13] = ct * snorm[n-1+m*13];
	       dp[m][n]      = ct * dp[m][n-1] - st * snorm[n-1+m*13];
	       goto S50;
	    }
	    if (n > 1 && n != m)
	    {
	       if (m > n-2)
	       {
		  snorm[n-2+m*13] = 0.0;
		  dp[m][n-2]      = 0.0;
	       }
	       snorm[n+m*13] = ct * snorm[n-1+m*13] - k[m][n] * snorm[n-2+m*13];
	       dp[m][n]      = ct * dp[m][n-1] - st * snorm[n-1+m*13] - k[m][n] * dp[m][n-2];
	    }
	 }

S50:
	 if (tim != otim)
	 {
	    tc[m][n] = c[m][n] + dt * cd[m][n];
	    if (m != 0)
	       tc[n][m-1] = c[n][m-1] + dt * cd[n][m-1];
	 }

	 double temp1 = tc[m][n] * cp[m] + (m > 0 ? tc[n][m-1] * sp[m] : 0.0);
	 double temp2 = tc[m][n] * sp[m] - (m > 0 ? tc[n][m-1] * cp[m] : 0.0);

	 bt -= (ar * temp1 * dp[m][n]);
	 bp += (fm[m] * temp2 * ar * snorm[n+m*13]);
	 br += (fn[n] * temp1 * ar * snorm[n+m*13]);

	 if (st == 0.0 && m == 1)
	 {
	    if (n == 1)
	       pp[n] = pp[n-1];
	    else
	       pp[n] = ct * pp[n-1] - k[m][n] * pp[n-2];
	    bpp += (fm[m] * temp2 * ar * pp[n]);
	 }
      }
   }

   if (st == 0.0)
      bp = bpp;
   else
      bp /= st;

   double bx = -bt * ca - br * sa;
   double by = bp;

   otim = tim;
   oalt = alt;
   olat = lat;
   olon = lon;

   return float(atan2(by, bx) * Angle::R2DCC);
}
