
#include "openeaagles/base/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/base/Logger.h"
#include "openeaagles/base/FileReader.h"
#include "openeaagles/base/Statistic.h"
#include "openeaagles/base/Transforms.h"
#include "openeaagles/base/Timers.h"

// Functors
#include "openeaagles/base/functors/Tables.h"
#include "openeaagles/base/functors/Functions.h"

// Numbers
#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/Complex.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Decibel.h"
#include "openeaagles/base/LatLon.h"
#include "openeaagles/base/Operators.h"

// Net handlers
#include "openeaagles/base/nethandlers/TcpHandler.h"
#include "openeaagles/base/nethandlers/TcpClient.h"
#include "openeaagles/base/nethandlers/TcpServerMultiple.h"
#include "openeaagles/base/nethandlers/TcpServerSingle.h"
#include "openeaagles/base/nethandlers/UdpBroadcastHandler.h"
#include "openeaagles/base/nethandlers/UdpMulticastHandler.h"
#include "openeaagles/base/nethandlers/UdpUnicastHandler.h"

// Colors
#include "openeaagles/base/Cie.h"
#include "openeaagles/base/Cmy.h"
#include "openeaagles/base/Hls.h"
#include "openeaagles/base/Hsv.h"
#include "openeaagles/base/Hsva.h"
#include "openeaagles/base/Rgb.h"
#include "openeaagles/base/Rgba.h"
#include "openeaagles/base/Yiq.h"

// Units
#include "openeaagles/base/units/AngularVelocity.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Areas.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/units/Energies.h"
#include "openeaagles/base/units/Forces.h"
#include "openeaagles/base/units/Frequencies.h"
#include "openeaagles/base/units/LinearVelocity.h"
#include "openeaagles/base/units/Masses.h"
#include "openeaagles/base/units/Powers.h"
#include "openeaagles/base/units/Times.h"

// Random number generator and distributions
#include "openeaagles/base/Rng.h"
#include "openeaagles/base/distributions/Exponential.h"
#include "openeaagles/base/distributions/Lognormal.h"
#include "openeaagles/base/distributions/Pareto.h"
#include "openeaagles/base/distributions/Uniform.h"

// Others
#include "openeaagles/base/EarthModel.h"
#include "openeaagles/base/IoData.h"
#include "openeaagles/base/IoHandler.h"
#include "openeaagles/base/ThreadPool.h"

// Ubf
#include "openeaagles/base/ubf/Agent.h"
#include "openeaagles/base/ubf/Arbiter.h"

#include <cstring>

namespace oe {
namespace base {

Object* factory(const char* name)
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
    else if ( std::strcmp(name, ubf::Agent::getFactoryName()) == 0 ) {
        obj = new ubf::Agent();
    }
    else if ( std::strcmp(name, ubf::Arbiter::getFactoryName()) == 0 ) {
        obj = new ubf::Arbiter();
    }

    return obj;
}

}
}

