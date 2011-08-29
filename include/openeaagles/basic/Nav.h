//------------------------------------------------------------------------------
// Class: Nav
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Nav_H__
#define __Eaagles_Basic_Nav_H__

#include "openeaagles/basic/EarthModel.h"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Basic {
   class EarthModel;

//------------------------------------------------------------------------------
// Class: Nav
// Description: General navigation and coordinate conversion functions ...
//------------------------------------------------------------------------------
class Nav
{

public:
   // osg::Vec2 and osg::Vec3 indexes
   enum { INORTH, IEAST, IDOWN };   // North, east and down (NED) position vector
   enum { IX, IY, IZ };             // [ X Y Z ] vector
   enum { ILAT, ILON, IALT };       // Lat/lon/alt position vector
   enum { IPHI, ITHETA, IPSI };     // Euler angles
   enum { IROLL, IPITCH, IYAW };    // Euler angles

public:
   static const double WGS84_A;     // WGS-84 semi major axis (Meters)
   static const double WGS84_B;     // WGS-84 semi minor axis (Meters)
   static const double WGS84_F;     // WGS-84 flattening
   static const double WGS84_E2;    // WGS-84 Eccentricity Squared

   static const double ERADM;       // Equatorial radius (WGS84 major axis) (Meters)
   static const double ERAD;        // Equatorial radius (WGS84 major axis) (NM)
   static const double ERAD60;      // Earth Radius (NM) (1 Arc Deg = 60 NM)

public:
//==============================================================================
// Navigation Functions
//==============================================================================

   //------------------------------------------------------------------------------
   // Flat-earth Brg/Dist to Lat/Lon, with Earth model (fdb2llE)
   //
   // Computes the destination (target) lat/lon from starting (ref)
   // point given the distance and initial bearing using flat earth projection
   // with an optional earth model, em
   //------------------------------------------------------------------------------
   static bool fbd2llE(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Destination latitude (degs)
         double* const dlon,        // OUT:  Destination longitude (degs)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Lat/Lon to Brg/Dist, with Earth model (fll2bdE)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using flat earth projection
   // with an optional earth model, em
   //------------------------------------------------------------------------------
   static bool fll2bdE(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True bearing (degs)
         double* const dist,        // OUT: distance (ground range) (nm)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Brg/Dist to Lat/Lon, with Spherical earth (fbd2llS)
   //
   // Computes the destination (target) lat/lon from starting (ref)
   // point given the distance and initial bearing using flat earth projection
   // with a spherical earth radius of 'ERAD60'
   //------------------------------------------------------------------------------
   static bool fbd2llS(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Destination latitude (degs)
         double* const dlon         // OUT:  Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Lat/Lon to Brg/Dist, with Spherical earth (fll2bdS)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using flat earth projection
   // with a spherical earth radius of 'ERAD60'
   //------------------------------------------------------------------------------
   static bool fll2bdS(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True bearing (degs)
         double* const dist         // OUT: distance (ground range) (nm)
      );

   //------------------------------------------------------------------------------
   // Legacy flat-earth projection; use fbd2llS() 
   //------------------------------------------------------------------------------
   static bool fbd2ll(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Destination latitude (degs)
         double* const dlon         // OUT:  Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Legacy flat-earth projection; use fll2bdS() 
   //------------------------------------------------------------------------------
   static bool fll2bd(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True bearing (degs)
         double* const dist         // OUT: distance (ground range) (nm)
      );


   //------------------------------------------------------------------------------
   // Converts a single set of Azimuth, Elevation and Range values to X, Y, Z (aer2xyz)
   //------------------------------------------------------------------------------

   // Using body angles
   static bool aer2xyz(
         osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  [meters]
         const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double az,           // IN:  azimuth (body)  (radians)
         const double el,           // IN:  elevation (body)  (positive up)   (radians)
         const double rng           // IN:  range [meters]
      );

   // Using NED angles
   static bool aer2xyz(
         osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  [meters]
         const double az,           // IN:  azimuth (NED)  (radians)
         const double el,           // IN:  elevation (NED)  (positive up)   (radians)
         const double rng           // IN:  range [meters]
      );

   //------------------------------------------------------------------------------
   // Convert 'n' sets of Azimuth, Elevation and Range values to X, Y, Z (aer2xyzArray)
   //------------------------------------------------------------------------------

   // Using body angles
   static bool aer2xyzArray(
         osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  [meters]
         const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double* const az,    // IN:  azimuth (body) array  (radians)
         const double* const el,    // IN:  elevation (body) array (positive up) (radians)
         const double* const rng,   // IN:  range array [meters]
         const unsigned int n       // IN:  number of sets to convert
      );

   // Using NED angles
   static bool aer2xyzArray(
         osg::Vec3d* const pos,     // OUT: position vector array (NED, player centered)  [meters]
         const double* const az,    // IN:  azimuth (NED) array  (radians)
         const double* const el,    // IN:  elevation (NED) array (positive up) (radians)
         const double* const rng,   // IN:  range array [meters]
         const unsigned int n       // IN:  number of sets to convert
      );

   //------------------------------------------------------------------------------
   // Converts a single set of X, Y, Z values to Azimuth, Elevation and Range (xyz2aer)
   //------------------------------------------------------------------------------

   // Computing NED angles
   static bool xyz2aer(
         osg::Vec3d* const aer,     // OUT: position vector  (aer)   [deg,deg,meters]
         const double x,            // IN:  x                        [meters]
         const double y,            // IN:  y                        [meters]
         const double z             // IN:  z                        [meters]
      );

   // Computing body angles
   static bool xyz2aer(
         osg::Vec3d* const aer,     // OUT: position vector  (aer)   [deg,deg,meters]
         const osg::Matrixd& rm,    // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double x,            // IN:  x                        [meters]
         const double y,            // IN:  y                        [meters]
         const double z             // IN:  z                        [meters]
      );

   //------------------------------------------------------------------------------
   // Great circle Brg/Dist to Lat/Lon (gbd2ll)
   //
   // Computes the destination lat/lon from starting (ref)
   // point given distance and initial bearing.
   //------------------------------------------------------------------------------

   // ---
   // Great circle method #1 -- Elliptical earth model
   // ---
   static bool gbd2ll(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Destination latitude (degs)
         double* const dlon,        // OUT: Destination longitude (degs)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Great circle method #2 -- Spherical earth with radius 'ERAD60'
   // reference: "ARRL"
   // ---
   static bool gbd2llS(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Destination latitude (degs)
         double* const dlon         // OUT:  Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Great circle Lat/Lon to Brg/Dist (gll2bd)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using great circle methods.
   //------------------------------------------------------------------------------

   // ---
   // Great circle method #1 -- Elliptical earth model
   // ---
   static bool gll2bd(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True bearing (degs)
         double* const dist,        // OUT: distance (ground range) (nm)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Great circle method #2 -- Spherical earth with radius 'ERAD60'
   // reference: "ARRL"
   // ---
   static bool gll2bdS(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True bearing (degs)
         double* const dist         // OUT: distance (ground range) (nm)
      );

   //------------------------------------------------------------------------------
   // Great circle Lat/Lon/Alt to Brg/Dist (glla2bd)
   //
   // Computes the initial bearing, slant range, ground distance and
   // elevation angle from the starting lat/lon (ref point) to the destination
   // lat/lon using great circle methods.
   // (elevation does not account for earth curvature at this time)
   //------------------------------------------------------------------------------
   static bool glla2bd(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double salt,         // IN: Starting (reference) altitude (meters)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         const double dalt,         // IN: Destination altitude (meters)
         double* const brg,         // OUT: True bearing (degs)
         double* const slantRng,    // OUT: slant range (nm)
         double* const dist,        // OUT: distance (ground range) (nm)
         double* const elev=0,      // OUT: Optional: elevation angle (degs)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Method #2 -- Spherical earth with radius 'ERAD60'
   // ---
   static bool glla2bdS(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double salt,         // IN: Starting (reference) altitude (meters)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         const double dalt,         // IN: Destination altitude (meters)
         double* const brg,         // OUT: True bearing (degs)
         double* const slantRng,    // OUT: slant range (nm)
         double* const dist,        // OUT: distance (ground range) (nm)
         double* const elev = 0     // OUT: (optional) elevation angle (degs)
      );

 
   //------------------------------------------------------------------------------
   // Vincenty direct method Lat/Lon to Brg/Dist (vbd2ll)
   //
   // Computes the destination lat/lon from starting (ref) point
   // given the distance and initial bearing using the Vincenty
   // direct method.
   //
   //    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
   //                Applications of Nested Equations"  by Thaddeus Vincenty
   //------------------------------------------------------------------------------
   static bool vbd2ll(
         const double slat,         // IN: Starting (Reference) latitude (degs)
         const double slon,         // IN: Starting (Reference) longitude (degs)
         const double brg,          // IN: True bearing (degs)
         const double dist,         // IN: Distance (ground range) (nm)
         double* const dlat,        // OUT: Target latitude (degs)
         double* const dlon,        // OUT: Target longitude (degs)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Vincenty inverse method Lat/Lon to Brg/Dist (vll2bd)
   //
   // Computes the initial and final bearings and the distance from the starting
   // lat/lon (ref point) to destination lat/lon using the Vincenty Inverse Method.
   //
   //    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
   //               Applications of Nested Equations"  by Thaddeus Vincenty
   //
   //------------------------------------------------------------------------------
   static bool vll2bd(
         const double slat,         // IN: Starting (reference) latitude (degs)
         const double slon,         // IN: Starting (reference) longitude (degs)
         const double dlat,         // IN: Destination latitude (degs)
         const double dlon,         // IN: Destination longitude (degs)
         double* const brg,         // OUT: True initial bearing (degs)
         double* const dist,        // OUT: geodesic distance (nm)
         double* const brg2=0,      // OUT: Optional: true final bearing (degs)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

//==============================================================================
// Matrix generators
//==============================================================================

   //------------------------------------------------------------------------------
   // Compute World transformation Matrix (computeWorldMatrix) from lat/lon (degrees)
   //
   //       World (ECEF) <==> Inertial (NED)
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
   //       Vi is an initial (NED) vector
   //       Vw is a world (ECEF) vector
   //------------------------------------------------------------------------------
   static bool computeWorldMatrix(
         const double latD,      // IN: Reference latitude (degs)
         const double lonD,      // IN: Reference longitude (degs)
         osg::Matrixd* const m   // OUT: Matrix
      );

   //------------------------------------------------------------------------------
   // Compute Rotational transformation Matrix (computeRotationalMatrix),
   // with optional sin/cos values) from euler angles
   //
   //    Reference system <==> Rotating system
   //
   // output:
   //    M  = Rx[phi] * Ry[theta] * Rz[psi]
   //
   // Usage:
   //    Vb = M * Vi;
   //    Vi = Vb * M;   // same as Vi = M(t) * Vb
   //       Vb is a body (or rotating) vector
   //       Vi is an initial (or fixed) vector
   //------------------------------------------------------------------------------

   // Version using radians
   static bool computeRotationalMatrix(
         const double phi,          // IN: roll angle (radians)
         const double theta,        // IN: pitch angle (radians)
         const double psi,          // IN: yaw angle (radians)
         osg::Matrixd* const rm,    // OUT: Rotational matrix
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );

   // Version using degrees
   static bool computeRotationalMatrixDeg(
         const double phiD,         // IN: roll angle (degrees)
         const double thetaD,       // IN: pitch angle (degrees)
         const double psiD,         // IN: yaw angle (degrees)
         osg::Matrixd* const rm,    // OUT: Rotational matrix
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );

   // Vec3 version
   static bool computeRotationalMatrix(
         const osg::Vec3d& angles,  // IN: euler angles [ phi theta psi ] (radians)
         osg::Matrixd* const m,     // OUT: Matrix
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );

   // Vec3 version using degrees
   static bool computeRotationalMatrixDeg(
         const osg::Vec3d& angles,  // IN: euler angles [ phi theta psi ] (degrees)
         osg::Matrixd* const m,     // OUT: Matrix
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );


   //------------------------------------------------------------------------------
   // Compute Euler Angles (computeEulerAngles), and optionally the sin/cos for
   // each, from a rotational matrix as defined by computeRotationalMatrix() above.
   //
   //    Input matrix = Rx[phi] * Ry[theta] * Rz[psi]
   //------------------------------------------------------------------------------

   // Version using radians
   static bool computeEulerAngles(
         const osg::Matrixd& rm,    // IN: Rotational matrix
         osg::Vec3d* const angles,  // OUT: euler angles (radians)
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );

   // Version using degrees
   static bool computeEulerAnglesDeg(
         const osg::Matrixd& rm,    // IN: Rotational matrix
         osg::Vec3d* const anglesD, // OUT: euler angles (degrees)
         osg::Vec2d* const scPhi=0, // OUT: Sin/Cos of phi (Optional)
         osg::Vec2d* const scTht=0, // OUT: Sin/Cos of theta (Optional)
         osg::Vec2d* const scPsi=0  // OUT: Sin/Cos of psi (Optional)
      );


//==============================================================================
// Latitude, longitude and altitude (LLA) to/from NED vector conversion functions
//==============================================================================

   //----------------------------------------------------------
   // NED position vector to LLA using a flat earth projection
   // with an optional earth model (default: WGS-84)
   //----------------------------------------------------------
   static bool convertPosVec2llE(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double sinSlat,      // IN: Sine of ref latitude
         const double cosSlat,      // IN: Cosine of ref latitude
         const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt,         // OUT: Altitude (meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   static bool convertPosVec2llE(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt,         // OUT: Altitude (meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // NED position vector to LLA using a flat earth projection 
   // with a spherical earth radius of ERAD60
   //----------------------------------------------------------
   static bool convertPosVec2llS(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double cosSlat,      // IN: Cosine of ref latitude
         const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );

   //----------------------------------------------------------
   // Legacy functions; use convertPosVec2llS()
   //----------------------------------------------------------
   // using double Vec3
   static bool convertPosVec2LL(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const osg::Vec3d& pos,     // IN: NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );
   // using float Vec3
   static bool convertPosVec2LL(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const osg::Vec3f& pos,     // IN: NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );

   //----------------------------------------------------------
   // LLA to NED position vector using a flat earth projection
   // with an optional earth model (default: WGS-84)
   //----------------------------------------------------------
   static bool convertLL2PosVecE(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double sinSlat,      // IN: Sine of ref latitude
         const double cosSlat,      // IN: Cosine of ref latitude
         const double lat,          // IN: Latitude (degs)
         const double lon,          // IN: Longitude (degs)
         const double alt,          // IN: Altitude (meters)
         osg::Vec3d* const pos,     // OUT: NED position vector from ref point (Meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   static bool convertLL2PosVecE(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double lat,          // IN: Latitude (degs)
         const double lon,          // IN: Longitude (degs)
         const double alt,          // IN: Altitude (meters)
         osg::Vec3d* const pos,     // OUT: NED position vector from ref point (Meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // LLA to NED position vector using a flat earth projection
   // with a spherical earth radius of ERAD60
   //----------------------------------------------------------
   static bool convertLL2PosVecS(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double cosSlat,      // IN: Cosine of ref latitude
         const double lat,          // IN: Latitude (degs)
         const double lon,          // IN: Longitude (degs)
         const double alt,          // IN: Altitude (meters)
         osg::Vec3d* const pos      // OUT: NED position vector from ref point (Meters)
      );

   //----------------------------------------------------------
   // Legacy functions; use convertLL2PosVecS()
   //----------------------------------------------------------
   // using double Vec3
   static bool convertLL2PosVec(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double lat,          // IN: Latitude (degs)
         const double lon,          // IN: Longitude (degs)
         const double alt,          // IN: Altitude (meters)
         osg::Vec3d* const pos      // OUT: NED position vector from ref point (Meters)
      );
   // using float Vec3
   static bool convertLL2PosVec(
         const double slat,         // IN: Reference latitude (degs)
         const double slon,         // IN: Reference longitude (degs)
         const double lat,          // IN: Latitude (degs)
         const double lon,          // IN: Longitude (degs)
         const double alt,          // IN: Altitude (meters)
         osg::Vec3f* const pos      // OUT: NED position vector from ref point (Meters)
      );


//==============================================================================
// Coordinate conversion functions
//    Earth centered, earth fixed (ECEF) coordinates <==> Geodetic coordinates
//
// Currently hardcoded to WGS84 geodetic coordinates
//==============================================================================

   //----------------------------------------------------------
   // Convert ECEF (XYZ coordinates) to Geodetic (LLA coordinates)
   //----------------------------------------------------------
   static bool convertEcef2Geod(
         const double x,      // IN: ECEF X component   (meters)
         const double y,      // IN: ECEF Y component   (meters)
         const double z,      // IN: ECEF Z component   (meters)
         double* const pLat,  // OUT: Geodetic latitude  (degrees)
         double* const pLon,  // OUT: Geodetic longitude (degrees)
         double* const pAlt,     // OUT: Geodetic altitude  (meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // Using osg::Vec3d vectors
   static bool convertEcef2Geod(
         const osg::Vec3d& vec,  // IN: ECEF [ IX IY IZ ]
         osg::Vec3d* const lla,  // OUT: Geodetic [ ILAT ILON IALT ]
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // Using arrays vectors
   static bool convertEcef2Geod(
         const double vec[3],    // IN: ECEF [ IX IY IZ ]
         double lla[3],          // OUT: Geodetic [ ILAT ILON IALT ]
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // Convert Geodetic (LLA coordinates) to ECEF (XYZ coordinates)
   //----------------------------------------------------------
   static bool convertGeod2Ecef(
         const double lat,    // IN: Geodetic latitude  (degrees)
         const double lon,    // IN: Geodetic longitude (degrees)
         const double alt,    // IN: Geodetic altitude  (meters)
         double* const pX,    // OUT: ECEF X component   (meters)   
         double* const pY,    // OUT: ECEF Y component   (meters)
         double* const pZ,       // OUT: ECEF Z component   (meters)
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // Using osg::Vec3d vectors
   static bool convertGeod2Ecef(
         const osg::Vec3d& lla,  // IN: Geodetic [ ILAT ILON IALT ]
         osg::Vec3d* const ecef,  // OUT: ECEF [ IX IY IZ ]
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );

   // Using array vectors
   static bool convertGeod2Ecef(
         const double lla[3],    // IN: Geodetic [ ILAT ILON IALT ]
         double ecef[3],         // OUT: ECEF [ IX IY IZ ]
         const EarthModel* const em=0 // IN: Pointer to an optional earth model (default: WGS-84)
      );


//==============================================================================
// Euler angle conversion functions
//    Body/NED <==> Body/ECEF
//==============================================================================

   //----------------------------------------------------------
   // Convert geodetic euler angles (body/NED) to geocentric euler angles (body/ECEF)
   //----------------------------------------------------------

   static bool convertGeodAngles2EcefAngles(
         const osg::Matrixd& wm, // IN: World matrix
         const osg::Matrixd& rm, // IN: Geodetic rotational matrix (body/NED directional cosines)
         osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   static bool convertGeodAngles2EcefAngles(
         const osg::Matrixd& wm, // IN: World matrix
         const osg::Vec3d& rpy,  // IN: Geodetic angles [ roll  pitch yaw ] (radians)
         osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   static bool convertGeodAngles2EcefAngles(
         const osg::Vec2d& ll,   // IN: Geodetic position  [ ILAT ILON ] [ degs degs ]
         const osg::Vec3d& rpy,  // IN: Geodetic angles [ roll  pitch yaw ] (radians)
         osg::Vec3d* const vc    // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   //----------------------------------------------------------
   // Convert geocentric euler angles (body/ECEF) to geodetic euler angles (body/NED)
   //----------------------------------------------------------
   static bool convertEcefAngles2GeodAngles(
         const osg::Matrixd& wm, // IN: World matrix
         const osg::Matrixd& rm, // IN: Geocentric rotational matrix (ECEF/body directional cosines)
         osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );

   static bool convertEcefAngles2GeodAngles(
         const osg::Matrixd& wm, // IN: World matrix
         const osg::Vec3d& rpy,  // IN: Geocentric angles (radians) [ phi theta psi ]
         osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );

   static bool convertEcefAngles2GeodAngles(
         const osg::Vec2d& ll,   // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
         const osg::Vec3d& rpy,  // IN: Geocentric angles (radians) [ phi theta psi ]
         osg::Vec3d* const vd    // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );


//==============================================================================
// Geodetic (WGS-84) to/from Geocentric (ECEF) Conversion functions
//   (Legacy from SubrDis -- no longer being used --)
//==============================================================================

   // Converts geodetic (WGS84) to geocentric (ECEF) coordinates (From SubrDis)
   static bool getGeocCoords(
      const double geodPos[3],   // IN: Geodetic (WGS84)  [ ILAT ILON IALT ] [ degs degs meters ]
      double geocPos[3]          // OUT: Geocentric (ECEF) [ IX IY IZ ] (meters)
   );

   // Converts geocentric to geodetic coordinates (From SubrDis)
   static bool getGeodCoords(
      const double geocPos[3],   // IN: Geocentric (ECEF) [ IX IY IZ ] (meters)
      double geodPos[3]          // OUT: Geodetic (WGS84) [ ILAT ILON IALT ] [ degs degs meters ]
   );

   // Converts geocentric angles to geodetic angles (From SubrDis)
   static bool getGeodAngle(
      const double geodPos[2],      // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geocAngle[3],    // IN: Geocentric euler angles (radians) [ IPHI ITHETA IPSI ]
      double geodAngle[3]           // OUT: Geodetic euler angles (radians) [ IPHI ITHETA IPSI ]
   );

   // Converts geodetic angles to geocentric angles (From SubrDis)
   static bool getGeocAngle(
      const double geodPos[2],      // IN: Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geodAngle[3],    // IN: Geodetic euler angles (radians) [ IPHI ITHETA IPSI ]
      double geocAngle[3]           // OUT: Geocentric euler angles (radians) [ IPHI ITHETA IPSI ]
   );

   // Converts World (Geocentric) to Simulation (Geodetic) (From SubrDis)
   static bool getSimPosAccVel(
      const double geocPos[3],   // IN: Geocentric position vector [ IX IY IZ ] (meters)
      const double geocVel[3],   // IN: Geocentric velocity vector [ IX IY IZ ] (m/sec)
      const double geocAcc[3],   // IN: Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geodPos[3],         // OUT: Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      double geodVel[3],         // OUT: Geodetic velocity vector [ IX IY IZ ] (m/sec)
      double geodAcc[3]          // OUT: Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
   );

   // Converts Simulation (Geodetic) to World (Geocentric) (From SubrDis)
   static bool getWorldPosAccVel(
      const double geodPos[3],   // IN: Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      const double geodVel[3],   // IN: Geodetic velocity vector [ IX IY IZ ] (m/sec)
      const double geodAcc[3],   // IN: Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geocPos[3],         // OUT: Geocentric position vector [ IX IY IZ ] (meters)
      double geocVel[3],         // OUT: Geocentric velocity vector [ IX IY IZ ] (m/sec)
      double geocAcc[3]          // OUT: Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
   );

}; // End class Nav


// inline functions
#include "openeaagles/basic/Nav.inl"


} // End Basic namespace
} // End Eaagles namespace

#endif
