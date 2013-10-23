//------------------------------------------------------------------------------
// Class:	RfSensor, SensorMgr
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_RfSensor_H__
#define __Eaagles_Simulation_RfSensor_H__

#include "openeaagles/simulation/RfSystem.h"

namespace Eaagles {
   namespace Basic {
      class Angle;
      class Frequency;
      class Integer;
      class List;
      class String;
      class Time;
   }
namespace Simulation {

class Antenna;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class:	RfSensor
// Description: Base class for R/F Sensor Models.  Provides a common
//              interface, and some default behavior, for R/F sensors.
//
// Form name: RfSensor
// Slots:
//    trackManagerName     <String>      ! Name of the requested Track Manager (default: 0)
//
//    modes                <PairStream>  ! List of submodes (default: 0)
//                         <RfSensor>    ! Single (only) submode
//
//    ranges               <List>        ! List of sensor ranges (nm) [vector] (default: 0)
//
//    initRangeIdx         <Number>      ! Initial range index [ 1 ... nRanges ] (default: 1)
//
//    PRF                  <Frequency>   ! Pulse Repetition Frequency (PRF) (must be greater than zero) (default: 0.0)
// ### NES: Initial value not greater than 0)
//                         <Number>      ! PRF in hertz
//
//    pulseWidth           <Time>        ! Pulse Width (must be greater than zero) (default: 0)
// ### NES: Initial value not greater than 0)
//                         <Number>      ! Pulse width in seconds
//
//    beamWidth            <Angle>       ! Beam Width  (must be greater than zero) (default: D2RCC * 3.5f)
//                         <Number>      ! Beam width in radians
//
//    typeId               <String>      ! R/F system type id (default: '\0')
//
//    syncXmitWithScan     <Number>      ! If true, transmitter on is sync'd with the antenna scan (default: false)
//
//
// Events:
//    TGT_DESIGNATE     ()          Target designate switch event
//    SENSOR_RTS        ()          Return-To-Search switch event
//    SCAN_START        (Int)       Start of scan event (bar number)
//    SCAN_END          (Int)       End of scan event (bar number)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
//------------------------------------------------------------------------------
class RfSensor : public RfSystem  
{
    DECLARE_SUBCLASS(RfSensor,RfSystem)

public:
    RfSensor();

    virtual LCreal getPRF() const;                       // Returns the PRF (hertz)
    virtual bool setPRF(const LCreal v);                 // Sets PRF (hertz; must be greater than 0)

    virtual LCreal getPulseWidth() const;                // Returns the pulse width (seconds)
    virtual bool setPulseWidth(const LCreal v);          // Sets the pulse width (seconds; must be greater than 0)

    virtual LCreal getBeamWidth() const;                 // Returns the beam width (radians; must be greater than 0)
    virtual bool setBeamWidth(const LCreal v);           // Sets the beam width (radians)

    virtual const char* getTypeId() const;               // Returns the type ID
    virtual bool setTypeId(const char* const str);       // Sets the type ID

    virtual LCreal getRange() const;                     // Returns the current range (nm)
    virtual bool setRange(const LCreal v);               // Sets the current range (nm; must be greater than or equal 0)

    virtual bool isScanOn() const;                       // Is the Sensor scanning
    virtual int getScanBar() const;                      // Returns the current bar number for a raster scan

    virtual const Basic::PairStream* getModes() const;        // Returns the list of sensor submodes
    virtual bool setMasterMode(RfSensor* const m);       // Set our master mode (container)

    virtual const Basic::String* getTrackManagerName() const; // Returns the requested track manager's name
    virtual bool setTrackManagerName(Basic::String* const a); // Sets the name of the track manager to use

    virtual TrackManager* getTrackManager();             // Returns our current track manager
    virtual const TrackManager* getTrackManager() const; // Returns our current track manager (const version)
    virtual bool setTrackManager(TrackManager* const a); // Sets the track manager

    // Support for a list of sensor ranges
    virtual bool incRange();                                  // Increment range index; returns true if successful
    virtual bool decRange();                                  // Decrement range index; returns true if successful
    virtual int getRanges(LCreal* const rngs, const int max); // Returns the list of ranges
    virtual bool setRanges(const LCreal* const rngs, const int n);// Sets the list of ranges
    virtual bool setInitRngIdx(const int idx);                // Sets the starting range index; returns true if successful

    // Slot functions
    virtual bool setSlotTrackManagerName(Basic::String* const v);   // Sets our track manager by name
    virtual bool setSlotModeStream(Basic::PairStream* const obj);   // Sets a list of R/F sensor submodes
    virtual bool setSlotModeSingle(RfSensor* const obj);       // Sets a single (only) R/F sensor submode
    virtual bool setSlotRanges(Basic::List* const list);            // Sets out list of valid ranges (nm)
    virtual bool setSlotInitRangeIdx(Basic::Number* const num);     // Sets out initial range index [ 1 .. nRanges ]
    virtual bool setSlotPrf(const Basic::Frequency* const msg);     // Sets PRF as a Basic::Frequency
    virtual bool setSlotPrf(const Basic::Number* const msg);        // Sets PRF in hertz
    virtual bool setSlotPulseWidth(const Basic::Time* const msg);   // Sets pulse width using Basic::Time
    virtual bool setSlotPulseWidth(const Basic::Number* const msg); // Sets pulse width in seconds
    virtual bool setSlotBeamWidth(const Basic::Angle* const msg);   // Sets beam width as a Basic::Angle
    virtual bool setSlotBeamWidth(const Basic::Number* const msg);  // Sets beam width in radians
    virtual bool setSlotTypeId(const Basic::String* const msg);     // Sets the type ID
    virtual bool setSlotSyncXmitWithScan(const Basic::Number* const msg); // Sets sync transmitter with antenna scan flag

    // RfSystem interface
    virtual bool isTransmitting() const;

    // Component Interface
    virtual bool event(const int event, Basic::Object* const obj = 0);
    virtual void updateData(const LCreal dt = 0.0);
    virtual void reset();

protected:
    virtual Basic::PairStream* getModes();                          // Returns the list of submodes
    
    // Event handler(s)
    virtual bool onStartScanEvent(const Basic::Integer* const bar);     // Start of scan (TGT_DESIGNATE) event handler
    virtual bool onEndScanEvent(const Basic::Integer* const bar);       // End of scan (SENSOR_RTS) event handler
    virtual bool onTgtDesignateEvent();                        // Target Designate (SCAN_START) event handler
    virtual bool onReturnToSearchEvent();                      // Return to search (SCAN_END) event handler

private:
    bool processModes();

    Basic::PairStream*  modes;           // Our Submodes 
    LCreal*        ranges;          // List of ranges (nm)
    int            nRanges;         // Number of ranges
    LCreal         rng;             // Current range (nm)
    int            rngIdx;          // Range index [ 1 .. nRanges ]
    int            initRngIdx;      // Initial range [ 1 .. nRanges ]
    bool           scanning;        // Scanning flag (should be transmitting)
    int            scanBar;         // Scan (bar) number
    bool           syncXmitWithScan; // Sync transmitter with antenna scan flag

    Basic::String* tmName;        // Name of our track manager 
    RfSensor*      masterModePtr;   // Our Master (Parent) mode (e.g., Sensor)
    TrackManager*  trackManager;    // Our Track manager -- managed by the onboard computer

    static const unsigned int TYPE_ID_LENGTH = 64;
    char typeId[TYPE_ID_LENGTH];    // R/F system type ID

    // Characteristics
    LCreal prf;                     // Pulse Repetition Frequency   (Hz)
    LCreal pulseWidth;              // Pulse Width                  (Sec)
    LCreal beamWidth;               // Beamwidth                    (R)

};

//------------------------------------------------------------------------------
// Class:	SensorMgr
// Base class:	Basic::Object -> ... -> RfSensor -> SensorMgr
//
// Description: Generic class for managing a list of R/F sensors
// Form name: SensorMgr
//------------------------------------------------------------------------------
class SensorMgr : public RfSensor  
{
    DECLARE_SUBCLASS(SensorMgr,RfSensor)

public:
    SensorMgr();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
