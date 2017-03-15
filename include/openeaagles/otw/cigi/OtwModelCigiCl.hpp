
#ifndef __oe_otw_OtwModelCigiCl_H__
#define __oe_otw_OtwModelCigiCl_H__

#include "openeaagles/otw/OtwModel.hpp"

#include "openeaagles/otw/cigi/OtwCigiCl.hpp"
#include <array>

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
   std::array<CigiEntityCtrlV3*, OtwCigiCl::NUM_BUFFERS> parentEC {};      // (entity_id)   Our main entity
   std::array<CigiEntityCtrlV3*, OtwCigiCl::NUM_BUFFERS> trailEC {};       // (entity_id+1) Trails (missile, smoke, wake, etc.)
   std::array<CigiEntityCtrlV3*, OtwCigiCl::NUM_BUFFERS> explosionEC {};   // (entity_id+2) Expolsions (air or ground)
   std::array<CigiEntityCtrlV3*, OtwCigiCl::NUM_BUFFERS> smokeEC {};       // (entity_id+3) Smoke from damage
   std::array<CigiCompCtrlV3*, OtwCigiCl::NUM_BUFFERS> damageCC {};        // Damage Component Control (CC)
   std::array<CigiCompCtrlV3*, OtwCigiCl::NUM_BUFFERS> animationCC {};     // animation component control
   std::array<CigiEntityCtrlV3*, OtwCigiCl::NUM_BUFFERS> attachedEC {};    // (entity_id+4) Attached missile
   std::array<CigiArtPartCtrlV3*, OtwCigiCl::NUM_BUFFERS> launcherAPC {};  // Entity's launcher Articulated Part Control (APC)
   std::array<CigiCompCtrlV3*, OtwCigiCl::NUM_BUFFERS> attachedCC {};      // attached missile component control

   bool parentActive {};
   bool trailActive {};
   bool explosionActive {};
   bool smokeActive {};
   bool damageActive {};
   bool animationActive {};
   bool attachedEcActive {};
   bool launcherApcActive {};
   bool attachedCcActive {};

   bool isGroundPlayer {};
   double effectsTimer {};

private:
   void initData();

   // entity ID
   unsigned short id {};
};

}
}

#endif
