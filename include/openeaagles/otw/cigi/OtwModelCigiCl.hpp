
#ifndef __oe_otw_OtwModelCigiCl_H__
#define __oe_otw_OtwModelCigiCl_H__

#include "openeaagles/otw/OtwModel.hpp"

#include "openeaagles/otw/cigi/OtwCigiCl.hpp"

class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiArtPartCtrlV3;

//class CigiIGCtrlV3;
//class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet
//class CigiViewCtrlV3;
//class CigiViewDefV3;
//class CigiHatHotReqV3;
//class CigiSOFV3;
//class CigiHatHotRespV3;
//class CigiLosRespV3;
//class CigiCollDetSegRespV3;
//class CigiCollDetVolRespV3;
//class CigiSensorCtrlV3;
//class CigiSensorRespV3;
//class CigiIGMsgV3;       // CGBCGB CIGI_IG_RESPONSE_MESSAGE
//class CigiHostSession;
//class CigiIncomingMsg;
//class CigiOutgoingMsg;

namespace oe {
namespace models { class Player; }
namespace otw {

//------------------------------------------------------------------------------
// Class: OtwModelCigiCl
// Description: CIGI OTW model
//------------------------------------------------------------------------------
class OtwModelCigiCl : public OtwModel
{
   DECLARE_SUBCLASS(OtwModelCigiCl, OtwModel)

public:
   OtwModelCigiCl();

   virtual void clear() override;

   virtual void initialize(models::Player* const p);

   unsigned short getID() const          { return id; }
   void setID(const unsigned short i)    { id = i; }

   // private: or at least should have been!
   CigiEntityCtrlV3* parentEC[OtwCigiCl::NUM_BUFFERS];      // (entity_id)   Our main entity
   CigiEntityCtrlV3* trailEC[OtwCigiCl::NUM_BUFFERS];       // (entity_id+1) Trails (missile, smoke, wake, etc.)
   CigiEntityCtrlV3* explosionEC[OtwCigiCl::NUM_BUFFERS];   // (entity_id+2) Expolsions (air or ground)
   CigiEntityCtrlV3* smokeEC[OtwCigiCl::NUM_BUFFERS];       // (entity_id+3) Smoke from damage
   CigiCompCtrlV3* damageCC[OtwCigiCl::NUM_BUFFERS];        // Damage Component Control (CC)
   CigiCompCtrlV3* animationCC[OtwCigiCl::NUM_BUFFERS];     // animation component control
   CigiEntityCtrlV3* attachedEC[OtwCigiCl::NUM_BUFFERS];    // (entity_id+4) Attached missile
   CigiArtPartCtrlV3* launcherAPC[OtwCigiCl::NUM_BUFFERS];  // Entity's launcher Articulated Part Control (APC)
   CigiCompCtrlV3* attachedCC[OtwCigiCl::NUM_BUFFERS];      // attached missile component control

   bool parentActive;
   bool trailActive;
   bool explosionActive;
   bool smokeActive;
   bool damageActive;
   bool animationActive;
   bool attachedEcActive;
   bool launcherApcActive;
   bool attachedCcActive;

   bool isGroundPlayer;
   double effectsTimer;

private:
   // entity ID
   unsigned short id;
};

}
}

#endif
