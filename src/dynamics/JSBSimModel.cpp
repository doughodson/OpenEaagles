
#include "openeaagles/dynamics/JSBSimModel.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

// JSBSim model headers
#include <JSBSim/FGFDMExec.h>
#include <JSBSim/models/FGModel.h>
#include <JSBSim/models/FGAircraft.h>
#include <JSBSim/models/FGFCS.h>
#include <JSBSim/models/FGPropagate.h>
#include <JSBSim/models/FGAccelerations.h>
#include <JSBSim/models/FGAuxiliary.h>
#include <JSBSim/models/FGPropulsion.h>
#include <JSBSim/models/FGGroundReactions.h>
#include <JSBSim/initialization/FGInitialCondition.h>
#include <JSBSim/initialization/FGTrim.h>
#include <JSBSim/models/FGAtmosphere.h>
#include <JSBSim/models/FGMassBalance.h>
#include <JSBSim/models/FGAerodynamics.h>
#include <JSBSim/models/FGLGear.h>
#include <JSBSim/input_output/FGPropertyManager.h>
#include <JSBSim/models/propulsion/FGEngine.h>
#include <JSBSim/models/propulsion/FGPiston.h>
#include <JSBSim/input_output/FGGroundCallback.h>
#include <JSBSim/models/propulsion/FGEngine.h>
#include <JSBSim/models/propulsion/FGTurbine.h>
#include <JSBSim/models/propulsion/FGTurboProp.h>
#include <JSBSim/models/propulsion/FGRocket.h>
#include <JSBSim/models/propulsion/FGElectric.h>
#include <JSBSim/models/propulsion/FGNozzle.h>
#include <JSBSim/models/propulsion/FGPropeller.h>
#include <JSBSim/models/propulsion/FGTank.h>


namespace Eaagles {
namespace Dynamics {

IMPLEMENT_SUBCLASS(JSBSimModel,"JSBSimModel")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(JSBSimModel)
    "rootDir",      //  1 root directory for JSBSim models
    "model",        //  2 JSBSim model
END_SLOTTABLE(JSBSimModel)

// Map slot table to handles
BEGIN_SLOT_MAP(JSBSimModel)
    ON_SLOT(1,setRootDir,Basic::String)
    ON_SLOT(2,setModel,Basic::String)
END_SLOT_MAP()

EMPTY_SERIALIZER(JSBSimModel)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
JSBSimModel::JSBSimModel()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void JSBSimModel::initData()
{
    rootDir = 0;
    model = 0;
    fdmex = 0;
    propMgr = 0;
    pitchTrimPos         = static_cast<LCreal>(0.0);
    pitchTrimRate        = static_cast<LCreal>(0.1);
    pitchTrimSw          = static_cast<LCreal>(0.0);
    rollTrimPos          = static_cast<LCreal>(0.0);
    rollTrimRate         = static_cast<LCreal>(0.1);
    rollTrimSw           = static_cast<LCreal>(0.0);
    headingHoldOn        = false;
    altitudeHoldOn       = false;
    velocityHoldOn       = false;
    commandedHeadingDeg  = 0.0;
    commandedAltitudeFt  = 0.0;
    commandedVelocityKts = 0.0;
    hasHeadingHold       = false;
    hasVelocityHold      = false;
    hasAltitudeHold      = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy (delete) member data
//------------------------------------------------------------------------------
void JSBSimModel::copyData(const JSBSimModel& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    fdmex = 0;
    propMgr = 0;

    setRootDir( org.rootDir );
    setModel( org.model );

    pitchTrimPos = org.pitchTrimPos;
    pitchTrimRate = org.pitchTrimRate;
    pitchTrimSw = org.pitchTrimSw;
    rollTrimPos = org.rollTrimPos;
    rollTrimRate = org.rollTrimRate;
    rollTrimSw = org.rollTrimSw;
    headingHoldOn = org.headingHoldOn;
    altitudeHoldOn = org.altitudeHoldOn;
    velocityHoldOn = org.velocityHoldOn;
    commandedHeadingDeg = org.commandedHeadingDeg;
    commandedAltitudeFt = org.commandedAltitudeFt;
    commandedVelocityKts = org.commandedVelocityKts;
    hasHeadingHold = org.hasHeadingHold;
    hasVelocityHold = org.hasVelocityHold;
    hasAltitudeHold = org.hasAltitudeHold;
}

//------------------------------------------------------------------------------
// deleteData() -- delete instance of JSBSim, if any
//------------------------------------------------------------------------------
void JSBSimModel::deleteData()
{
    if (fdmex != 0) {
        delete fdmex;
        fdmex = 0;
    }
    if (propMgr != 0) {
        delete propMgr;
        propMgr = 0;
    }
}

//------------------------------------------------------------------------------
// Get Vehicle data
//------------------------------------------------------------------------------
LCreal JSBSimModel::getGload() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->GetNlf());
}

LCreal JSBSimModel::getMach() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->GetMach());
}

LCreal JSBSimModel::getAngleOfAttack() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->Getalpha());
}

LCreal JSBSimModel::getSideSlip() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->Getbeta());
}

LCreal JSBSimModel::getFlightPath() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->GetGamma());
}

LCreal JSBSimModel::getGrossWeight() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGMassBalance* MassBalance = fdmex->GetMassBalance();
    if (MassBalance == 0) return 0.0;

    return static_cast<LCreal>(MassBalance->GetWeight());
}

LCreal JSBSimModel::getFuelWt() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0.0;

    LCreal fuelWt = 0.0;
    for (unsigned int i = 0; i < Propulsion->GetNumTanks(); i++) {
        JSBSim::FGTank* tank = Propulsion->GetTank(i);
        fuelWt += static_cast<LCreal>(tank->GetContents());
    }
    return fuelWt;
}

LCreal JSBSimModel::getFuelWtMax() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0.0;

    LCreal maxFuelWt = 0.0;
    for (unsigned int i = 0; i < Propulsion->GetNumTanks(); i++) {
        JSBSim::FGTank* tank = Propulsion->GetTank(i);
        maxFuelWt += static_cast<LCreal>(tank->GetContents() / tank->GetPctFull() * 100.0);
    }
    return maxFuelWt;
}

LCreal JSBSimModel::getCalibratedAirspeed() const
{
    if (fdmex == 0) return 0.0;
    JSBSim::FGAuxiliary* Auxiliary = fdmex->GetAuxiliary();
    if (Auxiliary == 0) return 0.0;

    return static_cast<LCreal>(Auxiliary->GetVcalibratedKTS());
}

//------------------------------------------------------------------------------
// Get engine parameters: num engines, thrust, fuel flow and pla
//------------------------------------------------------------------------------
int JSBSimModel::getNumberOfEngines() const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    return Propulsion->GetNumEngines();
}

int JSBSimModel::getEngThrust(LCreal* const fn, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    if (fn == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGThruster* thruster = Propulsion->GetEngine(i)->GetThruster();
        fn[i] = static_cast<LCreal>(thruster->GetThrust());
    }
    return num;
}

int JSBSimModel::getEngRPM(LCreal* const rpm, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    if (rpm == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
        switch (eng->GetType()) {
        case JSBSim::FGEngine::etPiston:
            {
            JSBSim::FGPiston* eng1 = static_cast<JSBSim::FGPiston*>(eng);
            rpm[i] = static_cast<LCreal>(eng1->getRPM());
            }
            break;
        case JSBSim::FGEngine::etElectric:
            {
            JSBSim::FGElectric* eng1 = static_cast<JSBSim::FGElectric*>(eng);
            rpm[i] = static_cast<LCreal>(eng1->getRPM());
            }
            break;
        case JSBSim::FGEngine::etTurbine:
            {
            JSBSim::FGTurbine* eng1 = static_cast<JSBSim::FGTurbine*>(eng);
            rpm[i] = static_cast<LCreal>(eng1->GetN2());
            }
            break;
        case JSBSim::FGEngine::etTurboprop:
            {
            JSBSim::FGTurboProp* eng1 = static_cast<JSBSim::FGTurboProp*>(eng);
            rpm[i] = static_cast<LCreal>(eng1->GetN1());
            }
            break;
        case JSBSim::FGEngine::etRocket:
            rpm[i] = 0.0;
            break;
        default:
            break;
        }
    }
    return num;
}

int JSBSimModel::getEngFuelFlow(LCreal* const ff, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    // Return fuel flow rate lbs/hour
    if (ff == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
        ff[i] = static_cast<LCreal>(eng->getFuelFlow_pph());
    }
    return num;
}

int JSBSimModel::getEngOilPressure(LCreal* const oil, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    // return Engine Oil Pressure
    if (oil == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
        switch (eng->GetType()) {
        case JSBSim::FGEngine::etPiston:
            {
            JSBSim::FGPiston* eng1 = static_cast<JSBSim::FGPiston*>(eng);
            oil[i] = static_cast<LCreal>(eng1->getOilPressure_psi());
            }
            break;
        case JSBSim::FGEngine::etTurbine:
            {
            JSBSim::FGTurbine* eng1 = static_cast<JSBSim::FGTurbine*>(eng);
            oil[i] = static_cast<LCreal>(eng1->getOilPressure_psi());
            }
            break;
        case JSBSim::FGEngine::etTurboprop:
            {
            JSBSim::FGTurboProp* eng1 = static_cast<JSBSim::FGTurboProp*>(eng);
            oil[i] = static_cast<LCreal>(eng1->getOilPressure_psi());
            }
            break;
        case JSBSim::FGEngine::etRocket:
        case JSBSim::FGEngine::etElectric:
            oil[i] = 0.0;
            break;
        default:
            break;
        }
    }
    return num;
}

int JSBSimModel::getEngInletTemp(LCreal* const tmp, const int max) const
{
    // return throttle PLA (degrees)
    int n = 0;
    if (tmp != 0 && max > 0) {
        tmp[0] = 0.0;
        n = 1;
    }
    return n;
}

int JSBSimModel::getEngPLA(LCreal* const pla, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return 0;

    // return throttle PLA (percent)
    if (pla == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
        double t = FCS->GetThrottlePos(i);
        double tmin = eng->GetThrottleMin();
        double tmax = eng->GetThrottleMax();
        double throttle = (t - tmin) / (tmax - tmin) * 100.0;
        pla[i] = static_cast<LCreal>(throttle);
    }
    return num;
}

int JSBSimModel::getEngNozzle(LCreal* const pla, const int max) const
{
    if (fdmex == 0) return 0;
    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    if (Propulsion == 0) return 0;

    // return nozzle position (percentage)
    if (pla == 0 || max <= 0) {
        return 0;
    }
    int num = getNumberOfEngines();
    if (max < num) {
        num = max;
    }
    for (int i = 0; i < num; i++) {
        JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
        switch (eng->GetType()) {
        case JSBSim::FGEngine::etTurbine:
            {
            JSBSim::FGTurbine* eng1 = static_cast<JSBSim::FGTurbine*>(eng);
            pla[i] = static_cast<LCreal>(eng1->GetNozzle() * 100.0);
            }
            break;
        case JSBSim::FGEngine::etTurboprop:
            {
            JSBSim::FGTurboProp* eng1 = static_cast<JSBSim::FGTurboProp*>(eng);
            pla[i] = static_cast<LCreal>(eng1->GetNozzle() * 100.0);
            }
            break;
        case JSBSim::FGEngine::etPiston:
        case JSBSim::FGEngine::etRocket:
        case JSBSim::FGEngine::etElectric:
            pla[i] = 0.0;
            break;
        default:
            break;
        }
    }
    return num;
}

//------------------------------------------------------------------------------
// setControlStickRollInput(Roll) --  Control inputs: normalized
//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void JSBSimModel::setControlStickRollInput(const LCreal roll)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    FCS->SetDaCmd(roll);
}


//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void JSBSimModel::setControlStickPitchInput(const LCreal pitch)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    FCS->SetDeCmd(-pitch);
}

//------------------------------------------------------------------------------
// int setThrottles(positions,num) -- Set throttle positions
//
//    positions -> Array of throttle positions
//      (for each throttle)
//              < 0.0       -> Cutoff
//              0.0         -> Idle
//              1.0         -> MIL
//              2.0         -> A/B
//    num -> number of throttle positions to get/set
//    returns the actual number of throttle positions
//------------------------------------------------------------------------------
int JSBSimModel::setThrottles(const LCreal* const positions, const int num)
{
    if (fdmex == 0) return 0;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return 0;

    if (positions == 0 || num <= 0) {
        return 0;
    }
    int n = getNumberOfEngines();
    if (num < n) {
        n = num;
    }
    for (int i = 0; i < n; i++) {
        double pos = positions[i] * 0.5; // CGB * 100.0;
        if (pos > 1.0) {
            pos = 1.0;
        }
        FCS->SetThrottleCmd(i, pos);
    }
    return n;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void JSBSimModel::setTrimSwitchRollInput(const LCreal rollTrim)
{
    rollTrimSw = rollTrim;
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void JSBSimModel::setTrimSwitchPitchInput(const LCreal pitchTrim)
{
    pitchTrimSw = -pitchTrim;
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void JSBSimModel::setRudderPedalInput(const LCreal pedal)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    FCS->SetDrCmd(-pedal);
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
LCreal JSBSimModel::getLandingGearPosition() const
{
    if (fdmex == 0) return 0;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return 0;

    return static_cast<LCreal>(FCS->GetGearPos() * 100.0);
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool JSBSimModel::isWeightOnWheels() const
{
    if (fdmex == 0) return 0;
    JSBSim::FGGroundReactions* GroundReactions = fdmex->GetGroundReactions();
    if (GroundReactions == 0) return 0;

    int Ngear = GroundReactions->GetNumGearUnits();
    bool wow = false;
    for (int i = 0; i < Ngear; i++) {
        JSBSim::FGLGear* gear = GroundReactions->GetGearUnit(i);
        if (gear->GetWOW()) {
            wow = true;
        }
    }
    return wow;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> handle up;  1 -> handle down
//------------------------------------------------------------------------------
void JSBSimModel::setGearHandleSwitch(const LCreal sw)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    FCS->SetGearCmd(sw);
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
LCreal JSBSimModel::getSpeedBrakePosition() const
{
    if (fdmex == 0) return 0;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return 0;

    return static_cast<LCreal>(FCS->GetDsbPos(JSBSim::ofNorm) * 100.0);
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void JSBSimModel::setSpeedBrakesSwitch(const LCreal sw)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    if (sw > 0.0) {
        FCS->SetDsbCmd(100.0);
    }
    else if (sw < 0.0) {
        FCS->SetDsbCmd(0.0);
    }
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void JSBSimModel::setBrakes(const LCreal left, const LCreal right)
{
    if (fdmex == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;

    FCS->SetLBrake(left);
    FCS->SetRBrake(right);
    FCS->SetCBrake(0.0);
}

//------------------------------------------------------------------------------
// dynamics() -- update player's vehicle dynamics
//------------------------------------------------------------------------------
void JSBSimModel::dynamics(const LCreal dt)
{

    // Get our Player (must have one!)
    Simulation::Player* p = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
    if (p == 0) return;

    if (fdmex == 0) return;

    JSBSim::FGPropagate* Propagate = fdmex->GetPropagate();
    if (Propagate == 0) return;
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (FCS == 0) return;
    JSBSim::FGAccelerations* Accelerations = fdmex->GetAccelerations();
    if (Accelerations == 0) return;

    pitchTrimPos += pitchTrimRate * pitchTrimSw * dt;
    if (pitchTrimPos > 1.0) {
        pitchTrimPos = 1.0;
    } else if (pitchTrimPos < -1.0) {
        pitchTrimPos = -1.0;
    }
    FCS->SetPitchTrimCmd(pitchTrimPos);

    rollTrimPos += rollTrimRate * rollTrimSw * dt;
    if (rollTrimPos > 1.0) {
        rollTrimPos = 1.0;
    } else if (rollTrimPos < -1.0) {
        rollTrimPos = -1.0;
    }
    FCS->SetRollTrimCmd(rollTrimPos);

    fdmex->Setdt(dt);
    // ---
    // Pass flags & Data
    // ---
#if 0
    // CGB TBD
    if (isFrozen() || dt == 0) freeze = -1;
    else freeze = 0;

    if (isPositionFrozen()) posFrz = -1;
    else posFrz = 0;

    if (isAltitudeFrozen()) altFrz = -1;
    else altFrz = 0;

    if (isFuelFrozen()) fuelFrz = -1;
    else fuelFrz = 0;

    rw_elev = getTerrainElevationFt();
#endif

    // ---
    // Run the model
    // ---
    fdmex->Run();     //loop JSBSim once w/o integrating

    // ---
    // Set values for Player & AirVehicle interfaces
    //    (Note: Player::dynamics() computes the new position)
    // ---
    p->setAltitude(Basic::Distance::FT2M * Propagate->GetAltitudeASL(), true);
    p->setVelocity(static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetVel(JSBSim::FGJSBBase::eNorth)),
                   static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetVel(JSBSim::FGJSBBase::eEast)),
                   static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetVel(JSBSim::FGJSBBase::eDown)));
    p->setVelocityBody(static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetUVW(1)),
                       static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetUVW(2)),
                       static_cast<LCreal>(Basic::Distance::FT2M * Propagate->GetUVW(3)));
//    LCreal accX = Basic::Distance::FT2M * Propagate->GetUVWdot(1);
//    LCreal accY = Basic::Distance::FT2M * Propagate->GetUVWdot(2);
//    LCreal accZ = Basic::Distance::FT2M * Propagate->GetUVWdot(3);
    const JSBSim::FGMatrix33& Tb2l = Propagate->GetTb2l();
    const JSBSim::FGColumnVector3& vUVWdot = Accelerations->GetUVWdot();

    p->setEulerAngles(static_cast<LCreal>(Propagate->GetEuler(JSBSim::FGJSBBase::ePhi)),
                      static_cast<LCreal>(Propagate->GetEuler(JSBSim::FGJSBBase::eTht)),
                      static_cast<LCreal>(Propagate->GetEuler(JSBSim::FGJSBBase::ePsi)));
    p->setAngularVelocities(static_cast<LCreal>(Propagate->GetPQR(JSBSim::FGJSBBase::eP)),
                            static_cast<LCreal>(Propagate->GetPQR(JSBSim::FGJSBBase::eQ)),
                            static_cast<LCreal>(Propagate->GetPQR(JSBSim::FGJSBBase::eR)));

    JSBSim::FGColumnVector3 vVeldot = Tb2l * vUVWdot;
    p->setAcceleration(static_cast<LCreal>(Basic::Distance::FT2M * vVeldot(1)),
                       static_cast<LCreal>(Basic::Distance::FT2M * vVeldot(2)),
                       static_cast<LCreal>(Basic::Distance::FT2M * vVeldot(3)));

    //std::printf("(%6.1f, %6.1f, %6.1f)   vel=%8.1f   alt=%8.1f alt2=%8.1f\n", acData->phi, acData->theta, acData->psi, acData->vp, acData->hp, (M2FT*getAltitude()) );
    //std::printf("f=%6.1f p=%6.1f, qa=%6.1f, a=%6.1f, g=%6.1f\n", hotasIO->pitchForce, acData->theta, acData->qa, acData->alpha, acData->gamma );

        //{
        // std::cout << "JSBSim: ---------------------------------" << std::endl;
        // osg::Vec4 fq;
        // fq.set(acData->e1, acData->e2, acData->e4, acData->e4);
        // osg::Matrix m2;
        // m2.set(
        //    acData->l1, acData->l2, acData->l3, 0,
        //    acData->m1, acData->m2, acData->m3, 0,
        //    acData->n1, acData->n2, acData->n3, 0,
        //    0,          0,          0,          1
        //    );
        // std::printf("Eaagles*EA: (%6.1f, %6.1f, %6.1f)\n", getRollD(), getPitchD(), getHeadingD());
        // osg::Matrix m0 = getRotationalMatrix();
        // osg::Quat q0 = getQuaternions();
        // setRotationalMatrix(m2);
        // //setQuaternions(fq);
        // osg::Quat eq = getQuaternions();
        // osg::Matrix m1 = getRotationalMatrix();
        // std::printf("Eaagles EA: (%6.1f, %6.1f, %6.1f)\n", getRollD(), getPitchD(), getHeadingD());
        // std::printf("JSBSim    EA: (%6.1f, %6.1f, %6.1f)\n", acData->phi, acData->theta, acData->psi);
        // std::printf("Eaagles* Q: (%6.3f, %6.3f, %6.3f, %6.3f)\n", q0[0], q0[1], q0[2], q0[3]);
        // std::printf("Eaagles  Q: (%6.3f, %6.3f, %6.3f, %6.3f)\n", eq[0], eq[1], eq[2], eq[3]);
        // std::printf("JSBSim     Q: (%6.3f, %6.3f, %6.3f, %6.3f)\n", fq[0], fq[1], fq[2], fq[3]);
        // std::printf("Eaagles*mm: (%6.3f, %6.3f, %6.3f, %6.3f)\n", m0(0,0), m0(0,1), m0(0,2), m0(0,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m0(1,0), m0(1,1), m0(1,2), m0(1,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m0(2,0), m0(2,1), m0(2,2), m0(2,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m0(3,0), m0(3,1), m0(3,2), m0(3,3));
        // std::printf("Eaagles mm: (%6.3f, %6.3f, %6.3f, %6.3f)\n", m1(0,0), m1(0,1), m1(0,2), m1(0,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m1(1,0), m1(1,1), m1(1,2), m1(1,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m1(2,0), m1(2,1), m1(2,2), m1(2,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m1(3,0), m1(3,1), m1(3,2), m1(3,3));
        // std::printf("JSBSim    mm: (%6.3f, %6.3f, %6.3f, %6.3f)\n", m2(0,0), m2(0,1), m2(0,2), m2(0,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m2(1,0), m2(1,1), m2(1,2), m2(1,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m2(2,0), m2(2,1), m2(2,2), m2(2,3));
        // std::printf("            (%6.3f, %6.3f, %6.3f, %6.3f)\n", m2(3,0), m2(3,1), m2(3,2), m2(3,3));
        //}

    // CGB Set Autopilot Properties directly for now
    JSBSim::FGPropertyManager* propMgr = fdmex->GetPropertyManager();
    if (propMgr != 0) {
        JSBSim::FGPropertyNode* propNode = propMgr->GetNode();
        if (propNode != 0) {
            if (hasHeadingHold) {
                propNode->SetBool("ap/heading_hold", isHeadingHoldOn());
                propNode->SetDouble("ap/heading_setpoint", getCommandedHeadingD());
            }
            if (hasVelocityHold) {
                propNode->SetBool("ap/airspeed_hold", isVelocityHoldOn());
                propNode->SetDouble("ap/airspeed_setpoint", getCommandedVelocityKts());
            }
            if (hasAltitudeHold) {
                propNode->SetBool("ap/altitude_hold", isAltitudeHoldOn());
                propNode->SetDouble("ap/altitude_setpoint", (getCommandedAltitude() * Basic::Distance::M2FT) );
            }
        }
    }

    BaseClass::dynamics(dt);
}



//------------------------------------------------------------------------------
// reset() --
//------------------------------------------------------------------------------
void JSBSimModel::reset()
{
    BaseClass::reset();

    pitchTrimPos  = static_cast<LCreal>(0.0);
    pitchTrimRate = static_cast<LCreal>(0.1);
    pitchTrimSw   = static_cast<LCreal>(0.0);
    rollTrimPos   = static_cast<LCreal>(0.0);
    rollTrimRate  = static_cast<LCreal>(0.1);
    rollTrimSw    = static_cast<LCreal>(0.0);

    // Get our Player (must have one!)
    Simulation::Player* p = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
    if (p == 0) return;

    // must have strings set
    if (rootDir == 0 || model == 0) return;

    // Must also have the JSBSim object
    if (fdmex == 0) {
        // must have a JSBSim property manager
        if (propMgr == 0) {
            propMgr = new JSBSim::FGPropertyManager();
        }
        fdmex = new JSBSim::FGFDMExec(propMgr);

        std::string RootDir(rootDir->getString());
        fdmex->SetAircraftPath(RootDir + "aircraft");
        fdmex->SetEnginePath(RootDir + "engine");
        fdmex->SetSystemsPath(RootDir + "systems"); // JSBSim-1.0 or after only

        fdmex->LoadModel(model->getString());
        JSBSim::FGPropertyManager* propMgr = fdmex->GetPropertyManager();
        if (propMgr != 0) {
            hasHeadingHold = propMgr->HasNode("ap/heading_hold") && propMgr->HasNode("ap/heading_setpoint");
            hasVelocityHold = propMgr->HasNode("ap/airspeed_hold") && propMgr->HasNode("ap/airspeed_setpoint");
            hasAltitudeHold = propMgr->HasNode("ap/altitude_hold") && propMgr->HasNode("ap/altitude_setpoint");
#if 0
            // CGB this isn't working for some reason. I set the values directly in "dynamics" for now.
            if (hasHeadingHold) {
                propMgr->Tie("ap/heading_hold", this, &JSBSimModel::isHeadingHoldOn);
                propMgr->Tie("ap/heading_setpoint", this, &JSBSimModel::getCommandedHeadingD);
            }
            if (hasVelocityHold) {
                propMgr->Tie("ap/airspeed_hold", this, &JSBSimModel::isVelocityHoldOn);
                propMgr->Tie("ap/airspeed_setpoint", this, &JSBSimModel::getCommandedVelocityKts);
            }
            if (hasAltitudeHold) {
                propMgr->Tie("ap/altitude_hold", this, &JSBSimModel::isAltitudeHoldOn);
                propMgr->Tie("ap/altitude_setpoint", this, &JSBSimModel::getCommandedAltitude * Basic::Distance::M2FT);
            }
#endif
        }
    }

#if 0
    // CGB TBD
    reset = 0;
    freeze = 0;
#endif
    JSBSim::FGInitialCondition* fgic = fdmex->GetIC();
    if (fgic == 0) return;

    fgic->SetAltitudeASLFtIC(Basic::Distance::M2FT * p->getAltitude());

#if 0
    fgic->SetTrueHeadingDegIC(Basic::Angle::R2DCC * p->getHeading());
    fgic->SetRollAngleDegIC(Basic::Angle::R2DCC * p->getRoll());
    fgic->SetPitchAngleDegIC(Basic::Angle::R2DCC * p->getPitch());
#else
    fgic->SetPsiDegIC(Basic::Angle::R2DCC * p->getHeading());
    fgic->SetPhiDegIC(Basic::Angle::R2DCC * p->getRoll());
    fgic->SetThetaDegIC(Basic::Angle::R2DCC * p->getPitch());
#endif
    fgic->SetVtrueKtsIC(Basic::Distance::M2NM * p->getTotalVelocity() * 3600.0f);
    fgic->SetLatitudeDegIC(p->getInitLatitude());
    fgic->SetLongitudeDegIC(p->getInitLongitude());

    JSBSim::FGPropulsion* Propulsion = fdmex->GetPropulsion();
    JSBSim::FGFCS* FCS = fdmex->GetFCS();
    if (Propulsion != 0 && FCS != 0) {
        Propulsion->SetMagnetos(3);
        for (unsigned int i=0; i < Propulsion->GetNumEngines(); i++) {
            FCS->SetMixtureCmd(i, 1.0);
            FCS->SetThrottleCmd(i, 1.0);
            FCS->SetPropAdvanceCmd(i, 1.0);
            FCS->SetMixturePos(i, 1.0);
            FCS->SetThrottlePos(i, 1.0);
            FCS->SetPropAdvance(i, 1.0);
            JSBSim::FGEngine* eng = Propulsion->GetEngine(i);
            eng->SetRunning(true);
            JSBSim::FGThruster* thruster = eng->GetThruster();
            thruster->SetRPM(1000.0);
        }
        Propulsion->SetFuelFreeze(p->isFuelFrozen());
        Propulsion->InitRunning(-1);     // -1 refers to "All Engines"
        Propulsion->GetSteadyState();
   }
   fdmex->RunIC();
}

//------------------------------------------------------------------------------
// Slot access functions
//------------------------------------------------------------------------------

// Sets root directory for JSBSim models
bool JSBSimModel::setRootDir(const Basic::String* const dir)
{
    if (rootDir != 0) {
       rootDir->unref();
       rootDir = 0;
    }
    if (dir != 0) {
       rootDir = dir->clone();
    }
    return true;
}

// Sets JSBSim model
bool JSBSimModel::setModel(const Basic::String* const mdl)
{
    if (model != 0) {
       model->unref();
       model = 0;
    }
    if (mdl != 0) {
       model = mdl->clone();
    }
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* JSBSimModel::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// commanded heading
//------------------------------------------------------------------------------
bool JSBSimModel::isHeadingHoldOn() const
{
    return hasHeadingHold && headingHoldOn;
}

double JSBSimModel::getCommandedHeadingD() const
{
    return commandedHeadingDeg;
}

bool JSBSimModel::setHeadingHoldOn(const bool b)
{
    if (hasHeadingHold) {
        headingHoldOn = b;
    }
    return hasHeadingHold;
}

bool JSBSimModel::setCommandedHeadingD(const double h, const double, const double)
{
    commandedHeadingDeg = h;
    return hasHeadingHold;
}


//------------------------------------------------------------------------------
// commanded velocity
//------------------------------------------------------------------------------
bool JSBSimModel::isVelocityHoldOn() const
{
    return hasVelocityHold && velocityHoldOn;
}

double JSBSimModel::getCommandedVelocityKts() const
{
    return commandedVelocityKts;
}

bool JSBSimModel::setVelocityHoldOn(const bool b)
{
    if (hasVelocityHold) {
        velocityHoldOn = b;
    }
    return hasVelocityHold;
}

bool JSBSimModel::setCommandedVelocityKts(const double v, const double vNps)
{
    commandedVelocityKts = v;
    return hasVelocityHold;
}


//------------------------------------------------------------------------------
// commanded altitude
//------------------------------------------------------------------------------
bool JSBSimModel::isAltitudeHoldOn() const
{
    return hasAltitudeHold && altitudeHoldOn;
}

double JSBSimModel::getCommandedAltitude() const
{
    return commandedAltitudeFt * Basic::Distance::FT2M;
}

bool JSBSimModel::setAltitudeHoldOn(const bool b)
{
    if (hasAltitudeHold) {
        altitudeHoldOn = b;
    }
    return hasAltitudeHold;
}

bool JSBSimModel::setCommandedAltitude(const double a, const double, const double)
{
    commandedAltitudeFt = a * Basic::Distance::M2FT;
    return hasAltitudeHold;
}

} // End Dynamics namespace
} // End Eaagles namespace

