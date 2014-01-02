//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/basic/Factory.h"

#include "openeaagles/basic/Object.h"

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
#include "openeaagles/basic/nethandlers/TcpHandler.h"
#include "openeaagles/basic/nethandlers/TcpClient.h"
#include "openeaagles/basic/nethandlers/TcpServerMultiple.h"
#include "openeaagles/basic/nethandlers/TcpServerSingle.h"
#include "openeaagles/basic/nethandlers/UdpBroadcastHandler.h"
#include "openeaagles/basic/nethandlers/UdpMulticastHandler.h"
#include "openeaagles/basic/nethandlers/UdpUnicastHandler.h"

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

Factory::Factory()
{}

Object* Factory::createObj(const char* name)
{
    Object* obj = 0;

    // Numbers
    if ( strcmp(name, Number::getFactoryName()) == 0 ) {
        obj = new Number();
    }
    else if ( strcmp(name, Complex::getFactoryName()) == 0 ) {
        obj = new Complex();
    }
    else if ( strcmp(name, Integer::getFactoryName()) == 0 ) {
        obj = new Integer();
    }
    else if ( strcmp(name, Float::getFactoryName()) == 0 ) {
        obj = new Float();
    }
    else if ( strcmp(name, Boolean::getFactoryName()) == 0 ) {
        obj = new Boolean();
    }
    else if ( strcmp(name, Decibel::getFactoryName()) == 0 ) {
        obj = new Decibel();
    }
    else if ( strcmp(name, LatLon::getFactoryName()) == 0 ) {
        obj = new LatLon();
    }
    else if ( strcmp(name, Add::getFactoryName()) == 0 ) {
        obj = new Add();
    }
    else if ( strcmp(name, Subtract::getFactoryName()) == 0 ) {
        obj = new Subtract();
    }
    else if ( strcmp(name, Multiply::getFactoryName()) == 0 ) {
        obj = new Multiply();
    }
    else if ( strcmp(name, Divide::getFactoryName()) == 0 ) {
        obj = new Divide();
    }
 
    // Components
    else if ( strcmp(name, FileReader::getFactoryName()) == 0 ) {
        obj = new FileReader();
    }
    else if ( strcmp(name, Logger::getFactoryName()) == 0 ) {
        obj = new Logger();
    }
    else if ( strcmp(name, Statistic::getFactoryName()) == 0 ) {
        obj = new Statistic();
    }

    // Transformations
    else if ( strcmp(name, Translation::getFactoryName()) == 0 ) {
        obj = new Translation();
    }
    else if ( strcmp(name, Rotation::getFactoryName()) == 0 ) {
        obj = new Rotation();
    }
    else if ( strcmp(name, Scale::getFactoryName()) == 0 ) {
        obj = new Scale();
    }
    
    // Tables
    else if ( strcmp(name, Table1::getFactoryName()) == 0 ) {
        obj = new Table1();
    }
    else if ( strcmp(name, Table2::getFactoryName()) == 0 ) {
        obj = new Table2();
    }
    else if ( strcmp(name, Table3::getFactoryName()) == 0 ) {
        obj = new Table3();
    }
    else if ( strcmp(name, Table4::getFactoryName()) == 0 ) {
        obj = new Table4();
    }
    else if ( strcmp(name, Table5::getFactoryName()) == 0 ) {
        obj = new Table5();
    }

    // Timers
    else if ( strcmp(name, UpTimer::getFactoryName()) == 0 ) {
        obj = new UpTimer();
    }
    else if ( strcmp(name, DownTimer::getFactoryName()) == 0 ) {
        obj = new DownTimer();
    }

    // Units: Angles
    else if ( strcmp(name, Degrees::getFactoryName()) == 0 ) {
        obj = new Degrees();
    }
    else if ( strcmp(name, Radians::getFactoryName()) == 0 ) {
        obj = new Radians();
    }
    else if ( strcmp(name, Semicircles::getFactoryName()) == 0 ) {
        obj = new Semicircles();
    }

    // Units: Areas
    else if ( strcmp(name, SquareMeters::getFactoryName()) == 0 ) {
        obj = new SquareMeters();
    }
    else if ( strcmp(name, SquareFeet::getFactoryName()) == 0 ) {
        obj = new SquareFeet();
    }
    else if ( strcmp(name, SquareInches::getFactoryName()) == 0 ) {
        obj = new SquareInches();
    }
    else if ( strcmp(name, SquareYards::getFactoryName()) == 0 ) {
        obj = new SquareYards();
    }
    else if ( strcmp(name, SquareMiles::getFactoryName()) == 0 ) {
        obj = new SquareMiles();
    }
    else if ( strcmp(name, SquareCentiMeters::getFactoryName()) == 0 ) {
        obj = new SquareCentiMeters();
    }
    else if ( strcmp(name, SquareMilliMeters::getFactoryName()) == 0 ) {
        obj = new SquareMilliMeters();
    }
    else if ( strcmp(name, SquareKiloMeters::getFactoryName()) == 0 ) {
        obj = new SquareKiloMeters();
    }
    else if ( strcmp(name, DecibelSquareMeters::getFactoryName()) == 0 ) {
        obj = new DecibelSquareMeters();
    }

    // Units: Distances
    else if ( strcmp(name, Meters::getFactoryName()) == 0 ) {
        obj = new Meters();
    }
    else if ( strcmp(name, CentiMeters::getFactoryName()) == 0 ) {
        obj = new CentiMeters();
    }
    else if ( strcmp(name, MicroMeters::getFactoryName()) == 0 ) {
        obj = new MicroMeters();
    }
    else if ( strcmp(name, Microns::getFactoryName()) == 0 ) {
        obj = new Microns();
    }
    else if ( strcmp(name, KiloMeters::getFactoryName()) == 0 ) {
        obj = new KiloMeters();
    }
    else if ( strcmp(name, Inches::getFactoryName()) == 0 ) {
        obj = new Inches();
    }
    else if ( strcmp(name, Feet::getFactoryName()) == 0 ) {
        obj = new Feet();
    }
    else if ( strcmp(name, NauticalMiles::getFactoryName()) == 0 ) {
        obj = new NauticalMiles();
    }
    else if ( strcmp(name, StatuteMiles::getFactoryName()) == 0 ) {
        obj = new StatuteMiles();
    }

    // Units: Energies
    else if ( strcmp(name, KiloWattHours::getFactoryName()) == 0 ) {
        obj = new KiloWattHours();
    }
    else if ( strcmp(name, BTUs::getFactoryName()) == 0 ) {
        obj = new BTUs();
    }
    else if ( strcmp(name, Calories::getFactoryName()) == 0 ) {
        obj = new Calories();
    }
    else if ( strcmp(name, FootPounds::getFactoryName()) == 0 ) {
        obj = new FootPounds();
    }
    else if ( strcmp(name, Joules::getFactoryName()) == 0 ) {
        obj = new Joules();
    }

    // Units: Forces
    else if ( strcmp(name, Newtons::getFactoryName()) == 0 ) {
        obj = new Newtons();
    }
    else if ( strcmp(name, KiloNewtons::getFactoryName()) == 0 ) {
        obj = new KiloNewtons();
    }
    else if ( strcmp(name, Poundals::getFactoryName()) == 0 ) {
        obj = new Poundals();
    }
    else if ( strcmp(name, PoundForces::getFactoryName()) == 0 ) {
        obj = new PoundForces();
    }

    // Units: Frequencies
    else if ( strcmp(name, Hertz::getFactoryName()) == 0 ) {
        obj = new Hertz();
    }
    else if ( strcmp(name, KiloHertz::getFactoryName()) == 0 ) {
        obj = new KiloHertz();
    }
    else if ( strcmp(name, MegaHertz::getFactoryName()) == 0 ) {
        obj = new MegaHertz();
    }
    else if ( strcmp(name, GigaHertz::getFactoryName()) == 0 ) {
        obj = new GigaHertz();
    }
    else if ( strcmp(name, TeraHertz::getFactoryName()) == 0 ) {
        obj = new TeraHertz();
    }

    // Units: Masses
    else if ( strcmp(name, Grams::getFactoryName()) == 0 ) {
        obj = new Grams();
    }
    else if ( strcmp(name, KiloGrams::getFactoryName()) == 0 ) {
        obj = new KiloGrams();
    }
    else if ( strcmp(name, Slugs::getFactoryName()) == 0 ) {
        obj = new Slugs();
    }

    // Units: Powers
    else if ( strcmp(name, KiloWatts::getFactoryName()) == 0 ) {
        obj = new KiloWatts();
    }
    else if ( strcmp(name, Watts::getFactoryName()) == 0 ) {
        obj = new Watts();
    }
    else if ( strcmp(name, MilliWatts::getFactoryName()) == 0 ) {
        obj = new MilliWatts();
    }
    else if ( strcmp(name, Horsepower::getFactoryName()) == 0 ) {
        obj = new Horsepower();
    }
    else if ( strcmp(name, DecibelWatts::getFactoryName()) == 0 ) {
        obj = new DecibelWatts();
    }
    else if ( strcmp(name, DecibelMilliWatts::getFactoryName()) == 0 ) {
        obj = new DecibelMilliWatts();
    }

    // Units: Time
    else if ( strcmp(name, Seconds::getFactoryName()) == 0 ) {
        obj = new Seconds();
    }
    else if ( strcmp(name, MilliSeconds::getFactoryName()) == 0 ) {
        obj = new MilliSeconds();
    }
    else if ( strcmp(name, MicroSeconds::getFactoryName()) == 0 ) {
        obj = new MicroSeconds();
    }
    else if ( strcmp(name, NanoSeconds::getFactoryName()) == 0 ) {
        obj = new NanoSeconds();
    }
    else if ( strcmp(name, Minutes::getFactoryName()) == 0 ) {
        obj = new Minutes();
    }
    else if ( strcmp(name, Hours::getFactoryName()) == 0 ) {
        obj = new Hours();
    }
    else if ( strcmp(name, Days::getFactoryName()) == 0 ) {
        obj = new Days();
    }

    // Units: Velocities
    else if ( strcmp(name, AngularVelocity::getFactoryName()) == 0 ) {
        obj = new AngularVelocity();
    }
    else if ( strcmp(name, LinearVelocity::getFactoryName()) == 0 ) {
        obj = new LinearVelocity();
    }

    // Colors
    else if ( strcmp(name, Color::getFactoryName()) == 0 ) {
        obj = new Color();
    }
    else if ( strcmp(name, Cie::getFactoryName()) == 0 ) {
        obj = new Cie();
    }
    else if ( strcmp(name, Cmy::getFactoryName()) == 0 ) {
        obj = new Cmy();
    }
    else if ( strcmp(name, Hls::getFactoryName()) == 0 ) {
        obj = new Hls();
    }
    else if ( strcmp(name, Hsv::getFactoryName()) == 0 ) {
        obj = new Hsv();
    }
    else if ( strcmp(name, Hsva::getFactoryName()) == 0 ) {
        obj = new Hsva();
    }
    else if ( strcmp(name, Rgb::getFactoryName()) == 0 ) {
        obj = new Rgb();
    }
    else if ( strcmp(name, Rgba::getFactoryName()) == 0 ) {
        obj = new Rgba();
    }
    else if ( strcmp(name, Yiq::getFactoryName()) == 0 ) {
        obj = new Yiq();
    }

    // Functions
    else if ( strcmp(name, Func1::getFactoryName()) == 0 ) {
        obj = new Func1();
    }
    else if ( strcmp(name, Func2::getFactoryName()) == 0 ) {
        obj = new Func2();
    }
    else if ( strcmp(name, Func3::getFactoryName()) == 0 ) {
        obj = new Func3();
    }
    else if ( strcmp(name, Func4::getFactoryName()) == 0 ) {
        obj = new Func4();
    }
    else if ( strcmp(name, Func5::getFactoryName()) == 0 ) {
        obj = new Func5();
    }
    else if ( strcmp(name, Polynomial::getFactoryName()) == 0 ) {
        obj = new Polynomial();
    }

    // Network handlers
    else if ( strcmp(name, TcpClient::getFactoryName()) == 0 ) {
        obj = new TcpClient();
    }
    else if ( strcmp(name, TcpServerSingle::getFactoryName()) == 0 ) {
        obj = new TcpServerSingle();
    }
    else if ( strcmp(name, TcpServerMultiple::getFactoryName()) == 0 ) {
        obj = new TcpServerMultiple();
    }
    else if ( strcmp(name, UdpBroadcastHandler::getFactoryName()) == 0 ) {
        obj = new UdpBroadcastHandler();
    }
    else if ( strcmp(name, UdpMulticastHandler::getFactoryName()) == 0 ) {
        obj = new UdpMulticastHandler();
    }
    else if ( strcmp(name, UdpUnicastHandler::getFactoryName()) == 0 ) {
        obj = new UdpUnicastHandler();
    }
    // Network handlers (backward compatible form names for UDP oriented communication)
    // the mapping to old form names was added 16 Nov 2013 -- should be removed in the future
    else if ( strcmp(name, "BroadcastHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'BroadcastHandler' has been depreciated, use 'UdpBroadcastHandler' instead.\n\n";
        obj = new UdpBroadcastHandler();
    }
    else if ( strcmp(name, "MulticastHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'MulticastHandler' has been depreciated, use 'UdpMulticastHandler' instead.\n\n";
        obj = new UdpMulticastHandler();
    }
    else if ( strcmp(name, "UdpHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'UdpHandler' has been depreciated, use 'UdpUnicastHandler' instead.\n\n";
        obj = new UdpUnicastHandler();
    }

    // Random number generator and distributions
    else if ( strcmp(name, Rng::getFactoryName()) == 0 ) {
        obj = new Rng();
    }
    else if ( strcmp(name, Exponential::getFactoryName()) == 0 ) {
        obj = new Exponential();
    }
    else if ( strcmp(name, Lognormal::getFactoryName()) == 0 ) {
        obj = new Lognormal();
    }
    else if ( strcmp(name, Pareto::getFactoryName()) == 0 ) {
        obj = new Pareto();
    }
    else if ( strcmp(name, Uniform::getFactoryName()) == 0 ) {
        obj = new Uniform();
    }

    // General I/O Devices
    else if ( strcmp(name, IoHandler::getFactoryName()) == 0 ) {
        obj = new IoHandler();
    }
    else if ( strcmp(name, IoData::getFactoryName()) == 0 ) {
        obj = new IoData();
    }

    // Earth models
    else if ( strcmp(name, EarthModel::getFactoryName()) == 0 ) {
        obj = new EarthModel();
    }

    // Thread pool
    else if ( strcmp(name, ThreadPool::getFactoryName()) == 0 ) {
       obj = new ThreadPool();
    }

    // Ubf
    else if ( strcmp(name, Ubf::Agent::getFactoryName()) == 0 ) {
        obj = new Ubf::Agent();
    }
    else if ( strcmp(name, Ubf::Arbiter::getFactoryName()) == 0 ) {
        obj = new Ubf::Arbiter();
    }

    return obj;
}

}  // end namespace Basic
}  // end namespace Eaagles


