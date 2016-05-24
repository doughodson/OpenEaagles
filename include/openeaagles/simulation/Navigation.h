//------------------------------------------------------------------------------
// Class: Navigation
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Navigation_H__
#define __oe_simulation_Navigation_H__

#include "openeaagles/simulation/System.h"
#include "openeaagles/base/osg/Vec2"
#include "openeaagles/base/osg/Matrix"

namespace oe {

namespace base { class Time; }

namespace simulation {
class Player;
class Route;
class Bullseye;

//------------------------------------------------------------------------------
// Classes:     Navigation
// Description: Generic Navigation System ...
//              Maintains present position; routes; steering cues;
//              NAV subsystems (INS, GPS); NAVAIDS (TACAN, ILS)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
// Factory name: Navigation
// Slots:
//    route    <Route>              ! Primary route (default: 0)
//
//    utc      <base::Time>        ! initial UTC time (default: 0)
//
//    feba     <base::PairStream>  ! Forward edge battle area (FEBA):
//                                  !   1) List of distance vectors [ North East ]
//                                  !   from the gaming area reference point.
//                                  !   2) Default unit is Nautical Miles
//                                  ! (default: 0)
//
//    bullseye <Bullseye>           ! Bullseye (just one for now) (default: 0)
//
//
// 'feba' slot example:
//
//       feba: {
//          [ 10 -10 ]                                   // Defaults to nautical miles
//          [ ( Kilometers 20 ) ( NauticalMiles -15 ) ]  // Distance nits defined
//       }
//
//
//------------------------------------------------------------------------------
class Navigation : public System
{
   DECLARE_SUBCLASS(Navigation,System)

public:
    Navigation();

    // Positional Data
   virtual bool isPositionDataValid() const;                // Is system position valid?
   virtual double getLatitude() const;                      // Returns system latitude (degs)
   virtual double getLongitude() const;                     // Returns system longitude (degs)
   virtual double getAltitudeFt() const;                    // Returns system altitude (ft)
   virtual double getAltitudeM() const;                     // Returns system altitude (m)

    // Attitude Data
   virtual bool isAttitudeDataValid() const;                // Is system attitude valid?
   virtual double getPitchDeg() const;                      // Returns system pitch (degs)
   virtual double getRollDeg() const;                       // Returns system roll  (degs)
   virtual double getHeadingDeg() const;                    // Returns system true heading (degs)
   virtual const osg::Matrixd& getRotMat() const;           // Rotational Matrix: (inertial to body)
                                                            //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw]
                                                            //    Usage:
                                                            //       Vb = M * Vi
                                                            //       Vi  = Vb * M
                                                            //    Where: 'Vb' is a body vector; 'Vi' is an inertial vector

    // Mag Var
   virtual bool isMagVarValid() const;                      // Is the magnetic variation valid?
   virtual double getMagVarDeg() const;                     // Returns the magnetic variation
   virtual double getMagHeadingD() const;                   // Returns the mag heading

   // Mag Var
   virtual bool areWindsValid() const;                      // Are the winds valid?
   virtual double getWindDirD() const;                      // Returns the wind 'from' direction (degs)
   virtual double getWindSpeedKts() const;                  // Returns the wind speed (kts)

    // UTC time
   virtual bool isUtcDataValid() const;                     // Is the UTC time valid?
   virtual double getUTC() const;                           // Returns UTC time (sec)

    // Velocity Data
   virtual bool isVelocityDataValid() const;                // Is system velocity valid?
   virtual double getGroundSpeedKts() const;                // Returns ground speed (kts)
   virtual double getTrueAirspeedKts() const;               // Returns true airspeed (kts)
   virtual double getGroundTrackDeg() const;                // Returns true ground track (degs)
   virtual const osg::Vec3d& getVelocity() const;           // Returns velocity vector (m/s)
   virtual const osg::Vec3d& getAcceleration() const;       // Returns acceleration vector (m/s/s)

    // Steering Data
   virtual bool isNavSteeringValid() const;                 // Is system steering data valid?
   virtual double getTrueBrgDeg() const;                    // Returns true bearing to dest (degs)
   virtual double getMagBrgDeg() const;                     // Returns mag bearing to dest (degs)
   virtual double getDistNM() const;                        // Returns distance to dest (NM)
   virtual double getTrueCrsDeg() const;                    // Returns true course to dest (degs)
   virtual double getMagCrsDeg() const;                     // Returns mag course to dest (degs)
   virtual double getCrossTrackErrorNM() const;             // Returns cross track error (NM)
   virtual double getTTG() const;                           // Returns Time-To-Go to dest (sec)
   virtual double getETA() const;                           // Returns Est-Time-of-Arrival at dest (sec)

    // Ref position
   virtual double getRefLatitude() const;                   // Returns the ref latitude (degs)
   virtual double getRefLongitude() const;                  // Returns the ref longitude (degs)

    // Primary route
   virtual Route* getPriRoute();                            // Returns the primary route (non-const version)
   virtual const Route* getPriRoute() const;                // Returns the primary route (const version)

    // Bullseye
   virtual Bullseye* getBullseye();                         // Returns the current bullseye (non-const version)
   virtual const Bullseye* getBullseye() const;             // Returns the current bullseye (const version)

    // FEBA [ North East ] (Nautical Miles)
    virtual int getFeba(osg::Vec2* const points, const int max) const;
    virtual bool setFeba(osg::Vec2* const points, const int n);

    // set/change the current route; does not change the initial route used by reset()
    virtual bool setRoute(Route* const msg);

    // Slot functions
    virtual bool setSlotRoute(const Route* const msg);  // and the initial route used by reset()
    virtual bool setSlotUtc(const base::Time* const msg);
    virtual bool setSlotFeba(const base::PairStream* const msg);
    virtual bool setSlotBullseye(Bullseye* const msg);

    void updateData(const double dt = 0.0) override;
    void reset() override;

protected:
   // Set positional data
   bool setPosition(const double latDeg, const double lonDeg, const double altM); // (sets data valid)
   bool setPosition(const bool dataValidFlg);

   // Set attitude data (angles are in degrees)
   bool setAttitude(const double roll, const double pitch, const double thdg); // (sets data valid)
   bool setAttitude(const bool dataValidFlg);

   // Mag Var
   bool setMagVar(const double mv); // (sets data valid);
   bool setMagVar(const bool dataValidFlg);

   // Winds
   bool setWinds(const double dirDeg, const double speedKts); // (sets data valid)
   bool setWinds(const bool dataValidFlg);

   // UTC time
   bool setUTC(const double time); // (sets data valid)
   bool setUTC(const bool dataValidFlg);

   // Velocity Data
   bool setGroundSpeedKts(const double kts);
   bool setTrueAirspeedKts(const double kts);
   bool setGroundTrackDeg(const double degs);
   bool setVelocity(const osg::Vec3d& v);
   bool setAcceleration(const osg::Vec3d& v);
   bool setVelocityDataValid(const bool dataValidFlg);

   // Steering Data
   bool setTrueBrgDeg(const double);
   bool setMagBrgDeg(const double);
   bool setDistNM(const double);
   bool setTrueCrsDeg(const double);
   bool setMagCrsDeg(const double);
   bool setCrossTrackErrorNM(const double);
   bool setTTG(const double);
   bool setETA(const double);
   bool setNavSteeringValid(const bool dataValidFlg);

   // Navigation methods
   virtual bool updateSysPosition();       // Method to compute nav system's positions
   virtual bool updateSysVelocity();       // Method to compute nav system's velocities
   virtual bool updateSysAttitude();       // Method to compute nav system's attitude angles
   virtual bool updateMagVar();            // Method to compute nav system's mag variation
   virtual bool updateNavSteering();       // Method to compute nav steering

   void process(const double dt) override;

private:
   void initData();

   base::safe_ptr<const Route> initRoute; // initial route (for reset purposes)
   base::safe_ptr<Route> priRoute;        // Primary route
   base::safe_ptr<Bullseye> bull;         // our bullseye (just one for now)

   // Positional data
   double      latitude;       // System Latitude          (degs)
   double      longitude;      // System Longitude         (degs)
   double      altitude;       // System altitude          (m)
   bool        posValid;       // Position data is valid

   // Attitude data
   osg::Matrixd rm;            // Rotational Matrix (inertial to body)
   double      heading;        // True Heading             (degs)
   double      pitch;          // Pitch angle              (degs)
   double      roll;           // Roll angle               (degs)
   bool        attValid;       // Attitude data is valid

   // Mag var data
   double      magvar;         // Mag Var                  (degs)
   double      mhdg;           // Mag heading              (degs)
   bool        magVarValid;    // Mag var data is valid

   // Winds
   bool        windsValid;       // Are the winds valid?
   double      windDirD;         // Returns the wind 'from' direction (degs)
   double      windSpdKts;       // Returns the wind speed (kts)

   // Velocity Data
   osg::Vec3d  velVec;         // Velocity Vector (earth)      (m/sec)       [ ue, ve, we ] NED
   osg::Vec3d  accelVec;       // Acceleration Vector (earth)  ((m/sec)/sec) [ due, dve, dwe ] NED
   double      gs;             // Ground speed             (kts)
   double      tas;            // True Air Speed           (kts)
   double      tk;             // Ground Track             (degs)
   bool        velValid;       // Velocity data is valid

   // Steering data
   bool        navStrValid;    // Nav steering data is valid
   double      tbrg;           // True bearing to destination    (deg)
   double      mbrg;           // Mag bearing to destination     (deg)
   double      dst;            // Direct distance to destination (nm)
   double      ttg;            // Time-To-Go to destination      (sec)
   double      tcrs;           // True course to destination     (degs)
   double      mcrs;           // Mag course to destination      (degs)
   double      xte;            // Cross Track Err                (nm)
   double      eta;            // Est Time of Arrival            (sec-utc)

   // UTC data
   double      utc;            // UTC time                       (sec)
   bool        utcValid;       // Nav steering data is valid
   double      initUTC;        // Initial UTC (from slots)

   // FEBA Line
   osg::Vec2*     feba;        // FEBA Points;  [ North East ]   (nm)
   int            nFeba;       // Number of FEBA points

   // Ref position
   double  refLat;             // Ref latitude (deg)
   double  refLon;             // Ref longitude (deg)
};

} // End simulation namespace
}

#endif
