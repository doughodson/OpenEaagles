//------------------------------------------------------------------------------
// Class: Nav
// Description: inline functions
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Nav_Inline__
#define __Eaagles_Basic_Nav_Inline__

//------------------------------------------------------------------------------
// Flat-Earth: Computes the destination (target) lat/lon from starting (ref)
// point given the distance and initial bearing using flat earth projection
// with an optional ellipsoidal earth model, em
//------------------------------------------------------------------------------
inline bool Nav::fbd2llE(
         const double slat,   // IN: Starting (reference) latitude (degs)
         const double slon,   // IN: Starting (reference) longitude (degs)
         const double brng,   // IN: True bearing (degs)
         const double dist,   // IN: Distance (ground range) (nm)
         double* const dlat,  // OUT: Destination latitude (degs)
         double* const dlon,  // OUT:  Destination longitude (degs)
         const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
      )
{
   // Initialize earth model parameters
   const EarthModel* pModel = em;
   if (pModel == nullptr) { pModel = &EarthModel::wgs84; }

   const double a  = Distance::M2NM * pModel->getA();   // semi-major axis
   const double e2 = pModel->getE2();  // eccentricity squared

   // Define Local Constants
   const double sinSlat = std::sin(Angle::D2RCC * slat);
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   const double sinBrng = std::sin(Angle::D2RCC * brng);
   const double cosBrng = std::cos(Angle::D2RCC * brng);
   const double q       = 1.0 - e2 * sinSlat * sinSlat;
   const double rn      = a / std::sqrt(q);
   const double rm      = rn * (1.0 - e2) / q;

   // Compute new lat/lon
   double dN = cosBrng * dist;
   double dE = sinBrng * dist;

   if (dlat != 0) *dlat = slat + Angle::R2DCC * (dN / rm);

   if (dlon != 0) {
      if (cosSlat != 0)
         *dlon = Angle::aepcdDeg( slon + Angle::R2DCC * (dE / rn) / cosSlat );
      else
         *dlon = slon;
   }

   return true;
}


//------------------------------------------------------------------------------
// Flat-Earth: Computes the initial bearing and the distance from the starting
// lat/lon (ref point) to the destination lat/lon using flat earth projection
// with an optional ellipsoidal earth model, em
//------------------------------------------------------------------------------
inline bool Nav::fll2bdE(
         const double slat,   // IN: Starting (reference) latitude (degs)
         const double slon,   // IN: Starting (reference) longitude (degs)
         const double dlat,   // IN: Destination latitude (degs)
         const double dlon,   // IN: Destination longitude (degs)
         double* const brng,  // OUT: True bearing (degs)
         double* const dist,  // OUT: distance (ground range) (nm)
         const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
      )
{
   // Initialize earth model parameters
   const EarthModel* pModel = em;
   if (pModel == nullptr) { pModel = &EarthModel::wgs84; }

   const double a  = Distance::M2NM * pModel->getA();   // semi-major axis
   const double e2 = pModel->getE2();  // eccentricity squared

   // Define Local Constants
   const double sinSlat = std::sin(Angle::D2RCC * slat);
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   const double q       = 1.0 - e2 * sinSlat * sinSlat;
   const double rn      = a / std::sqrt(q);
   const double rm      = rn * (1.0 - e2) / q;

   // IDENTICAL_POINTS;
   if ((slat == dlat) && (slon == dlon)) {
      *brng = 0.0;
      *dist = 0.0;
   }

   // Compute brg/dist
   else {
      double dN = Angle::D2RCC * Angle::aepcdDeg(dlat - slat) * rm;
      double dE = Angle::D2RCC * Angle::aepcdDeg(dlon - slon) * rn * cosSlat;

      *brng = Angle::R2DCC * std::atan2(dE, dN);
      *dist = std::sqrt(dN*dN + dE*dE);
   }


   return true;
}


//------------------------------------------------------------------------------
// Flat-Earth: Computes the destination (target) lat/lon from starting (ref)
// point given the distance and initial bearing using flat earth projection
// and a spherical earth radius of 'ERAD60'
//------------------------------------------------------------------------------
inline bool Nav::fbd2llS(
      const double slat,   // IN: Starting (reference) latitude (degs)
      const double slon,   // IN: Starting (reference) longitude (degs)
      const double brg,    // IN: True bearing (degs)
      const double dist,   // IN: Distance (ground range) (nm)
      double* const dlat,  // OUT: Destination latitude (degs)
      double* const dlon   // OUT:  Destination longitude (degs)
   )
{
   bool ok = false;
   if (dlat != nullptr && dlon != nullptr) {
      double ang = brg * Angle::D2RCC;
      double ew = std::sin(ang) * dist;
      double ns = std::cos(ang) * dist;

      *dlat = slat + (ns/60.0);

      double tlat = slat;
      if (tlat > 89.0 || tlat < -89.0) tlat = 89.0;

      *dlon = Angle::aepcdDeg( slon + ( ew / (60.0 * std::cos(tlat*Angle::D2RCC)) ) );
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Flat-Earth: Computes the initial bearing and the distance from the starting
// lat/lon (ref point) to the destination lat/lon using flat earth projection
// and a spherical earth radius of 'ERAD60'
//------------------------------------------------------------------------------
inline bool Nav::fll2bdS(
      const double slat,   // IN: Starting (reference) latitude (degs)
      const double slon,   // IN: Starting (reference) longitude (degs)
      const double dlat,   // IN: Destination latitude (degs)
      const double dlon,   // IN: Destination longitude (degs)
      double* const brg,   // OUT: True bearing (degs)
      double* const dist   // OUT: distance (ground range) (nm)
   )
{
   bool ok = false;
   if (brg != nullptr && dist != nullptr) {
      double ns = ( Angle::aepcdDeg(dlat - slat) * 60.0 );
      double ew = ( Angle::aepcdDeg(dlon - slon) * 60.0 * std::cos(slat*Angle::D2RCC) );
      *brg = std::atan2(ew,ns) * Angle::R2DCC;
      *dist = sqrt(ns*ns + ew*ew);
      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Legacy flat-earth projection; use fbd2llS()
//------------------------------------------------------------------------------
inline bool Nav::fbd2ll(
            const double slat,         // IN: Starting (reference) latitude (degs)
            const double slon,         // IN: Starting (reference) longitude (degs)
            const double brg,          // IN: True bearing (degs)
            const double dist,         // IN: Distance (ground range) (nm)
            double* const dlat,        // OUT: Destination latitude (degs)
            double* const dlon         // OUT:  Destination longitude (degs)
         )
{
   return fbd2llS(slat, slon, brg, dist, dlat, dlon);
}


//------------------------------------------------------------------------------
// Legacy flat-earth projection; use fll2bdS()
//------------------------------------------------------------------------------
inline bool Nav::fll2bd(
            const double slat,         // IN: Starting (reference) latitude (degs)
            const double slon,         // IN: Starting (reference) longitude (degs)
            const double dlat,         // IN: Destination latitude (degs)
            const double dlon,         // IN: Destination longitude (degs)
            double* const brg,         // OUT: True bearing (degs)
            double* const dist         // OUT: distance (ground range) (nm)
         )
{
   return fll2bdS(slat, slon, dlat, dlon, brg, dist);
}


//------------------------------------------------------------------------------
// aer2xyz() -- convert single set of azimuth, elevation and range values to x, y, z positions
//------------------------------------------------------------------------------

// Using body angles
inline bool Nav::aer2xyz(
      osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  (meters)
      const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
      const double az,           // IN:  azimuth (body)  (radians)
      const double el,           // IN:  elevation (body)  (positive up)   (radians)
      const double rng           // IN:  range (meters)
   )
{
   bool ok = false;

   if (pos != 0) {
      // Compute sin/cos of azimuth
      double saz, caz;
      sinCos(az,&saz,&caz);

      // Compute sin/cos of elevation
      double sel, cel;
      sinCos(el,&sel,&cel);

      // ---
      // Compute to x, y and z positions (player coordinates)
      // ---
      double d = -rng * sel;
      double r = rng * cel;
      double n = r * caz;
      double e = r * saz;
      osg::Vec3d pos0(n,e,d);

      // Rotate from player to NED coordinates (NED, player centered)
      *pos = pos0 * rm;

      // all is well.
      ok = true;
   }

   return ok;
}

// Using NED angles
inline bool Nav::aer2xyz(
      osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  (meters)
      const double az,           // IN:  azimuth (NED)  (radians)
      const double el,           // IN:  elevation (NED)  (positive up)   (radians)
      const double rng           // IN:  range (meters)
   )
{
   bool ok = false;

   if (pos != nullptr) {
      // Compute sin/cos of azimuth
      double saz, caz;
      sinCos(az,&saz,&caz);

      // Compute sin/cos of elevation
      double sel, cel;
      sinCos(el,&sel,&cel);

      // ---
      // Compute to x, y and z positions (player coordinates)
      // ---
      double d = -rng * sel;
      double r = rng * cel;
      double n = r * caz;
      double e = r * saz;
      osg::Vec3d pos0(n,e,d);

      // Rotate from player to NED coordinates (NED, player centered)
      *pos = pos0;

      // all is well.
      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Converts a single set of xyz values to Azimuth, Elevation and Range (AER) vector
//------------------------------------------------------------------------------

// Computing NED angles
inline bool Nav::xyz2aer(
      osg::Vec3d* const aer,     // OUT: NED angles (az, elev, rng) [deg,deg,meters]
      const double x,            // IN:  x                        [meters]
      const double y,            // IN:  y                        [meters]
      const double z             // IN:  z                        [meters]
   )
{
   // Compute AER position vector
   double ranj = std::sqrt(x*x + y*y + z*z);
   double elev = Angle::R2DCC * std::asin(-z / ranj);
   double azim = Angle::R2DCC * std::atan2(y, x);

   (*aer)[0] = azim;
   (*aer)[1] = elev;
   (*aer)[2] = ranj;

   return true;
}

// Computing body angles
inline bool Nav::xyz2aer(
      osg::Vec3d* const aer,     // OUT: position vector  (aer)   [deg,deg,meters]
      const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
      const double x0,           // IN:  x                        [meters]
      const double y0,           // IN:  y                        [meters]
      const double z0            // IN:  z                        [meters]
   )
{
   osg::Vec3d vi(x0, y0, z0);    // Earth vector (NED)
   osg::Vec3d vb = rm * vi;      // Body vector
   double x = vb[0];
   double y = vb[1];
   double z = vb[2];

   // Compute AER position vector
   double ranj = std::sqrt(x*x + y*y + z*z);
   double elev = Angle::R2DCC * std::asin(-z / ranj);
   double azim = Angle::R2DCC * std::atan2(y, x);

   (*aer)[0] = azim;
   (*aer)[1] = elev;
   (*aer)[2] = ranj;

   return true;
}


//------------------------------------------------------------------------------
// Earth centered, earth fixed (ECEF) coordinates <==> Geodetic coordinates
//------------------------------------------------------------------------------

// convertEcef2Geod(): using osg::Vec3d vectors
inline bool Nav::convertEcef2Geod(
            const osg::Vec3d& vec,     // IN: ECEF [ IX IY IZ ]
            osg::Vec3d* const lla,     // OUT: Geodetic [ ILAT ILON IALT ]
            const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
         )
{
   bool ok = false;
   if (lla != nullptr) {
      double lat(0.0), lon(0.0), alt(0.0);
      ok = convertEcef2Geod(vec[IX], vec[IY], vec[IZ], &lat, &lon, &alt, em);
      if (ok) lla->set(lat, lon, alt);
   }
   return ok;
}

// convertEcef2Geod(): using arrays vectors
inline bool Nav::convertEcef2Geod(
            const double vec[3],       // IN: ECEF [ IX IY IZ ]
            double lla[3],             // OUT: Geodetic [ ILAT ILON IALT ]
            const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
         )
{
   bool ok = false;
   if (lla != 0) {
      double lat(0.0), lon(0.0), alt(0.0);
      ok = convertEcef2Geod(vec[IX], vec[IY], vec[IZ], &lat, &lon, &alt, em);
      if (ok) { lla[ILAT] = lat; lla[ILON] = lon; lla[IALT] = alt; }
   }
   return ok;
}

// convertGeod2Ecef(): using osg::Vec3d vectors
inline bool Nav::convertGeod2Ecef(
            const osg::Vec3d& lla,     // IN: Geodetic [ ILAT ILON IALT ]
            osg::Vec3d* const ecef,    // OUT: ECEF [ IX IY IZ ]
            const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
         )
{
   bool ok = false;
   if (ecef != nullptr) {
      double x(0.0), y(0.0), z(0.0);
      ok = convertGeod2Ecef(lla[ILAT], lla[ILON], lla[IALT], &x, &y, &z, em);
      if (ok) ecef->set(x, y, z);
   }
   return ok;
}

// convertGeod2Ecef(): using arrays vectors
inline bool Nav::convertGeod2Ecef(
            const double lla[3],       // IN: Geodetic [ ILAT ILON IALT ]
            double ecef[3],            // OUT: ECEF [ IX IY IZ ]
            const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
         )
{
   bool ok = false;
   if (ecef != 0) {
      double x(0.0), y(0.0), z(0.0);
      ok = convertGeod2Ecef(lla[ILAT], lla[ILON], lla[IALT], &x, &y, &z, em);
      if (ok) { ecef[IX] = x; ecef[IY] = y; ecef[IZ] = z; }
   }
   return ok;
}


//------------------------------------------------------------------------------
// Rotational transformation matrix (and optional sin/cos values) from Euler angles
//    Reference system <==> Rotating system
//------------------------------------------------------------------------------

// computeRotationalMatrix() using degrees
inline bool Nav::computeRotationalMatrixDeg(
            const double phiD,         // IN: roll angle (degrees)
            const double thetaD,       // IN: pitch angle (degrees)
            const double psiD,         // IN: yaw angle (degrees)
            osg::Matrixd* const rm,    // OUT: Rotational matrix
            osg::Vec2d* const scPhi,   // OUT: Sin/Cos of phi (Optional)
            osg::Vec2d* const scTht,   // OUT: Sin/Cos of theta (Optional)
            osg::Vec2d* const scPsi    // OUT: Sin/Cos of psi (Optional)
         )
{
   return computeRotationalMatrix(
         phiD*Angle::D2RCC, thetaD*Angle::D2RCC, psiD*Angle::D2RCC,
         rm,
         scPhi, scTht, scPsi
      );
}

// computeRotationalMatrix() using arrays vectors
inline bool Nav::computeRotationalMatrix(
            const osg::Vec3d& angles,  // IN: Euler angles [ phi theta psi ] (radians)
            osg::Matrixd* const m,     // OUT: Matrix
            osg::Vec2d* const scPhi,   // OUT: Sin/Cos of phi (Optional)
            osg::Vec2d* const scTht,   // OUT: Sin/Cos of theta (Optional)
            osg::Vec2d* const scPsi    // OUT: Sin/Cos of psi (Optional)
         )
{
   return computeRotationalMatrix(
         angles[0], angles[1], angles[2],
         m,
         scPhi, scTht, scPsi
      );
}

// computeRotationalMatrix() using degrees and arrays vectors
inline bool Nav::computeRotationalMatrixDeg(
            const osg::Vec3d& angles,  // IN: Euler angles [ phi theta psi ] (degrees)
            osg::Matrixd* const m,     // OUT: Matrix
            osg::Vec2d* const scPhi,   // OUT: Sin/Cos of phi (Optional)
            osg::Vec2d* const scTht,   // OUT: Sin/Cos of theta (Optional)
            osg::Vec2d* const scPsi    // OUT: Sin/Cos of psi (Optional)
         )
{
   return computeRotationalMatrix(
         angles[0]*Angle::D2RCC, angles[1]*Angle::D2RCC, angles[2]*Angle::D2RCC,
         m,
         scPhi, scTht, scPsi
      );
}

// computeEulerAnglesDeg() using degrees
inline bool Nav::computeEulerAnglesDeg(
            const osg::Matrixd& rm,    // IN: Rotational matrix
            osg::Vec3d* const anglesD, // OUT: Euler angles (degrees)
            osg::Vec2d* const scPhi, // OUT: Sin/Cos of phi (Optional)
            osg::Vec2d* const scTht, // OUT: Sin/Cos of theta (Optional)
            osg::Vec2d* const scPsi  // OUT: Sin/Cos of psi (Optional)
         )
{
   osg::Vec3d angles;
   bool ok = computeEulerAngles(rm, &angles, scPhi, scTht, scPsi);
   if (ok && anglesD != nullptr) {
      anglesD->set(
         angles[0] * Basic::Angle::R2DCC,
         angles[1] * Basic::Angle::R2DCC,
         angles[2] * Basic::Angle::R2DCC
         );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Euler angle conversion functions
//    Body/NED <==> Body/ECEF
//------------------------------------------------------------------------------

// convertGeodAngles2EcefAngles()
inline bool Nav::convertGeodAngles2EcefAngles(
            const osg::Matrixd& wm, // IN: World matrix
            const osg::Matrixd& rm, // IN: Geodetic rotational matrix (body/NED directional cosines)
            osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
         )
{
   bool ok = false;
   if (vc != nullptr) {
      // compute body/ECEF directional cosines
      osg::Matrixd T = rm * wm;
      // compute geocentric orientation angles
      computeEulerAngles(T, vc);
      ok = true;
   }
   return ok;
}

// convertGeodAngles2EcefAngles()
inline bool Nav::convertGeodAngles2EcefAngles(
            const osg::Matrixd& wm, // IN: World matrix
            const osg::Vec3d& rpy,  // IN: Geodetic angles [ roll  pitch yaw ] (radians)
            osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
         )
{
   osg::Matrixd rm; // Rotational matrix (local/body directional cosines)
   computeRotationalMatrix(rpy[0], rpy[1], rpy[2], &rm);
   return convertGeodAngles2EcefAngles(wm,rm,vc);
}

// convertGeodAngles2EcefAngles()
inline bool Nav::convertGeodAngles2EcefAngles(
            const osg::Vec2d& ll,   // IN: Geodetic position  [ ILAT ILON ] [ degs degs ]
            const osg::Vec3d& rpy,  // IN: Geodetic angles [ roll  pitch yaw ] (radians)
            osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
         )
{
   osg::Matrixd wm; // World (earth) matrix
   computeWorldMatrix(ll[0], ll[1], &wm);
   return convertGeodAngles2EcefAngles(wm,rpy,vc);
}


//----------------------------------------------------------
// Convert geocentric Euler angles (body/ECEF) to geodetic Euler angles (body/NED)
//----------------------------------------------------------

// convertEcefAngles2GeodAngles()
inline bool Nav::convertEcefAngles2GeodAngles(
            const osg::Matrixd& wm, // IN: World matrix
            const osg::Matrixd& rm, // IN: Geocentric rotational matrix (ECEF/body directional cosines)
            osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
         )
{
   bool ok = false;
   if (vd != nullptr) {
      // Transpose world matrix
      osg::Matrixd wmT = wm;
      wmT.transpose();
      // Compute body/NED directional cosines
      osg::Matrixd T = rm * wmT;
      // compute Geodetic orientation angles
      computeEulerAngles(T, vd);
      ok = true;
   }
   return ok;
}

// convertEcefAngles2GeodAngles()
inline bool Nav::convertEcefAngles2GeodAngles(
            const osg::Matrixd& wm, // IN: World matrix
            const osg::Vec3d& rpy,  // IN: Geocentric angles (radians) [ phi theta psi ]
            osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
         )
{
   osg::Matrixd rm;
   computeRotationalMatrix(rpy[0], rpy[1], rpy[2], &rm);
   return convertEcefAngles2GeodAngles(wm, rm, vd);
}

// convertEcefAngles2GeodAngles()
inline bool Nav::convertEcefAngles2GeodAngles(
            const osg::Vec2d& ll,   // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
            const osg::Vec3d& rpy,  // IN: Geocentric angles (radians) [ phi theta psi ]
            osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
         )
{
   osg::Matrixd wm;
   computeWorldMatrix(ll[0], ll[1], &wm);
   osg::Matrixd rm;
   computeRotationalMatrix(rpy[0], rpy[1], rpy[2], &rm);
   return convertEcefAngles2GeodAngles(wm, rm, vd);
}

//----------------------------------------------------------
// Latitude, longitude and altitude (LLA) to/from NED vector conversion functions
//----------------------------------------------------------

//----------------------------------------------------------
// NED position vector to LLA using a flat earth projection
// with an optional earth model (default: WGS-84)
//----------------------------------------------------------
inline bool Nav::convertPosVec2llE(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const double sinSlat,      // IN: Sine of ref latitude
      const double cosSlat,      // IN: Cosine of ref latitude
      const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
      double* const lat,         // OUT: Latitude (degs)
      double* const lon,         // OUT: Longitude (degs)
      double* const alt,         // OUT: Altitude (meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   // Initialize earth model parameters
   const EarthModel* pModel = em;
   if (pModel == nullptr) { pModel = &EarthModel::wgs84; }

   const double a  = pModel->getA();   // semi-major axis
   const double e2 = pModel->getE2();  // eccentricity squared

   // Define Local Constants
   const double q  = 1.0 - e2 * sinSlat * sinSlat;
   const double rn = a / std::sqrt(q);
   const double rm = rn * (1.0 - e2) / q;

   // compute position
   double north = pos.x();
   double east = pos.y();
   double down = pos.z();
   if (lat != 0) {
      *lat = slat + Angle::R2DCC * (north / rm);
   }
   if (lon != 0) {
      if (cosSlat != 0) {
         *lon = Angle::aepcdDeg( slon + Angle::R2DCC * (east / rn) / cosSlat );
      }
      else {
         *lon = slon;
      }
   }
   if (alt != 0) {
      *alt = -down;
   }

   return true;
}

// convertPosVec2llE() without the sin/cos of latitude
inline bool Nav::convertPosVec2llE(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
      double* const lat,         // OUT: Latitude (degs)
      double* const lon,         // OUT: Longitude (degs)
      double* const alt,         // OUT: Altitude (meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   const double sinSlat = std::sin(Angle::D2RCC * slat);
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   return convertPosVec2llE(slat, slon, sinSlat, cosSlat, pos, lat, lon, alt, em);
}


//----------------------------------------------------------
// NED position vector to LLA using a flat earth projection
// with a spherical earth radius of ERAD60
//----------------------------------------------------------
inline bool Nav::convertPosVec2llS(
      const double slat,      // IN: Starting (reference) latitude (degs)
      const double slon,      // IN: Starting (reference) longitude (degs)
      const double cosSlat,   // IN: Cosine of ref latitude
      const osg::Vec3d& pos,  // IN: NED position vector from ref point (Meters)
      double* const lat,      // OUT: Latitude (degs)
      double* const lon,      // OUT: Longitude (degs)
      double* const alt       // OUT: Altitude (meters)
   )
{
   bool ok = false;
   if (lat != nullptr && lon != nullptr && alt != nullptr) {

      *lat = (pos[INORTH] * Distance::M2NM)/60.0 + slat;

      if (cosSlat != 0)
         *lon = Angle::aepcdDeg( (pos[IEAST] * Distance::M2NM)/(60.0 * cosSlat) + slon );
      else
         *lon = slon;

      *alt = -pos[IDOWN];

      ok = true;
   }
   return ok;
}

// legacy convertPosVec2LL()
inline bool Nav::convertPosVec2LL(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
      double* const lat,         // OUT: Latitude (degs)
      double* const lon,         // OUT: Longitude (degs)
      double* const alt          // OUT: Altitude (meters)
   )
{
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   return convertPosVec2llS(slat, slon, cosSlat, pos, lat, lon, alt);
}

// legacy convertPosVec2LL()
inline bool Nav::convertPosVec2LL(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const osg::Vec3f& pos,     // IN: NED position vector from ref point (Meters)
      double* const lat,         // OUT: Latitude (degs)
      double* const lon,         // OUT: Longitude (degs)
      double* const alt          // OUT: Altitude (meters)
   )
{
   osg::Vec3d posD = pos;
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   return convertPosVec2llS(slat, slon, cosSlat, posD, lat, lon, alt);
}

//----------------------------------------------------------
// LLA to NED position vector using a flat earth projection
// with an optional earth model (default: WGS-84)
//----------------------------------------------------------
inline bool Nav::convertLL2PosVecE(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const double sinSlat,      // IN: Sine of ref latitude
      const double cosSlat,      // IN: Cosine of ref latitude
      const double lat,          // IN: Latitude (degs)
      const double lon,          // IN: Longitude (degs)
      const double alt,          // IN: Altitude (meters)
      osg::Vec3d* const pos,     // OUT: NED position vector from ref point (Meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   bool ok = false;
   if (pos != 0) {
      // Initialize earth model parameters
      const EarthModel* pModel = em;
      if (pModel == nullptr) { pModel = &EarthModel::wgs84; }

      const double a  = pModel->getA();   // semi-major axis
      const double e2 = pModel->getE2();  // eccentricity squared

      // Define Constants
      const double q       = 1.0 - e2 * sinSlat * sinSlat;
      const double rn      = a / std::sqrt(q);
      const double rm      = rn * (1.0 - e2) / q;

      // Compute NED variables
      double x = Angle::D2RCC * Angle::aepcdDeg(lat - slat) * rm;
      double y = Angle::D2RCC * Angle::aepcdDeg(lon - slon) * rn * cosSlat;
      double z = ( -alt );
      pos->set(x, y, z);
      ok = true;
   }

   return ok;
}


//------------------------------------------------------------------------------
// LLA to NED position vector using a flat earth projection
// with a spherical earth radius of ERAD60
//------------------------------------------------------------------------------
inline bool Nav::convertLL2PosVecS(
      const double slat,      // IN: Reference latitude (degs)
      const double slon,      // IN: Reference longitude (degs)
      const double cosSlat,   // IN: Cosine of ref latitude
      const double lat,       // IN: Latitude (degs)
      const double lon,       // IN: Longitude (degs)
      const double alt,       // IN: Altitude (meters)
      osg::Vec3d* const pos   // OUT: NED position vector from ref point (Meters)
   )
{
   bool ok = false;
   if (pos != nullptr) {
      double x = ( Angle::aepcdDeg(lat - slat) * 60.0 * Distance::NM2M );
      double y = ( Angle::aepcdDeg(lon - slon) * 60.0 * Distance::NM2M * cosSlat );
      double z = ( -alt );
      pos->set(x, y, z);
      ok = true;
   }
   return ok;
}

// convertLL2PosVecE() without sin/cos of latitude
inline bool Nav::convertLL2PosVecE(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const double lat,          // IN: Latitude (degs)
      const double lon,          // IN: Longitude (degs)
      const double alt,          // IN: Altitude (meters)
      osg::Vec3d* const pos,     // OUT: NED position vector from ref point (Meters)
      const EarthModel* const em // IN: Pointer to an optional earth model (default: WGS-84)
   )
{
   const double sinSlat = std::sin(Angle::D2RCC * slat);
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   return convertLL2PosVecE(slat, slon, sinSlat, cosSlat, lat, lon, alt, pos, em);
}

// legacy convertLL2PosVec()
inline bool Nav::convertLL2PosVec(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const double lat,          // IN: Latitude (degs)
      const double lon,          // IN: Longitude (degs)
      const double alt,          // IN: Altitude (meters)
      osg::Vec3d* const pos      // OUT: NED position vector from ref point (Meters)
   )
{
   const double cosSlat = std::cos(Angle::D2RCC * slat);
   return convertLL2PosVecS(slat, slon, cosSlat, lat, lon, alt, pos);
}

// legacy convertLL2PosVec()
inline bool Nav::convertLL2PosVec(
      const double slat,         // IN: Reference latitude (degs)
      const double slon,         // IN: Reference longitude (degs)
      const double lat,          // IN: Latitude (degs)
      const double lon,          // IN: Longitude (degs)
      const double alt,          // IN: Altitude (meters)
      osg::Vec3f* const pos      // OUT: NED position vector from ref point (Meters)
   )
{
   bool ok = false;
   if (pos != nullptr) {
      const double cosSlat = std::cos(Angle::D2RCC * slat);
      osg::Vec3d posD;
      ok = convertLL2PosVecS(slat, slon, cosSlat, lat, lon, alt, &posD);
      *pos = posD;
   }
   return ok;
}

#endif
