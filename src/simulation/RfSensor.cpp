//------------------------------------------------------------------------------
// Classes: RfSensor, SensorMgr
//------------------------------------------------------------------------------
#include "openeaagles/simulation/RfSensor.hpp"

#include "openeaagles/simulation/Antenna.hpp"
#include "openeaagles/simulation/Emission.hpp"
#include "openeaagles/simulation/OnboardComputer.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/simulation/Simulation.hpp"
#include "openeaagles/simulation/TrackManager.hpp"

#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Frequencies.hpp"
#include "openeaagles/base/units/Powers.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/util/str_utils.hpp"

namespace oe {
namespace simulation {

//==============================================================================
// Class: RfSensor
//==============================================================================
IMPLEMENT_SUBCLASS(RfSensor,"RfSensor")

// Slot table
BEGIN_SLOTTABLE(RfSensor)
    "trackManagerName", // 1: Name of the requested Track Manager (base::String)
    "modes",            // 2: Submodes
    "ranges",           // 3: Sensor ranges (nm) [vector]
    "initRangeIdx",     // 4: initial range index [ 1 ... nRanges ]
    "PRF",              // 5: Radar Pulse Repetition  (Frequency) or (Number: hertz)
    "pulseWidth",       // 6: Pulse Width             (Time) or (Number: Seconds)
    "beamWidth",        // 7: (Deprecated: moved to Antenna) Beam Width (Angle) or (Number: Radian)
    "typeId",           // 8: R/F Sensor type ID      (String)
    "syncXmitWithScan", // 9: Flag: If true, transmitter on is sync'd with the antenna scan (default: false)
END_SLOTTABLE(RfSensor)

//  Map slot table
BEGIN_SLOT_MAP(RfSensor)
    ON_SLOT(1,setSlotTrackManagerName,base::String)
    ON_SLOT(2,setSlotModeStream,base::PairStream)
    ON_SLOT(2,setSlotModeSingle,RfSensor)
    ON_SLOT(3,setSlotRanges,base::List)
    ON_SLOT(4,setSlotInitRangeIdx,base::Number)
    ON_SLOT(5,setSlotPrf,base::Frequency)        // Check for base::Frequency before base::Number
    ON_SLOT(5,setSlotPrf,base::Number)
    ON_SLOT(6,setSlotPulseWidth,base::Time)      // Check for base::Time before base::Number
    ON_SLOT(6,setSlotPulseWidth,base::Number)
    ON_SLOT(7,setSlotBeamWidth,base::Angle)      // Check for base::Angle before base::Number
    ON_SLOT(7,setSlotBeamWidth,base::Number)
    ON_SLOT(8,setSlotTypeId,base::String)
    ON_SLOT(9,setSlotSyncXmitWithScan,base::Number)
END_SLOT_MAP()

// Event() map
BEGIN_EVENT_HANDLER(RfSensor)
    ON_EVENT(TGT_DESIGNATE,onTgtDesignateEvent)
    ON_EVENT(SENSOR_RTS,onReturnToSearchEvent)
    ON_EVENT_OBJ(SCAN_START, onStartScanEvent, base::Integer)
    ON_EVENT_OBJ(SCAN_END, onEndScanEvent, base::Integer)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
RfSensor::RfSensor() : modes(nullptr), ranges(nullptr), masterModePtr(nullptr), trackManager(nullptr)
{
    STANDARD_CONSTRUCTOR()

    tmName = nullptr;
    scanning = false;
    scanBar = 0;
    nRanges = 0;
    rng = 50.0;
    rngIdx = 1;
    initRngIdx = 1;
    prf = 0.0;
    pulseWidth = 0.0;
    beamWidth = (static_cast<double>(base::Angle::D2RCC) * 3.5);
    syncXmitWithScan = false;

    typeId[0] = '\0';
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RfSensor::copyData(const RfSensor& org, const bool cc)
{
    BaseClass::copyData(org);

    // If copy constructor, init the following pointers
    if (cc) {
        modes = nullptr;
        ranges = nullptr;
        nRanges = 0;
        tmName = nullptr;
        trackManager = nullptr;
        masterModePtr = nullptr;
    }

    // Copy subpages
    if (modes != nullptr) { modes->unref(); }
    if (org.modes != nullptr) {
        modes = org.modes->clone();
        processModes();
    }
    else modes = nullptr;

    if (org.tmName != nullptr) {
       base::String* clone = org.tmName->clone();
       setTrackManagerName(clone);
       clone->unref();
    }
    else {
      setTrackManagerName(nullptr);
    }

    scanning = org.scanning;
    scanBar = org.scanBar;
    rng = org.rng;
    rngIdx = org.rngIdx;
    initRngIdx = org.initRngIdx;
    setRanges(org.ranges, org.nRanges);
    syncXmitWithScan = org.syncXmitWithScan;

    prf = org.prf;
    pulseWidth = org.pulseWidth;
    beamWidth = org.beamWidth;

    setTypeId(org.typeId);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RfSensor::deleteData()
{
    setRanges(nullptr,0);
    setTrackManager(nullptr);
    setMasterMode(nullptr);
    setTrackManagerName(nullptr);

    if (modes != nullptr) {
       modes->unref();
       modes = nullptr;
    }
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool RfSensor::shutdownNotification()
{
   setTrackManager(nullptr);
   setMasterMode(nullptr);

   if (modes != nullptr) {
       modes->unref();
       modes = nullptr;
    }

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void RfSensor::reset()
{
    BaseClass::reset();

    // ---
    // Do we need to find the track manager?
    // ---
    if (getTrackManager() == nullptr && getTrackManagerName() != nullptr && getOwnship() != nullptr) {
        // We have a name of the track manager, but not the track manager itself
        const char* name = *getTrackManagerName();

        // Get the named track manager from the onboard computer
        OnboardComputer* obc = getOwnship()->getOnboardComputer();
        if (obc != nullptr) {
            setTrackManager( obc->getTrackManagerByName(name) );
        }

        if (getTrackManager() == nullptr) {
            // The assigned track manager was not found!
            std::cerr << "RfSensor::reset() ERROR -- track manager, " << name << ", was not found!" << std::endl;
            setTrackManagerName(nullptr);
        }
    }

    scanning = false;
    scanBar = 0;
    if (nRanges > 0 && ranges != nullptr) {
        rngIdx = 1;
        if (initRngIdx >= 1 && initRngIdx <= nRanges) {
            rngIdx = initRngIdx;
        }
        setRange( ranges[rngIdx-1] );
    }
}

//------------------------------------------------------------------------------
// onStartScanEvent() -- process the start of a scan
//------------------------------------------------------------------------------
bool RfSensor::onStartScanEvent(const base::Integer* const bar)
{
    scanning = true;
    scanBar = bar->getInt();
    return true;
}

//------------------------------------------------------------------------------
// onEndScanEvent() -- process the end of a scan
//------------------------------------------------------------------------------
bool RfSensor::onEndScanEvent(const base::Integer* const)
{
    scanning = false;
    return true;
}

//------------------------------------------------------------------------------
// isTransmitting() -- Returns true if the Radar is transmitting
//------------------------------------------------------------------------------
bool RfSensor::isTransmitting() const
{
    return BaseClass::isTransmitting() && (!syncXmitWithScan || isScanOn());
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void RfSensor::updateData(const double dt)
{
    // Proper range values
    if (nRanges > 0) {
        if (rngIdx < 1) rngIdx = 1;
        if (rngIdx > nRanges) rngIdx = nRanges;
        setRange( ranges[rngIdx-1] );
    }

    BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Access (get) functions
//------------------------------------------------------------------------------

// Returns the PRF (hertz)
double RfSensor::getPRF() const
{
   return prf;
}

// Returns the pulse width (seconds)
double RfSensor::getPulseWidth() const
{
   return pulseWidth;
}

// Returns the beam width (radians)
double RfSensor::getBeamWidth() const
{
   return beamWidth;
}

// Returns the type ID (e.g., "Emitter Name"; DIS Electromagnetic Emission PDU)
const char* RfSensor::getTypeId() const
{
   return typeId;
}

// Returns the current range (nm)
double RfSensor::getRange() const
{
   return rng;
}

// Returns a maximum of 'max' ranges in 'rngs' and returns actual number of ranges.
int RfSensor::getRanges(double* const rngs, const int max)
{
    // Do we have something to do?
    if (rngs == nullptr || max == 0 || nRanges == 0) return 0;

    int n = nRanges;
    if (n > max) n = max;
    for (int i = 0; i < n; i++) {
        rngs[i] = ranges[i];
    }
    return n;
}

// Is the Sensor scanning
bool RfSensor::isScanOn() const
{
   return scanning;
}

// Returns the current bar number for a raster scan
int RfSensor::getScanBar() const
{
   return scanBar;
}

// Returns the requested track manager's name
const base::String* RfSensor::getTrackManagerName() const
{
   return tmName;
}

// Returns our current track manager
TrackManager* RfSensor::getTrackManager()
{
   return trackManager;
}

// Returns our current track manager (const version)
const TrackManager* RfSensor::getTrackManager() const
{
   return trackManager;
}

// Returns the list of sensor submodes
const base::PairStream* RfSensor::getModes() const
{
   return modes;
}

// Returns the list of submodes
base::PairStream* RfSensor::getModes()
{
   return modes;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setRanges() -- set the valid ranges for this sensor
bool RfSensor::setRanges(const double* const rngs, const int n)
{
    // Clear old ranges
    if (ranges != nullptr) delete[] ranges;
    ranges = nullptr;
    nRanges = 0;
    rngIdx = 1;

    // Copy new ranges
    if (n > 0 && rngs != nullptr) {
        double* tmp = new double[n];
        for (int i = 0; i < n; i++) {
            tmp[i] = rngs[i];
        }
        ranges = tmp;
        nRanges = n;
        if (initRngIdx > nRanges) initRngIdx = nRanges;
        if (rngIdx > nRanges) rngIdx = nRanges;
        setRange( ranges[rngIdx-1] );
    }
    return true;
}

// Sets PRF (hertz; must be greater than 0)
bool RfSensor::setPRF(const double v)
{
   bool ok = false;
   if (v > 0) {
      prf = v;
      ok = true;
   }
   return ok;
}

// Sets the pulse width (seconds; must be greater than 0)
bool RfSensor::setPulseWidth(const double v)
{
   bool ok = false;
   if (v > 0) {
      pulseWidth = v;
      ok = true;
   }
   return ok;
}

// Sets the beam width (radians; must be greater than 0)
bool RfSensor::setBeamWidth(const double v)
{
   bool ok = false;
   if (v > 0) {
      beamWidth = v;
      ok = true;
   }
   return ok;
}

// Sets the type ID
bool RfSensor::setTypeId(const char* const str)
{
   base::utStrcpy(typeId, TYPE_ID_LENGTH, str);
   return true;
}

// Sets the current range (nm; must be greater than or equal 0)
bool RfSensor::setRange(const double v)
{
   bool ok = false;
   if (v >= 0) {
      rng = v;
      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
//  setSlotModeStream() -- takes a PairStream in and inits the mode list
//------------------------------------------------------------------------------
bool RfSensor::setSlotModeStream (base::PairStream* const obj)
{
    if (obj != nullptr) {
        // When a PairStream (i.e., more than one, a list) of pages
        if (modes != nullptr) modes->unref();
        modes = obj;
        modes->ref();

    }
    return processModes();
}

//------------------------------------------------------------------------------
//  setSlotModeSingle() -- takes a single Mode and inits the mode list
//------------------------------------------------------------------------------
bool RfSensor::setSlotModeSingle(RfSensor* const obj)
{
    if (modes != nullptr) modes->unref();

    modes = new base::PairStream();

    base::Pair* p = new base::Pair("1",obj);
    modes->put( p );
    p->unref();

    return processModes();
}

//------------------------------------------------------------------------------
//  setSlotRanges() -- Our list of valid ranges (nm)
//------------------------------------------------------------------------------
bool RfSensor::setSlotRanges(base::List* const list)
{
    bool ok = false;
    if (list != nullptr) {
        double rngs[100];
        int n = list->getNumberList(rngs,100);
        ok = setRanges(rngs, n);
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotInitRangeIdx() -- Our initial range index
//------------------------------------------------------------------------------
bool RfSensor::setSlotInitRangeIdx(base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        ok = setInitRngIdx(num->getInt());
    }
    return ok;
}

//------------------------------------------------------------------------------
// setInitRngIdx() -- set the initial range index for this sensor
//------------------------------------------------------------------------------
bool RfSensor::setInitRngIdx(const int idx)
{
    bool ok = false;
    if (idx > 0) {
        initRngIdx = idx;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotPRF() -- Set the Pulse Repetition Frequency (PRF)
//------------------------------------------------------------------------------

// Sets PRF as a base::Frequency
bool RfSensor::setSlotPrf(const base::Frequency* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      const double x = base::Hertz::convertStatic(*msg);
      ok = setPRF( x );
      if (!ok) {
         std::cerr << "RfSensor::setSlotPRF: Error setting PRF!" << std::endl;
      }
   }

   return ok;
}

// Sets PRF in hertz
bool RfSensor::setSlotPrf(const base::Number* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      // Standard base::Number
      const double x = msg->getReal();
      ok = setPRF( x );
      if (!ok) {
         std::cerr << "RfSensor::setSlotPRF: Error setting PRF!" << std::endl;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotPulseWidth() -- Set the Pulse Width
//------------------------------------------------------------------------------

// Sets pulse width using base::Time
bool RfSensor::setSlotPulseWidth(const base::Time* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      const double x = base::Seconds::convertStatic( *msg );
      ok = setPulseWidth( x );
      if (!ok) {
         std::cerr << "RfSensor::setPulseWidth: Error setting pulse width!" << std::endl;
      }
   }

   return ok;
}

// Sets pulse width in seconds
bool RfSensor::setSlotPulseWidth(const base::Number* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      ok = setPulseWidth( msg->getReal() );
      if (!ok) {
         std::cerr << "RfSensor::setPulseWidth: Error setting pulse width!" << std::endl;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotBeamWidth() -- Set the beam width
//------------------------------------------------------------------------------

// Sets beam width as an base::Angle
bool RfSensor::setSlotBeamWidth(const base::Angle* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      const double x = static_cast<double>(base::Radians::convertStatic( *msg ));
      ok = setBeamWidth( x );
      if (!ok) {
         std::cerr << "RfSensor::setBeamWidth: Error setting beam width!" << std::endl;
      }
   }

   return ok;
}

// Sets beam width in radians
bool RfSensor::setSlotBeamWidth(const base::Number* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      ok = setBeamWidth( msg->getReal() );
      if (!ok) {
         std::cerr << "RfSensor::setBeamWidth: Error setting beam width!" << std::endl;
      }

   }

   return ok;
}

// Sets the type ID
bool RfSensor::setSlotTypeId(const base::String* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      ok = setTypeId( msg->getString() );
   }

   return ok;
}

// Sets sync transmitter with antenna scan flag
bool RfSensor::setSlotSyncXmitWithScan(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      syncXmitWithScan = msg->getBoolean();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// onTgtDesignateEvent() -- Manage the target designate event
//------------------------------------------------------------------------------
bool RfSensor::onTgtDesignateEvent()
{
    return true;
}

//------------------------------------------------------------------------------
// onReturnToSearchEvent() -- Manage the sensor Return-To-Search event
//------------------------------------------------------------------------------
bool RfSensor::onReturnToSearchEvent()
{
    // Just reset the track manager ...
    if (trackManager != nullptr) trackManager->event(RESET_EVENT);
    return true;
}

//------------------------------------------------------------------------------
// setTrackManagerName() -- Sets the track manager's name
//------------------------------------------------------------------------------
bool RfSensor::setTrackManagerName(base::String* name)
{
    if (tmName != nullptr) {
        tmName->unref();
    }
    tmName = name;
    if (tmName != nullptr) {
        tmName->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setTrackManager() -- Sets the track manager
//------------------------------------------------------------------------------
bool RfSensor::setTrackManager(TrackManager* tm)
{
    if (trackManager != nullptr) {
        trackManager->unref();
    }
    trackManager = tm;
    if (trackManager != nullptr) {
        trackManager->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setMasterMode() -- set our master mode
//------------------------------------------------------------------------------
bool RfSensor::setMasterMode(RfSensor* const m)
{
    if (masterModePtr != nullptr) masterModePtr->unref();
    masterModePtr = m;
    if (masterModePtr != nullptr) masterModePtr->ref();
    return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// setSlotTrackManagerName() -- sets the name of the track manager;
// we'll lookup the actual track manager in reset() later
bool RfSensor::setSlotTrackManagerName(base::String* const v)
{
    return setTrackManagerName(v);
}

//------------------------------------------------------------------------------
// processModes() -- process our mode list; make sure they are all of
//  type Mode (or derived from it)and tell them that we are their
//  master mode.
//------------------------------------------------------------------------------
bool RfSensor::processModes()
{
    bool ok = true;
    if (modes != nullptr) {
        // Make sure we have only Mode and tell all of the objects
        // that we are their master mode.
        const base::List::Item* item = modes->getFirstItem();
        while (item != nullptr) {
            base::Pair* p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            item = item->getNext();
            RfSensor* m = dynamic_cast<RfSensor*>(p->object());
            if (m != nullptr) {
                // It MUST be of type Mode
                m->setMasterMode(this);
                m->container(this);
            }
            else {
                ok = false;
                std::cerr << "RfSensor::processModes(): object is not a RfSensor!" << std::endl;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// incRange() -- increase range
//------------------------------------------------------------------------------
bool RfSensor::incRange()
{
    bool ok = false;
    if (rngIdx < nRanges) {
        rngIdx++;
        setRange( ranges[rngIdx-1] );
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// decRange() -- decrement range
//------------------------------------------------------------------------------
bool RfSensor::decRange()
{
    bool ok = false;
    if (rngIdx > 1) {
        rngIdx--;
        setRange( ranges[rngIdx-1] );
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* RfSensor::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& RfSensor::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    //"trackManagerName", // Name of the requested Track Manager (base::String)
    if (tmName != nullptr) {
        indent(sout,i+j);
        sout << "trackManagerName: " << tmName->getString() << std::endl;
    }

    //"modes",            // Submodes
    if (modes != nullptr) {
        indent(sout,i+j);
        sout << "modes: {" << std::endl;
        modes->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    //"ranges",           // Sensor ranges (nm) [vector]
    if (nRanges > 0) {
        indent(sout,i+j);
        sout << "ranges:    [ ";
        for (int ii = 0; ii < nRanges; ii++) {
            sout << ranges[ii] << " ";
        }
        sout << " ] " << std::endl;
    }

    //"initRangeIdx",     // initial range index [ 1 ... nRanges ]
    indent(sout,i+j);
    sout << "initRangeIdx: " << initRngIdx << std::endl;

    //"PRF",              // Radar Pulse Repetition  (Frequency) or (Number: hertz)
    indent(sout,i+j);
    sout << "PRF: ( Hertz " << prf << " ) " << std::endl;

    //"pulseWidth",       // Pulse Width             (Time) or (Number: Seconds)
    indent(sout,i+j);
    sout << "pulseWidth: ( Seconds " << pulseWidth << " ) " << std::endl;

    //"beamWidth",        // Beam Width              (Angle) or (Number: Radian)
    indent(sout,i+j);
    sout << "beamWidth: ( Degrees " << beamWidth*base::Angle::R2DCC << " ) " << std::endl;

    //"syncXmitWithScan", // Flag: If true, transmitter on is sync'd with the antenna scan (default: false)
    indent(sout,i+j);
    if (syncXmitWithScan) {
        sout << "syncXmitWithScan: TRUE " << std::endl;
    }
    else {
        sout << "syncXmitWithScan: FALSE " << std::endl;
    }

    indent(sout,i+j);
    sout << "//--- inherits RfSystem ------ "  << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//==============================================================================
// Class: SensorMgr
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SensorMgr,"SensorMgr")
EMPTY_SERIALIZER(SensorMgr)

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
SensorMgr::SensorMgr()
{
    STANDARD_CONSTRUCTOR()
}
//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SensorMgr::copyData(const SensorMgr& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SensorMgr::deleteData()
{
}

}
}
