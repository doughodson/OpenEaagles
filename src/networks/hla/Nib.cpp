//------------------------------------------------------------------------------
// Class: Nib
//------------------------------------------------------------------------------

#include "openeaagles/networks/hla/Nib.hpp"
#include "openeaagles/networks/hla/NetIO.hpp"
#include "openeaagles/networks/hla/Ambassador.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/Pair.hpp"

#include <cstdio>

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace hla {

//==============================================================================
// Class: Nib
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Nib, "HlaNib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

Nib::Nib(const simulation::NetIO::IoType ioType) : simulation::Nib(ioType), oname()
{
   STANDARD_CONSTRUCTOR()
   handle = 0;
   objectClassIndex = 0;
   setTimeoutEnabled(true);
   clearAllAttributeUpdateEnabledFlags();
   clearAllAttributeUpdateRequiredFlags();
}

Nib::Nib(const Nib& org) : simulation::Nib(org.getIoType())
{ 
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

Nib::~Nib()
{
   STANDARD_DESTRUCTOR()
}

Nib& Nib::operator=(const Nib& org)
{
   deleteData();
   copyData(org,false);
   return *this;
}

Nib* Nib::clone() const
{
   return new Nib(*this);
}

void Nib::copyData(const Nib& org, const bool)
{
   BaseClass::copyData(org);
   oname = org.oname;
   handle = org.handle;
   objectClassIndex = org.objectClassIndex;

   for (unsigned int i = 0; i < NetIO::MAX_ATTRIBUTES; i++) {
      updateEnabled[i] = org.updateEnabled[i];
      updateRequired[i] = org.updateRequired[i];
   }
}

void Nib::deleteData()
{
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Nib::setObjectHandle(RTI::ObjectHandle h)
{
   handle = h;
}

void Nib::setClassIndex(const unsigned int idx)
{
   objectClassIndex = idx;
}

void Nib::setObjectName(const char* s)
{
   oname = s;
}

// Makes a default RTI name
void Nib::makeObjectName()
{
   char name[256];
   const char* fname = *getFederateName();
   std::sprintf(name, "P%d_%s", int(getPlayerID()), fname );
   setObjectName(name);
}                            

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.
//------------------------------------------------------------------------------
void Nib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet&)
{
   //std::cout << "Nib::reflectAttributeValues() here!" << std::endl;
}

//------------------------------------------------------------------------------
// HLA attribute update required flags
//------------------------------------------------------------------------------
void Nib::setAttributeUpdateRequiredFlag(const unsigned int attribIndex, const bool flg)
{
   if (attribIndex >= 1 && attribIndex <= NetIO::MAX_ATTRIBUTES) updateRequired[attribIndex-1] = flg;
}

void Nib::setAllAttributeUpdateRequiredFlags()
{
   for (unsigned int i = 1; i <= NetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateRequiredFlag(i,true);
   }
}

void Nib::clearAllAttributeUpdateRequiredFlags()
{
   for (unsigned int i = 1; i <= NetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateRequiredFlag(i,false);
   }
}

//------------------------------------------------------------------------------
// HLA attribute update enabled flags
//------------------------------------------------------------------------------
void Nib::setAttributeUpdateEnabledFlag(const unsigned int attribIndex, const bool flg)
{
   if (attribIndex >= 1 && attribIndex <= NetIO::MAX_ATTRIBUTES) updateEnabled[attribIndex-1] = flg;
}

void Nib::clearAllAttributeUpdateEnabledFlags()
{
   for (unsigned int i = 1; i <= NetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateEnabledFlag(i,false);
   }
}

//------------------------------------------------------------------------------
// isPlayerStateUpdateRequiredForOutput() -- check to see if an update is required
//------------------------------------------------------------------------------
bool Nib::isPlayerStateUpdateRequired(const double curExecTime)
{
    if (!isRegistered()) return true;
    else return BaseClass::isPlayerStateUpdateRequired(curExecTime);
}

//------------------------------------------------------------------------------
// turnUpdatesOn() -- (Output support)
//  Called by our FederateAmbassador to handle the turnUpdatesOnForObjectInstance()
//  callbacks for this object instance.
//------------------------------------------------------------------------------
void Nib::turnUpdatesOn(const RTI::AttributeHandleSet& theAttributes)
{
   NetIO* hlaIO = static_cast<NetIO*>(getNetIO());
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " ON ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle =  theAttributes.getHandle(i);
         unsigned int theIndex = hlaIO->findAttributeIndex(theHandle);
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateEnabledFlag(theIndex, true);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

//------------------------------------------------------------------------------
// turnUpdatesOff() -- (Output support)
//  Called by our FederateAmbassador to handle the turnUpdatesOffForObjectInstance()
//  callbacks for this object instance.
//------------------------------------------------------------------------------
void Nib::turnUpdatesOff(const RTI::AttributeHandleSet& theAttributes)
{
   NetIO* hlaIO = static_cast<NetIO*>(getNetIO());
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " OFF ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle =  theAttributes.getHandle(i);
         unsigned int theIndex = hlaIO->findAttributeIndex(theHandle);
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateEnabledFlag(theIndex, false);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

//------------------------------------------------------------------------------
// provideAttributeValueUpdate() -- (Output support)
//  Called by our FederateAmbassador to request attribute value updates
//------------------------------------------------------------------------------
void Nib::provideAttributeValueUpdate(const RTI::AttributeHandleSet& theAttributes)
{
   NetIO* hlaIO = static_cast<NetIO*>(getNetIO());
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " Update ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle =  theAttributes.getHandle(i);
         unsigned int theIndex = hlaIO->findAttributeIndex(theHandle);
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateRequiredFlag(theIndex, false);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

}
}

