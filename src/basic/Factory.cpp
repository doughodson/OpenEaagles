//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/basic/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/basic/Logger.h"
#include "openeaagles/basic/FileReader.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/basic/Transforms.h"
#include "openeaagles/basic/Timers.h"

// Functors
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/functors/Functions.h"

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
#include "openeaagles/basic/EarthModel.h"
#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/ThreadPool.h"

// Ubf
#include "openeaagles/basic/ubf/Agent.h"
#include "openeaagles/basic/ubf/Arbiter.h"

#include <cstring>

namespace Eaagles {
namespace Basic {

Factory::Factory()
{}

Object* Factory::createObj(const char* name)
{
    Object* obj = nullptr;

    // Numbers
    if ( std::strcmp(name, Number::getFactoryName()) == 0 ) {
        obj = new Number();
    }
    else if ( std::strcmp(name, Complex::getFactoryName()) == 0 ) {
        obj = new Complex();
    }
    else if ( std::strcmp(name, Integer::getFactoryName()) == 0 ) {
        obj = new Integer();
    }
    else if ( std::strcmp(name, Float::getFactoryName()) == 0 ) {
        obj = new Float();
    }
    else if ( std::strcmp(name, Boolean::getFactoryName()) == 0 ) {
        obj = new Boolean();
    }
    else if ( std::strcmp(name, Decibel::getFactoryName()) == 0 ) {
        obj = new Decibel();
    }
    else if ( std::strcmp(name, LatLon::getFactoryName()) == 0 ) {
        obj = new LatLon();
    }
    else if ( std::strcmp(name, Add::getFactoryName()) == 0 ) {
        obj = new Add();
    }
    else if ( std::strcmp(name, Subtract::getFactoryName()) == 0 ) {
        obj = new Subtract();
    }
    else if ( std::strcmp(name, Multiply::getFactoryName()) == 0 ) {
        obj = new Multiply();
    }
    else if ( std::strcmp(name, Divide::getFactoryName()) == 0 ) {
        obj = new Divide();
    }

    // Components
    else if ( std::strcmp(name, FileReader::getFactoryName()) == 0 ) {
        obj = new FileReader();
    }
    else if ( std::strcmp(name, Logger::getFactoryName()) == 0 ) {
        obj = new Logger();
    }
    else if ( std::strcmp(name, Statistic::getFactoryName()) == 0 ) {
        obj = new Statistic();
    }

    // Transformations
    else if ( std::strcmp(name, Translation::getFactoryName()) == 0 ) {
        obj = new Translation();
    }
    else if ( std::strcmp(name, Rotation::getFactoryName()) == 0 ) {
        obj = new Rotation();
    }
    else if ( std::strcmp(name, Scale::getFactoryName()) == 0 ) {
        obj = new Scale();
    }

    // Functors
    else if ( std::strcmp(name, Func1::getFactoryName()) == 0 ) {
        obj = new Func1();
    }
    else if ( std::strcmp(name, Func2::getFactoryName()) == 0 ) {
        obj = new Func2();
    }
    else if ( std::strcmp(name, Func3::getFactoryName()) == 0 ) {
        obj = new Func3();
    }
    else if ( std::strcmp(name, Func4::getFactoryName()) == 0 ) {
        obj = new Func4();
    }
    else if ( std::strcmp(name, Func5::getFactoryName()) == 0 ) {
        obj = new Func5();
    }
    else if ( std::strcmp(name, Polynomial::getFactoryName()) == 0 ) {
        obj = new Polynomial();
    }
    else if ( std::strcmp(name, Table1::getFactoryName()) == 0 ) {
        obj = new Table1();
    }
    else if ( std::strcmp(name, Table2::getFactoryName()) == 0 ) {
        obj = new Table2();
    }
    else if ( std::strcmp(name, Table3::getFactoryName()) == 0 ) {
        obj = new Table3();
    }
    else if ( std::strcmp(name, Table4::getFactoryName()) == 0 ) {
        obj = new Table4();
    }
    else if ( std::strcmp(name, Table5::getFactoryName()) == 0 ) {
        obj = new Table5();
    }

    // Timers
    else if ( std::strcmp(name, UpTimer::getFactoryName()) == 0 ) {
        obj = new UpTimer();
    }
    else if ( std::strcmp(name, DownTimer::getFactoryName()) == 0 ) {
        obj = new DownTimer();
    }

    // Units: Angles
    else if ( std::strcmp(name, Degrees::getFactoryName()) == 0 ) {
        obj = new Degrees();
    }
    else if ( std::strcmp(name, Radians::getFactoryName()) == 0 ) {
        obj = new Radians();
    }
    else if ( std::strcmp(name, Semicircles::getFactoryName()) == 0 ) {
        obj = new Semicircles();
    }

    // Units: Areas
    else if ( std::strcmp(name, SquareMeters::getFactoryName()) == 0 ) {
        obj = new SquareMeters();
    }
    else if ( std::strcmp(name, SquareFeet::getFactoryName()) == 0 ) {
        obj = new SquareFeet();
    }
    else if ( std::strcmp(name, SquareInches::getFactoryName()) == 0 ) {
        obj = new SquareInches();
    }
    else if ( std::strcmp(name, SquareYards::getFactoryName()) == 0 ) {
        obj = new SquareYards();
    }
    else if ( std::strcmp(name, SquareMiles::getFactoryName()) == 0 ) {
        obj = new SquareMiles();
    }
    else if ( std::strcmp(name, SquareCentiMeters::getFactoryName()) == 0 ) {
        obj = new SquareCentiMeters();
    }
    else if ( std::strcmp(name, SquareMilliMeters::getFactoryName()) == 0 ) {
        obj = new SquareMilliMeters();
    }
    else if ( std::strcmp(name, SquareKiloMeters::getFactoryName()) == 0 ) {
        obj = new SquareKiloMeters();
    }
    else if ( std::strcmp(name, DecibelSquareMeters::getFactoryName()) == 0 ) {
        obj = new DecibelSquareMeters();
    }

    // Units: Distances
    else if ( std::strcmp(name, Meters::getFactoryName()) == 0 ) {
        obj = new Meters();
    }
    else if ( std::strcmp(name, CentiMeters::getFactoryName()) == 0 ) {
        obj = new CentiMeters();
    }
    else if ( std::strcmp(name, MicroMeters::getFactoryName()) == 0 ) {
        obj = new MicroMeters();
    }
    else if ( std::strcmp(name, Microns::getFactoryName()) == 0 ) {
        obj = new Microns();
    }
    else if ( std::strcmp(name, KiloMeters::getFactoryName()) == 0 ) {
        obj = new KiloMeters();
    }
    else if ( std::strcmp(name, Inches::getFactoryName()) == 0 ) {
        obj = new Inches();
    }
    else if ( std::strcmp(name, Feet::getFactoryName()) == 0 ) {
        obj = new Feet();
    }
    else if ( std::strcmp(name, NauticalMiles::getFactoryName()) == 0 ) {
        obj = new NauticalMiles();
    }
    else if ( std::strcmp(name, StatuteMiles::getFactoryName()) == 0 ) {
        obj = new StatuteMiles();
    }

    // Units: Energies
    else if ( std::strcmp(name, KiloWattHours::getFactoryName()) == 0 ) {
        obj = new KiloWattHours();
    }
    else if ( std::strcmp(name, BTUs::getFactoryName()) == 0 ) {
        obj = new BTUs();
    }
    else if ( std::strcmp(name, Calories::getFactoryName()) == 0 ) {
        obj = new Calories();
    }
    else if ( std::strcmp(name, FootPounds::getFactoryName()) == 0 ) {
        obj = new FootPounds();
    }
    else if ( std::strcmp(name, Joules::getFactoryName()) == 0 ) {
        obj = new Joules();
    }

    // Units: Forces
    else if ( std::strcmp(name, Newtons::getFactoryName()) == 0 ) {
        obj = new Newtons();
    }
    else if ( std::strcmp(name, KiloNewtons::getFactoryName()) == 0 ) {
        obj = new KiloNewtons();
    }
    else if ( std::strcmp(name, Poundals::getFactoryName()) == 0 ) {
        obj = new Poundals();
    }
    else if ( std::strcmp(name, PoundForces::getFactoryName()) == 0 ) {
        obj = new PoundForces();
    }

    // Units: Frequencies
    else if ( std::strcmp(name, Hertz::getFactoryName()) == 0 ) {
        obj = new Hertz();
    }
    else if ( std::strcmp(name, KiloHertz::getFactoryName()) == 0 ) {
        obj = new KiloHertz();
    }
    else if ( std::strcmp(name, MegaHertz::getFactoryName()) == 0 ) {
        obj = new MegaHertz();
    }
    else if ( std::strcmp(name, GigaHertz::getFactoryName()) == 0 ) {
        obj = new GigaHertz();
    }
    else if ( std::strcmp(name, TeraHertz::getFactoryName()) == 0 ) {
        obj = new TeraHertz();
    }

    // Units: Masses
    else if ( std::strcmp(name, Grams::getFactoryName()) == 0 ) {
        obj = new Grams();
    }
    else if ( std::strcmp(name, KiloGrams::getFactoryName()) == 0 ) {
        obj = new KiloGrams();
    }
    else if ( std::strcmp(name, Slugs::getFactoryName()) == 0 ) {
        obj = new Slugs();
    }

    // Units: Powers
    else if ( std::strcmp(name, KiloWatts::getFactoryName()) == 0 ) {
        obj = new KiloWatts();
    }
    else if ( std::strcmp(name, Watts::getFactoryName()) == 0 ) {
        obj = new Watts();
    }
    else if ( std::strcmp(name, MilliWatts::getFactoryName()) == 0 ) {
        obj = new MilliWatts();
    }
    else if ( std::strcmp(name, Horsepower::getFactoryName()) == 0 ) {
        obj = new Horsepower();
    }
    else if ( std::strcmp(name, DecibelWatts::getFactoryName()) == 0 ) {
        obj = new DecibelWatts();
    }
    else if ( std::strcmp(name, DecibelMilliWatts::getFactoryName()) == 0 ) {
        obj = new DecibelMilliWatts();
    }

    // Units: Time
    else if ( std::strcmp(name, Seconds::getFactoryName()) == 0 ) {
        obj = new Seconds();
    }
    else if ( std::strcmp(name, MilliSeconds::getFactoryName()) == 0 ) {
        obj = new MilliSeconds();
    }
    else if ( std::strcmp(name, MicroSeconds::getFactoryName()) == 0 ) {
        obj = new MicroSeconds();
    }
    else if ( std::strcmp(name, NanoSeconds::getFactoryName()) == 0 ) {
        obj = new NanoSeconds();
    }
    else if ( std::strcmp(name, Minutes::getFactoryName()) == 0 ) {
        obj = new Minutes();
    }
    else if ( std::strcmp(name, Hours::getFactoryName()) == 0 ) {
        obj = new Hours();
    }
    else if ( std::strcmp(name, Days::getFactoryName()) == 0 ) {
        obj = new Days();
    }

    // Units: Velocities
    else if ( std::strcmp(name, AngularVelocity::getFactoryName()) == 0 ) {
        obj = new AngularVelocity();
    }
    else if ( std::strcmp(name, LinearVelocity::getFactoryName()) == 0 ) {
        obj = new LinearVelocity();
    }

    // Colors
    else if ( std::strcmp(name, Color::getFactoryName()) == 0 ) {
        obj = new Color();
    }
    else if ( std::strcmp(name, Cie::getFactoryName()) == 0 ) {
        obj = new Cie();
    }
    else if ( std::strcmp(name, Cmy::getFactoryName()) == 0 ) {
        obj = new Cmy();
    }
    else if ( std::strcmp(name, Hls::getFactoryName()) == 0 ) {
        obj = new Hls();
    }
    else if ( std::strcmp(name, Hsv::getFactoryName()) == 0 ) {
        obj = new Hsv();
    }
    else if ( std::strcmp(name, Hsva::getFactoryName()) == 0 ) {
        obj = new Hsva();
    }
    else if ( std::strcmp(name, Rgb::getFactoryName()) == 0 ) {
        obj = new Rgb();
    }
    else if ( std::strcmp(name, Rgba::getFactoryName()) == 0 ) {
        obj = new Rgba();
    }
    else if ( std::strcmp(name, Yiq::getFactoryName()) == 0 ) {
        obj = new Yiq();
    }

    // Network handlers
    else if ( std::strcmp(name, TcpClient::getFactoryName()) == 0 ) {
        obj = new TcpClient();
    }
    else if ( std::strcmp(name, TcpServerSingle::getFactoryName()) == 0 ) {
        obj = new TcpServerSingle();
    }
    else if ( std::strcmp(name, TcpServerMultiple::getFactoryName()) == 0 ) {
        obj = new TcpServerMultiple();
    }
    else if ( std::strcmp(name, UdpBroadcastHandler::getFactoryName()) == 0 ) {
        obj = new UdpBroadcastHandler();
    }
    else if ( std::strcmp(name, UdpMulticastHandler::getFactoryName()) == 0 ) {
        obj = new UdpMulticastHandler();
    }
    else if ( std::strcmp(name, UdpUnicastHandler::getFactoryName()) == 0 ) {
        obj = new UdpUnicastHandler();
    }
    // Network handlers (backward compatible form names for UDP oriented communication)
    // the mapping to old form names was added 16 Nov 2013 -- should be removed in the future
    else if ( std::strcmp(name, "BroadcastHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'BroadcastHandler' has been depreciated, use 'UdpBroadcastHandler' instead.\n\n";
        obj = new UdpBroadcastHandler();
    }
    else if ( std::strcmp(name, "MulticastHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'MulticastHandler' has been depreciated, use 'UdpMulticastHandler' instead.\n\n";
        obj = new UdpMulticastHandler();
    }
    else if ( std::strcmp(name, "UdpHandler") == 0 ) {
        std::cerr << "\nWARNING! Name 'UdpHandler' has been depreciated, use 'UdpUnicastHandler' instead.\n\n";
        obj = new UdpUnicastHandler();
    }

    // Random number generator and distributions
    else if ( std::strcmp(name, Rng::getFactoryName()) == 0 ) {
        obj = new Rng();
    }
    else if ( std::strcmp(name, Exponential::getFactoryName()) == 0 ) {
        obj = new Exponential();
    }
    else if ( std::strcmp(name, Lognormal::getFactoryName()) == 0 ) {
        obj = new Lognormal();
    }
    else if ( std::strcmp(name, Pareto::getFactoryName()) == 0 ) {
        obj = new Pareto();
    }
    else if ( std::strcmp(name, Uniform::getFactoryName()) == 0 ) {
        obj = new Uniform();
    }

    // General I/O Devices
    else if ( std::strcmp(name, IoHandler::getFactoryName()) == 0 ) {
        obj = new IoHandler();
    }
    else if ( std::strcmp(name, IoData::getFactoryName()) == 0 ) {
        obj = new IoData();
    }

    // Earth models
    else if ( std::strcmp(name, EarthModel::getFactoryName()) == 0 ) {
        obj = new EarthModel();
    }

    // Thread pool
    else if ( std::strcmp(name, ThreadPool::getFactoryName()) == 0 ) {
       obj = new ThreadPool();
    }

    // Ubf
    else if ( std::strcmp(name, Ubf::Agent::getFactoryName()) == 0 ) {
        obj = new Ubf::Agent();
    }
    else if ( std::strcmp(name, Ubf::Arbiter::getFactoryName()) == 0 ) {
        obj = new Ubf::Arbiter();
    }

    return obj;
}

}  // end namespace Basic
}  // end namespace Eaagles


