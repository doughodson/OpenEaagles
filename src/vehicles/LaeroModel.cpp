//==============================================================================
// File: LaeroModel.cpp
//==============================================================================
#include "openeaagles/vehicles/LaeroModel.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/AirVehicle.h"

#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Quat"
#include "openeaagles/basic/osg/Math"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>

namespace Eaagles {
namespace Vehicle {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LaeroModel,"LaeroModel")
EMPTY_SERIALIZER(LaeroModel)
EMPTY_DELETEDATA(LaeroModel)

//----------------------------------------------------------
// define conversion constants 
//----------------------------------------------------------
const double LaeroModel::HALF_PI             = PI / 2.0;
const double LaeroModel::EPSILON             = 1.0E-10;
//const double LaeroModel::Eaagles::ETHG            = Eaagles::ETHG;
//const double LaeroModel::Eaagles::ETHGM            = Eaagles::ETHG * 0.3048;
//const double LaeroModel::STD_RATE_TURN_DPS   = 3.0;
//const double LaeroModel::MAX_BANK_DEG        = 30.0;

//----------------------------------------------------------
// Constructor(s)
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
   // Loiter variables
   //turnDir    = RIGHT;
   //entryMode  = PREENTRY;
   //entryPhase = 0;
   //isInbound  = false;
   //inboundCrs = 0.0;
   //anchorLat  = 0.1;
   //anchorLon  = 0.1;
   //mirrorLat  = 0.0;
   //mirrorLon  = 0.0;
   acftSpeed  = 0.0;

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
   u        = 200.0 * Basic::Distance::NM2M / Basic::Time::H2S;
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

   // Commanded parameters
   cmdVel   = 0.0;
}

//----------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//----------------------------------------------------------
void LaeroModel::copyData(const LaeroModel& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Loiter variables
   //turnDir    = org.turnDir;
   //entryMode  = org.entryMode;
   //entryPhase = org.entryPhase;
   //isInbound  = org.isInbound;
   //inboundCrs = org.inboundCrs;
   //anchorLat  = org.anchorLat;
   //anchorLon  = org.anchorLon;
   //mirrorLat  = org.mirrorLat;
   //mirrorLon  = org.mirrorLon;
   acftSpeed  = org.acftSpeed;

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

   // Commanded parameters
   cmdVel   = org.cmdVel;
}


//----------------------------------------------------------
// updateTC() -- update time critical stuff here
//----------------------------------------------------------
void LaeroModel::dynamics(const LCreal dt)
{
    update4DofModel(dt);  
    dT = dt;
}

//----------------------------------------------------------
// updateTestRAC -- update robot aircraft equations of motion
//----------------------------------------------------------
void LaeroModel::update4DofModel(const LCreal dt)
{   
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   
   if (pPlr != 0) {

      //==============================================================
      // ROTATIONAL EOM
      //==============================================================

      //----------------------------------------------------
      // integrate Euler angles using Adams-Bashforth
      //----------------------------------------------------
      phi += 0.5 * (3.0 * phiDot - phiDot1) * dT;
      if (phi >  PI) phi = -PI;
      if (phi < -PI) phi =  PI;
      
      tht += 0.5 * (3.0 * thtDot - thtDot1) * dT;
      if (tht >=  HALF_PI) tht =  (HALF_PI - EPSILON);
      if (tht <= -HALF_PI) tht = -(HALF_PI - EPSILON);

      psi += 0.5 * (3.0 * psiDot - psiDot1) * dT;
      if (psi >  PI) psi = -PI;
      if (psi < -PI) psi =  PI;

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
      // update acceleraton in NED system
      //----------------------------------------------------
      //double A = uDot + w*q - v*r;
      //double B = vDot + u*r - w*p;
      //double C = wDot + v*p - u*q;

      //accN  = l1*A + m1*B + n1*C;
      //accE  = l2*A + m2*B + n2*C;
      //accD  = l3*A + m3*B + n3*C;

      //pPlr->setAcceleration(accN, accE, accD);
      
      //==============================================================
      // CONTROL INPUTS
      //==============================================================
      //cmdVel = pPlr->getCommandedVelocityKts();
      //cmdAlt = pPlr->getCommandedAltitudeFt();
      //cmdHdg = pPlr->getCommandedHeadingD();

      //cmdVel = 200.0;
      //cmdAlt = 9000.0;
      //cmdHdg = 330.0;
      //
      //flyVel(cmdVel);
      //flyALT(cmdAlt);
      //flyHDG(cmdHdg);

      //flyPsi(45.0);
      //(45.0);
      //flyPsi(45.0);

      //flyEntry();

      //setTurnDir(LEFT);
      //flyLoiter();
      //flyLoiter(0.01, 0.02, 320.0);

   }
}

//==============================================================================
// FUNCTIONS
//==============================================================================

bool LaeroModel::flyPhi(const double phiCmdDeg, const double phiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   bool ok = (pPlr != 0);
   if (ok) {
   
      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------   
      double phiCmdRad    = phiCmdDeg * Basic::Angle::D2RCC;
      double phiDotCmdRps = phiDotCmdDps * Basic::Angle::D2RCC;

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
      double phiDotRps = sign(phiErrRad) * phiDotCmdRps;
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

//==============================================================================
bool LaeroModel::flyTht(const double thtCmdDeg, const double thtDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   bool ok = (pPlr != 0);
   if (ok) {
   
      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------   
      double thtCmdRad    = thtCmdDeg * Basic::Angle::D2RCC;
      double thtDotCmdRps = thtDotCmdDps * Basic::Angle::D2RCC;

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
      double thtDotRps = sign(thtErrRad) * thtDotCmdRps;
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

//==============================================================================
bool LaeroModel::flyPsi(const double psiCmdDeg, const double psiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   bool ok = (pPlr != 0);
   if (ok) {
   
      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------   
      double psiCmdRad    = psiCmdDeg * Basic::Angle::D2RCC;
      double psiDotCmdRps = psiDotCmdDps * Basic::Angle::D2RCC;

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
      double psiDotRps = sign(psiErrRad) * psiDotCmdRps;
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

//==============================================================================
bool LaeroModel::flyVel(const double velCmdKts, const double velDotCmdNps)
{
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   bool ok = (pPlr != 0);
   if (ok) {

      //-------------------------------------------------------
      // define local constants 
      //-------------------------------------------------------
      const double KTS2MPS = Basic::Distance::NM2M / Basic::Time::H2S;

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------   
      double velCmdMps     = velCmdKts * KTS2MPS;
      double velDotCmdMps2 = velDotCmdNps * KTS2MPS;

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
      double velDotMps2 = sign(velErrMps) * velDotCmdMps2;
      if (std::abs(velErrMps) < velErrBrkMps) {
         velDotMps2 = (velErrMps / velErrBrkMps) * velDotCmdMps2;
      }

      //-------------------------------------------------------
      // assign result to velocity control
      //-------------------------------------------------------
      uDot = velDotMps2;
   }

   return ok;
}

//==============================================================================
//bool LaeroModel::flyALT(const double altCmdFt, const double altDotCmdFpm)
//{
   ////-------------------------------------------------------
   //// get data pointers 
   ////-------------------------------------------------------
   //Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   //bool ok = (pPlr != 0);
   //if (ok) {
   //
   //   //-------------------------------------------------------
   //   // define local constants 
   //   //-------------------------------------------------------
   //   const double ALTDOT_FPM     = altDotCmdFpm;
   //   const double ALTDOT_MPS     = ALTDOT_FPM * (Basic::Distance::FT2M / Basic::Time::M2S);
   //   const double ALTDOT_BRK_MPS = ALTDOT_MPS;
   //   const double ALTERR_BRK_FT  = 100.0;
   //   const double ALTERR_BRK_MTR = ALTERR_BRK_FT * Basic::Distance::FT2M;
   //   const double TAU            = 4.0;  // time constant [sec]

   //   //-------------------------------------------------------
   //   // get current alt error (mtr)
   //   //-------------------------------------------------------
   //   double altFt     = pPlr->getAltitudeFt();
   //   double altErrFt  = altCmdFt - altFt;
   //   double altErrMtr = altErrFt * Basic::Distance::FT2M;

   //   //-------------------------------------------------------
   //   // get alt error break point (mtr)
   //   //-------------------------------------------------------   
   //   double altDotCmdMps = altDotCmdFpm * Basic::Distance::FT2M / Basic::Time::M2S;
   //   double altErrBrkMtr = altDotCmdMps * TAU;

   //   //-------------------------------------------------------
   //   // get commanded altDot (mps) 
   //   //-------------------------------------------------------
   //   double altDotMps = sign(altErrMtr) * altDotCmdMps;
   //   if (std::abs(altErrMtr) < altErrBrkMtr) {
   //      altDotMps = altErrMtr * (altDotCmdMps / altErrBrkMtr);
   //   }

   //   //-------------------------------------------------------
   //   // assign result to altitude control
   //   //-------------------------------------------------------
   //   double thtCmdDeg = (altDotMps / u) * Basic::Angle::R2DCC;
   //   ok = flyTht(thtCmdDeg);
   //}

   //return ok;
//}

//==============================================================================
//bool LaeroModel::flySRT(const TurnDir td)
//{
//   //-------------------------------------------------------
//   // get data pointers 
//   //-------------------------------------------------------
//   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
//
//   bool ok = (pPlr != 0);
//   if (ok) {
//   
//      //----------------------------------------------------
//      // define local constants 
//      //----------------------------------------------------
//      const double MAX_BANK_RAD  = MAX_BANK_DEG * Basic::Angle::D2RCC;
//      const double SRT_RPS       = STD_RATE_TURN_DPS * Basic::Angle::D2RCC;
//
//      //----------------------------------------------------
//      // get current data
//      //----------------------------------------------------
//      double velMps    = pPlr->getTotalVelocity();   
//      double phiCmdRad = std::atan2(velMps*SRT_RPS, Eaagles::ETHGM);
//      if (phiCmdRad > MAX_BANK_RAD) {
//         phiCmdRad = MAX_BANK_RAD;
//      }
//
//      double psiDotCmdRps = Eaagles::ETHGM * std::tan(phiCmdRad) / velMps;
//
//      if (td == LEFT) {
//         phiCmdRad    = -phiCmdRad;
//         psiDotCmdRps = -psiDotCmdRps;
//      }
//
//      //-------------------------------------------------------
//      // assign result to altitude control
//      //-------------------------------------------------------
//      double phiCmdDeg = phiCmdRad * Basic::Angle::R2DCC;
//      ok = flyPhi(phiCmdDeg);
//      
//      psiDot = psiDotCmdRps;
//   }
//   
//   return ok;
//}

////==============================================================================
//bool LaeroModel::flyHDG(const double hdgCmdDeg, const double hdgDotCmdDps)
//{
//   //-------------------------------------------------------
//   // get data pointers 
//   //-------------------------------------------------------
//   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
//
//   bool ok = (pPlr != 0);
//   if (ok) {
//   
//      //----------------------------------------------------
//      // define local constants 
//      //----------------------------------------------------
//      const double MAX_BANK_RAD = MAX_BANK_DEG * Basic::Angle::D2RCC;
//      //const double TAU = 2.0;  // time constant [sec]
//      const double TAU = 1.0;  // time constant [sec]
//
//      //-------------------------------------------------------
//      // get current data
//      //-------------------------------------------------------   
//      double velMps        = pPlr->getTotalVelocity();
//      double hdgDeg        = pPlr->getHeadingD();
//      double hdgErrDeg     = Basic::Angle::aepcdDeg(hdgCmdDeg - hdgDeg);
//      double hdgErrAbsDeg  = std::fabs(hdgErrDeg);
//
//      //-------------------------------------------------------
//      // get absolute heading rate of change (hdgDotAbsDps) 
//      //-------------------------------------------------------
//      double hdgDotMaxAbsRps = Eaagles::ETHGM * std::tan(MAX_BANK_RAD) / velMps;
//      double hdgDotMaxAbsDps = hdgDotMaxAbsRps * Basic::Angle::R2DCC;
//
//      double hdgDotAbsDps = hdgDotCmdDps;
//      if (hdgDotAbsDps > hdgDotMaxAbsDps) {
//         hdgDotAbsDps = hdgDotMaxAbsDps;
//      }
//
//      double hdgErrBrkAbsDeg = TAU * hdgDotAbsDps;
//      if (hdgErrAbsDeg < hdgErrBrkAbsDeg) {
//         hdgDotAbsDps = hdgErrAbsDeg / TAU;
//      }
//
//      //-------------------------------------------------------
//      // define direction of heading rate of change (hsdDotDps)
//      //-------------------------------------------------------
//      double hdgDotDps = sign(hdgErrDeg) * hdgDotAbsDps;
//      psiDot = hdgDotDps * Basic::Angle::D2RCC;
//      //flyPsi(hdgCmdDeg, hdgDotDps);
//
//      //-------------------------------------------------------
//      // define bank angle as a function of turn rate
//      //-------------------------------------------------------
//      double phiCmdDeg = std::atan2(psiDot * velMps, Eaagles::ETHGM) * Basic::Angle::R2DCC;
//      ok = flyPhi(phiCmdDeg);
//   }
//
//   return ok;
//}

//==============================================================================
//bool LaeroModel::flyCRS(const double latDeg, const double lonDeg, const double crsDeg)
//{
   ////-------------------------------------------------------
   //// get data pointers 
   ////-------------------------------------------------------
   //Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   //bool ok = (pPlr != 0);
   //if (ok) {
   //
   //   //----------------------------------------------------
   //   // define local constants 
   //   //----------------------------------------------------
   //   const double MAX_BANK_RAD = MAX_BANK_DEG * Basic::Angle::D2RCC;
   //   const double OFFSET_MTR   = 20.0;

   //   //----------------------------------------------------
   //   // get current data
   //   //----------------------------------------------------
   //   double velMps = pPlr->getTotalVelocity();
   //   double hdgDeg = pPlr->getHeadingD();
   //   double osLat  = pPlr->getLatitude();
   //   double osLon  = pPlr->getLongitude();
   //   double brgDeg = 0.0;
   //   double distNM = 0.0;
   //   Basic::Nav::fll2bd(osLat, osLon, latDeg, lonDeg, &brgDeg, &distNM);

   //   //-------------------------------------------------------
   //   // get current gama error (deg)
   //   //-------------------------------------------------------   
   //   double posErrDeg = Basic::Angle::aepcdDeg(brgDeg - crsDeg);
   //   double posErrRad = posErrDeg * Basic::Angle::D2RCC;

   //   double rocMtr    = velMps * velMps / Eaagles::ETHGM / std::tan(MAX_BANK_RAD);
   //   double rocNM     = rocMtr * Basic::Distance::M2NM;

   //   double xtRngNM   = std::fabs(distNM * std::sin(posErrRad));
   //   double xtRngMtr  = xtRngNM * Basic::Distance::NM2M;
   //   double xtRngRoc  = xtRngMtr / rocMtr;

   //   double hdgCmdDeg = hdgDeg;
   //   if (xtRngRoc >= 1.2) {
   //      hdgCmdDeg = sign(posErrDeg) * 90.0 + crsDeg;
   //   }
   //   else {
   //      double x = 1.0 - xtRngRoc;
   //      if (x >  1.0) x =  1.0;
   //      if (x < -1.0) x = -1.0;
   //      double alfaDeg = std::acos(x) * Basic::Angle::R2DCC;

   //      double y = (rocMtr - OFFSET_MTR) / rocMtr;
   //      double betaDeg = std::acos(y) * Basic::Angle::R2DCC;

   //      double gamaDeg = sign(posErrDeg) * (alfaDeg - betaDeg);
   //      hdgCmdDeg = Basic::Angle::aepcdDeg(gamaDeg + crsDeg);
   //   }

   //   //-------------------------------------------------------
   //   // fly to heading necessary to follow commanded course
   //   //-------------------------------------------------------
   //   ok = flyHDG(hdgCmdDeg);
   //}

   //return ok;
//   return true;
//}

//==============================================================================
//bool LaeroModel::fly2LL(const double latDeg, const double lonDeg)
//{
//   //-------------------------------------------------------
//   // get data pointers 
//   //-------------------------------------------------------
//   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
//
//   bool ok = (pPlr != 0);
//   if (ok) {
//   
//      //----------------------------------------------------
//      // get current data
//      //----------------------------------------------------
//      double osLatDeg  = pPlr->getLatitude();
//      double osLonDeg  = pPlr->getLongitude();
//      double brgDeg = 0.0;
//      double rngMtr = 0.0;
//      Basic::Nav::gll2bd(osLatDeg, osLonDeg, latDeg, lonDeg, &brgDeg,&rngMtr);
//
//      //-------------------------------------------------------
//      // fly to heading necessary to intercept lat/lon
//      //-------------------------------------------------------
//      ok = flyHDG(brgDeg);
//   }
//
//   return ok;
//}

//==============================================================================
//bool LaeroModel::flyLoiter()
//{
   ////-------------------------------------------------------
   //// get data pointers 
   ////-------------------------------------------------------
   //Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   //bool ok = (pPlr != 0);
   //if (ok) {
   //
   //   // get current data
   //   double osLatDeg = pPlr->getLatitude();
   //   double osLonDeg = pPlr->getLongitude();
   //   double obCrs    = Basic::Angle::aepcdDeg(inboundCrs + 180.0);
   //   calcMirrorLatLon();

   //   double brgDeg = 0.0;
   //   double distNM = 0.0;
   //   if (isInbound) {
   //      ok = Basic::Nav::fll2bd(osLatDeg, osLonDeg, anchorLat, anchorLon, &brgDeg, &distNM);
   //   }
   //   else {
   //      ok = Basic::Nav::fll2bd(osLatDeg, osLonDeg, mirrorLat, mirrorLon, &brgDeg, &distNM);
   //   }

   //   // get position error to determine inbound or outbound
   //   double posErrDeg = 0.0;
   //   if (isInbound) {
   //      std::cout << "Inbound" << std::endl;
   //      ok = flyCRS(anchorLat, anchorLon, inboundCrs);
   //      posErrDeg = Basic::Angle::aepcdDeg(brgDeg - inboundCrs);
   //      if (std::fabs(posErrDeg) > 90.0) { isInbound = false; }
   //   }
   //   else {
   //      std::cout << "Outbound" << std::endl;
   //      ok = flyCRS(mirrorLat, mirrorLon, obCrs);
   //      posErrDeg = Basic::Angle::aepcdDeg(brgDeg - obCrs);
   //      if (std::fabs(posErrDeg) > 90.0) { isInbound = true; }
   //   }
   //}

   //return ok;
//   return true;
//}

//==============================================================================
//bool LaeroModel::flyEntry(double aLat, double aLon)
//{
   ////-------------------------------------------------------
   //// get data pointers 
   ////-------------------------------------------------------
   //Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   //bool ok = (pPlr != 0);
   //if (ok) {
   //
   //   //----------------------------------------------------
   //   // define local constants 
   //   //----------------------------------------------------
   //   const double MAX_BANK_RAD = MAX_BANK_DEG * Basic::Angle::D2RCC;
   //   const double OFFSET_MTR   = 20.0;

   //   //----------------------------------------------------
   //   // get current data
   //   //----------------------------------------------------
   //   double velMps    = pPlr->getTotalVelocity();
   //   double hdgDeg    = pPlr->getHeadingD();
   //   double rocMtr    = velMps * velMps / Eaagles::ETHGM / std::tan(MAX_BANK_RAD);
   //   double rocNM     = rocMtr * Basic::Distance::M2NM;
   //   double obCrsDeg  = Basic::Angle::aepcdDeg(inboundCrs + 180.0);

   //   double osLatDeg  = pPlr->getLatitude();
   //   double osLonDeg  = pPlr->getLongitude();

   //   double brgDeg    = 0.0;
   //   double distNM    = 0.0;
   //   Basic::Nav::fll2bd(osLatDeg, osLonDeg, anchorLat, anchorLon, &brgDeg, &distNM);

   //   double hdgErrDeg = Basic::Angle::aepcdDeg(hdgDeg - inboundCrs);
   //   double posErrDeg = Basic::Angle::aepcdDeg(brgDeg - inboundCrs);
   //   double distCmdNM = 0;
   //   double hdgCmdDeg = 0;

   //   switch (entryMode) {
   //      //-----------------------------------------------------------
   //      case PREENTRY:
   //      {
   //         std::cout << "PREENTRY" << std::endl;
   //         if (entryPhase == 0) {
   //            anchorLat = aLat;
   //            anchorLon = aLon;
   //            ok = fly2LL(anchorLat, anchorLon);
   //            if (distNM < 0.1) {
   //               entryPhase = 1;
   //               if (turnDir == RIGHT) {
   //                  if      (hdgErrDeg < -70.0)  { entryMode = PARALLEL; }
   //                  else if (hdgErrDeg < 110.0)  { entryMode = DIRECT; }
   //                  else                         { entryMode = TEARDROP; }
   //               }
   //               else {
   //                  if      (hdgErrDeg < -110.0) { entryMode = TEARDROP; }
   //                  else if (hdgErrDeg <  70.0)  { entryMode = DIRECT; }
   //                  else                         { entryMode = PARALLEL; }
   //               }
   //            }
   //         }
   //      }
   //      break;  // end case PREENTRY

   //      //-----------------------------------------------------------
   //      case DIRECT:
   //      {
   //         std::cout << "DIRECT" << std::endl;
   //         if (entryPhase == 1) {
   //            ok = flySRT(turnDir);
   //            if (std::fabs(hdgErrDeg) > 90.0) {
   //               entryMode = LOITER;
   //               entryPhase = 0;
   //            }
   //         }
   //      }
   //      break;  // end case DIRECT

   //      //-----------------------------------------------------------
   //      case PARALLEL:
   //      {
   //         std::cout << "PARALLEL" << std::endl;
   //         if (entryPhase == 1) {
   //            ok = flyHDG(obCrsDeg);
   //            //distCmdNM = velMps * 60.0 * Basic::Distance::M2NM;  // 1 min = 60 sec
   //            distCmdNM = 4.0 * rocNM;  // 1/tan(15 deg) = 0.268 = 3.732 ~= 4.0
   //            if (distNM > distCmdNM) { entryPhase = 2; }
   //         }
   //         else if (entryPhase == 2) {
   //            if (turnDir == RIGHT) { ok = flySRT(LEFT); }
   //            else                  { ok = flySRT(RIGHT); }
   //            if (std::fabs(hdgErrDeg) < 90.0) { 
   //               entryPhase = 3;
   //            }
   //         }
   //         else if (entryPhase == 3) {
   //            ok = flyCRS(anchorLat, anchorLon, inboundCrs);
   //            if (distNM < 0.1) {
   //               entryMode = LOITER;
   //               entryPhase = 0;
   //            }
   //         }
   //      }
   //      break;  // end case PARALLEL

   //      //-----------------------------------------------------------
   //      case TEARDROP:
   //      {
   //         std::cout << "TEARDROP" << std::endl;
   //         if (entryPhase == 1) {
   //            if (turnDir == RIGHT) { hdgCmdDeg = Basic::Angle::aepcdDeg(obCrsDeg - 30.0); }
   //            else                  { hdgCmdDeg = Basic::Angle::aepcdDeg(obCrsDeg + 30.0); }
   //            flyHDG(hdgCmdDeg);
   //            //distCmdNM = velMps * 60.0 * Basic::Distance::M2NM;  // 1 min = 60 sec
   //            distCmdNM = 4.0 * rocNM;  // 1/tan(15 deg) = 0.268 = 3.732 ~= 4.0
   //            if (distNM > distCmdNM) { entryPhase = 2; }
   //         }
   //         else if (entryPhase == 2) {
   //            flySRT(turnDir); 
   //            if (std::fabs(hdgErrDeg) < 90.0) {
   //               entryPhase = 3;
   //            }
   //         }
   //         else if (entryPhase == 3) {
   //            flyCRS(anchorLat, anchorLon, inboundCrs);
   //            if (distNM < 0.1) {
   //               entryMode = LOITER;
   //               entryPhase = 0;
   //            }
   //         }
   //      }
   //      break;  // end case TEARDROP

   //      //-----------------------------------------------------------
   //      case LOITER:
   //      {
   //         std::cout << "LOITER" << std::endl;
   //         flyLoiter();
   //      }
   //      break;  // end case LOITER

   //      //-----------------------------------------------------------
   //      default:
   //         break;
   //   }  // end switch
   //}

   //return ok;
//   return true;
//}


//==============================================================================
//bool LaeroModel::calcMirrorLatLon()
//{
   ////-------------------------------------------------------
   //// get data pointers 
   ////-------------------------------------------------------
   //Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   //bool ok = (pPlr != 0);
   //if (ok) {
   //
   //   //----------------------------------------------------
   //   // define local constants
   //   //----------------------------------------------------
   //   const double MAX_BANK_RAD  = MAX_BANK_DEG * Basic::Angle::D2RCC;
   //   const double SRT_RPS       = STD_RATE_TURN_DPS * Basic::Angle::D2RCC;

   //   //----------------------------------------------------
   //   // get current data
   //   //----------------------------------------------------
   //   double osLatDeg  = pPlr->getLatitude();
   //   double osLonDeg  = pPlr->getLongitude();
   //   double velMps    = pPlr->getTotalVelocity();
   //   double phiCmdRad = std::atan2(velMps * SRT_RPS, Eaagles::ETHGM);
   //   if (phiCmdRad > MAX_BANK_RAD) { phiCmdRad = MAX_BANK_RAD; }

   //   double rocMtr    = velMps * velMps / (Eaagles::ETHGM * std::tan(phiCmdRad));
   //   double rocNM     = rocMtr * Basic::Distance::M2NM;
   //   double xtDistNM  = 2.0 * rocNM;
   //   double obTimeSec = 2.0 * Basic::Time::M2S;
   //   double obDistNM  = velMps * obTimeSec * Basic::Distance::M2NM;
   //   double altFt     = pPlr->getAltitudeFt();
   //   if (altFt > 14000.0) { obDistNM *= 1.5; }

   //   double distNM    = std::sqrt(xtDistNM*xtDistNM + obDistNM*obDistNM);
   //   double obCrsDeg  = Basic::Angle::aepcdDeg(inboundCrs + 180.0);
   //   double brgDeg    = std::atan2(xtDistNM, obDistNM) * Basic::Angle::R2DCC;

   //   if (turnDir == RIGHT) { brgDeg = Basic::Angle::aepcdDeg(obCrsDeg - brgDeg); }
   //   else                  { brgDeg = Basic::Angle::aepcdDeg(obCrsDeg + brgDeg); }
   //   
   //   ok = Basic::Nav::fbd2ll(anchorLat, anchorLon, brgDeg, distNM, &mirrorLat, &mirrorLon);
   //}

   //return ok;
//   return true;
//}

// Dynamics model interface
bool LaeroModel::setCommandedHeadingD(const double h, const double hDps, const double maxBank)
{ 
   //-------------------------------------------------------
   // get data pointers 
   //-------------------------------------------------------
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   bool ok = (pPlr != 0);
   if (ok) {
   
      //----------------------------------------------------
      // define local constants 
      //----------------------------------------------------
      const double MAX_BANK_RAD = maxBank * Basic::Angle::D2RCC;
      //const double TAU = 2.0;  // time constant [sec]
      const double TAU = 1.0;  // time constant [sec]

      //-------------------------------------------------------
      // get current data
      //-------------------------------------------------------   
      double velMps        = pPlr->getTotalVelocity();
      double hdgDeg        = pPlr->getHeadingD();
      double hdgErrDeg     = Basic::Angle::aepcdDeg(h - hdgDeg);
      double hdgErrAbsDeg  = std::fabs(hdgErrDeg);

      //-------------------------------------------------------
      // get absolute heading rate of change (hdgDotAbsDps) 
      //-------------------------------------------------------
      double hdgDotMaxAbsRps = Eaagles::ETHGM * std::tan(MAX_BANK_RAD) / velMps;
      double hdgDotMaxAbsDps = hdgDotMaxAbsRps * Basic::Angle::R2DCC;

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
      double hdgDotDps = sign(hdgErrDeg) * hdgDotAbsDps;
      psiDot = hdgDotDps * Basic::Angle::D2RCC;

      //-------------------------------------------------------
      // define bank angle as a function of turn rate
      //-------------------------------------------------------
      double phiCmdDeg = std::atan2(psiDot * velMps, Eaagles::ETHGM) * Basic::Angle::R2DCC;
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
   Simulation::Player* pPlr = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );

   bool ok = (pPlr != 0);
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
      double altDotMps = sign(altErrMtr) * altDotCmdMps;
      if (std::abs(altErrMtr) < altErrBrkMtr) {
         altDotMps = altErrMtr * (altDotCmdMps / altErrBrkMtr);
      }

      //-------------------------------------------------------
      // assign result to altitude control
      //-------------------------------------------------------
      double thtCmdDeg = (altDotMps / u) * Basic::Angle::R2DCC;
      // SLS - TO DO: Limit commanded pitch to max pitch angle as well.
      ok = flyTht(thtCmdDeg);
   }

   return ok;
}
// SLS - TO DO: Limit velocity rate of acceleration
bool LaeroModel::setCommandedVelocityKts(const double a)
{
   flyVel(a);
   return true;
}


} 
} 
