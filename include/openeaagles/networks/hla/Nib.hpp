
#ifndef __oe_hla_Nib_H__
#define __oe_hla_Nib_H__

#include "openeaagles/simulation/Nib.hpp"
#include "openeaagles/networks/hla/NetIO.hpp"

#include <RTI.hh>
#include <fedtime.hh>

namespace oe {
namespace hla {

class Ambassador;

//------------------------------------------------------------------------------
// Class: hla::Nib
// Description: Unique NIB for HLA support.
//
// Note: updateRequired[] flags are used ...
//        -- (input)  an attribute has received a value (via reflectAttributeValues())
//        -- (output) an attribute update is required (via provideAttributeValueUpdate())
//------------------------------------------------------------------------------
class Nib : public simulation::Nib
{
   DECLARE_SUBCLASS(Nib, simulation::Nib)

public:
   Nib(const simulation::NetIO::IoType ioType);

   // Returns true if the object has been registered
   bool isRegistered() const                                { return (handle != 0); }

   // RTI Object Handle
   bool isObjectHandle(RTI::ObjectHandle h) const           { return h == handle; }
   RTI::ObjectHandle getObjectHandle() const                { return handle; }
   virtual void setObjectHandle(RTI::ObjectHandle h);

   // Object class index
   unsigned int getClassIndex() const                       { return objectClassIndex; }
   bool isClassIndex(const unsigned int idx)                { return (idx == objectClassIndex); }
   virtual void setClassIndex(const unsigned int idx);

   // Object name
   const char* getObjectName() const                        { return oname; }
   virtual void setObjectName(const char* s);

   // Makes a default RTI name
   virtual void makeObjectName();

   // Input support functions
   virtual void reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs);

   // HLA attribute update enabled flags
   virtual void setAttributeUpdateEnabledFlag(const unsigned int attribIndex, const bool flg);
   virtual void turnUpdatesOn(const RTI::AttributeHandleSet& theAttributes);
   virtual void turnUpdatesOff(const RTI::AttributeHandleSet& theAttributes);
   bool isAttributeUpdateEnabled(const unsigned int attribIndex) const
      { return (attribIndex >= 1 && attribIndex <= NetIO::MAX_ATTRIBUTES) ? updateEnabled[attribIndex-1] : 0; }

   // HLA attribute update required flags
   virtual void setAttributeUpdateRequiredFlag(const unsigned int attribIndex, const bool flg);
   virtual void setAllAttributeUpdateRequiredFlags();
   virtual void provideAttributeValueUpdate(const RTI::AttributeHandleSet& theAttrs);
   bool isAttributeUpdateRequired(const unsigned int attribIndex) const
      { return (attribIndex >= 1 && attribIndex <= NetIO::MAX_ATTRIBUTES) ? updateRequired[attribIndex-1] : 0; }

   // Simulation::Nib Interface
   virtual bool isPlayerStateUpdateRequired(const double curExecTime) override;

protected:
    virtual void clearAllAttributeUpdateEnabledFlags();
    virtual void clearAllAttributeUpdateRequiredFlags();

private:
    base::String oname;                         // Our object name
    RTI::ObjectHandle handle;                   // Our object handle
    unsigned int objectClassIndex;              // We are of this FOM object class
    bool updateEnabled[NetIO::MAX_ATTRIBUTES];  // If true, an attribute update is enabled
    bool updateRequired[NetIO::MAX_ATTRIBUTES]; // If true, an attribute update is required (see note above)
};

}
}

#endif


