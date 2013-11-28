//------------------------------------------------------------------------------
// Classes: Radio, CommRadio
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Radio_H__
#define __Eaagles_Simulation_Radio_H__

#include "RfSystem.h"

namespace Eaagles {
   namespace Basic { class PairStream; }

namespace Simulation {
class Datalink;

//------------------------------------------------------------------------------
// Class: Radio
// Description: Base class for all radios (Comm, Nav, IFF, etc.)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
//    For many Radio models, the maximum detection range, which is set using the
//    'maxDetectRange' slot, along with the frequency are sufficient to determine
//    if the radio is "tuned and in range".  For more complex models, the radio's
//    R/F parameters, its antennas and its capability to transmit and received
//    emission packets are located in the base RfSystem class.
//
//    Radios can have frequencies selected by channel numbers, which are assigned
//    to the channels using the 'channels' slot.  Channel frequencies can also
//    be assigned using the setChannelFrequency() function.  The 'numChannels'
//    slot defines the number of channels that the radio model is capable of,
//    and it must be set prior to setting any channel frequencies.  
//
//    The radio is in 'channel tuned' mode if the radio frequency is the same as
//    the frequency assigned to the current channel number; otherwise the radio
//    is in 'manual tuned' mode.
//
//          The channel number can be changed using the setChannel() function.
//          This function changes both the channel number and tunes the radio
//          to the frequency assigned to the channel.  If the channel number
//          is zero the radio is tuned to the frequency of the current channel
//          (i.e., switching from manual tuned to channel tuned).
//
//          The radio frequency was set manually using the base class function
//          RfSystem::setFrequency()).
//
//
// Form name: Radio
// Slots:
//    numChannels    <Basic::Number>      ! Number of channels (less than or equal MAX_CHANNELS) (default: 0)
//
//    channels       <Basic::PairStream>  ! Our channels (list of Basic::Frequency objects) --
//                                        ! -- make sure to set the number of channels first. (default: 0)
//
//    channel        <Basic::Number>      ! Channel number [ 1 .. 'numChannels' ] (default: 0) ### NES Should the lower value be 0?
//
//    maxDetectRange <Basic::Number>      ! Maximum detection capability (NM) (default: 120NM)
//
//    radioID        <Basic::Number>      ! ID number used by DIS (default: 0)
//
//
// Example EDL:
//
//       ( Radio
//          numChannels:  4
//          channels: {
//             ( MegaHertz 212.0 ) // Channel #1
//             ( MegaHertz 214.0 ) // Channel #2
//             ( MegaHertz 216.0 ) // Channel #3
//             ( MegaHertz 218.0 ) // Channel #4
//          }
//          channel: 3
//       )
//
//------------------------------------------------------------------------------
class Radio : public RfSystem  
{
   DECLARE_SUBCLASS(Radio,RfSystem)

public:
   enum { MAX_CHANNELS = 500 };

public:
   Radio();

   // Is the radio tuned to its current channel's frequency
   bool isChannelTuned() const;

   // Is the radio manually tuned (i.e., the current frequency
   // is not the same as the current channel's frequency)
   bool isManualTuned() const;

   // Current channel number or zero if not set.
   unsigned short getChannel() const;

   // Number of channels
   unsigned short getNumberOfChannels() const;

   // Get a channel's frequency (Hz)
   // Returns -1 if the channel is invalid
   LCreal getChannelFrequency(const unsigned short chan) const;

   // Set a channel's frequency (Hz)
   virtual  bool setChannelFrequency(const unsigned short chan, const LCreal freq);

   // Returns the radio's maximum detection range (NM)
   LCreal getMaxDetectRange() const;

   // DIS radio ID
   unsigned short getRadioId() const;

   // Sets the radio's channel number and changes the radio frequency
   // If 'chan' is zero then the radio frequency is changed to the
   // current channel number (if any)
   virtual bool setChannel(const unsigned short chan = 0);

   virtual bool setRadioId(const unsigned short num);
   virtual bool setMaxDetectRange(const LCreal num);

protected:
   virtual void receivedEmissionReport(Emission* const em);

   // Sets the number of channels; previous channels are lost!
   virtual bool setNumberOfChannels(const unsigned short n);

   // Slot functions
   virtual bool setSlotNumChannels(Basic::Number* const msg);
   virtual bool setSlotChannels(const Basic::PairStream* const msg);
   virtual bool setSlotChannel(Basic::Number* const msg);
   virtual bool setSlotMaxDetectRange(Basic::Number* const num);
   virtual bool setSlotRadioId(Basic::Number* const num);

   // System class -- phase callbacks
   virtual void receive(const LCreal dt);

private:
   void initData();

   unsigned short channel;    // Channel number [ 1 .. numChan ]; or zero for "not set"
   unsigned short numChan;    // Number of channels

   // Channel table
   LCreal* chanFreqTbl;       // Channel Frequency table (hertz)

   LCreal  maxDetectRange;      // Radio maximum detection range  (NM)
   unsigned short radioId;
};

//------------------------------------------------------------------------------
// Class: CommRadio
// Description: Generic class for all communication radio models
//
// Form name: CommRadio
//------------------------------------------------------------------------------
class CommRadio : public Radio  
{
   DECLARE_SUBCLASS(CommRadio,Radio)

public:
   CommRadio();

   // The optional datalink system attached to this radio
   Datalink* getDatalink();
   const Datalink* getDatalink() const;
   virtual bool setDatalink(Datalink* const p);

   // Transmit a datalink message
   virtual bool transmitDataMessage(Basic::Object* const msg);

protected:
   virtual void receivedEmissionReport(Emission* const em);

private:
   void initData();

   SPtr<Datalink> datalink;   // Our companion datalink system
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
