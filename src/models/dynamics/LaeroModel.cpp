
#include "openeaagles/models/dynamics/LaeroModel.hpp"

#include "openeaagles/models/players/Player.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Quat"
#include "openeaagles/base/osg/Math"

#include <iostream>
#include <iomanip>
#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LaeroModel, "LaeroModel")
EMPTY_SERIALIZER(LaeroModel)
EMPTY_DELETEDATA(LaeroModel)

//----------------------------------------------------------
// conversion constants
//----------------------------------------------------------
const double LaeroModel::HALF_PI    = base::PI / 2.0;
const double LaeroModel::EPSILON    = 1.0E-10;

//----------------------------------------------------------
// constructor
//----------------------------------------------------------
LaeroModel::LaeroModel()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

//----------------------------------------------------------
// initData() -- initialize member data
//----------------------------------------------------------
void LaeroModel::initData()
{
   dT       = 0.0;

   // Body angular vel, acc components
   p        = 0.0;
   q        = 0.0;
   r        = 0.0;
   pDot     = 0.0;
   qDot     = 0.0;
   rDot     = 0.0;

   // Euler rotation angle, rate components
   phi      = 0.0;
   tht      = 0.0;
   psi      = 0.0;
   phiDot   = 0.0;
   thtDot   = 0.0;
   psiDot   = 0.0;

   // Body linear vel, acc components
   u        = 0.0;
   v        = 0.0;
   w        = 0.0;
   uDot     = 0.0;
   vDot     = 0.0;
   wDot     = 0.0;

   // NED pos, vel, acc components
   refPosN  = 0.0;
   refPosE  = 0.0;
   refPosD  = 0.0;
   posN     = 0.0;
   posE     = 0.0;
   posD     = 0.0;

   velN     = 0.0;
   velE     = 0.0;
   velD     = 0.0;
   accN     = 0.0;
   accE     = 0.0;
   accD     = 0.0;

   // Hold components
   phiDot1  = 0.0;
   thtDot1  = 0.0;
   psiDot1  = 0.0;

   uDot1    = 0.0;
   vDot1    = 0.0;
   wDot1    = 0.0;
}

//----------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//----------------------------------------------------------
void LaeroModel::copyData(const LaeroModel& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   dT       = org.dT;

   // Body angular vel, acc components
   p        = org.p;
   q        = org.q;
   r        = org.r;
   pDot     = org.pDot;
   qDot     = org.qDot;
   rDot     = org.rDot;

   // Euler rotation angle, rate components
   phi      = org.phi;
   tht      = org.tht;
   psi      = org.psi;
   phiDot   = org.phiDot;
   thtDot   = org.thtDot;
   psiDot   = org.psiDot;

   // Body linear vel, acc components
   u        = org.u;
   v        = org.v;
   w        = org.w;
   uDot     = org.uDot;
   vDot     = org.vDot;
   wDot     = org.wDot;

   // NED pos, vel, acc components
   refPosN  = org.refPosN;
   refPosE  = org.refPosE;
   refPosD  = org.refPosD;
   posN     = org.posN;
   posE     = org.posE;
   posD     = org.posD;

   velN     = org.velN;
   velE     = org.velE;
   velD     = org.velD;
   accN     = org.accN;
   accE     = org.accE;
   accD     = org.accD;

   // Hold components
   phiDot1  = org.phiDot1;
   thtDot1  = org.thtDot1;
   psiDot1  = org.psiDot1;
   uDot1    = org.uDot1;
   vDot1    = org.vDot1;
   wDot1    = org.wDot1;
}

//------------------------------------------------------------------------------
// dynamics() -- update player's vehicle dynamics
//------------------------------------------------------------------------------
void LaeroModel::dynamics(const double dt)
{
    update4DofModel(dt);
    dT = dt;
}

//----------------------------------------------------------
// reset() -- sets up our initial flying values
//----------------------------------------------------------
void LaeroModel::reset()
{
   BaseClass::reset();

   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   if (pPlr != nullptr) {
      double initVel = pPlr->getInitVelocity();
      u = initVel * base::Distance::NM2M / base::Time::H2S;
   }
}

//----------------------------------------------------------
// update4DofModel -- update equations of motion
//----------------------------------------------------------
void LaeroModel::update4DofModel(const double dt)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   if (pPlr != nullptr) {

      //==============================================================
      // ROTATIONAL EOM
      //==============================================================

      //----------------------------------------------------
      // integrate Euler angles using Adams-Bashforth
      //----------------------------------------------------
      phi += 0.5 * (3.0 * phiDot - phiDot1) * dT;
      if (phi >  base::PI) phi = -base::PI;
      if (phi < -base::PI) phi =  base::PI;

      tht += 0.5 * (3.0 * thtDot - thtDot1) * dT;
      if (tht >=  HALF_PI) tht =  (HALF_PI - EPSILON);
      if (tht <= -HALF_PI) tht = -(HALF_PI - EPSILON);

      psi += 0.5 * (3.0 * psiDot - psiDot1) * dT;
      if (psi >  base::PI) psi = -base::PI;
      if (psi < -base::PI) psi =  base::PI;

      //----------------------------------------------------
      // update Euler angles
      //----------------------------------------------------
      pPlr->setEulerAngles(phi, tht, psi);

      //----------------------------------------------------
      // hold current rotational control values for next iteration
      //----------------------------------------------------
      phiDot1 = phiDot;
      thtDot1 = thtDot;
      psiDot1 = psiDot;

      //----------------------------------------------------
      // compute sin, cos of Euler angles
      //----------------------------------------------------
      double sinPhi = std::sin(phi);
      double cosPhi = std::cos(phi);
      double sinTht = std::sin(tht);
      double cosTht = std::cos(tht);
      double sinPsi = std::sin(psi);
      double cosPsi = std::cos(psi);

      //----------------------------------------------------
      // compute local to body axes matrix
      //----------------------------------------------------
      double l1 =  cosTht * cosPsi;
      double l2 =  cosTht * sinPsi;
      double l3 = -sinTht;
      double m1 =  sinPhi * sinTht * cosPsi - cosPhi * sinPsi;
      double m2 =  sinPhi * sinTht * sinPsi + cosPhi * cosPsi;
      double m3 =  sinPhi * cosTht;
      double n1 =  cosPhi * sinTht * cosPsi + sinPhi * sinPsi;
      double n2 =  cosPhi * sinTht * sinPsi - sinPhi * cosPsi;
      double n3 =  cosPhi * cosTht;

      //----------------------------------------------------
      // update p,q,r angular velocity components (body)
      //----------------------------------------------------
      p = phiDot                    +       (-sinTht)*psiDot;
      q =           (cosPhi)*thtDot + (cosTht*sinPhi)*psiDot;
      r =          (-sinPhi)*thtDot + (cosTht*cosPhi)*psiDot;

      //----------------------------------------------------
      // update angular velocities
      //----------------------------------------------------
      pPlr->setAngularVelocities(p, q, r);


      //==============================================================
      // TRANSLATIONAL EOM
      //==============================================================

      //----------------------------------------------------
      // integrate u,v,w velocity components (body)
      //----------------------------------------------------
      u  += 0.5*(3.0*uDot - uDot1)*dT;
      v  += 0.5*(3.0*vDot - vDot1)*dT;
      w  += 0.5*(3.0*wDot - wDot1)*dT;

      //----------------------------------------------------
      // hold current translational control values for next iteration
      //----------------------------------------------------
      uDot1   = uDot;
      vDot1   = vDot;
      wDot1   = wDot;

      //----------------------------------------------------
      // update velocity in NED system
      //----------------------------------------------------
      velN  = l1*u + m1*v + n1*w;
      velE  = l2*u + m2*v + n2*w;
      velD  = l3*u + m3*v + n3*w;

      pPlr->setVelocity(velN, velE, velD);

      //----------------------------------------------------
      // update acceleration in NED system
      //----------------------------------------------------
      //double A = uDot + w*q - v*r;
      //double B = vDot + u*r - w*p;
      //double C = wDot + v*p - u*q;

      //accN  = l1*A + m1*B + n1*C;
      //accE  = l2*A + m2*B + n2*C;
      //accD  = l3*A + m3*B + n3*C;

      //pPlr->setAcceleration(accN, accE, accD);
   }
}

//
// flight interface
//

bool LaeroModel::flyPhi(const double phiCmdDeg, const double phiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok = (pPlr != nullptr);
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double phiCmdRad    = phiCmdDeg * base::Angle::D2RCC;
      double phiDotCmdRps = phiDotCmdDps * base::Angle::D2RCC;

      //-------------------------------------------------------
      // current phi error (rad)
      //-------------------------------------------------------
      double phiRad    = pPlr->getRollR();
      double phiErrRad = phiCmdRad - phiRad;

      //-------------------------------------------------------
      // phi error break point (rad)
      //-------------------------------------------------------
      const double TAU = 1.0;  // time constant [sec]
      double phiErrBrkRad = phiDotCmdRps * TAU;

      //-------------------------------------------------------
      // control signal for commanded phi (rps)
      //-------------------------------------------------------
      double phiDotRps = base::sign(phiErrRad) * phiDotCmdRps;
      if (std::abs(phiErrRad) < phiErrBrkRad) {
         phiDotRps = (phiErrRad / phiErrBrkRad) * phiDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to roll control
      //-------------------------------------------------------
      phiDot = phiDotRps;
   }

   return ok;
}

bool LaeroModel::flyTht(const double thtCmdDeg, const double thtDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok = (pPlr != nullptr);
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double thtCmdRad    = thtCmdDeg * base::Angle::D2RCC;
      double thtDotCmdRps = thtDotCmdDps * base::Angle::D2RCC;

      //-------------------------------------------------------
      // current tht error (rad)
      //-------------------------------------------------------
      double thtRad    = pPlr->getPitchR();
      double thtErrRad = thtCmdRad - thtRad;

      //-------------------------------------------------------
      // tht error break point (rad)
      //-------------------------------------------------------
      const double TAU = 1.0;  // time constant [sec]
      double thtErrBrkRad = thtDotCmdRps * TAU;

      //-------------------------------------------------------
      // control signal for commanded tht (rps)
      //-------------------------------------------------------
      double thtDotRps = base::sign(thtErrRad) * thtDotCmdRps;
      if (std::abs(thtErrRad) < thtErrBrkRad) {
         thtDotRps = (thtErrRad / thtErrBrkRad) * thtDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to pitch control
      //-------------------------------------------------------
      thtDot = thtDotRps;
   }

   return ok;
}

bool LaeroModel::flyPsi(const double psiCmdDeg, const double psiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok = (pPlr != nullptr);
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double psiCmdRad    = psiCmdDeg * base::Angle::D2RCC;
      double psiDotCmdRps = psiDotCmdDps * base::Angle::D2RCC;

      //-------------------------------------------------------
      // current psi error (rad)
      //-------------------------------------------------------
      double psiRad    = pPlr->getHeadingR();
      double psiErrRad = psiCmdRad - psiRad;

      //-------------------------------------------------------
      // psi error break point (rad)
      //-------------------------------------------------------
      const double TAU = 1.0;  // time constant [sec]
      double psiErrBrkRad = psiDotCmdRps * TAU;

      //-------------------------------------------------------
      // control signal for commanded psi (rps)
      //-------------------------------------------------------
      double psiDotRps = base::sign(psiErrRad) * psiDotCmdRps;
      if (std::abs(psiErrRad) < psiErrBrkRad) {
         psiDotRps = (psiErrRad / psiErrBrkRad) * psiDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to azimuth control
      //-------------------------------------------------------
      psiDot = psiDotRps;
   }

   return ok;
}

////==============================================================================
//bool LaeroModel::flyVel(const double velCmdKts, const double velDotCmdNps)
//{
//   //-------------------------------------------------------
//   // get data pointers
//   //-------------------------------------------------------
//   simulation::Player* pPlr = static_cast<simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
//   bool ok = (pPlr != nullptr);
//   if (ok) {
//
//      //-------------------------------------------------------
//      // define local constants
//      //-------------------------------------------------------
//      const double KTS2MPS = base::Distance::NM2M / base::Time::H2S;
//
//      //-------------------------------------------------------
//      // convert argument units (deg -> rad)
//      //-------------------------------------------------------
//      double velCmdMps     = velCmdKts * KTS2MPS;
//      double velDotCmdMps2 = velDotCmdNps * KTS2MPS;
//
//      //-------------------------------------------------------
//      // current vel error (rad)
//      //-------------------------------------------------------
//      double velMps    = pPlr->getTotalVelocityKts() * KTS2MPS;
//      double velErrMps = velCmdMps - velMps;
//
//      //-------------------------------------------------------
//      // vel error break point (rad)
//      //-------------------------------------------------------
//      const double TAU = 1.0;  // time constant [sec]
//      double velErrBrkMps = velDotCmdMps2 * TAU;
//
//      //-------------------------------------------------------
//      // control signal for commanded vel (rps)
//      //-------------------------------------------------------
//      double velDotMps2 = sign(velErrMps) * velDotCmdMps2;
//      if (std::abs(velErrMps) < velErrBrkMps) {
//         velDotMps2 = (velErrMps / velErrBrkMps) * velDotCmdMps2;
//      }
//
//      //-------------------------------------------------------
//      // assign result to velocity control
//      //-------------------------------------------------------
//      uDot = velDotMps2;
//   }
//
//   return ok;
//}

//==============================================================================
//bool LaeroModel::fly2LL(const double latDeg, const double lonDeg)
//{
//   //-------------------------------------------------------
//   // get data pointers
//   //-------------------------------------------------------
//   simulation::Player* pPlr = static_cast<simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
//
//   bool ok = (pPlr != nullptr);
//   if (ok) {
//
//      //----------------------------------------------------
//      // get current data
//      //----------------------------------------------------
//      double osLatDeg  = pPlr->getLatitude();
//      double osLonDeg  = pPlr->getLongitude();
//      double brgDeg = 0.0;
//      double rngMtr = 0.0;
//      base::Nav::gll2bd(osLatDeg, osLonDeg, latDeg, lonDeg, &brgDeg,&rngMtr);
//
//      //-------------------------------------------------------
//      // fly to heading necessary to intercept lat/lon
//      //-------------------------------------------------------
//      ok = flyHDG(brgDeg);
//   }
//
//   return ok;
//}


// Dynamics model interface
bool LaeroModel::setCommandedHeadingD(const double h, const double hDps, const double maxBank)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   bool ok = (pPlr != nullptr);
   if (ok) {

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double MAX_BANK_RAD = maxBank * base::Angle::D2RCC;
      //const double TAU = 2.0;  // time constant [sec]
      const double TAU = 1.0;  // time constant [sec]

      //-------------------------------------------------------
      // get current data
      //-------------------------------------------------------
      double velMps        = pPlr->getTotalVelocity();
      double hdgDeg        = pPlr->getHeadingD();
      double hdgErrDeg     = base::Angle::aepcdDeg(h - hdgDeg);
      double hdgErrAbsDeg  = std::fabs(hdgErrDeg);

      //-------------------------------------------------------
      // get absolute heading rate of change (hdgDotAbsDps)
      //-------------------------------------------------------
      double hdgDotMaxAbsRps = base::ETHGM * std::tan(MAX_BANK_RAD) / velMps;
      double hdgDotMaxAbsDps = hdgDotMaxAbsRps * base::Angle::R2DCC;

      double hdgDotAbsDps = hDps;
      if (hdgDotAbsDps > hdgDotMaxAbsDps) {
         hdgDotAbsDps = hdgDotMaxAbsDps;
      }

      double hdgErrBrkAbsDeg = TAU * hdgDotAbsDps;
      if (hdgErrAbsDeg < hdgErrBrkAbsDeg) {
         hdgDotAbsDps = hdgErrAbsDeg / TAU;
      }

      //-------------------------------------------------------
      // define direction of heading rate of change (hdgDotDps)
      //-------------------------------------------------------
      double hdgDotDps = base::sign(hdgErrDeg) * hdgDotAbsDps;
      psiDot = hdgDotDps * base::Angle::D2RCC;

      //-------------------------------------------------------
      // define bank angle as a function of turn rate
      //-------------------------------------------------------
      double phiCmdDeg = std::atan2(psiDot * velMps, base::ETHGM) * base::Angle::R2DCC;
      ok = flyPhi(phiCmdDeg);
   }

   return ok;
}

// Dynamics model interface - all input values in meters
bool LaeroModel::setCommandedAltitude(const double a, const double aMps, const double maxPitch)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   bool ok = (pPlr != nullptr);
   if (ok) {

      //-------------------------------------------------------
      // define local constants
      //-------------------------------------------------------
      const double TAU            = 4.0;  // time constant [sec]

      //-------------------------------------------------------
      // get current alt error (mtr)
      //-------------------------------------------------------
      double altMtr     = pPlr->getAltitude();
      double altErrMtr = a - altMtr;

      //-------------------------------------------------------
      // get alt error break point (mtr)
      //-------------------------------------------------------
      double altDotCmdMps = aMps;
      double altErrBrkMtr = altDotCmdMps * TAU;

      //-------------------------------------------------------
      // get commanded altDot (mps)
      //-------------------------------------------------------
      double altDotMps = base::sign(altErrMtr) * altDotCmdMps;
      if (std::abs(altErrMtr) < altErrBrkMtr) {
         altDotMps = altErrMtr * (altDotCmdMps / altErrBrkMtr);
      }

      //-------------------------------------------------------
      // assign result to altitude control
      //-------------------------------------------------------
      double thtCmdDeg = (altDotMps / u) * base::Angle::R2DCC;
      // SLS - TO DO: Limit commanded pitch to max pitch angle as well.
      ok = flyTht(thtCmdDeg);
   }

   return ok;
}

// setCommandedVelocityKts() - also can limit velocity rate of acceleration
bool LaeroModel::setCommandedVelocityKts(const double v, const double vNps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok = (pPlr != nullptr);
   if (ok) {

      //-------------------------------------------------------
      // define local constants
      //-------------------------------------------------------
      const double KTS2MPS = base::Distance::NM2M / base::Time::H2S;

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double velCmdMps     = v * KTS2MPS;
      double velDotCmdMps2 = vNps * KTS2MPS;

      //-------------------------------------------------------
      // current vel error (rad)
      //-------------------------------------------------------
      double velMps    = pPlr->getTotalVelocityKts() * KTS2MPS;
      double velErrMps = velCmdMps - velMps;

      //-------------------------------------------------------
      // vel error break point (rad)
      //-------------------------------------------------------
      const double TAU = 1.0;  // time constant [sec]
      double velErrBrkMps = velDotCmdMps2 * TAU;

      //-------------------------------------------------------
      // control signal for commanded vel (rps)
      //-------------------------------------------------------
      double velDotMps2 = base::sign(velErrMps) * velDotCmdMps2;
      if (std::abs(velErrMps) < velErrBrkMps) {
         velDotMps2 = (velErrMps / velErrBrkMps) * velDotCmdMps2;
      }

      //-------------------------------------------------------
      // assign result to velocity control
      //-------------------------------------------------------
      uDot = velDotMps2;
   }

   return true;
}

}
}
