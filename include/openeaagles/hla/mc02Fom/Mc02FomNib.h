//------------------------------------------------------------------------------
// Class: Hla::Mc02Fom::Nib
//
// Millenium Challenge 2002 FOM
// Version: RPR 1.0 Draft 2 w/extensions: $Revision: 1.6
//------------------------------------------------------------------------------
#ifndef __Mc02FomNib_H_B47879A1_CED4_42e5_9E65_CF07CF92310F__
#define __Mc02FomNib_H_B47879A1_CED4_42e5_9E65_CF07CF92310F__

#include "openeaagles/hla/Nib.h"

namespace Eaagles {
   namespace Simulation { class Player; }

namespace Network {
namespace Hla {
namespace Mc02Fom {
   class BaseEntity;

//==============================================================================
// Class:       Mc02Fom::Nib
// Base class:  ... -> Simulation::Nib -> Hla::Nib -> Hla::Mc02Fom::Nib
// GUID: {B47879A1-CED4-42e5-9E65-CF07CF92310F}
//==============================================================================
class Nib : public Hla::Nib
{
   DECLARE_SUBCLASS(Nib,Hla::Nib)
    
public:
   static const int MAX_EMITTER_BEAMS = 6;
    
public:
   Nib(const Simulation::NetworkIO::IoType ioType);

   unsigned short getWeaponFireEvent() const       { return fireEvent; }
   virtual void setWeaponFireEvent(const unsigned short e);

   // Site & App IDs
   unsigned short getSiteID() const                { return siteID; }
   virtual void setSiteID(const unsigned short v);
   unsigned short getApplicationID() const         { return appID; }
   virtual void setApplicationID(const unsigned short v);

   // Standard (DIS) entity type codes 
   unsigned char getEntityKind() const      { return disKind; }                 // DIS kind type code (or 255 if not valid)
   unsigned char getEntityDomain() const    { return disDomain; }               // DIS domain type code 
   unsigned short getEntityCountry() const  { return disCountry; }              // DIS country type code
   unsigned char getEntityCategory() const  { return disCategory; }             // DIS category type code
   unsigned char getEntitySubcategory() const  { return disSubcategory; }       // DIS subcategory type code
   unsigned char getEntitySpecific() const  { return disSpecific; }             // DIS specific type code
   unsigned char getEntityExtra() const     { return disExtra; }                // DIS extra type code

   // Sets the standard (DIS) entity type codes
   virtual bool setEntityType(
      const unsigned char  kind,
      const unsigned char  domain,
      const unsigned short countryCode,
      const unsigned char  category,
      const unsigned char  subcategory = 0,
      const unsigned char  specific = 0,
      const unsigned char  extra = 0
   );

   // Input support functions
   virtual void reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs);
   virtual void entityState2Nib();
   virtual void updateTheIPlayer();

   // Output support functions
   virtual void updateBasicEntity(RTI::AttributeHandleValuePairSet* attrs, const LCreal curExecTime);
   virtual void updatePhysicalEntity(RTI::AttributeHandleValuePairSet* attrs, const LCreal curExecTime);
   virtual void updatePlatform(RTI::AttributeHandleValuePairSet* attrs, const LCreal curExecTime);

   // MC02 data structures
   BaseEntity* getBaseEntity()               { return baseEntity; }
   const BaseEntity* getBaseEntity() const   { return baseEntity; }
   virtual void setBaseEntity(BaseEntity* const p);

   // Simulation::Nib Interface
   virtual bool setOutputPlayerType(const Simulation::Player* const p);
   virtual bool isPlayerStateUpdateRequired(const LCreal curExecTime);
   virtual bool entityStateManager(const LCreal curExecTime);
   virtual bool emitterBeamsManager(const LCreal curExecTime);
   virtual bool weaponFireMsgFactory(const LCreal curExecTime);
   virtual bool munitionDetonationMsgFactory(const LCreal curExecTime);

private:
   unsigned short  siteID;     // Site ID
   unsigned short  appID;      // Application ID

   // Standard (DIS based) type codes
   unsigned char  disKind;          // DIS kind code
   unsigned char  disDomain;        // DIS domain code
   unsigned short disCountry;       // DIS country code
   unsigned char  disCategory;      // DIS category code
   unsigned char  disSubcategory;   // DIS subcategory code
   unsigned char  disSpecific;      // DIS specific code
   unsigned char  disExtra;         // DIS extra code

   BaseEntity* baseEntity; // The object's data
   double geodPos[3];              // Geodetic position of entity
   unsigned short fireEvent;       // Weapon fire event (for Weapon type objects)

   // Things we need before creating an IPlayer
   bool    haveEntityIdFlg;
   bool    haveEntityTypeFlg;
   bool    haveWorldLocationFlg;
   bool    haveOrientationFlg;
};

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif // __Mc02FomNib_H_B47879A1_CED4_42e5_9E65_CF07CF92310F__
