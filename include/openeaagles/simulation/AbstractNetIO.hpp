
#ifndef __oe_simulation_AbstractNetIO_H__
#define __oe_simulation_AbstractNetIO_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: AbstractNetIO
// Description: Abstract interface class for interoperability Network I/O.
//
//    Defines the framework for interoperability networks (e.g., DIS, HLA,
//    TENA, etc).  Derived classes will manage the network specific protocols.
//
// Factory name: AbstractNetIO
//
// Input/Output frames:
//
//    The functions inputFrame() and outputFrame() need to be called by our
//    container object (e.g., Station class) to process the incoming and out-
//    going entities, respectively.  For DIS, these can be called by different
//    threads, and for HLA they need to be called from the same thread.
//------------------------------------------------------------------------------
class AbstractNetIO : public base::Component
{
   DECLARE_SUBCLASS(AbstractNetIO, base::Component)

public:
   AbstractNetIO();

   // Max network ID
   static const unsigned int MAX_NETWORD_ID = 2;

   //
   // interface used by Station class
   //

   // Updates the 'input' side of the network
   virtual void inputFrame(const double dt) =0;

   // Updates the 'output' side of the network
   virtual void outputFrame(const double dt) =0;

   // Network ID number
   virtual unsigned short getNetworkID() const =0;

};

}
}

#endif
