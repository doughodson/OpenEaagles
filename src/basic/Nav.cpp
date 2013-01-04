// Navigation

#include "openeaagles/basic/Nav.h"


namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Navigation constants
//------------------------------------------------------------------------------
const double Nav::WGS84_A  = 6378137.0;                  // WGS-84 semi major axis (Meters)
const double Nav::WGS84_B  = 6356752.314;                // WGS-84 semi minor axis (Meters)
const double Nav::WGS84_F  = (WGS84_A-WGS84_B)/WGS84_A;  // WGS-84 flattening
const double Nav::WGS84_E2 = WGS84_F*(2.0 - WGS84_F);    // Eccentricity Squared
const double Nav::ERADM    = Nav::WGS84_A;               // Equatorial radius (WGS84 major axis) (Meters)
const double Nav::ERAD     = Nav::ERADM/1852.0;          // Equatorial radius (WGS84 major axis) (NM)
const double Nav::ERAD60   = 3437.74677078;              // Earth Radius (NM) (1 Arc Deg = 60 NM)

//------------------------------------------------------------------------------
// Local WGS84 constants used by the legacy SubrDis conversion routines
//------------------------------------------------------------------------------
const double ellipseA    = 6378137;                         /* semi-major axis in meters */
const double ellipseF    = 1/298.257223563;                 /* flattening */
const double ellipseE2   = ellipseF * (2.0 - ellipseF);     /* Eccentricity Squared */
const double ellipseB    = ellipseA * (1.0 - ellipseF);     /* semi-minor axis in meters */
const double ellipseAsq  = ellipseA * ellipseA;
const double ellipseBsq  = ellipseB * ellipseB;


//------------------------------------------------------------------------------
// Local status codes
//------------------------------------------------------------------------------
enum Status { NORMAL, SPECIAL_CASE, BAD_INPUT, TOO_MANY_LOOPS, POLAR_POINT,
              IDENTICAL_POINTS, ANTIPODAL_POINTS };



//==============================================================================
// Navigation Functions
//==============================================================================

//------------------------------------------------------------------------------
// aer2xyzArray() -- convert 'n' sets of azimuth, elevation and range values to x, y, z positions
//------------------------------------------------------------------------------

// Using body angles
bool Nav::aer2xyzArray(
      osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  (meters)
      const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
      const double* const az,    // IN:  azimuth (body) array  (radians)
      const double* const el,    // IN:  elevation (body) array (positive up) (radians)
      const double* const rng,   // IN:  range array (meters)
      const unsigned int n       // IN:  number of sets to convert
   )
{
   // Compute sin/cos of azimuth
   double* saz = new double[n];
   double* caz = new double[n];
   sinCosArray(az,saz,caz,n);

   // Compute sin/cos of elevation
   double* sel = new double[n];
   double* cel = new double[n];
   sinCosArray(el,sel,cel,n);

   // ---
   // Compute to x, y and z positions (player coordinates)
   // ---
   osg::Vec3d* pos0 = new osg::Vec3d[n];
   for (unsigned int i = 0; i < n; i++) {
      double d = -rng[i] * sel[i];    // Down
      double r = rng[i] * cel[i];     // [Ground Range]
      double n = r * caz[i];          // North
      double e = r * saz[i];          // East
      pos0[i].set(n,e,d);
   }

   // Rotate from player to earth coordinates (NED, player centered)
   preMultVec3Array(pos0,rm,pos,n);

   return true;
}

// Using NED angles
bool Nav::aer2xyzArray(
      osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  (meters)
      const double* const az,    // IN:  azimuth (NED) array  (radians)
      const double* const el,    // IN:  elevation (NED) array (positive up) (radians)
      const double* const rng,   // IN:  range array (meters)
      const unsigned int n       // IN:  number of sets to convert
   )
{
   // Compute sin/cos of azimuth
   double* saz = new double[n];
   double* caz = new double[n];
   sinCosArray(az,saz,caz,n);

   // Compute sin/cos of elevation
   double* sel = new double[n];
   double* cel = new double[n];
   sinCosArray(el,sel,cel,n);

   // ---
   // Compute to x, y and z positions (player coordinates)
   // ---
   //osg::Vec3d* pos0 = new osg::Vec3d[n];
   for (unsigned int i = 0; i < n; i++) {
      double d = -rng[i] * sel[i];    // Down
      double r = rng[i] * cel[i];     // [Ground Range]
      double n = r * caz[i];          // North
      double e = r * saz[i];          // East
      pos[i].set(n,e,d);
   }

   return true;
}

//------------------------------------------------------------------------------
// Great circle: Computes the destination (target) lat/lon from starting (ref)
// point given distance and initial bearing.
//------------------------------------------------------------------------------

// ---
// Great circle method #1 -- Elliptical earth model
// ---
bool Nav::gbd2ll(
      const double slat,   // IN: Starting (Reference) latitude (degs)
      const double slon,   // IN: Starting (Reference) longitude (degs)
      const double brg,    // IN: True bearing (degs)
      const double dist,   // IN: Distance (ground range) (nm)
      double* const dlat,  // OUT: Target latitude (degs)
      double* const dlon,  // OUT: Target longitude (degs)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   // Initialize earth model parameters
   const EarthModel* pModel = em;
   if (pModel == 0) { pModel = &EarthModel::wgs84; }

   const double eemA  = Distance::M2NM * pModel->getA();
   //const double eemF  = pModel->getF();
   const double eemE2 = pModel->getE2();

   // ---
   // *** Convert slat/slon & brg to radians **
   // ---
   double slatr = slat * Angle::D2RCC;
   double slonr = slon * Angle::D2RCC;
   double psi = Angle::aepcdDeg(brg) * Angle::D2RCC;

   // ---
   // *** Transform source point about zero longitude **
   // ---
   double tslatr = slatr;
   //double tslonr = 0.0;

   // ---
   // *** Calculate Gaussian radius of curvature at source lat **
   // ---
   double grad = eemA * (1.0 - ((eemE2 / 2.0) * std::cos(2.0 * tslatr))); // Gaussian radius

   // ---
   // *** Compute transformed destination lat/lon **
   // ---
   double tdlatr = 0.0;
   double tdlonr = -slonr;
   if (dist <= 10000.0) {
      double x = std::cos(dist / grad) * std::sin(tslatr);
      double y = std::sin(dist / grad) * std::cos(tslatr) * std::cos(psi);
      tdlatr = std::asin(x + y);

      x = std::cos(dist / grad) - std::sin(tslatr) * std::sin(tdlatr);
      y = std::cos(tslatr) * std::cos(tdlatr);

      double z = 0;
      if (y != 0.0) z = x / y;
      else z = (x >= 0 ? 1.0 : -1.0);
      z = alimd(z, 1.0);

      tdlonr = std::acos(z);

      if( psi < 0.0 ) tdlonr = -tdlonr;
   }

   // ---
   // *** Retransform destination point **
   // ---
   double dlatr = tdlatr;
   double dlonr = tdlonr + slonr;

   // ---
   // *** Convert to degrees **
   // ---
   double dlat0 = dlatr * Angle::R2DCC;
   double dlon0 = dlonr * Angle::R2DCC;

   // ---
   // *** Apply ellipsoidal correction **
   // ---
   double ellip = 0.00334 * pow( std::cos(tslatr), 2 );
   dlat0 = dlat0 - ellip * (dlat0 - slat);
   dlon0 = dlon0 + ellip * (dlon0 - slon);

   // *** Limit check for destination longitude **
   if (dlon0 > +180.) { dlon0 = dlon0 - 360.0; }
   else if (dlon0 < -180.) { dlon0 = dlon0 + 360.0; }

   // ---
   // *** Return to caller **
   // ---
   *dlat = dlat0;
   *dlon = dlon0;
   return true;
}

// ---
// Great circle method #2 -- Spherical earth with radius 'ERAD60'
// reference: "ARRL"
// ---
bool Nav::gbd2llS(
      const double slat,   // IN: Starting (Reference) latitude (degs)
      const double slon,   // IN: Starting (Reference) (degs)
      const double brg,    // IN: True bearing (degs)
      const double dist,   // IN: Distance (ground range) (nm)
      double* const dlat,  // OUT: Target latitude (degs)
      double* const dlon   // OUT: Target longitude (degs)
   )
{
   double arc     = dist / ERAD60;
   double sinArc  = std::sin(arc);
   double cosArc  = std::cos(arc);
   double sinLat1 = std::sin(Angle::D2RCC * slat);
   double cosLat1 = std::cos(Angle::D2RCC * slat);
   double sinBrng = std::sin(Angle::D2RCC * brg);

   // -----------------------------------------------------
   // compute latitude
   double k1 = sinLat1 * cosArc;
   double k2 = cosLat1 * sinArc * std::cos(Angle::D2RCC * brg);
   double k3 = k1 + k2;
   k3 = alimd(k3, 1.0);
   double k4 = dist * std::asin(k3);
   k4 = alimd(k4, 1.0);
   *dlat = Angle::R2DCC * std::asin(k3);

   // -----------------------------------------------------
   // compute longitude
   k1 = sinBrng * sinArc * cosLat1;
   k2 = cosArc - sinLat1 * (k1 + k2);
   k3 = Angle::R2DCC * std::atan2(k1, k2);
   *dlon = Angle::aepcdDeg(slon + k3);

   return true;
}

//------------------------------------------------------------------------------
// Great circle: Computes the initial bearing and the distance from the starting
// lat/lon (ref point) to the destination lat/lon using great circle methods.
//------------------------------------------------------------------------------

// ---
// Great circle method #1 -- Elliptical earth model
// ---
bool Nav::gll2bd(
      const double slat,   // IN: Starting (reference) latitude (degs)
      const double slon,   // IN: Starting (reference) longitude (degs)
      const double dlat,   // IN: Destination latitude (degs)
      const double dlon,   // IN: Destination longitude (degs)
      double* const brg,   // OUT: True bearing (degs)
      double* const dist,     // OUT: distance (ground range) (nm)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   // Initialize earth model parameters
   const EarthModel* pModel = em;
   if (pModel == 0) { pModel = &EarthModel::wgs84; }

   const double eemA  = Distance::M2NM * pModel->getA();
   //const double eemF  = pModel->getF();
   const double eemE2 = pModel->getE2();

   // Early out: check for source and destination at same point.
   if( (dlat == slat) && ( dlon == slon )) {
      *dist = 0.0;
      *brg  = 0.0;
      return true;
   }

   // ---
   // *** Transform destination lat/lon into the equivalent spherical lat/lon
   // ---

   // Ellipsoidal correction factor
   double ellip = 0.00334 * pow( std::cos(slat*Angle::D2RCC), 2 );

   double dlat0 = Angle::aepcdDeg( dlat + ellip * Angle::aepcdDeg(dlat - slat) );
   double dlon0 = Angle::aepcdDeg( dlon - ellip * Angle::aepcdDeg(dlon - slon) );

   // *** Transform lat/lon about zero longitude **
   double tslat = slat;          // Transformed source lat (deg)
   double tdlat = dlat0;         // Transformed destination lat (deg)
   //double tslon = 0.0;         // Transformed source lon (deg)
   double tdlon = dlon0 - slon;  // Transformed destination lon (deg)
   if (tdlon < -180.0) { tdlon = tdlon + 360.0; }
   else if (tdlon > +180.0) { tdlon = tdlon - 360.0; }

   // ---
   // *** Convert lat/lon to radians **
   // ---
   double tslatr = tslat * Angle::D2RCC;  // Transformed source lat (rad)
   //double tslonr = tslon * Angle::D2RCC;  // Transformed source lon (rad)
   double tdlatr = tdlat * Angle::D2RCC;  // Transformed destination lat (rad)
   double tdlonr = tdlon * Angle::D2RCC;  // Transformed destination lon (rad)

   // ---
   // *** Calculate Gaussian radius of curvature at source lat **
   // ---
   double grad = eemA * (1.0 - ((eemE2 / 2.0) * std::cos(2.0 * tslatr)));   // Gaussian radius

   // ---
   // *** Compute great circle distance **
   // ---
   double tzlonr = tdlonr;// Lon deviation(rad)
   double x = std::sin(tslatr) * std::sin(tdlatr);
   double y = std::cos(tslatr) * std::cos(tdlatr) * std::cos(tzlonr);
   double z = x + y;
   z = alimd(z, 1.0);

   *dist = grad * fabs(std::acos(z));
   if(*dist == 0.0) {
      *brg = 0.0;
      return true;
   }

   // ---
   // *** Compute great circle bearing **
   // ---
   x = std::sin(tdlatr) - std::sin(tslatr) * std::cos(*dist / grad);
   y = std::sin(*dist / grad) * std::cos(tslatr);
   if (y != 0.0) z = x / y;
   else z = (x >= 0 ? 1.0 : -1.0);
   z = alimd(z, 1.0);

   x = std::acos(z) * Angle::R2DCC;
   if (tzlonr < 0.0) x = 360.0 - x;
   *brg = Angle::aepcdDeg(x);

   return true;
}

// ---
// Great circle method #2 -- Spherical earth with radius 'ERAD60'
// reference: "ARRL"
// ---
bool Nav::gll2bdS(
      const double slat,   // IN: Starting (reference) latitude (degs)
      const double slon,   // IN: Starting (reference) longitude (degs)
      const double dlat,   // IN: Destination latitude (degs)
      const double dlon,   // IN: Destination longitude (degs)
      double* const brg,   // OUT: True bearing (degs)
      double* const dist   // OUT: distance (ground range) (nm)
   )
{
   double sinLat1 = std::sin(Angle::D2RCC * slat);
   double cosLat1 = std::cos(Angle::D2RCC * slat);
   double sinLat2 = std::sin(Angle::D2RCC * dlat);
   double cosLat2 = std::cos(Angle::D2RCC * dlat);
   double sinDLon = std::sin(Angle::D2RCC * (dlon - slon));
   double cosDLon = std::cos(Angle::D2RCC * (dlon - slon));

   // -----------------------------------------------------
   // compute distance
   if (dist != 0) {
      double k  = sinLat1 * sinLat2 + cosLat1 * cosLat2 * cosDLon;
      k = alimd(k, 1.0);
      *dist = ERAD60 * std::acos(k);
   }

   // -----------------------------------------------------
   // compute bearing
   if (brg != 0) {
      double k1 = sinDLon * cosLat2;
      double k2 = cosLat1 * sinLat2 - sinLat1 * cosLat2 * cosDLon;
      double k3 = std::atan2(k1, k2);
      *brg = Angle::aepcdDeg(Angle::R2DCC * k3);
   }

   return true;
}


//------------------------------------------------------------------------------
// Great circle: Computes the initial bearing, slant range, ground distance and
// elevation angle from the starting lat/lon (ref point) to the destination
// lat/lon using great circle methods.
// (elevation does not account for earth curvature at this time)
//------------------------------------------------------------------------------

// ---
// Great circle method #1 -- Elliptical earth model
// ---
bool Nav::glla2bd(
      const double slat,         // IN: Starting (reference) latitude (degs)
      const double slon,         // IN: Starting (reference) longitude (degs)
      const double salt,         // IN: Starting (reference) altitude (meters)
      const double dlat,         // IN: Destination latitude (degs)
      const double dlon,         // IN: Destination longitude (degs)
      const double dalt,         // IN: Destination altitude (meters)
      double* const brg,         // OUT: True bearing (degs)
      double* const slantRng,    // OUT: slant range (nm)
      double* const dist,        // OUT: distance (ground range) (nm)
      double* const elev,        // OUT: Optional: elevation angle (degs)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   // Delta altitudes (in NMs)
   double deltaAlt = (dalt - salt) * Distance::M2NM;

   // Early out: check for source and destination at same point.
   if( (dlat == slat) && ( dlon == slon ) ) {
      *slantRng = deltaAlt;
      *dist = 0.0;
      *brg  = 0.0;
      if (elev != 0) {
         if (deltaAlt > 0)
            *elev = 90.0;
         else
            *elev = -90.0;
      }
      return true;
   }

   bool ok = Nav::gll2bd(slat,slon, dlat, dlon, brg, dist, em);

   if (ok) {
      *slantRng = std::sqrt ( (*dist) * (*dist) + deltaAlt * deltaAlt );

      if (elev != 0 && (*slantRng) > 0) {
         *elev = Angle::R2DCC * std::asin(deltaAlt/ (*slantRng));
      }
   }

   return ok;
}

// ---
// Great circle method #2 -- Spherical earth with radius 'ERAD60'
// ---
bool Nav::glla2bdS(
      const double slat,         // IN: Starting (reference) latitude (degs)
      const double slon,         // IN: Starting (reference) longitude (degs)
      const double salt,         // IN: Starting (reference) altitude (meters)
      const double dlat,         // IN: Destination latitude (degs)
      const double dlon,         // IN: Destination longitude (degs)
      const double dalt,         // IN: Destination altitude (meters)
      double* const brg,         // OUT: True bearing (degs)
      double* const slantRng,    // OUT: slant range (nm)
      double* const dist,        // OUT: distance (ground range) (nm)
      double* const elev         // OUT: (optional) elevation angle (degs)
   )
{
   // Delta altitudes (in NMs)
   double deltaAlt = (dalt - salt) * Distance::M2NM;

   // Early out: check for source and destination at same point.
   if( (dlat == slat) && ( dlon == slon ) ) {
      *slantRng = deltaAlt;
      *dist = 0.0;
      *brg  = 0.0;
      if (elev != 0) {
         if (deltaAlt > 0)
            *elev = 90.0;
         else
            *elev = -90.0;
      }
      return true;
   }

   bool ok = Nav::gll2bdS(slat,slon, dlat, dlon, brg, dist);

   if (ok) {
      *slantRng = std::sqrt ( (*dist) * (*dist) + deltaAlt * deltaAlt );
      if (elev != 0 && (*slantRng) > 0) {
         *elev = Angle::R2DCC * std::asin(deltaAlt/ (*slantRng));
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Vincenty Direct: Computes the destination (target) lat/lon from starting
// (ref) point given the distance and initial bearing using the Vincenty
// direct method.
//
//    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
//                Applications of Nested Equations"  by Thaddeus Vincenty
//------------------------------------------------------------------------------
bool Nav::vbd2ll(
      const double slat,         // IN: Starting (Reference) latitude (degs)
      const double slon,         // IN: Starting (Reference) longitude (degs)
      const double brng,         // IN: True bearing (degs)
      const double dist,         // IN: Distance (ground range) (nm)
      double* const dlat,        // OUT: Target latitude (degs)
      double* const dlon,        // OUT: Target longitude (degs)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   //---------------------------------------------
   // Initialize earth model parameters
   //---------------------------------------------
   const EarthModel* pModel = em;
   if (pModel == 0) { pModel = &EarthModel::wgs84; }

   const double eemA  = pModel->getA();
   const double eemF  = pModel->getF();
   const double eemB  = pModel->getB();
   //const double eemE2 = pModel->getE2();

   //-----------------------------------
   // local constants
   //-----------------------------------
   const double tanU1       = (1.0 - eemF) * std::tan(Angle::D2RCC * slat);
   const double u1          = std::atan(tanU1);
   const double sinU1       = std::sin(u1);
   const double cosU1       = std::cos(u1);
   const double alpha1      = Angle::D2RCC * brng;
   const double sinAlpha1   = std::sin(alpha1);
   const double cosAlpha1   = std::cos(alpha1);
   const double sigma1      = std::atan2(tanU1, cosAlpha1);
   //const double tanSigma1   = tanU1 / cosAlpha1;                        // Eq. 1
   const double sinAlpha    = cosU1 * sinAlpha1;                        // Eq. 2
   const double cosSqrAlpha = 1.0 - sinAlpha * sinAlpha;

   const double ra          = eemA;
   const double rb          = eemB;
   const double Usqr        = cosSqrAlpha * (ra*ra - rb*rb) / (rb*rb);
   const double a = 1.0 + (Usqr/16384.0)*(4096.0 + Usqr*(-768.0 + Usqr*(320.0 - 175.0*Usqr)));   // Eq. 3
   const double b =       (Usqr/ 1024.0)*( 256.0 + Usqr*(-128.0 + Usqr*( 74.0 -  47.0*Usqr)));   // Eq. 4

   const double EPS         = 1.0E-12;

   //-----------------------------------
   // intermediate variables
   //-----------------------------------
   double lastSigma     = 0.0;
   double twoSigmaM     = 0.0;
   double cos2SigmaM    = 0.0;
   double cosSqr2SigmaM = 0.0;
   double sinSigma      = 0.0;
   double cosSigma      = 0.0;
   double delSigma      = 0.0;
   double sinSqrSigma   = 0.0;
   double lambda        = 0.0;
   double c             = 0.0;
   double err           = 0.0;
   double p             = 0.0;
   double q             = 0.0;
   double r             = 0.0;

   //-----------------------------------
   // initialization
   //-----------------------------------
   double s     = dist*Distance::NM2M;  // geodesic distance in meters
   double baseS = (s / eemB / a);
   double sigma = baseS;

   //-----------------------------------
   // begin iteration loop
   //-----------------------------------
   do {
      twoSigmaM     = 2.0*sigma1 + sigma;                               // Eq. 5
      cos2SigmaM    = std::cos(twoSigmaM);
      cosSqr2SigmaM = cos2SigmaM * cos2SigmaM;

      cosSigma      = std::cos(sigma);
      sinSigma      = std::sin(sigma);
      sinSqrSigma   = sinSigma * sinSigma;

      p = -3.0 + 4.0*cosSqr2SigmaM;
      q = (b/6.0) * cos2SigmaM * (-3.0 + 4.0*sinSqrSigma) * p;
      r = (b/4.0) * (cosSigma  * (-1.0 + 2.0*cosSqr2SigmaM) - q);
      delSigma      = b * sinSigma * (cos2SigmaM + r);                  // Eq. 6

      lastSigma     = sigma;
      sigma         = baseS + delSigma;                                 // Eq. 7
      err           = std::fabs(sigma - lastSigma);
   } while (err > EPS);

   //-----------------------------------
   // calculate destination latitude
   //-----------------------------------
   sinSigma      = std::sin(sigma);
   cosSigma      = std::cos(sigma);

   twoSigmaM     = 2.0*sigma1 + sigma;                                  // Eq. 5
   cos2SigmaM    = std::cos(twoSigmaM);
   cosSqr2SigmaM = cos2SigmaM * cos2SigmaM;

   p = sinU1 * cosSigma + cosU1 * sinSigma * cosAlpha1;
   q = sinU1 * sinSigma - cosU1 * cosSigma * cosAlpha1;
   r = (1.0 - eemF) * std::sqrt(sinAlpha * sinAlpha + q*q);
   *dlat = Angle::R2DCC * std::atan2(p, r);                             // Eq. 8

   //-----------------------------------
   // calculate destination longitude
   //-----------------------------------
   p = sinSigma * sinAlpha1;
   q = cosU1 * cosSigma - sinU1 * sinSigma * cosAlpha1;
   lambda = std::atan2(p, q);                                           // Eq. 9

   r = (4.0 + eemF * (4.0 - 3.0 * cosSqrAlpha));
   c = (eemF / 16.0) * cosSqrAlpha * r;                                 // Eq. 10
   p = (cos2SigmaM + c * cosSigma * (-1.0 + 2.0 * cosSqr2SigmaM));
   q = (1.0 - c) * eemF * sinAlpha * (sigma + c * sinSigma * p);
   *dlon = Angle::aepcdDeg(slon + Angle::R2DCC*(lambda - q));

   return true;
}


//------------------------------------------------------------------------------
// Vincenty Inverse: Computes the initial and final bearings and the distance
// from the starting lat/lon (ref point) to destination lat/lon using the
// Vincenty Inverse Method.
//
//    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
//               Applications of Nested Equations"  by Thaddeus Vincenty
//
//------------------------------------------------------------------------------
bool Nav::vll2bd(
      const double slat,         // IN: Starting (reference) latitude (degs)
      const double slon,         // IN: Starting (reference) longitude (degs)
      const double dlat,         // IN: Destination latitude (degs)
      const double dlon,         // IN: Destination longitude (degs)
      double* const brng,        // OUT: True initial bearing (degs)
      double* const dist,        // OUT: geodesic distance (nm)
      double* const brng2,       // OUT: Optional:  True final bearing (degs)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   //---------------------------------------------
   // Initialize earth model parameters
   //---------------------------------------------
   const EarthModel* pModel = em;
   if (pModel == 0) { pModel = &EarthModel::wgs84; }

   const double eemA  = pModel->getA();
   const double eemF  = pModel->getF();
   const double eemB  = pModel->getB();
   //const double eemE2 = pModel->getE2();

   //-----------------------------------
   // local constants
   //-----------------------------------
   //const double EPS      = 1.0E-12;
   const double deltaLon = Angle::aepcdDeg(dlon - slon);
   const double l        = Angle::D2RCC * deltaLon;
   const double u1       = std::atan((1.0 - eemF) * std::tan(Angle::D2RCC * slat));
   const double u2       = std::atan((1.0 - eemF) * std::tan(Angle::D2RCC * dlat));
   const double sinU1    = std::sin(u1);
   const double cosU1    = std::cos(u1);
   const double sinU2    = std::sin(u2);
   const double cosU2    = std::cos(u2);
   const double ra       = eemA;
   const double rb       = eemB;

   //-----------------------------------
   // intermediate variables
   //-----------------------------------
   double oldLambda     = 0.0;
   double sinLambda     = 0.0;
   double cosLambda     = 0.0;
   double cosSqrAlfa    = 0.0;
   double cos2SigmaM    = 0.0;
   double cosSqr2SigmaM = 0.0;
   double sigma         = 0.0;
   double sinSigma      = 0.0;
   double cosSigma      = 0.0;
   double sinSqrSigma   = 0.0;
   double sinAlfa       = 0.0;
   double c             = 0.0;
   double p             = 0.0;
   double q             = 0.0;
   double r             = 0.0;

   //-----------------------------------
   // check for identical or antipodal input points
   //-----------------------------------
   //int status = NORMAL;

   bool b1 = (slat ==  dlat) && (slon == dlon);                         // identical points
   //if (b1) status = IDENTICAL_POINTS;

   bool b2 = (slat == -dlat) && (std::fabs(deltaLon) == 180.0);         // antipodal points
   //if (b2) status = ANTIPODAL_POINTS;

   if (b1 || b2)  return false;

   //-----------------------------------
   // initialization
   //-----------------------------------
   double lambda = l;

   //-----------------------------------
   // begin iteration loop
   //-----------------------------------
   do {
      sinLambda  = std::sin(lambda);
      cosLambda  = std::cos(lambda);
      p          = cosU2 * sinLambda;
      q          = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
      sinSigma   = std::sqrt(p*p + q*q);                                // Eq. 14
      cosSigma   = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;                 // Eq. 15
      sigma      = std::atan2(sinSigma, cosSigma);                      // Eq. 16
      sinAlfa    = cosU1 * cosU2 * sinLambda / sinSigma;                      // Eq. 17
      cosSqrAlfa = 1.0 - sinAlfa * sinAlfa;

      if (cosSqrAlfa != 0.0)
         cos2SigmaM = cosSigma - 2.0 * sinU1 * sinU2 / cosSqrAlfa;           // Eq. 18
      else
         cos2SigmaM = 0.0;

      cosSqr2SigmaM = cos2SigmaM * cos2SigmaM;
      p = (4.0 + eemF * (4.0 - 3.0 * cosSqrAlfa));
      c = (eemF/16.0) * cosSqrAlfa * p;                                 // Eq. 10

      oldLambda = lambda;
      p = (cos2SigmaM + c * cosSigma * (-1.0 + 2.0 * cosSqr2SigmaM));
      q = eemF * sinAlfa * (sigma + c * sinSigma * p);
      lambda = l + (1.0 - c) * q;                                       // Eq. 11

   } while (std::fabs(lambda - oldLambda) > 1.0e-10);

   //-----------------------------------
   // calculate start bearing (deg)
   //-----------------------------------
   sinLambda = std::sin(lambda);
   cosLambda = std::cos(lambda);

   p = cosU2 * sinLambda;
   q = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
   double Alfa1 = std::atan2(p, q);                                     // Eq. 20
   *brng = Angle::aepcdDeg(Angle::R2DCC * Alfa1);

   //-----------------------------------
   // calculate destination bearing (deg)
   //-----------------------------------
   if (brng2 != 0) {
      p =  cosU1 * sinLambda;
      q = -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda;
      double Alfa2 = std::atan2(p, q);                                     // Eq. 21
      *brng2 = Angle::aepcdDeg(180.0 + Angle::R2DCC * Alfa2);
   }

   //-----------------------------------
   // calculate geodesic distance (NM)
   //-----------------------------------
   double Usqr  = cosSqrAlfa * (ra*ra - rb*rb) / (rb*rb);

   p = (-768.0 + Usqr * (320.0 - 175.0 * Usqr));
   q = (Usqr / 16384.0) * (4096.0 + Usqr * p);
   double a = 1.0 + q;                                                  // Eq. 3

   p = (-128.0 + Usqr * ( 74.0 -  47.0 * Usqr));
   q = (Usqr / 1024.0) * ( 256.0 + Usqr * p);
   double b = q;                                                        // Eq. 4

   p = (-3.0 + 4.0 * cosSqr2SigmaM);
   q = (b / 6.0) *  cos2SigmaM * (-3.0 + 4.0 * sinSqrSigma) * p;
   r = (b / 4.0) * (cosSigma * (-1.0 + 2.0 * cosSqr2SigmaM) - q);
   double delSigma = b * sinSigma * (cos2SigmaM + r);                   // Eq. 6

   *dist = (eemB * a * (sigma - delSigma)) * Distance::M2NM;

   return true;
}

//==============================================================================
// Matrix generators
//==============================================================================

//------------------------------------------------------------------------------
// Compute a rotational matrix from Euler angles
//------------------------------------------------------------------------------
bool Nav::computeRotationalMatrix(
      const double phi,          // IN: roll angle (radians)
      const double theta,        // IN: pitch angle (radians)
      const double psi,          // IN: yaw angle (radians)
      osg::Matrixd* const m,     // OUT: Matrix M
      osg::Vec2d* const scPhi,   // OUT: Sin/Cos of phi (Optional)
      osg::Vec2d* const scTht,   // OUT: Sin/Cos of theta (Optional)
      osg::Vec2d* const scPsi    // OUT: Sin/Cos of psi (Optional)
   )
{
   double sphi = std::sin(phi);
   double cphi = std::cos(phi);
   double stht = std::sin(theta);
   double ctht = std::cos(theta);
   double spsi = std::sin(psi);
   double cpsi = std::cos(psi);

   if (scPhi != 0) scPhi->set( sphi, cphi  );
   if (scTht != 0) scTht->set( stht, ctht  );
   if (scPsi != 0) scPsi->set( spsi, cpsi  );

   if (m != 0) {
      (*m)(0,0) = (+ctht*cpsi);
      (*m)(0,1) = (+ctht*spsi);
      (*m)(0,2) = (-stht);
      (*m)(0,3) = 0;

      (*m)(1,0) = (-cphi*spsi + sphi*stht*cpsi);
      (*m)(1,1) = (+cphi*cpsi + sphi*stht*spsi);
      (*m)(1,2) = (+sphi*ctht);
      (*m)(1,3) = 0;

      (*m)(2,0) = (+sphi*spsi + cphi*stht*cpsi);
      (*m)(2,1) = (-sphi*cpsi + cphi*stht*spsi);
      (*m)(2,2) = (+cphi*ctht);
      (*m)(2,3) = 0;

      (*m)(3,0) = 0;
      (*m)(3,1) = 0;
      (*m)(3,2) = 0;
      (*m)(3,3) = 1;
   }

   return true;
}

//------------------------------------------------------------------------------
// Euler angles from a rotational matrix:
//------------------------------------------------------------------------------
bool Nav::computeEulerAngles(
      const osg::Matrixd& rm,    // IN: Rotational matrix
      osg::Vec3d* const angles,  // OUT: Euler angles (radians)
      osg::Vec2d* const scPhi,   // OUT: Sin/Cos of phi (Optional)
      osg::Vec2d* const scTht,   // OUT: Sin/Cos of theta (Optional)
      osg::Vec2d* const scPsi    // OUT: Sin/Cos of psi (Optional)
   )
{
   double stht = -rm(0,2);
   if (-1.0 > stht) stht = -1.0;
   if ( 1.0 < stht) stht =  1.0;

   double ctht = std::sqrt(1.0 - stht*stht);


   double sphi = 0;
   double cphi = 1;
   if (ctht > 0) {
      sphi = rm(1,2)/ctht;
      if ( 1.0 < sphi) sphi =  1.0;
      if (-1.0 > sphi) sphi = -1.0;

      cphi = rm(2,2)/ctht;
      if ( 1.0 < cphi) cphi =  1.0;
      if (-1.0 > cphi) cphi = -1.0;
   }

   double spsi = rm(2,0)*sphi - rm(1,0)*cphi;
   if ( 1.0 < spsi) spsi =  1.0;
   if (-1.0 > spsi) spsi = -1.0;

   double cpsi = rm(1,1)*cphi - rm(2,1)*sphi;
   if ( 1.0 < cpsi) cpsi =  1.0;
   if (-1.0 > cpsi) cpsi = -1.0;

   // ---
   // Set output values
   // ---
   if (angles != 0) {
      angles->set( atan2(sphi,cphi), atan2(stht,ctht), atan2(spsi,cpsi) );
   }
   if (scPhi != 0) scPhi->set( sphi, cphi  );
   if (scTht != 0) scTht->set( stht, ctht  );
   if (scPsi != 0) scPsi->set( spsi, cpsi  );

   return true;
}

//------------------------------------------------------------------------------
// Earth transformation matrix:
//       World (ECEF) <==> Inertial (NED)
//
//
//    World (ECEF): origin is at the center of the earth where X+ points to zero
//    latitude and zero longitude (i.e., equator at the Greenwich meridian) and Z+
//    points to the north pole -- ECEF rotates with the earth.
//
//    Inertial (NED): the local tangent plane where X+ points north, Y+ points east,
//    and Z+ points 'down' and is perpendicular to the tangent plane
//
// output:
//    M = Ry[-(90+lat)] * Rz[lon]
//
// Usage:
//    Vi  = M * Vw;
//    Vw = Vi * M;    // same as Vw = M[T] * Vi
//       Vi is an initial vector
//       Vw is a world (ECEF) vector
//------------------------------------------------------------------------------
bool Nav::computeWorldMatrix(
      const double latD,      // IN: Reference latitude (degs)
      const double lonD,      // IN: Reference longitude (degs)
      osg::Matrixd* const m   // OUT: Matrix M
   )
{
   double phi  = 0;
   double theta = -(90.0 + latD) * Angle::D2RCC;
   double psi   = lonD * Angle::D2RCC;
   computeRotationalMatrix(phi, theta, psi, m);
   return true;
}


//==============================================================================
// Coordinate conversion functions
//    Earth centered, earth fixed (ECEF) coordinates <==> Geodetic coordinates
//
// Currently hardcoded to WGS84 geodetic coordinates
//==============================================================================

//----------------------------------------------------------
// Convert ECEF (XYZ coordinates) to Geodetic (LLA coordinates)
//----------------------------------------------------------
bool Nav::convertEcef2Geod(
      const double x,      // IN: ECEF X component   (meters)
      const double y,      // IN: ECEF Y component   (meters)
      const double z,      // IN: ECEF Z component   (meters)
      double* const pLat,  // OUT: Geodetic latitude  (degrees)
      double* const pLon,  // OUT: Geodetic longitude (degrees)
      double* const pAlt,  // OUT: Geodetic altitude  (meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   //---------------------------------------------
   // Initialize earth model parameters
   //---------------------------------------------
   const EarthModel* pModel = em;
   if (pModel == 0) { pModel = &EarthModel::wgs84; }

   const double a  = pModel->getA();
   //const double f  = pModel->getF();
   const double b  = pModel->getB();
   const double e2 = pModel->getE2();

   //---------------------------------------------
   // Define Local Constants
   //---------------------------------------------
   const double p  = std::sqrt(x*x + y*y);
   const double ACCURACY = 0.1;  // iterate to accuracy of 0.1 meter
   const double EPS = 1.0E-10;
   const int    MAX_LOOPS = 10;

   //---------------------------------------------
   // Initialize Local Variables
   //---------------------------------------------
   int status  = NORMAL;

   double rn   = a;
   double phi  = 0.0;
   double oldH = 0.0;
   double newH = 100.0 * ACCURACY;  // (newH - oldH) significantly different
   int idx = 0;

   //---------------------------------------------
   // check status
   //---------------------------------------------
   double polarXY = std::fabs(x) + std::fabs(y);
   if (polarXY < EPS) { status = POLAR_POINT; }

   //---------------------------------------------
   // iterate for accurate latitude and altitude
   //---------------------------------------------
   if (status == NORMAL) {
      while ((++idx <= MAX_LOOPS) && (std::fabs(newH - oldH) > ACCURACY)) {
         double sinPhi = z / (newH + rn*(1.0 - e2));
         double q      = z + e2*rn*sinPhi;
         phi           = std::atan2(q, p);
         double cosPhi = std::cos(phi);
         double w      = std::sqrt(1.0 - e2*sinPhi*sinPhi);
         rn            = a/w;
         oldH          = newH;
         newH          = p/cosPhi - rn;
      }
   }

   //---------------------------------------------
   // re-check status after iteration
   //---------------------------------------------
   if (idx > MAX_LOOPS) { status = TOO_MANY_LOOPS; }

   //---------------------------------------------
   // process based on status
   //---------------------------------------------
   switch (status) {
      case NORMAL: {
   // begin iteration loop

   // Calculate Outputs
         *pLat = Angle::R2DCC * phi;
   *pLon = Angle::R2DCC * std::atan2(y, x);
         *pAlt = newH;
         break;
      } // NORMAL

      case POLAR_POINT: {
         if (z < 0.0) {
            *pLat = -90.0;
            *pLon = 0.0;
            *pAlt = -b - z;
         }
         else {
            *pLat = +90.0;
            *pLon = 0.0;
            *pAlt = -b + z;
         }
         break;
      } // POLAR_POINT

      case TOO_MANY_LOOPS: {
         break;
      } // TOO_MANY_LOOPS

      default: {
         break;
      } // DEFAULT

   } // end switch

   return (status == NORMAL || status == POLAR_POINT);
}

//----------------------------------------------------------
// Convert Geodetic (LLA coordinates) to ECEF (XYZ coordinates)
//----------------------------------------------------------
bool Nav::convertGeod2Ecef(
      const double lat,    // IN: Geodetic latitude  (degrees)
      const double lon,    // IN: Geodetic longitude (degrees)
      const double alt,    // IN: Geodetic altitude  (meters)
      double* const pX,    // OUT: ECEF X component   (meters)
      double* const pY,    // OUT: ECEF Y component   (meters)
      double* const pZ,    // OUT: ECEF Z component   (meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   //---------------------------------------------
   // Initialize earth model parameters
   //---------------------------------------------
   const EarthModel* p = em;
   if (p == 0) { p = &EarthModel::wgs84; }

   const double a  = p->getA();
   //const double f  = p->getF();
   const double b  = p->getB();
   const double e2 = p->getE2();

   //---------------------------------------------
   // Define Local Constants
   //---------------------------------------------
   const double EPS = 0.5;  // degrees

   const double sinLat = std::sin(Angle::D2RCC * lat);
   const double cosLat = std::cos(Angle::D2RCC * lat);
   const double sinLon = std::sin(Angle::D2RCC * lon);
   const double cosLon = std::cos(Angle::D2RCC * lon);
   const double w      = std::sqrt(1.0 - e2*sinLat*sinLat);
   const double rn     = a/w;

   //---------------------------------------------
   // Initialize Local Variables
   //---------------------------------------------
   int status = NORMAL;

   //---------------------------------------------
   // check status
   //---------------------------------------------
   bool b1 = (lat <  -90.0) || (lat >  +90.0);
   bool b2 = (lon < -180.0) || (lon > +180.0);
   bool b3 = (90.0 - lat) < EPS;
   bool b4 = (90.0 + lat) < EPS;

   if (b1 || b2) {
      status = BAD_INPUT;
   }
   else if (b3 || b4) {
      status = POLAR_POINT;
   }
   else {
      status = NORMAL;
   }

   //---------------------------------------------
   // process according to status
   //---------------------------------------------
   switch (status) {
      case NORMAL: {
         *pX = (alt + rn) * cosLat * cosLon;
         *pY = (alt + rn) * cosLat * sinLon;
         *pZ = (alt + rn*(1.0 - e2)) * sinLat;
         break;
      } // NORMAL

      case BAD_INPUT: {
         *pX = 0.0;
         *pY = 0.0;
         *pZ = 0.0;
         break;
      } // BAD_INPUT

      case POLAR_POINT: {
         *pX = 0.0;
         *pY = 0.0;
         if (lat > 0.0)
            { *pZ = +(b + alt); }
         else
            { *pZ = -(b + alt); }
         break;
      } // POLAR_POINT

      default: {
         break;
      } // DEFAULT

   } // end switch

   return (status == NORMAL || status == POLAR_POINT);
}


//==============================================================================
// Legacy functions ...
//
//    Modified from original SubrDis code
//==============================================================================

//------------------------------------------------------------------------------
// Converts geodetic (WGS84) to geocentric (ECEF) coordinates (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getGeocCoords(
      const double geodPos[3],   // IN: Geodetic (WGS84)  [ ILAT ILON IALT ] [ degs degs meters ]
      double geocPos[3]          // OUT: Geocentric (ECEF) [ IX IY IZ ] (meters)
   )
{
   static const double ellipseC1   = (1.0 - ellipseF) * (1.0 - ellipseF);

   double lat = geodPos[ILAT] * Angle::D2RCC;
   double lon = geodPos[ILON] * Angle::D2RCC;
   double alt = geodPos[IALT]; /* Meters */

   double sinlat = std::sin(lat);
   double temp1 = ellipseA /  std::sqrt(1.0 - (ellipseE2 * (sinlat * sinlat)));
   double temp2 = temp1 * ellipseC1;
   temp1 += alt;
   temp2 += alt;                          /* equ. A-10a */

   /* Obtain the projected horz position on the equatorial plane */
   double w = temp1 * std::cos(lat);

   /* Obtain the projected vert position on the polar axis */
   double z = temp2 * sinlat;             /* equ. A-10b */

   /* Project the horizontal position on the two axes
   * in the equitorial plane */
   double x = w * std::cos(lon);          /* equ. A-11 */

   double y = w * std::sin(lon);          /* modified by Huat Ng, IST */

   geocPos[IX] = x;
   geocPos[IY] = y;
   geocPos[IZ] = z;

   return true;
}


//------------------------------------------------------------------------------
// Converts geocentric to geodetic coordinates (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getGeodCoords(
      const double geocPos[3],   // IN: Geocentric (ECEF) [ IX IY IZ ] (meters)
      double geodPos[3]          // OUT: Geodetic (WGS84) [ ILAT ILON IALT ] [ degs degs meters ]
   )
{
   static const double ellipseAsqOverB  = ellipseAsq / ellipseB;
   static const double PIOVER2 = PI / 2.0;

   long special_case2 = 0;

   double xp = geocPos[IX];
   double yp = geocPos[IY];
   double zp = geocPos[IZ];

   double lat = 0;
   double lon = 0;
   if( xp != 0 )
   {
      /* normal (non-zero divide) case */
      lon = std::atan2( yp, xp );
   }
   else
   {
      /* xp is zero */
      if( yp > 0 )
      {
         lon = PIOVER2;
      }
      else
      {
         if( yp < 0 )
         {
            lon = -PIOVER2;
         }
         else
         {
            /* xp and yp are both zero, special_case2 */
            special_case2 = 1;
            if( zp > 0.0 )
            {
               lat = PIOVER2;
            }
            else
            {
               if( zp < 0.0 )
               {
                  lat = -PIOVER2;
               }
               else
               {
                  // lat and h are undefined when X = Y = Z = 0
                  return false;
               }
            }
            // lon is undefined when X = Y = 0
            return false;
         }
      }
   }

   /* calculate squares of xp, yp and zp */
   double xp_sq = xp * xp;
   double yp_sq = yp * yp;
   double zp_sq = zp * zp;

   /* calculate wp_sq */
   double wp_sq = xp_sq + yp_sq;
   double wp = sqrt( wp_sq );

   /* initial guess */
   double temp_m = ellipseAsq * zp_sq + ellipseBsq * wp_sq;
   double temp_sq = (std::sqrt (temp_m) - ellipseA * ellipseB);
   double m = 0.5 * ((ellipseA * ellipseB * temp_m * (temp_sq))
      / (ellipseAsq * ellipseAsq * zp_sq + ellipseBsq *
      ellipseBsq * wp_sq));

   /* calculate x,y z */
   double x = (1.0 / (1.0 + (2.0 * m) / ellipseAsq)) * xp;
   double y = (1.0 / (1.0 + (2.0 * m) / ellipseAsq)) * yp;
   double z = (1.0 / (1.0 + (2.0 * m) / ellipseBsq)) * zp;

   /* calculate alt */
   double h = std::sqrt((xp - x) * (xp - x) + (yp - y) * (yp - y) + (zp - z) * (zp - z));

   /* begin the iteration for convergence */
   double w = 0;
   double h_previous = h;

   int index = 0;

   do
   {
      h_previous = h;

      double temp1 = ellipseA + (2.0 * m) / ellipseA;
      double temp2 = ellipseB + (2.0 * m) / ellipseB;
      double temp1_sq = temp1 * temp1;

      double temp2_sq = temp2 * temp2;

      /* calculate f and f_prime */

      double f = wp_sq / temp1_sq + zp_sq / temp2_sq - 1.0;
      double f_prime = - (4.0 * wp_sq) / (ellipseA * temp1 * temp1_sq)
         - (4.0 * zp_sq) / (ellipseB * temp2 * temp2_sq);

      /* Newton-Raphson's convergence algorithm */

      m = m - f / f_prime;
      w = (1.0 / (1.0 + (2.0 * m) / ellipseAsq)) * wp;
      z = (1.0 / (1.0 + (2.0 * m) / ellipseBsq)) * zp;

      /* recalculate alt */

      h = std::sqrt ((wp - w) * (wp - w) + (zp - z) * (zp - z));

      index++;
   } while( fabs( h - h_previous ) > 0.5 );


   /* convert x,y,z into latitude, longitude and height */

   double w_sq = w * w;
   double alt = h;

   if( (wp_sq + zp_sq) < (w_sq + z * z) )

   {
      alt = -(alt);
   }

   /* calculate for latitude */

   if( special_case2 == 0 )
   {
      double tanphi = 0;
      //if( wp != w )     /* if denominator not 0 */
      if( (wp - w) > 1.0 )     /* if denominator not 0 */
      {
         tanphi = (zp - z) / (wp - w);
      }
      else              /* denominator is 0 */
      {
         tanphi = (ellipseAsqOverB * std::sqrt( fabs(1.0 - w_sq / ellipseAsq))) / w;
      }
      lat = atan( tanphi );
      if( zp < 0  && lat > 0)
      {
         lat = -(lat);
      }
   }

   geodPos[ILAT] = lat * Angle::R2DCC; /* return as degrees */
   geodPos[ILON] = lon * Angle::R2DCC; /* return as degrees */
   geodPos[IALT] = alt;   /* return as meters */

   return true;
}

//------------------------------------------------------------------------------
// Converts geocentric angles to geodetic angles (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getGeodAngle(
      const double geodPos[2],      // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geocAngle[3],    // IN: Geocentric Euler angles (radians) [ IPHI ITHETA IPSI ]
      double geodAngle[3]           // OUT: Geodetic Euler angles (radians) [ IPHI ITHETA IPSI ]
   )
{
   double phi    = geodPos[ILAT] * Angle::D2RCC; // Latitude
   double lambda = geodPos[ILON] * Angle::D2RCC; // Longitude

   double dis_roll  = geocAngle[IPHI];
   double dis_pitch = geocAngle[ITHETA];
   double dis_yaw   = geocAngle[IPSI];

   double cos_lat = std::cos(phi);
   double sin_lat = std::sin(phi);
   double cos_lon = std::cos(lambda);
   double sin_lon = std::sin(lambda);

   double sin_sin = sin_lat * sin_lon;
   double sin_cos = sin_lat * cos_lon;
   double cos_sin = cos_lat * sin_lon;
   double cos_cos = cos_lat * cos_lon;

   double cos_r = std::cos(dis_roll);
   double sin_r = std::sin(dis_roll);

   double cos_p = std::cos(dis_pitch);
   double sin_p = std::sin(dis_pitch);

   double cos_y = std::cos(dis_yaw);
   double sin_y = std::sin(dis_yaw);

   double pitch = std::asin(cos_cos * cos_p * cos_y + cos_sin * cos_p * sin_y - sin_lat * sin_p);

   double poly1 = cos_p * cos_y;
   double poly2 = cos_p * sin_y;

   double b_sub_11 = -sin_lon * poly1 + cos_lon * poly2;
   double b_sub_12 = -sin_cos * poly1 - sin_sin * poly2 - cos_lat * sin_p;

   double yaw = std::atan2(b_sub_11, b_sub_12);

   double b_sub_23 = cos_cos * (-cos_r * sin_y + sin_r * sin_p * cos_y) +
      cos_sin * ( cos_r * cos_y + sin_r * sin_p * sin_y) +
      sin_lat * ( sin_r * cos_p);
   double b_sub_33 = cos_cos * ( sin_r * sin_y + cos_r * sin_p * cos_y) +
      cos_sin * (-sin_r * cos_y + cos_r * sin_p * sin_y) +
      sin_lat * (cos_r * cos_p);

   double roll = std::atan2(-b_sub_23, -b_sub_33);

   geodAngle[IPITCH] = pitch;
   geodAngle[IROLL]  = roll;
   geodAngle[IYAW]   = yaw;

   return true;
}


//------------------------------------------------------------------------------
// Converts geodetic angles to geocentric angles (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getGeocAngle(
      const double geodPos[2],      // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geodAngle[3],    // IN: Geodetic Euler angles (radians) [ IPHI ITHETA IPSI ]
      double geocAngle[3]           // OUT: Geocentric Euler angles (radians) [ IPHI ITHETA IPSI ]
   )
{
	osg::Matrixd mat;
	osg::Matrixd mat2;

	mat.makeRotate(geodAngle[IROLL],   osg::Y_AXIS,
				   geodAngle[IPITCH],  osg::X_AXIS,
				   geodAngle[IYAW],    osg::Z_AXIS);

  	mat2.makeRotate(osg::DegreesToRadians(0.0),                   osg::Y_AXIS,
					osg::DegreesToRadians(90.0 - geodPos[ILAT]),  osg::X_AXIS,
					osg::DegreesToRadians(geodPos[ILON]),         osg::Z_AXIS);

	mat = mat * mat2;

	double yawd =0,pitchd =0,rolld =0;

	osg::Vec3d hpVec(0.0, 1.0, 0.0);

	hpVec = mat.transform3x3(hpVec, mat);
	double d = std::sqrt(hpVec.x() * hpVec.x() + hpVec.y() * hpVec.y());

	yawd   = -1.0 * atan2(hpVec.x(), hpVec.y());
	pitchd = atan2((double)hpVec.z(), d);

	osg::Vec3d rollVec(1.0, 0.0, 0.0);

	rollVec = mat.transform3x3(rollVec, mat);

	osg::Matrixd hpMat;
	hpMat.makeRotate(0.0,    osg::Y_AXIS,
				     pitchd, osg::X_AXIS,
				     yawd,   osg::Z_AXIS);

	hpMat.invert(hpMat);

	rollVec = hpMat.transform3x3(rollVec, hpMat);
	rolld    = -1.0 * atan2(rollVec.z(), rollVec.x());
	pitchd   = -1.0 * pitchd;

	geocAngle[IPHI]   =  rolld;
	geocAngle[ITHETA] =  pitchd;
	geocAngle[IPSI]   =  yawd;
   return true;
}

//------------------------------------------------------------------------------
// Converts World (Geocentric) to Simulation (Geodetic) (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getSimPosAccVel(
      const double geocPos[3],   // IN: Geocentric position vector [ IX IY IZ ] (meters)
      const double geocVel[3],   // IN: Geocentric velocity vector [ IX IY IZ ] (m/sec)
      const double geocAcc[3],   // IN: Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geodPos[3],         // OUT: Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      double geodVel[3],         // OUT: Geodetic velocity vector [ IX IY IZ ] (m/sec)
      double geodAcc[3]          // OUT: Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
   )
{
   getGeodCoords(geocPos, geodPos);

   double lat = geodPos[ILAT] * Angle::D2RCC;
   double lon = geodPos[ILON] * Angle::D2RCC;

   double cos_lat = std::cos(lat);
   double sin_lat = std::sin(lat);
   double cos_lon = std::cos(lon);
   double sin_lon = std::sin(lon);

   double sin_sin = sin_lat * sin_lon;
   double sin_cos = sin_lat * cos_lon;
   double cos_sin = cos_lat * sin_lon;
   double cos_cos = cos_lat * cos_lon;

   double p_dworld = geocVel[IX];
   double q_dworld = geocVel[IY];
   double r_dworld = geocVel[IZ];
   geodVel[INORTH] = ( (p_dworld * -sin_cos + q_dworld * -sin_sin + r_dworld * cos_lat) );
   geodVel[IEAST]  = ( (p_dworld * -sin_lon + q_dworld * cos_lon) );
   geodVel[IDOWN]  = ( (p_dworld * -cos_cos + q_dworld * -cos_sin + r_dworld * -sin_lat) );

   p_dworld = geocAcc[IX];
   q_dworld = geocAcc[IY];
   r_dworld = geocAcc[IZ];
   geodAcc[INORTH] = ( (p_dworld * -sin_cos + q_dworld * -sin_sin + r_dworld * cos_lat) );
   geodAcc[IEAST]  = ( (p_dworld * -sin_lon + q_dworld * cos_lon) );
   geodAcc[IDOWN]  = ( (p_dworld * -cos_cos + q_dworld * -cos_sin + r_dworld * -sin_lat) );

   return true;
}

//------------------------------------------------------------------------------
// Converts Simulation (Geodetic) to World (Geocentric) (From SubrDis)
//------------------------------------------------------------------------------
bool Nav::getWorldPosAccVel(
      const double geodPos[3],   // IN: Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      const double geodVel[3],   // IN: Geodetic velocity vector [ IX IY IZ ] (m/sec)
      const double geodAcc[3],   // IN: Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geocPos[3],         // OUT: Geocentric position vector [ IX IY IZ ] (meters)
      double geocVel[3],         // OUT: Geocentric velocity vector [ IX IY IZ ] (m/sec)
      double geocAcc[3]          // OUT: Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
   )
{
   /* get the geocentric position */
   getGeocCoords(geodPos, geocPos);

   double lat = geodPos[ILAT] * Angle::D2RCC;
   double lon = geodPos[ILON] * Angle::D2RCC;

   double cos_lat = std::cos(lat);
   double sin_lat = std::sin(lat);
   double cos_lon = std::cos(lon);
   double sin_lon = std::sin(lon);

   double sin_sin = sin_lat * sin_lon;
   double sin_cos = sin_lat * cos_lon;
   double cos_sin = cos_lat * sin_lon;
   double cos_cos = cos_lat * cos_lon;

   double p_aworld =  geodVel[INORTH];
   double q_aworld =  geodVel[IEAST];
   double r_aworld = geodVel[IDOWN];

   geocVel[IX] = ( p_aworld * -sin_cos + q_aworld * -sin_lon + r_aworld * -cos_cos );
   geocVel[IY] = ( p_aworld * -sin_sin + q_aworld *  cos_lon + r_aworld * -cos_sin );
   geocVel[IZ] = ( p_aworld *  cos_lat                       + r_aworld * -sin_lat );

   p_aworld =  geodAcc[INORTH];
   q_aworld =  geodAcc[IEAST];
   r_aworld = geodAcc[IDOWN];

   geocAcc[IX] = ( p_aworld * -sin_cos + q_aworld * -sin_lon + r_aworld * -cos_cos );
   geocAcc[IY] = ( p_aworld * -sin_sin + q_aworld *  cos_lon + r_aworld * -cos_sin );
   geocAcc[IZ] = ( p_aworld *  cos_lat                       + r_aworld * -sin_lat );

   return true;
}


//------------------------------------------------------------------------------
// UTM support functions
//------------------------------------------------------------------------------

static char getLatZone(const double lat)
{
   char latZone = '*';

   if      (lat >   84.0) latZone = 'Z';  // error north
   else if (lat >=  72.0) latZone = 'X';
   else if (lat >=  64.0) latZone = 'W';
   else if (lat >=  56.0) latZone = 'V';
   else if (lat >=  48.0) latZone = 'U';
   else if (lat >=  40.0) latZone = 'T';
   else if (lat >=  32.0) latZone = 'S';
   else if (lat >=  24.0) latZone = 'R';
   else if (lat >=  16.0) latZone = 'Q';
   else if (lat >=   8.0) latZone = 'P';
   else if (lat >=   0.0) latZone = 'N';
   else if (lat >=  -8.0) latZone = 'M';
   else if (lat >= -16.0) latZone = 'L';
   else if (lat >= -24.0) latZone = 'K';
   else if (lat >= -32.0) latZone = 'J';
   else if (lat >= -40.0) latZone = 'H';
   else if (lat >= -48.0) latZone = 'G';
   else if (lat >= -56.0) latZone = 'F';
   else if (lat >= -64.0) latZone = 'E';
   else if (lat >= -72.0) latZone = 'D';
   else if (lat >= -80.0) latZone = 'C';
   else                   latZone = 'A';  // error south

   return latZone;
}

static int getLonZone(const double latDeg, const double lonDeg)
{
   //-----------------------------------
   // get longitude zone number
   //-----------------------------------   
   double lonZone = (lonDeg >= 0.0) ? 31.0 + lonDeg/6.0
                                    : 1.0 + (lonDeg + 180.0)/6.0;

   //-----------------------------------
   // special case longitude zone number
   //-----------------------------------   
   if ( (latDeg >= 56.0) && (latDeg < 64.0) && (lonDeg >= 3.0) && (lonDeg < 12.0) ) {
      lonZone = 32;
   }
   
   //-----------------------------------
   // Special zones for Svalbard
   //-----------------------------------
   if (latDeg >= 72.0 && latDeg < 84.0) {
      if      ( lonDeg >= 0.0  && lonDeg <  9.0 ) lonZone = 31;
      else if ( lonDeg >= 9.0  && lonDeg < 21.0 ) lonZone = 33;
      else if ( lonDeg >= 21.0 && lonDeg < 33.0 ) lonZone = 35;
      else if ( lonDeg >= 33.0 && lonDeg < 42.0 ) lonZone = 37;
   }
   
   return int(lonZone);
}

static bool inNorthHemi(const char latZone)
{
   bool northHemi = true;
   
   if (latZone == 'M' ||
       latZone == 'L' ||
       latZone == 'K' ||
       latZone == 'J' ||
       latZone == 'H' ||
       latZone == 'G' ||
       latZone == 'F' ||
       latZone == 'E' ||
       latZone == 'D' ||
       latZone == 'C') northHemi = false;
       
   return northHemi;
}


//------------------------------------------------------------------------------
// convertLL2Utm - Converts Latitude, Longitude to UTM Northing and Easting
//
// Reference:  Defense Mapping Agency (DMA) Technical Manual - DMATM 8358.2
//             "The Universal Grids: Universal Transverse Mercator (UTM) and
//             Universal Polar Stereographic (UPS)"
//------------------------------------------------------------------------------
bool Nav::convertLL2Utm(
      const double            lat,        // IN:  Latitude        [DEG]
      const double            lon,        // IN:  Longitude       [DEG]
      char* const             pLatZone,   // OUT: Latitude Zone
      int*  const             pLonZone,   // OUT: Longitude Zone
      double* const           pNorthing,  // OUT: Northing        [M]
      double* const           pEasting,   // OUT: Easting         [M]
      const EarthModel* const pEM)        // IN:  Pointer to an optional earth model (default: WGS-84)
{
   //-----------------------------------
   // local variables
   //-----------------------------------
   double k1 = 0.0;
   double k2 = 0.0;
   double k3 = 0.0;
   double k4 = 0.0;
   
   //-----------------------------------
   // local constants
   //-----------------------------------
   const double A       = pEM->getA();
   const double B       = pEM->getB();   
   const double E2      = pEM->getE2();
   
   //-----------------------------------
   // check input terms valid 
   //-----------------------------------
   bool latValid =  (-80.0 <= lat) && (lat <=  +84.0);
   bool lonValid = (-180.0 <= lon) && (lon <= +180.0);
   bool ok = latValid && lonValid;
   if (ok) {
      const double LATDEG  = lat;
      const double LATRAD  = lat*Angle::D2RCC;
      const double LONDEG  = lon;

      //-----------------------------------
      // powers of sin,cos,tan of latitude
      //-----------------------------------
      const double SIN1  = std::sin(LATRAD);
      const double COS1  = std::cos(LATRAD);

      const double SIN2 = SIN1*SIN1;
      
      const double COS2 = COS1*COS1;
      const double COS3 = COS1*COS2;
      const double COS4 = COS1*COS3;
      const double COS5 = COS1*COS4;
      const double COS6 = COS1*COS5;
      const double COS7 = COS1*COS6;
      const double COS8 = COS1*COS7;
      
      const double TAN1 = SIN1/COS1;
      const double TAN2 = TAN1*TAN1;
      const double TAN3 = TAN1*TAN2;
      const double TAN4 = TAN1*TAN3;
      const double TAN5 = TAN1*TAN4;
      const double TAN6 = TAN1*TAN5;

      //---------------
      // term S
      //---------------
      const double N1 = (A - B)/(A + B);
      const double N2 = N1*N1;
      const double N3 = N1*N2;
      const double N4 = N1*N3;
      const double N5 = N1*N4;

      const double Ap =               A*( 1.0 - N1 + (1.25)  *(N2 - N3) + (81.0/64.0)*(N4 - N5) );
      const double Bp =         (1.5)*A*(  N1 - N2 + (0.875) *(N3 - N4) + (55.0/64.0)*(N5) ); 
      const double Cp =      (0.9375)*A*(  N2 - N3 + (0.75)  *(N4 - N5) );
      const double Dp =   (35.0/48.0)*A*(  N3 - N4 + (0.6875)*(N5) );
      const double Ep = (315.0/512.0)*A*(  N4 - N5 );

      const double SIN2LAT = std::sin(2.0*LATRAD);
      const double SIN4LAT = std::sin(4.0*LATRAD);
      const double SIN6LAT = std::sin(6.0*LATRAD);
      const double SIN8LAT = std::cos(6.0*LATRAD);

      const double S  = Ap*LATRAD - Bp*SIN2LAT + Cp*SIN4LAT - Dp*SIN6LAT + Ep*SIN8LAT;
      
      //---------------
      // constants
      //---------------   
      const double EP2  = (E2 != 1.0) ? E2/(1.0 - E2) : -1.0;  // -1.0 indicates an error
      const double EP4  = EP2*EP2;
      const double EP6  = EP2*EP4;
      const double EP8  = EP2*EP6;

      const double K0   = 0.9996;
      const double P    = A*(1.0 - E2)/pow((1.0 - E2*SIN2), 1.5);
      const double Q    = P*(1.0 + EP2*COS2);

      //---------------
      // term T1
      //---------------
      const double T1 = S*K0;
      
      //---------------
      // term T2
      //---------------
      const double T2 = Q*SIN1*COS1*K0/2.0;
      
      //---------------
      // term T3
      //---------------
      k1 = Q*SIN1*COS3*K0/24.0;
      k2 = 5.0 - TAN2 + 9.0*EP2*COS2 + 4.0*EP4*COS4;
      const double T3 = k1*k2;
      
      //---------------
      // term T4
      //---------------
      k1 = Q*SIN1*COS5*K0/720.0;
      k2 = 61.0 - 58.0*TAN2 + TAN4 + 270.0*EP2*COS2 - 330.0*TAN2*EP2*COS2;
      k3 = 445.0*EP4*COS4 + 324.0*EP6*COS6 - 680.0*TAN2*EP4*COS4;
      k4 = 88.0*EP8*COS8 - 600.0*TAN2*EP6*COS6 - 192.0*TAN2*EP8*COS8;
      const double T4 =  k1*(k2 + k3 + k4);
      
      //---------------
      // term T5
      //---------------
      k1 = Q*SIN1*COS7*K0/40320.0;
      k2 = 1385.0 - 3111.0*TAN2 + 543.0*TAN4 - TAN6;
      const double T5 = k1*k2;
      
      //---------------
      // term T6
      //---------------
      const double T6 = Q*COS1*K0;
      
      //---------------
      // term T7
      //---------------
      k1 = Q*COS3*K0/6.0;
      k2 = 1.0 - TAN2 + EP2*COS2;
      const double T7 = k1*k2;
      
      //---------------
      // term T8
      //---------------
      k1 = Q*COS5*K0/120.0;
      k2 = 5.0 - 18.0*TAN2 + TAN4 + 14.0*EP2*COS2 - 58.0*TAN2*EP2*COS2;
      k3 = 13.0*EP4*COS4 + 4.0*EP6*COS6 - 64.0*TAN2*EP4*COS4 - 24.0*TAN2*EP6*COS6;
      const double T8 = k1*(k2 + k3);
      
      //---------------
      // term T9
      //---------------
      k1 = Q*COS7*K0/5040.0;
      k1 = 61.0 - 479.0*TAN2 + 179*TAN4 - TAN6;
      const double T9 = k1*k2;

      //-----------------------------------
      // get latitude and longitude zones
      //-----------------------------------
      *pLatZone = getLatZone(lat);
      *pLonZone = getLonZone(lat, lon);

      const int    LONZONE = 1 + int((LONDEG + 180.0)/6.0);
      const double LONORIG = 6.0*(LONZONE - 1) - 180.0 + 3.0;

      const double dL1 = Angle::D2RCC*(LONDEG - LONORIG);
      const double dL2 = dL1*dL1;
      const double dL3 = dL1*dL2;
      const double dL4 = dL1*dL3;
      const double dL5 = dL1*dL4;
      const double dL6 = dL1*dL5;
      const double dL7 = dL1*dL6;
      const double dL8 = dL1*dL7;

      const double FN  = (LATDEG < 0.0) ? 1.0e7 : 0.0;
      const double FE  = 500000.0;
      
      *pNorthing = FN + T1 + dL2*T2 + dL4*T3 + dL6*T4 + dL8*T5;
      *pEasting  = FE +      dL1*T6 + dL3*T7 + dL5*T8 + dL7*T9;
   }
   
   return ok;
}


//------------------------------------------------------------------------------
// convertUtm2LL - Converts UTM Northing and Easting to Latitude, Longitude
//
// Reference:  Defense Mapping Agency (DMA) Technical Manual - DMATM 8358.2
//             "The Universal Grids: Universal Transverse Mercator (UTM) and
//             Universal Polar Stereographic (UPS)"
//------------------------------------------------------------------------------
bool Nav::convertUtm2LL(
      const double            northing, // IN:  Northing       [M]
      const double            easting,  // IN:  Easting        [M]
      char                    latZone,  // IN:  Latitude Zone
      int                     lonZone,  // IN:  Longitude Zone
      double* const           pLat,     // OUT: Latitude       [DEG]
      double* const           pLon,     // OUT: Longitude      [DEG]
      const EarthModel* const pEM)      // IN:  Pointer to an optional earth model (default: WGS-84)      
{
   //-----------------------------------
   // local variables
   //-----------------------------------
   double k1 = 0.0;
   double k2 = 0.0;
   double k3 = 0.0;
   double k4 = 0.0;
   double k5 = 0.0;
   
   //-----------------------------------
   // local constants
   //-----------------------------------
   const double A    = pEM->getA();
   const double B    = pEM->getB();
   
   const double E2   = pEM->getE2();
   const double E4   = E2*E2;
   const double E6   = E2*E4;
   
   const double EP2  = (E2 != 1.0) ? E2/(1.0 - E2) : -1.0;
   const double EP4  = EP2*EP2;
   const double EP6  = EP2*EP4;
   const double EP8  = EP2*EP6;
   
   const double K01  = 0.9996;   // central scale factor for UTM
   const double K02  = K01*K01;
   const double K03  = K01*K02;
   const double K04  = K01*K03;
   const double K05  = K01*K04;
   const double K06  = K01*K05;
   const double K07  = K01*K06;
   const double K08  = K01*K07;
   
   //-----------------------------------
   // calculate the footprint latitude (FPLAT)
   //-----------------------------------
   k1 = inNorthHemi(latZone) ? northing : (northing - 1.0e7);
   k2 = A*K01*(1.0 - E2/4.0 - (3.0/64.0)*E4 - (5.0/256.0)*E6);
   
   const double MU      = k1/k2;
   const double SIN2MU  = std::sin(2.0*MU);
   const double SIN4MU  = std::cos(4.0*MU);
   const double SIN6MU  = std::sin(6.0*MU);
   const double SIN8MU  = std::cos(8.0*MU);
   
   const double N1      = (A - B)/(A + B);
   const double N2      = N1*N1;
   const double N3      = N1*N2;
   const double N4      = N1*N3;
                     
   const double J1      =      (3.0/2.0)*N1 - (27.0/32.0)*N3;
   const double J2      =    (21.0/16.0)*N2 - (55.0/32.0)*N4;
   const double J3      =   (151.0/96.0)*N3;
   const double J4      = (1097.0/512.0)*N4;

   const double FPLAT   = MU + J1*SIN2MU + J2*SIN4MU + J3*SIN6MU + J4*SIN8MU;
   
   //-----------------------------------
   // powers of sin,cos,tan of FPLAT
   //-----------------------------------
   const double SIN1    = std::sin(FPLAT);
   const double SIN2    = SIN1*SIN1;

   const double COS1    = std::cos(FPLAT);   
   const double COS2    = COS1*COS1;
   const double COS3    = COS1*COS2;
   const double COS4    = COS1*COS3;
   const double COS5    = COS1*COS4;
   const double COS6    = COS1*COS5;
   const double COS7    = COS1*COS6;
   const double COS8    = COS1*COS7;
   
   const double TAN1    = SIN1/COS1;
   const double TAN2    = TAN1*TAN1;
   const double TAN3    = TAN1*TAN2;
   const double TAN4    = TAN1*TAN3;
   const double TAN5    = TAN1*TAN4;
   const double TAN6    = TAN1*TAN5;

   const double P       = A*(1.0 - E2)/pow((1.0 - E2*SIN2), 1.5);
   const double Q1      = P*(1.0 + EP2*COS2);
   const double Q2      = Q1*Q1;
   const double Q3      = Q1*Q2;
   const double Q4      = Q1*Q3;
   const double Q5      = Q1*Q4;
   const double Q6      = Q1*Q5;
   const double Q7      = Q1*Q6;

   const double FE      = 500000.0;
   const double DE1     = easting - FE;
   const double DE2     = DE1*DE1;
   const double DE3     = DE1*DE2;
   const double DE4     = DE1*DE3;
   const double DE5     = DE1*DE4;
   const double DE6     = DE1*DE5;
   const double DE7     = DE1*DE6;
   const double DE8     = DE1*DE7;
   
   //---------------
   // term T10
   //---------------
   k1 = 2.0*P*Q1*K02;
   const double T10 = TAN1/k1;

   //---------------
   // term T11
   //---------------
   k1 = 5.0 + 3.0*TAN2 + EP2*COS2 - 4.0*EP4*COS4 - 9.0*TAN2*EP2*COS2;
   k2 = 24.0*P*Q3*K04;
   const double T11 = TAN1*k1/k2;

   //---------------
   // term T12
   //---------------
   k1 = 61.0 + 90.0*TAN2 + 46.0*EP2*COS2 + 45.0*TAN4 - 252.0*TAN2*EP2*COS2;
   k2 = - 3.0*EP4*COS4 + 100.0*EP6*COS6 -66.0*TAN2*EP4*COS4;
   k3 = - 90.0*TAN4*EP2*COS2 + 88.0*EP8*COS8 + 225.0*TAN4*EP4*COS4;
   k4 = + 84.0*TAN2*EP6*COS6 - 192.0*TAN2*EP8*COS8;
   k5 = 720.0*P*Q5*K06;
   const double T12 = TAN1*(k1 + k2 + k3 + k4)/k5;

   //---------------
   // term T13
   //---------------
   k1 = 1385.0 + 3633.0*TAN2 + 4095.0*TAN4 + 1575.0*TAN6;
   k2 = 40320.0*P*Q7*K08;
   const double T13 = TAN1*k1/k2;

   //---------------
   // term T14
   //---------------
   k1 = Q1*COS1*K01;
   const double T14 = 1.0/k1;

   //---------------
   // term T15
   //---------------
   k1 = 1.0 + 2.0*TAN2 + EP2*COS2;
   k2 = 6.0*Q3*COS1*K03;
   const double T15 = k1/k2;

   //---------------
   // term T16
   //---------------
   k1 = 5.0 + 6.0*EP2*COS2 + 28.0*TAN2 - 3.0*EP4*COS4 + 8.0*TAN2*EP2*COS2;
   k2 = 24.0*TAN4 - 4.0*EP6*COS6 + 4.0*TAN2*EP4*COS4 + 24.0*TAN2*EP6*COS6;
   k3 = 120.0*Q5*COS1*K05;
   const double T16 = (k1 + k2)/k3;

   //---------------
   // term T17
   //---------------
   k1 = 61.0 + 662.0*TAN2 + 1320.0*TAN4 + 720.0*TAN6;
   k2 = 5040.0*Q7*COS1*K07;
   const double T17 = k1/k2;
   

   //-----------------------------------
   // calculate latitude and longitude
   //-----------------------------------
   const double LATDEG = FPLAT*Angle::R2DCC;
   const double LONDEG = 6.0*(lonZone - 1.0) - 180.0 + 3.0;
   *pLat = LATDEG + Angle::R2DCC*(-DE2*T10 + DE4*T11 - DE6*T12 + DE8*T13);
   *pLon = LONDEG + Angle::R2DCC*(+DE1*T14 - DE3*T15 + DE5*T16 - DE7*T17);

   return true;
}

}  // End Basic namespace
}  // End Eaagles namespace
