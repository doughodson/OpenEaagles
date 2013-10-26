//------------------------------------------------------------------------------
// Form function for the 'basic' library
//------------------------------------------------------------------------------

#include "openeaagles/basic/basicFF.h"

#include "openeaagles/basic/Logger.h"
#include "openeaagles/basic/FileReader.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/basic/Transforms.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/Timers.h"

// Numbers
#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Complex.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Decibel.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/Operators.h"

// Net handlers
#include "openeaagles/basic/nethandlers/BroadcastHandler.h"
#include "openeaagles/basic/nethandlers/MulticastHandler.h"
#include "openeaagles/basic/nethandlers/UdpHandler.h"
#include "openeaagles/basic/nethandlers/TcpHandler.h"

// Colors
#include "openeaagles/basic/Cie.h"
#include "openeaagles/basic/Cmy.h"
#include "openeaagles/basic/Hls.h"
#include "openeaagles/basic/Hsv.h"
#include "openeaagles/basic/Hsva.h"
#include "openeaagles/basic/Rgb.h"
#include "openeaagles/basic/Rgba.h"
#include "openeaagles/basic/Yiq.h"

// Units
#include "openeaagles/basic/units/AngularVelocity.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Areas.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Energies.h"
#include "openeaagles/basic/units/Forces.h"
#include "openeaagles/basic/units/Frequencies.h"
#include "openeaagles/basic/units/LinearVelocity.h"
#include "openeaagles/basic/units/Masses.h"
#include "openeaagles/basic/units/Powers.h"
#include "openeaagles/basic/units/Times.h"

// Random number generator and distributions
#include "openeaagles/basic/Rng.h"
#include "openeaagles/basic/distributions/Exponential.h"
#include "openeaagles/basic/distributions/Lognormal.h"
#include "openeaagles/basic/distributions/Pareto.h"
#include "openeaagles/basic/distributions/Uniform.h"

// Others
#include "openeaagles/basic/Functions.h"
#include "openeaagles/basic/EarthModel.h"
#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/ThreadPool.h"

// Ubf
#include "openeaagles/basic/ubf/Agent.h"
#include "openeaagles/basic/ubf/Arbiter.h"


namespace Eaagles {
namespace Basic {

Object* basicFormFunc(const char* formname)
{
    Object* newform = 0;

    // Numbers
    if ( strcmp(formname, Number::getFormName()) == 0 ) {
        newform = new Number();
    }
    else if ( strcmp(formname, Complex::getFormName()) == 0 ) {
        newform = new Complex();
    }
    else if ( strcmp(formname, Integer::getFormName()) == 0 ) {
        newform = new Integer();
    }
    else if ( strcmp(formname, Float::getFormName()) == 0 ) {
        newform = new Float();
    }
    else if ( strcmp(formname, Boolean::getFormName()) == 0 ) {
        newform = new Boolean();
    }
    else if ( strcmp(formname, Decibel::getFormName()) == 0 ) {
        newform = new Decibel();
    }
    else if ( strcmp(formname, LatLon::getFormName()) == 0 ) {
        newform = new LatLon();
    }
    else if ( strcmp(formname, Add::getFormName()) == 0 ) {
        newform = new Add();
    }
    else if ( strcmp(formname, Subtract::getFormName()) == 0 ) {
        newform = new Subtract();
    }
    else if ( strcmp(formname, Multiply::getFormName()) == 0 ) {
        newform = new Multiply();
    }
    else if ( strcmp(formname, Divide::getFormName()) == 0 ) {
        newform = new Divide();
    }
 
    // Components
    else if ( strcmp(formname, FileReader::getFormName()) == 0 ) {
        newform = new FileReader();
    }
    else if ( strcmp(formname, Logger::getFormName()) == 0 ) {
        newform = new Logger();
    }
    else if ( strcmp(formname, Statistic::getFormName()) == 0 ) {
        newform = new Statistic();
    }

    // Tranformations
    else if ( strcmp(formname, Translation::getFormName()) == 0 ) {
        newform = new Translation();
    }
    else if ( strcmp(formname, Rotation::getFormName()) == 0 ) {
        newform = new Rotation();
    }
    else if ( strcmp(formname, Scale::getFormName()) == 0 ) {
        newform = new Scale();
    }
    
    // Tables
    else if ( strcmp(formname, Table1::getFormName()) == 0 ) {
        newform = new Table1();
    }
    else if ( strcmp(formname, Table2::getFormName()) == 0 ) {
        newform = new Table2();
    }
    else if ( strcmp(formname, Table3::getFormName()) == 0 ) {
        newform = new Table3();
    }
    else if ( strcmp(formname, Table4::getFormName()) == 0 ) {
        newform = new Table4();
    }
    else if ( strcmp(formname, Table5::getFormName()) == 0 ) {
        newform = new Table5();
    }

    // Timers
    else if ( strcmp(formname, UpTimer::getFormName()) == 0 ) {
        newform = new UpTimer();
    }
    else if ( strcmp(formname, DownTimer::getFormName()) == 0 ) {
        newform = new DownTimer();
    }

    // Units: Angles
    else if ( strcmp(formname, Degrees::getFormName()) == 0 ) {
        newform = new Degrees();
    }
    else if ( strcmp(formname, Radians::getFormName()) == 0 ) {
        newform = new Radians();
    }
    else if ( strcmp(formname, Semicircles::getFormName()) == 0 ) {
        newform = new Semicircles();
    }

    // Units: Areas
    else if ( strcmp(formname, SquareMeters::getFormName()) == 0 ) {
        newform = new SquareMeters();
    }
    else if ( strcmp(formname, SquareFeet::getFormName()) == 0 ) {
        newform = new SquareFeet();
    }
    else if ( strcmp(formname, SquareInches::getFormName()) == 0 ) {
        newform = new SquareInches();
    }
    else if ( strcmp(formname, SquareYards::getFormName()) == 0 ) {
        newform = new SquareYards();
    }
    else if ( strcmp(formname, SquareMiles::getFormName()) == 0 ) {
        newform = new SquareMiles();
    }
    else if ( strcmp(formname, SquareCentiMeters::getFormName()) == 0 ) {
        newform = new SquareCentiMeters();
    }
    else if ( strcmp(formname, SquareMilliMeters::getFormName()) == 0 ) {
        newform = new SquareMilliMeters();
    }
    else if ( strcmp(formname, SquareKiloMeters::getFormName()) == 0 ) {
        newform = new SquareKiloMeters();
    }
    else if ( strcmp(formname, DecibelSquareMeters::getFormName()) == 0 ) {
        newform = new DecibelSquareMeters();
    }

    // Units: Distances
    else if ( strcmp(formname, Meters::getFormName()) == 0 ) {
        newform = new Meters();
    }
    else if ( strcmp(formname, CentiMeters::getFormName()) == 0 ) {
        newform = new CentiMeters();
    }
    else if ( strcmp(formname, MicroMeters::getFormName()) == 0 ) {
        newform = new MicroMeters();
    }
    else if ( strcmp(formname, Microns::getFormName()) == 0 ) {
        newform = new Microns();
    }
    else if ( strcmp(formname, KiloMeters::getFormName()) == 0 ) {
        newform = new KiloMeters();
    }
    else if ( strcmp(formname, Inches::getFormName()) == 0 ) {
        newform = new Inches();
    }
    else if ( strcmp(formname, Feet::getFormName()) == 0 ) {
        newform = new Feet();
    }
    else if ( strcmp(formname, NauticalMiles::getFormName()) == 0 ) {
        newform = new NauticalMiles();
    }
    else if ( strcmp(formname, StatuteMiles::getFormName()) == 0 ) {
        newform = new StatuteMiles();
    }

    // Units: Energies
    else if ( strcmp(formname, KiloWattHours::getFormName()) == 0 ) {
        newform = new KiloWattHours();
    }
    else if ( strcmp(formname, BTUs::getFormName()) == 0 ) {
        newform = new BTUs();
    }
    else if ( strcmp(formname, Calories::getFormName()) == 0 ) {
        newform = new Calories();
    }
    else if ( strcmp(formname, FootPounds::getFormName()) == 0 ) {
        newform = new FootPounds();
    }
    else if ( strcmp(formname, Joules::getFormName()) == 0 ) {
        newform = new Joules();
    }

    // Units: Forces
    else if ( strcmp(formname, Newtons::getFormName()) == 0 ) {
        newform = new Newtons();
    }
    else if ( strcmp(formname, KiloNewtons::getFormName()) == 0 ) {
        newform = new KiloNewtons();
    }
    else if ( strcmp(formname, Poundals::getFormName()) == 0 ) {
        newform = new Poundals();
    }
    else if ( strcmp(formname, PoundForces::getFormName()) == 0 ) {
        newform = new PoundForces();
    }

    // Units: Frequencies
    else if ( strcmp(formname, Hertz::getFormName()) == 0 ) {
        newform = new Hertz();
    }
    else if ( strcmp(formname, KiloHertz::getFormName()) == 0 ) {
        newform = new KiloHertz();
    }
    else if ( strcmp(formname, MegaHertz::getFormName()) == 0 ) {
        newform = new MegaHertz();
    }
    else if ( strcmp(formname, GigaHertz::getFormName()) == 0 ) {
        newform = new GigaHertz();
    }
    else if ( strcmp(formname, TeraHertz::getFormName()) == 0 ) {
        newform = new TeraHertz();
    }

    // Units: Masses
    else if ( strcmp(formname, Grams::getFormName()) == 0 ) {
        newform = new Grams();
    }
    else if ( strcmp(formname, KiloGrams::getFormName()) == 0 ) {
        newform = new KiloGrams();
    }
    else if ( strcmp(formname, Slugs::getFormName()) == 0 ) {
        newform = new Slugs();
    }

    // Units: Powers
    else if ( strcmp(formname, KiloWatts::getFormName()) == 0 ) {
        newform = new KiloWatts();
    }
    else if ( strcmp(formname, Watts::getFormName()) == 0 ) {
        newform = new Watts();
    }
    else if ( strcmp(formname, MilliWatts::getFormName()) == 0 ) {
        newform = new MilliWatts();
    }
    else if ( strcmp(formname, Horsepower::getFormName()) == 0 ) {
        newform = new Horsepower();
    }
    else if ( strcmp(formname, DecibelWatts::getFormName()) == 0 ) {
        newform = new DecibelWatts();
    }
    else if ( strcmp(formname, DecibelMilliWatts::getFormName()) == 0 ) {
        newform = new DecibelMilliWatts();
    }

    // Units: Time
    else if ( strcmp(formname, Seconds::getFormName()) == 0 ) {
        newform = new Seconds();
    }
    else if ( strcmp(formname, MilliSeconds::getFormName()) == 0 ) {
        newform = new MilliSeconds();
    }
    else if ( strcmp(formname, MicroSeconds::getFormName()) == 0 ) {
        newform = new MicroSeconds();
    }
    else if ( strcmp(formname, NanoSeconds::getFormName()) == 0 ) {
        newform = new NanoSeconds();
    }
    else if ( strcmp(formname, Minutes::getFormName()) == 0 ) {
        newform = new Minutes();
    }
    else if ( strcmp(formname, Hours::getFormName()) == 0 ) {
        newform = new Hours();
    }
    else if ( strcmp(formname, Days::getFormName()) == 0 ) {
        newform = new Days();
    }

    // Units: Velocites
    else if ( strcmp(formname, AngularVelocity::getFormName()) == 0 ) {
        newform = new AngularVelocity();
    }
    else if ( strcmp(formname, LinearVelocity::getFormName()) == 0 ) {
        newform = new LinearVelocity();
    }

    // Colors
    else if ( strcmp(formname, Color::getFormName()) == 0 ) {
        newform = new Color();
    }
    else if ( strcmp(formname, Cie::getFormName()) == 0 ) {
        newform = new Cie();
    }
    else if ( strcmp(formname, Cmy::getFormName()) == 0 ) {
        newform = new Cmy();
    }
    else if ( strcmp(formname, Hls::getFormName()) == 0 ) {
        newform = new Hls();
    }
    else if ( strcmp(formname, Hsv::getFormName()) == 0 ) {
        newform = new Hsv();
    }
    else if ( strcmp(formname, Hsva::getFormName()) == 0 ) {
        newform = new Hsva();
    }
    else if ( strcmp(formname, Rgb::getFormName()) == 0 ) {
        newform = new Rgb();
    }
    else if ( strcmp(formname, Rgba::getFormName()) == 0 ) {
        newform = new Rgba();
    }
    else if ( strcmp(formname, Yiq::getFormName()) == 0 ) {
        newform = new Yiq();
    }

    // Functions
    else if ( strcmp(formname, Func1::getFormName()) == 0 ) {
        newform = new Func1();
    }
    else if ( strcmp(formname, Func2::getFormName()) == 0 ) {
        newform = new Func2();
    }
    else if ( strcmp(formname, Func3::getFormName()) == 0 ) {
        newform = new Func3();
    }
    else if ( strcmp(formname, Func4::getFormName()) == 0 ) {
        newform = new Func4();
    }
    else if ( strcmp(formname, Func5::getFormName()) == 0 ) {
        newform = new Func5();
    }
    else if ( strcmp(formname, Polynomial::getFormName()) == 0 ) {
        newform = new Polynomial();
    }

    // Network Handlers
    else if ( strcmp(formname, BroadcastHandler::getFormName()) == 0 ) {
        newform = new BroadcastHandler();
    }
    else if ( strcmp(formname, MulticastHandler::getFormName()) == 0 ) {
        newform = new MulticastHandler();
    }
    else if ( strcmp(formname, UdpHandler::getFormName()) == 0 ) {
        newform = new UdpHandler();
    }
    else if ( strcmp(formname, TcpClient::getFormName()) == 0 ) {
        newform = new TcpClient();
    }
    else if ( strcmp(formname, TcpServerSingle::getFormName()) == 0 ) {
        newform = new TcpServerSingle();
    }
    else if ( strcmp(formname, TcpServerMulti::getFormName()) == 0 ) {
        newform = new TcpServerMulti();
    }

    // Random number generator and distributions
    else if ( strcmp(formname, Rng::getFormName()) == 0 ) {
        newform = new Rng();
    }
    else if ( strcmp(formname, Exponential::getFormName()) == 0 ) {
        newform = new Exponential();
    }
    else if ( strcmp(formname, Lognormal::getFormName()) == 0 ) {
        newform = new Lognormal();
    }
    else if ( strcmp(formname, Pareto::getFormName()) == 0 ) {
        newform = new Pareto();
    }
    else if ( strcmp(formname, Uniform::getFormName()) == 0 ) {
        newform = new Uniform();
    }

    // General I/O Devices
    else if ( strcmp(formname, IoHandler::getFormName()) == 0 ) {
        newform = new IoHandler();
    }
    else if ( strcmp(formname, IoData::getFormName()) == 0 ) {
        newform = new IoData();
    }

    // Earth models
    else if ( strcmp(formname, EarthModel::getFormName()) == 0 ) {
        newform = new EarthModel();
    }

    // Thread pool
    else if ( strcmp(formname, ThreadPool::getFormName()) == 0 ) {
       newform = new ThreadPool();
    }

    // Ubf
    else if ( strcmp(formname, Ubf::Agent::getFormName()) == 0 ) {
        newform = new Ubf::Agent();
    }
    else if ( strcmp(formname, Ubf::Arbiter::getFormName()) == 0 ) {
        newform = new Ubf::Arbiter();
    }

    return newform;
}

} // End Basic namespace
} // End Eaagles namespace

