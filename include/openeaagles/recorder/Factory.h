//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_Factory_H__
#define __Eaagles_Recorder_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Recorder {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Recorder
}  // end namespace Eaagles

#endif
