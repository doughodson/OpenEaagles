//------------------------------------------------------------------------------
// Class: Nib
//------------------------------------------------------------------------------
#ifndef __Hla_NibHlaIO_H_3EB96940_4B3C_408b_AB45_316CE968FAD4__
#define __Hla_NibHlaIO_H_3EB96940_4B3C_408b_AB45_316CE968FAD4__

#include "eaagles/simulation/Nib.h"
#include "eaagles/hla/HlaIO.h"
#include <RTI.hh>
#include <fedtime.hh>

namespace Eaagles {
namespace Network {
namespace Hla {

class Ambassador;

//==============================================================================
// Class:       Hla::Nib
// Base class:  Basic::Object -> Simulation::Nib -> Hla::Nib
// Description: Unqiue NIB for HLA support.
//
// Note: updateRequired[] flags are used ...
//        -- (input)  an attribute has received a value (via reflectAttributeValues())
//        -- (output) an attribute update is required (via provideAttributeValueUpdate())
//
// GUID: {3EB96940-4B3C-408b-AB45-316CE968FAD4}
//==============================================================================
class Nib : public Simulation::Nib  
{
   DECLARE_SUBCLASS(Nib,Simulation::Nib)

public:
   Nib(const Simulation::NetworkIO::IoType ioType);

   // Returns true if the object has been registerd
   bool isRegistered() const                       { return (handle != 0); }

   // RTI Object Handle
   bool isObjectHandle(RTI::ObjectHandle h) const  { return h == handle; }
   RTI::ObjectHandle getObjectHandle() const       { return handle; }
   virtual void setObjectHandle(RTI::ObjectHandle h);

   // Object class index
   int getClassIndex() const                       { return objectClassIndex; }
   bool isClassIndex(const int idx)                { return (idx == objectClassIndex); }
   virtual void setClassIndex(const int idx);

   // Object name
   const char* getObjectName() const               { return oname; }
   virtual void setObjectName(const char* s);

   // Makes a default RTI name
   virtual void makeObjectName();

   // Input support functions
   virtual void reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs);

   // HLA attribute update enabled flags
   virtual void setAttributeUpdateEnabledFlag(const int attribIndex, const bool flg);
   virtual void turnUpdatesOn(const RTI::AttributeHandleSet& theAttributes);
   virtual void turnUpdatesOff(const RTI::AttributeHandleSet& theAttributes);
   bool isAttributeUpdateEnabled(const int attribIndex) const
      { return (attribIndex >= 1 && attribIndex <= HlaIO::MAX_ATTRIBUTES) ? updateEnabled[attribIndex-1] : 0; }

   // HLA attribute update required flags
   virtual void setAttributeUpdateRequiredFlag(const int attribIndex, const bool flg);
   virtual void setAllAttributeUpdateRequiredFlags();
   virtual void provideAttributeValueUpdate(const RTI::AttributeHandleSet& theAttrs);
   bool isAttributeUpdateRequired(const int attribIndex) const
      { return (attribIndex >= 1 && attribIndex <= HlaIO::MAX_ATTRIBUTES) ? updateRequired[attribIndex-1] : 0; }

   // Simulation::Nib Interface
   virtual bool isPlayerStateUpdateRequired(const LCreal curExecTime);

protected:
    virtual void clearAllAttributeUpdateEnabledFlags();
    virtual void clearAllAttributeUpdateRequiredFlags();
    
private:
    Basic::String  oname;                            // Our object name 
    RTI::ObjectHandle handle;                   // Our object handle
    int objectClassIndex;                       // We are of this FOM object class
    bool updateEnabled[HlaIO::MAX_ATTRIBUTES];  // If true, an attribute update is enabled
    bool updateRequired[HlaIO::MAX_ATTRIBUTES]; // If true, an attribute update is required (see note above)
};

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif // __Hla_NibHlaIO_H_3EB96940_4B3C_408b_AB45_316CE968FAD4__
