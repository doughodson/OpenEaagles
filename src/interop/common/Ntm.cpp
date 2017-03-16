
#include "openeaagles/interop/common/Ntm.hpp"

#include "openeaagles/models/players/Player.hpp"

namespace oe {
namespace interop {

IMPLEMENT_ABSTRACT_SUBCLASS(Ntm, "Ntm")

BEGIN_SLOTTABLE(Ntm)
   "template",       // 1) Template player
END_SLOTTABLE(Ntm)

BEGIN_SLOT_MAP(Ntm)
    ON_SLOT(1, setSlotTemplatePlayer, models::Player)
END_SLOT_MAP()

Ntm::Ntm()
{
   STANDARD_CONSTRUCTOR()
}

void Ntm::copyData(const Ntm& org, const bool)
{
   BaseClass::copyData(org);

   setSlotTemplatePlayer( org.tPlayer );
}

void Ntm::deleteData()
{
   setSlotTemplatePlayer( nullptr );
}

//------------------------------------------------------------------------------
// Set Slot functions
//------------------------------------------------------------------------------

// Sets the template player
bool Ntm::setSlotTemplatePlayer(const models::Player* const msg)
{
   tPlayer = msg;
   return true;
}

std::ostream& Ntm::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (tPlayer != nullptr) {
      indent(sout,i+j);
      //sout << "template:" << std::endl;
      //tPlayer->serialize(sout,(i+j+4));
      sout << "template: ( " << tPlayer->getFactoryName();
      const base::String* tt = tPlayer->getType();
      if (tt != nullptr) {
         sout << " type: " << *tt;
      }
      sout << " )";
      sout << std::endl;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
