
#ifndef __oe_dis_Ntm_H__
#define __oe_dis_Ntm_H__

#include "openeaagles/networks/interop/Ntm.hpp"
#include "openeaagles/networks/dis/NetIO.hpp"

namespace oe {
namespace base { class List; }
namespace dis {

//------------------------------------------------------------------------------
// Class: Ntm
//
// Description: DIS Network Type Mapper
//
//    This class, which is derived from the simulation::Ntm, maps OpenEaagles
//    player types to/from DIS entity type codes.
//
//
// Factory name: DisNtm
// Slots:
//    disEntityType  <vector>   ! DIS Entity type vector:
//                              !  [ kind domain country category subcategory specific extra ]
//                              !  example: [ 1 2 333 4 5 6 7 ]
//
//------------------------------------------------------------------------------
class Ntm : public interop::Ntm
{
    DECLARE_SUBCLASS(Ntm, interop::Ntm)

public:
   Ntm();

   unsigned char getEntityKind() const         { return disKind;        }       // DIS kind type code (or 255 if not valid)
   unsigned char getEntityDomain() const       { return disDomain;      }       // DIS domain type code
   unsigned short getEntityCountry() const     { return disCountry;     }       // DIS country type code
   unsigned char getEntityCategory() const     { return disCategory;    }       // DIS category type code
   unsigned char getEntitySubcategory() const  { return disSubcategory; }       // DIS subcategory type code
   unsigned char getEntitySpecific() const     { return disSpecific;    }       // DIS specific type code
   unsigned char getEntityExtra() const        { return disExtra;       }       // DIS extra type code

   // Sets the player's entity type codes
   virtual bool setEntityType(
      const unsigned char  kind,
      const unsigned char  domain,
      const unsigned short countryCode,
      const unsigned char  category,
      const unsigned char  subcategory = 0,
      const unsigned char  specific = 0,
      const unsigned char  extra = 0
   );

   // Slot functions
   virtual bool setSlotDisEntityType(const base::List* const);         // Sets DIS entity type enumerations

   virtual bool copyEntityType(interop::Nib* const targetNib) const override;

private:
    // Standard (DIS based) type codes
    unsigned char  disKind {};      // DIS kind code
    unsigned char  disDomain {};    // DIS domain code
    unsigned short disCountry {};   // DIS country code
    unsigned char  disCategory {};  // DIS category code
    unsigned char  disSubcategory {}; // DIS subcategory code
    unsigned char  disSpecific {};  // DIS specific code
    unsigned char  disExtra {};     // DIS extra code
};

}
}

#endif
