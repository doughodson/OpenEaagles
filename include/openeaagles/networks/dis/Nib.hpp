
#ifndef __oe_dis_Nib_H__
#define __oe_dis_Nib_H__

#include "openeaagles/simulation/Nib.hpp"
#include "openeaagles/networks/dis/NetIO.hpp"

namespace oe {

namespace dis {

class Ntm;

//------------------------------------------------------------------------------
// Class: dis::Nib
// Description: Unique DIS Network Interface Block (NIB)
//
//    This class, which is derived from the simulation::Nib, is used to manage
//    the relationship between players and their corresponding DIS entities.
//
//------------------------------------------------------------------------------
class Nib : public simulation::Nib
{
    DECLARE_SUBCLASS(Nib, simulation::Nib)

public:
    enum { MAX_EM_SYSTEMS = 16 };

public:
    Nib(const simulation::NetIO::IoType ioType);

   // Site & App IDs
   unsigned short getSiteID() const                           { return siteID; }
   virtual void setSiteID(const unsigned short v);
   unsigned short getApplicationID() const                    { return appID; }
   virtual void setApplicationID(const unsigned short v);

   // Input support functions
   virtual void updateTheIPlayer();
   virtual void entityStatePdu2Nib(const EntityStatePDU* const pdu);

   // Update check functions
   virtual bool isIffUpdateRequired(const double curExecTime, const simulation::Iff* const iffSystem);

   //These are all going to be moved to separate classes, but for now, are just virtual functions, see above in disIO)
   virtual bool IffManager(const double curExecTime);
   virtual bool emitterBeamsManager(const double curExecTime);
   virtual bool processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const pdu);

   // Standard (DIS) entity type codes
   unsigned char getEntityKind() const         { return disKind;        }       // DIS kind type code (or 255 if not valid)
   unsigned char getEntityDomain() const       { return disDomain;      }       // DIS domain type code
   unsigned short getEntityCountry() const     { return disCountry;     }       // DIS country type code
   unsigned char getEntityCategory() const     { return disCategory;    }       // DIS category type code
   unsigned char getEntitySubcategory() const  { return disSubcategory; }       // DIS subcategory type code
   unsigned char getEntitySpecific() const     { return disSpecific;    }       // DIS specific type code
   unsigned char getEntityExtra() const        { return disExtra;       }       // DIS extra type code

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

   virtual bool entityStateManager(const double curExecTime) override;
   virtual bool weaponFireMsgFactory(const double curExecTime) override;
   virtual bool munitionDetonationMsgFactory(const double curExecTime) override;
   virtual bool networkOutputManagers(const double curExecTime) override;

protected:
   unsigned char manageArticulationParameters(EntityStatePDU* const pdu);
   void processArticulationParameters(const EntityStatePDU* const pdu);

   virtual bool shutdownNotification() override;

   // Electromagnetic Emitter handlers (protected for now)
   unsigned char    numEmissionSystems;                           // Number of emission systems
   unsigned char    emissionSystemsIndex;                         // Emission systems index (outgoing only)
   EmissionPduHandler* emitterSysHandler[MAX_EM_SYSTEMS];         // Handler for each emitter system

private:
   unsigned short  siteID;     // Site ID
   unsigned short  appID;      // Application ID

   // IFF PDU data
   FundamentalOpData* iffFunOpData;   // IFF Functional Operational Data
   double        iffLastExecTime;     // Exec time of last IFF update
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

   // timeOffset has 2 uses
   // 1) In a time synchronized network, it is merely as estimation of PDU latency
   //    Over time, it is updated so that i will eventually become the minimum observed latency
   // 2) If time is not synchronized across the network, then it becomes the time difference + latency
   //    This allows applications to run time relative
   double timeOffset;
};

}
}

#endif
