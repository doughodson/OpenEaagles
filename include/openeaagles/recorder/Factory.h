//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_recorder_Factory_H__
#define __oe_recorder_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace recorder {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Recorder
}  // end namespace oe

#endif
