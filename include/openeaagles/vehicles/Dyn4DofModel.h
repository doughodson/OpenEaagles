//------------------------------------------------------------------------------
// Class: Simulation::Aeordynamiceodel -> Dyn4DofModel
// Description:
//    Small, simple, reconfigurable 4 degree of freedom aerodynamic model.  
//    Special thanks to Larry Buckner for making this happen!
//------------------------------------------------------------------------------
#ifndef __Eaagles_Vehicle_Dyn4DofModel_H__
#define __Eaagles_Vehicle_Dyn4DofModel_H__

#include "openeaagles/simulation/DynamicsModels.h"

namespace Eaagles {
namespace Vehicle {

class Dyn4DofModel : public Simulation::AerodynamicsModel
{
   DECLARE_SUBCLASS(Dyn4DofModel, Simulation::AerodynamicsModel )

private:
   static const double HALF_PI;
   static const double EPSILON;
   //static const double AOG_FPS2;
   //static const double AOG_MPS2;
   //static const double STD_RATE_TURN_DPS;
   //static const double MAX_BANK_DEG;

   enum TurnDir { LEFT=0, RIGHT };
   //enum EntryMode { PREENTRY=0, SIMPLE, DIRECT, PARALLEL, TEARDROP, LOITER };  // external or member variables 

public: // public interface
   Dyn4DofModel();

   //EntryMode getLoiterMode();

   //-----------------------------------
   // fly functions
   //-----------------------------------
   bool flyPhi(const double phiCmdDeg, const double phiDotCmdDps=10.0);
   bool flyTht(const double thtCmdDeg, const double thtDotCmdDps=10.0);
   bool flyPsi(const double psiCmdDeg, const double psiDotCmdDps=3.0);
   bool flyVel(const double velCmdKts, const double velDotCmdNps=5.0);

   //bool flyTAS(const double tasCmdKts) { return flyVel(tasCmdKts); }
   bool flyTGS(const double tgsCmdKts) { return flyVel(tgsCmdKts); }
   bool flyALT(const double altCmdFt, const double altDotCmdFpm=2000.0);
   //bool flySRT(const TurnDir td=RIGHT);
   //bool flyHDG(const double hdgCmdDeg, const double hdgDotCmdDps=STD_RATE_TURN_DPS);
   bool flyCRS(const double latDeg, const double lonDeg, const double crsCmdDeg);
   //bool fly2LL(const double latDeg, const double lonDeg);

   //bool flyLoiter();
   //bool flyEntry(double aLat, double aLon);

   //-----------------------------------
   // get functions
   //-----------------------------------
   //double getCmdVel()     const  { return cmdVel; }
   //double getAnchorLat()  const  { return anchorLat; }
   //double getAnchorLon()  const  { return anchorLon; }
   //double getInboundCrs() const  { return inboundCrs; }
   //double getMirrorLat()  const  { return mirrorLat; }
   //double getMirrorLon()  const  { return mirrorLon; }

   //bool calcMirrorLatLon(const double aLat, const double aLon, const double ibCrs, const bool st=true);
   //bool calcMirrorLatLon();

   //-----------------------------------
   // set functions
   //-----------------------------------
   void setCmdVel(const double vel)       { cmdVel     = vel; }
   //void setInboundCourse(double ibCrsDeg) { inboundCrs = ibCrsDeg; }
   //void setAnchorLat(const double latDeg) { anchorLat  = latDeg; }
   //void setAnchorLon(const double lonDeg) { anchorLon  = lonDeg; }
   //void setMirrorLat(const double latDeg) { mirrorLat  = latDeg; }
   //void setMirrorLon(const double lonDeg) { mirrorLon  = lonDeg; }
   //void setTurnDir(const TurnDir td)      { turnDir    = td; }
   void setAcftSpeed(const double tas)    { acftSpeed  = tas; }
   //void setAnchorWP(const double latDeg, const double lonDeg) { anchorLat = latDeg; anchorLon = lonDeg; }
   //void setMirrorWP(const double latDeg, const double lonDeg) { mirrorLat = latDeg; mirrorLon = lonDeg; }

   virtual void dynamics(const LCreal dt);  // One pass model update; called from Player::dynamics()

   // Component interface
   //virtual void reset();

   // DynamicsModel interface
   //virtual bool setCommandedHeadingD(const double h, const double hdgDotCmdDps=DynamicsModel::STD_RATE_TURN_DPS);
   virtual bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0);
   virtual bool setCommandedAltitude(const double a, const double aMps = 0);
   virtual bool setCommandedVelocityKts(const double a);

private:
   void initData();
   void update4DofModel(const LCreal dt);
   
   double dT;

   //TurnDir        turnDir;
   //EntryMode      entryMode;
   //unsigned int   entryPhase;   
   //bool           isInbound;
   //double         inboundCrs;
   //double         anchorLat;
   //double         anchorLon;
   //double         mirrorLat;
   //double         mirrorLon;
   double         acftSpeed;

   // Body angular vel, acc components
   double   p;
   double   q;
   double   r;
   double   pDot;
   double   qDot;
   double   rDot; 

   // Euler rotation angle, rate components
   double   phi;
   double   tht;
   double   psi; 
   double   phiDot;
   double   thtDot;
   double   psiDot;

   // Body linear vel, acc components
   double   u;
   double   v;
   double   w;
   double   uDot;
   double   vDot;
   double   wDot;

   // NED pos, vel, acc components
   double   refPosN;
   double   refPosE;
   double   refPosD;
   double   posN;
   double   posE;
   double   posD;

   double   velN;
   double   velE;
   double   velD;
   double   accN;
   double   accE;
   double   accD;

   // Hold components
   double   phiDot1;
   double   thtDot1;
   double   psiDot1;
   double   uDot1;
   double   vDot1;
   double   wDot1;

   // Commanded parameters
   double   cmdVel;
};

} 
} 

#endif 
