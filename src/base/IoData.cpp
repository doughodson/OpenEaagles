// Class: IoData

#include "openeaagles/base/IoData.h"

namespace oe {
namespace base {

//==============================================================================
// IoData
//==============================================================================
IMPLEMENT_SUBCLASS(IoData,"GenericIoData")
EMPTY_SLOTTABLE(IoData)
EMPTY_CONSTRUCTOR(IoData)
EMPTY_COPYDATA(IoData)
EMPTY_DELETEDATA(IoData)
EMPTY_SERIALIZER(IoData)

// -----------------------------------------------------------------------------
// Default quantity functions
// -----------------------------------------------------------------------------
unsigned int IoData::getNumAnalogInputChannels() const     { return 0; }
unsigned int IoData::getNumAnalogOutputChannels() const    { return 0; }
unsigned int IoData::getNumDiscreteInputChannels() const   { return 0; }
unsigned int IoData::getNumDiscreteOutputChannels() const  { return 0; }

// -----------------------------------------------------------------------------
// getAnalogInput() - default
// -----------------------------------------------------------------------------
bool IoData::getAnalogInput(const unsigned int, double* const) const
{
   return false;
}

// -----------------------------------------------------------------------------
// getAnalogOutput() - default
// -----------------------------------------------------------------------------
bool IoData::getAnalogOutput(const unsigned int, double* const) const
{
   return false;
}

// -----------------------------------------------------------------------------
// getDiscreteInput() - default
// -----------------------------------------------------------------------------
bool IoData::getDiscreteInput(const unsigned int, bool* const) const 
{
   return false;
}

// -----------------------------------------------------------------------------
// getDiscreteOutput() - default
// -----------------------------------------------------------------------------
bool IoData::getDiscreteOutput(const unsigned int, bool* const) const
{
   return false;
}

// -----------------------------------------------------------------------------
// setAnalogInput() - default
// -----------------------------------------------------------------------------
bool IoData::setAnalogInput(const unsigned int, const double)
{
   return false;
}

// -----------------------------------------------------------------------------
// setAnalogOutput() - default
// -----------------------------------------------------------------------------
bool IoData::setAnalogOutput(const unsigned int, const double)
{
   return false;
}

// -----------------------------------------------------------------------------
// setDiscreteInput() - default
// -----------------------------------------------------------------------------
bool IoData::setDiscreteInput(const unsigned int, const bool)
{
   return false;
}

// -----------------------------------------------------------------------------
// setDiscreteOutput() - default
// -----------------------------------------------------------------------------
bool IoData::setDiscreteOutput(const unsigned int, const bool)
{
   return false;
}

// -----------------------------------------------------------------------------
// Process the input channels, if needed (application specific).
// Called by the IoHandler just after the input devices are processed.
// -----------------------------------------------------------------------------
bool IoData::processInputs()
{
   return true;
}

// -----------------------------------------------------------------------------
// Process the output channels, if needed (application specific).
// Called by the IoHandler just before the output devices are processed.
// -----------------------------------------------------------------------------
bool IoData::processOutputs()
{
   return true;
}

// -----------------------------------------------------------------------------
// Default initialize the network 
// -----------------------------------------------------------------------------
bool IoData::initNetwork()
{
   return false;
}

// -----------------------------------------------------------------------------
// Default clear the data
// -----------------------------------------------------------------------------
void IoData::clear()
{
}

}
}
