
#ifndef __oe_otw_CigiCl_H__
#define __oe_otw_CigiCl_H__

#include "openeaagles/base/Component.hpp"

class CigiHostSession;
class CigiIGCtrlV3;
class CigiViewCtrlV3;
class CigiSensorCtrlV3;
class CigiViewDefV3;
class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiArtPartCtrlV3;
class CigiHatHotReqV3;
class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet

namespace oe {
namespace otw {
class OtwCigiCl;

//------------------------------------------------------------------------------
// Class: CigiCl
// Description: CIGI I/O handler
//
// Factory name: CigiCl
//------------------------------------------------------------------------------
class CigiCl : public base::Component
{
   DECLARE_SUBCLASS(CigiCl, base::Component)

public:
   CigiCl();

   // ID number of this CIGI session
   CigiHostSession* getSession() const  { return session; }

   // Init this CIGI handler
   virtual bool initialize(OtwCigiCl* const p);

   // True if we've been successfully initialized
   virtual bool isInitialized();

   // R/T frame sync -- called from OtwCigiCl::frameSync() in the R/T thread
   virtual void frameSync();

   // Background frame sync -- called from OtwCigiCl::updateData() in the background
   virtual void frameBg();

   // CIGI functions
   virtual void startMessage() =0;
   virtual void endMessage() =0;
   virtual int  getOutgoingBufferSize() =0;

   virtual void addPacketIGCtrl(CigiIGCtrlV3* const p) =0;
   virtual void addPacketViewCtrl(CigiViewCtrlV3* const p) =0;
   virtual void addPacketSensorCtrl(CigiSensorCtrlV3* const p)=0;
   virtual void addPacketViewDef(CigiViewDefV3* const p) =0;
   virtual void addPacketEntityCtrl(CigiEntityCtrlV3* const p) =0;
   virtual void addPacketComponentCtrl(CigiCompCtrlV3* const p) =0;
   virtual void addPacketArtPartCtrl(CigiArtPartCtrlV3* const p) =0;
   virtual void addPacketHatHotReq(CigiHatHotReqV3* const p) = 0;
   virtual void addPacketLosRangeReq(CigiLosVectReqV3* const p) =0;

protected:

   // Our parent OtwCigiCl
   OtwCigiCl* getOtwCigi() { return otwCigi; }

   // Return the Cigi object for this session id
   static CigiCl* getSession(const int id) { return (id >= 0 && id < MAX_SESSIONS) ? sessions[id] : 0; }

private:
   OtwCigiCl* otwCigi {};   // Our parent OtwCigiCl
   // (no need to ref(), it owns us and it's not going to be deleted before us)

   CigiHostSession* session {};

   // Static table of CIGI session objects ...
   static const int MAX_SESSIONS = 8;
   static CigiCl* sessions[MAX_SESSIONS];

   // CIGI init support
   static bool initCigi();                 // init the CIGI static system
   static bool cigiInitialized;            // CIGI has been initialized
   static bool cigiInitFailed;             // CIGI initialization has failed
};

}
}

#endif
