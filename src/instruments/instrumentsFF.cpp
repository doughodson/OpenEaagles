//--------------------------------------------------------------------------------
// Form function for the 'instruments' library
//--------------------------------------------------------------------------------
#include "openeaagles/instruments/instrumentsFF.h"

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



namespace Eaagles {
namespace Instruments {

Basic::Object* instrumentsFormFunc(const char* formname)
{
    // start out by checking the basicGL formFuncs
    Basic::Object* newform = 0;
    
    // Here is the analog dial and its components
    
    // Instrument
    if ( strcmp(formname, Instrument::getFormName()) == 0 ) {
        newform = new Instrument;
    } 
    // Analog Dial
    else if ( strcmp(formname, AnalogDial::getFormName()) == 0 ) {
        newform = new AnalogDial;
    }
    // Tick Marks for the analog dial
    else if ( strcmp(formname, DialTickMarks::getFormName()) == 0 ) {
        newform = new DialTickMarks;
    }
    // Arc Segments for the analog dial
    else if ( strcmp(formname, DialArcSegment::getFormName()) == 0 ) {
        newform = new DialArcSegment;
    }
    // Dial Pointer
    else if ( strcmp(formname, DialPointer::getFormName()) == 0 ) {
        newform = new DialPointer;
    }
    // CompassRose
    else if ( strcmp(formname, CompassRose::getFormName()) == 0 ) {
        newform = new CompassRose;
    }
    // Bearing Pointer
    else if ( strcmp(formname, BearingPointer::getFormName()) == 0 ) {
        newform = new BearingPointer;
    }        
    // AltitudeDial
    else if ( strcmp(formname, AltitudeDial::getFormName()) == 0 ) {
        newform = new AltitudeDial;
    }
    // GMeterDial
    else if ( strcmp(formname, GMeterDial::getFormName()) == 0 ) {
        newform = new GMeterDial;
    }    
    // Here is the analog gauge and its pieces
    // AnalogGauge
    else if ( strcmp(formname, AnalogGauge::getFormName()) == 0 ) {
        newform = new AnalogGauge;
    }
    else if ( strcmp(formname, GaugeSlider::getFormName()) == 0 ) {
        newform = new GaugeSlider;
    }
    // Tape
    else if ( strcmp(formname, Tape::getFormName()) == 0 ) {
        newform = new Tape;
    }
    // digital AOA gauge
    else if ( strcmp(formname, AoAIndexer::getFormName()) == 0 ) {
        newform = new AoAIndexer;
    }    
    // Tick Marks (horizontal and vertical)
    else if ( strcmp(formname, TickMarks::getFormName()) == 0 ) {
        newform = new TickMarks;
    }    
    // Landing Gear
    else if ( strcmp(formname, LandingGear::getFormName()) == 0 ) {
        newform = new LandingGear;
    }
    // Landing Lights
    else if ( strcmp(formname, LandingLight::getFormName()) == 0 ) {
        newform = new LandingLight;
    }
    // EngPage
    else if ( strcmp(formname, EngPage::getFormName()) == 0 ) {
        newform = new EngPage;
    }          
    // Button
    else if (strcmp(formname, Button::getFormName()) == 0 ) {
        newform = new Button;
    }      
    // Push Button
	 else if (strcmp(formname, PushButton::getFormName()) == 0 ) {
        newform = new PushButton;
    }      
    // Rotary Switch
    else if (strcmp(formname, RotarySwitch::getFormName()) == 0 ) {
        newform = new RotarySwitch;
    }      
    // Knob
    else if (strcmp(formname, Knob::getFormName()) == 0 ) {
        newform = new Knob;
    }      
    // Switch
    else if (strcmp(formname, Switch::getFormName()) == 0 ) {
        newform = new Switch;
    }      
    // Hold Switch
    else if (strcmp(formname, SolenoidSwitch::getFormName()) == 0 ) {
        newform = new SolenoidSwitch;
    }      
    // Hold Button
    else if (strcmp(formname, SolenoidButton::getFormName()) == 0 ) {
        newform = new SolenoidButton;
    }      
    // Adi
    else if (strcmp(formname, Adi::getFormName()) == 0 ) {
        newform = new Adi;
    }      
    // Ghost Horizon
    else if (strcmp(formname, GhostHorizon::getFormName()) == 0 ) {
        newform = new GhostHorizon;
    }
    // Eadi3D
    else if (strcmp(formname, Eadi3DPage::getFormName()) == 0 ) {
        newform = new Eadi3DPage;
    }
    // TestEadiPage
//    else if (strcmp(formname, TestEadiPage::getFormName()) == 0 ) {
//        newform = new TestEadiPage;
//    }      
     
    
    return newform;
}
}; // end of Instruments namespace  
}; // end of Eaagles namespace
