//------------------------------------------------------------------------------
// Classes: Dis::Nib    -- DIS Network Interface Block class
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Dis_Nib_H__
#define __Eaagles_Network_Dis_Nib_H__

#include "openeaagles/simulation/Nib.h"
#include "openeaagles/dis/NetIO.h"

namespace Eaagles {
namespace Network {
namespace Dis {
class Ntm;

//==============================================================================
// Class: Dis::Nib
// Description: Unique DIS Network Interface Block (NIB)
//
//    This class, which is derived from the Simulation::Nib, is used to manage
//    the relationship between players and their corresponding DIS entities.
//
//==============================================================================
class Nib : public Simulation::Nib  
{
    DECLARE_SUBCLASS(Nib,Simulation::Nib)

public:
    enum { MAX_EM_SYSTEMS = 16 };

public:
    Nib(const Simulation::NetIO::IoType ioType);
    
   // Site & App IDs
   unsigned short getSiteID() const                { return siteID; }
   virtual void setSiteID(const unsigned short v);
   unsigned short getApplicationID() const         { return appID; }
   virtual void setApplicationID(const unsigned short v);
   
    // Input support functions
    virtual void updateTheIPlayer();
    virtual void entityStatePdu2Nib(const EntityStatePDU* const pdu);

    // Update check functions
    virtual bool isIffUpdateRequired(const LCreal curExecTime, const Simulation::Iff* const iffSystem);

    //These are all going to be moved to separate classes, but for now, are just virtual functions, see above in disIO)
    virtual bool IffManager(const LCreal curExecTime);
    virtual bool emitterBeamsManager(const LCreal curExecTime);
    virtual bool processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const pdu);

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

    // Nib Interface
    virtual bool entityStateManager(const LCreal curExecTime);
    virtual bool weaponFireMsgFactory(const LCreal curExecTime);
    virtual bool munitionDetonationMsgFactory(const LCreal curExecTime);
    virtual bool networkOutputManagers(const LCreal curExecTime);

protected:
   unsigned char manageArticulationParameters(EntityStatePDU* const pdu);
   void processArticulationParameters(const EntityStatePDU* const pdu);

   // Electromagnetic Emitter handlers (protected for now)
   unsigned char    numEmissionSystems;                           // Number of emission systems 
   unsigned char    emissionSystemsIndex;                         // Emission systems index (outgoing only)
   EmissionPduHandler* emitterSysHandler[MAX_EM_SYSTEMS];         // Handler for each emitter system

private:
   unsigned short  siteID;     // Site ID
   unsigned short  appID;      // Application ID

   // IFF PDU data
   FundamentalOpData* iffFunOpData;   // IFF Functional Operational Data
   LCreal        iffLastExecTime;     // Exec time of last IFF update
   unsigned short iffEventId;         // IFF event ID
   unsigned char  iffOptions;         // IFF PDU option bits

   // Attached missile entity types
   const Ntm* apartMslTypes[MAX_AMSL];

   // Standard (DIS based) type codes
   unsigned char  disKind;          // DIS kind code
   unsigned char  disDomain;        // DIS domain code
   unsigned short disCountry;       // DIS country code
   unsigned char  disCategory;      // DIS category code
   unsigned char  disSubcategory;   // DIS subcategory code
   unsigned char  disSpecific;      // DIS specific code
   unsigned char  disExtra;         // DIS extra code
}; 

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
