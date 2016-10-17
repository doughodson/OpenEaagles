
#ifndef __oe_base_IoAdapter_H__
#define __oe_base_IoAdapter_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base {
   class IoData;
   class IoDevice;

//------------------------------------------------------------------------------
// Class: IoAdapter
//
// Description: Abstract class that manages the flow of individual I/O items
//              between the I/O devices (IoDevice) and the I/O data buffers
//              (IoData).  IoAdapter objects are owned and managed by I/O
//              device objects.
//
//------------------------------------------------------------------------------
class IoAdapter : public Component
{
    DECLARE_SUBCLASS(IoAdapter, Component)

public:
   IoAdapter();

   // Process input data item(s) from the input device to the input buffer
   virtual void processInputs(
         const double dt,
         const IoDevice* const device,
         IoData* const inData
      ) =0;

   // Process output data item(s) from the output buffer to the output device
   virtual void processOutputs(
         const double dt,
         const IoData* const outData,
         IoDevice* const device
      ) =0;
};

}
}

#endif
