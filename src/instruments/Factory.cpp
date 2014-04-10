//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/instruments/Factory.h"

#include "openeaagles/basic/Object.h"

// Top Level objects
#include "openeaagles/instruments/Instrument.h"

// Analog Dial components
#include "openeaagles/instruments/dials/AnalogDial.h"
#include "openeaagles/instruments/dials/DialArcSegment.h"
#include "openeaagles/instruments/dials/DialTickMarks.h"
#include "openeaagles/instruments/dials/DialPointer.h"
#include "openeaagles/instruments/dials/AltitudeDial.h"
#include "openeaagles/instruments/dials/GMeterDial.h"

// Analog Gauge components
#include "openeaagles/instruments/gauges/AnalogGauge.h"
#include "openeaagles/instruments/gauges/GaugeSlider.h"
#include "openeaagles/instruments/gauges/AoaIndexer.h"
#include "openeaagles/instruments/gauges/TickMarks.h"
#include "openeaagles/instruments/gauges/Tape.h"

// Generic compass rose and bearing pointer, and dynamic maps
#include "openeaagles/instruments/maps/CompassRose.h"
#include "openeaagles/instruments/maps/BearingPointer.h"

// Generic Landing gear
#include "openeaagles/instruments/landingGear/LandingGear.h"
#include "openeaagles/instruments/landingGear/LandingLight.h"

// Generic engine page
#include "openeaagles/instruments/eng/EngPage.h"

// Buttons
#include "openeaagles/instruments/buttons/Button.h"
#include "openeaagles/instruments/buttons/PushButton.h"
#include "openeaagles/instruments/buttons/RotarySwitch.h" 
#include "openeaagles/instruments/buttons/Knob.h" 
#include "openeaagles/instruments/buttons/Switch.h"
#include "openeaagles/instruments/buttons/SolenoidSwitch.h"

// Adi
#include "openeaagles/instruments/adi/Adi.h"
 
// Ghost Horizon
#include "openeaagles/instruments/adi/GhostHorizon.h"

// Eadi3D
#include "openeaagles/instruments/eadi3D/Eadi3DPage.h"

#include <cstring>

namespace Eaagles {
namespace Instruments {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // Instrument
    if ( std::strcmp(name, Instrument::getFactoryName()) == 0 ) {
        obj = new Instrument;
    }
    // Analog Dial
    else if ( std::strcmp(name, AnalogDial::getFactoryName()) == 0 ) {
        obj = new AnalogDial;
    }
    // Tick Marks for the analog dial
    else if ( std::strcmp(name, DialTickMarks::getFactoryName()) == 0 ) {
        obj = new DialTickMarks;
    }
    // Arc Segments for the analog dial
    else if ( std::strcmp(name, DialArcSegment::getFactoryName()) == 0 ) {
        obj = new DialArcSegment;
    }
    // Dial Pointer
    else if ( std::strcmp(name, DialPointer::getFactoryName()) == 0 ) {
        obj = new DialPointer;
    }
    // CompassRose
    else if ( std::strcmp(name, CompassRose::getFactoryName()) == 0 ) {
        obj = new CompassRose;
    }
    // Bearing Pointer
    else if ( std::strcmp(name, BearingPointer::getFactoryName()) == 0 ) {
        obj = new BearingPointer;
    }
    // AltitudeDial
    else if ( std::strcmp(name, AltitudeDial::getFactoryName()) == 0 ) {
        obj = new AltitudeDial;
    }
    // GMeterDial
    else if ( std::strcmp(name, GMeterDial::getFactoryName()) == 0 ) {
        obj = new GMeterDial;
    }    
    // Here is the analog gauge and its pieces
    // AnalogGauge
    else if ( std::strcmp(name, AnalogGauge::getFactoryName()) == 0 ) {
        obj = new AnalogGauge;
    }
    else if ( std::strcmp(name, GaugeSlider::getFactoryName()) == 0 ) {
        obj = new GaugeSlider;
    }
    // Tape
    else if ( std::strcmp(name, Tape::getFactoryName()) == 0 ) {
        obj = new Tape;
    }
    // digital AOA gauge
    else if ( std::strcmp(name, AoAIndexer::getFactoryName()) == 0 ) {
        obj = new AoAIndexer;
    }
    // Tick Marks (horizontal and vertical)
    else if ( std::strcmp(name, TickMarks::getFactoryName()) == 0 ) {
        obj = new TickMarks;
    }
    // Landing Gear
    else if ( std::strcmp(name, LandingGear::getFactoryName()) == 0 ) {
        obj = new LandingGear;
    }
    // Landing Lights
    else if ( std::strcmp(name, LandingLight::getFactoryName()) == 0 ) {
        obj = new LandingLight;
    }
    // EngPage
    else if ( std::strcmp(name, EngPage::getFactoryName()) == 0 ) {
        obj = new EngPage;
    }
    // Button
    else if (std::strcmp(name, Button::getFactoryName()) == 0 ) {
        obj = new Button;
    }
    // Push Button
    else if (std::strcmp(name, PushButton::getFactoryName()) == 0 ) {
        obj = new PushButton;
    }
    // Rotary Switch
    else if (std::strcmp(name, RotarySwitch::getFactoryName()) == 0 ) {
        obj = new RotarySwitch;
    }
    // Knob
    else if (std::strcmp(name, Knob::getFactoryName()) == 0 ) {
        obj = new Knob;
    }
    // Switch
    else if (std::strcmp(name, Switch::getFactoryName()) == 0 ) {
        obj = new Switch;
    }
    // Hold Switch
    else if (std::strcmp(name, SolenoidSwitch::getFactoryName()) == 0 ) {
        obj = new SolenoidSwitch;
    }
    // Hold Button
    else if (std::strcmp(name, SolenoidButton::getFactoryName()) == 0 ) {
        obj = new SolenoidButton;
    }
    // Adi
    else if (std::strcmp(name, Adi::getFactoryName()) == 0 ) {
        obj = new Adi;
    }
    // Ghost Horizon
    else if (std::strcmp(name, GhostHorizon::getFactoryName()) == 0 ) {
        obj = new GhostHorizon;
    }
    // Eadi3D
    else if (std::strcmp(name, Eadi3DPage::getFactoryName()) == 0 ) {
        obj = new Eadi3DPage;
    }

    return obj;
}

}  // end namespace Instruments
}  // end namespace Eaagles

