
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

#include <string>

namespace oe {
namespace base {

Object* factory(const std::string& name)
{
    Object* obj = nullptr;

    // Numbers
    if ( name == Number::getFactoryName() ) {
        obj = new Number();
    }
    else if ( name == Complex::getFactoryName() ) {
        obj = new Complex();
    }
    else if ( name == Integer::getFactoryName() ) {
        obj = new Integer();
    }
    else if ( name == Float::getFactoryName() ) {
        obj = new Float();
    }
    else if ( name == Boolean::getFactoryName() ) {
        obj = new Boolean();
    }
    else if ( name == Decibel::getFactoryName() ) {
        obj = new Decibel();
    }
    else if ( name == LatLon::getFactoryName() ) {
        obj = new LatLon();
    }
    else if ( name == Add::getFactoryName() ) {
        obj = new Add();
    }
    else if ( name == Subtract::getFactoryName() ) {
        obj = new Subtract();
    }
    else if ( name == Multiply::getFactoryName() ) {
        obj = new Multiply();
    }
    else if ( name == Divide::getFactoryName() ) {
        obj = new Divide();
    }

    // Components
    else if ( name == FileReader::getFactoryName() ) {
        obj = new FileReader();
    }
    else if ( name == Logger::getFactoryName() ) {
        obj = new Logger();
    }
    else if ( name == Statistic::getFactoryName() ) {
        obj = new Statistic();
    }

    // Transformations
    else if ( name == Translation::getFactoryName() ) {
        obj = new Translation();
    }
    else if ( name == Rotation::getFactoryName() ) {
        obj = new Rotation();
    }
    else if ( name == Scale::getFactoryName() ) {
        obj = new Scale();
    }

    // Functors
    else if ( name == Func1::getFactoryName() ) {
        obj = new Func1();
    }
    else if ( name == Func2::getFactoryName() ) {
        obj = new Func2();
    }
    else if ( name == Func3::getFactoryName() ) {
        obj = new Func3();
    }
    else if ( name == Func4::getFactoryName() ) {
        obj = new Func4();
    }
    else if ( name == Func5::getFactoryName() ) {
        obj = new Func5();
    }
    else if ( name == Polynomial::getFactoryName() ) {
        obj = new Polynomial();
    }
    else if ( name == Table1::getFactoryName() ) {
        obj = new Table1();
    }
    else if ( name == Table2::getFactoryName() ) {
        obj = new Table2();
    }
    else if ( name == Table3::getFactoryName() ) {
        obj = new Table3();
    }
    else if ( name == Table4::getFactoryName() ) {
        obj = new Table4();
    }
    else if ( name == Table5::getFactoryName() ) {
        obj = new Table5();
    }

    // Timers
    else if ( name == UpTimer::getFactoryName() ) {
        obj = new UpTimer();
    }
    else if ( name == DownTimer::getFactoryName() ) {
        obj = new DownTimer();
    }

    // Units: Angles
    else if ( name == Degrees::getFactoryName() ) {
        obj = new Degrees();
    }
    else if ( name == Radians::getFactoryName() ) {
        obj = new Radians();
    }
    else if ( name == Semicircles::getFactoryName() ) {
        obj = new Semicircles();
    }

    // Units: Areas
    else if ( name == SquareMeters::getFactoryName() ) {
        obj = new SquareMeters();
    }
    else if ( name == SquareFeet::getFactoryName() ) {
        obj = new SquareFeet();
    }
    else if ( name == SquareInches::getFactoryName() ) {
        obj = new SquareInches();
    }
    else if ( name == SquareYards::getFactoryName() ) {
        obj = new SquareYards();
    }
    else if ( name == SquareMiles::getFactoryName() ) {
        obj = new SquareMiles();
    }
    else if ( name == SquareCentiMeters::getFactoryName() ) {
        obj = new SquareCentiMeters();
    }
    else if ( name == SquareMilliMeters::getFactoryName() ) {
        obj = new SquareMilliMeters();
    }
    else if ( name == SquareKiloMeters::getFactoryName() ) {
        obj = new SquareKiloMeters();
    }
    else if ( name == DecibelSquareMeters::getFactoryName() ) {
        obj = new DecibelSquareMeters();
    }

    // Units: Distances
    else if ( name == Meters::getFactoryName() ) {
        obj = new Meters();
    }
    else if ( name == CentiMeters::getFactoryName() ) {
        obj = new CentiMeters();
    }
    else if ( name == MicroMeters::getFactoryName() ) {
        obj = new MicroMeters();
    }
    else if ( name == Microns::getFactoryName() ) {
        obj = new Microns();
    }
    else if ( name == KiloMeters::getFactoryName() ) {
        obj = new KiloMeters();
    }
    else if ( name == Inches::getFactoryName() ) {
        obj = new Inches();
    }
    else if ( name == Feet::getFactoryName() ) {
        obj = new Feet();
    }
    else if ( name == NauticalMiles::getFactoryName() ) {
        obj = new NauticalMiles();
    }
    else if ( name == StatuteMiles::getFactoryName() ) {
        obj = new StatuteMiles();
    }

    // Units: Energies
    else if ( name == KiloWattHours::getFactoryName() ) {
        obj = new KiloWattHours();
    }
    else if ( name == BTUs::getFactoryName() ) {
        obj = new BTUs();
    }
    else if ( name == Calories::getFactoryName() ) {
        obj = new Calories();
    }
    else if ( name == FootPounds::getFactoryName() ) {
        obj = new FootPounds();
    }
    else if ( name == Joules::getFactoryName() ) {
        obj = new Joules();
    }

    // Units: Forces
    else if ( name == Newtons::getFactoryName() ) {
        obj = new Newtons();
    }
    else if ( name == KiloNewtons::getFactoryName() ) {
        obj = new KiloNewtons();
    }
    else if ( name == Poundals::getFactoryName() ) {
        obj = new Poundals();
    }
    else if ( name == PoundForces::getFactoryName() ) {
        obj = new PoundForces();
    }

    // Units: Frequencies
    else if ( name == Hertz::getFactoryName() ) {
        obj = new Hertz();
    }
    else if ( name == KiloHertz::getFactoryName() ) {
        obj = new KiloHertz();
    }
    else if ( name == MegaHertz::getFactoryName() ) {
        obj = new MegaHertz();
    }
    else if ( name == GigaHertz::getFactoryName() ) {
        obj = new GigaHertz();
    }
    else if ( name == TeraHertz::getFactoryName() ) {
        obj = new TeraHertz();
    }

    // Units: Masses
    else if ( name == Grams::getFactoryName() ) {
        obj = new Grams();
    }
    else if ( name == KiloGrams::getFactoryName() ) {
        obj = new KiloGrams();
    }
    else if ( name == Slugs::getFactoryName() ) {
        obj = new Slugs();
    }

    // Units: Powers
    else if ( name == KiloWatts::getFactoryName() ) {
        obj = new KiloWatts();
    }
    else if ( name == Watts::getFactoryName() ) {
        obj = new Watts();
    }
    else if ( name == MilliWatts::getFactoryName() ) {
        obj = new MilliWatts();
    }
    else if ( name == Horsepower::getFactoryName() ) {
        obj = new Horsepower();
    }
    else if ( name == DecibelWatts::getFactoryName() ) {
        obj = new DecibelWatts();
    }
    else if ( name == DecibelMilliWatts::getFactoryName() ) {
        obj = new DecibelMilliWatts();
    }

    // Units: Time
    else if ( name == Seconds::getFactoryName() ) {
        obj = new Seconds();
    }
    else if ( name == MilliSeconds::getFactoryName() ) {
        obj = new MilliSeconds();
    }
    else if ( name == MicroSeconds::getFactoryName() ) {
        obj = new MicroSeconds();
    }
    else if ( name == NanoSeconds::getFactoryName() ) {
        obj = new NanoSeconds();
    }
    else if ( name == Minutes::getFactoryName() ) {
        obj = new Minutes();
    }
    else if ( name == Hours::getFactoryName() ) {
        obj = new Hours();
    }
    else if ( name == Days::getFactoryName() ) {
        obj = new Days();
    }

    // Units: Velocities
    else if ( name == AngularVelocity::getFactoryName() ) {
        obj = new AngularVelocity();
    }
    else if ( name == LinearVelocity::getFactoryName() ) {
        obj = new LinearVelocity();
    }

    // Colors
    else if ( name == Color::getFactoryName() ) {
        obj = new Color();
    }
    else if ( name == Cie::getFactoryName() ) {
        obj = new Cie();
    }
    else if ( name == Cmy::getFactoryName() ) {
        obj = new Cmy();
    }
    else if ( name == Hls::getFactoryName() ) {
        obj = new Hls();
    }
    else if ( name == Hsv::getFactoryName() ) {
        obj = new Hsv();
    }
    else if ( name == Hsva::getFactoryName() ) {
        obj = new Hsva();
    }
    else if ( name == Rgb::getFactoryName() ) {
        obj = new Rgb();
    }
    else if ( name == Rgba::getFactoryName() ) {
        obj = new Rgba();
    }
    else if ( name == Yiq::getFactoryName() ) {
        obj = new Yiq();
    }

    // Network handlers
    else if ( name == TcpClient::getFactoryName() ) {
        obj = new TcpClient();
    }
    else if ( name == TcpServerSingle::getFactoryName() ) {
        obj = new TcpServerSingle();
    }
    else if ( name == TcpServerMultiple::getFactoryName() ) {
        obj = new TcpServerMultiple();
    }
    else if ( name == UdpBroadcastHandler::getFactoryName() ) {
        obj = new UdpBroadcastHandler();
    }
    else if ( name == UdpMulticastHandler::getFactoryName() ) {
        obj = new UdpMulticastHandler();
    }
    else if ( name == UdpUnicastHandler::getFactoryName() ) {
        obj = new UdpUnicastHandler();
    }

    // Random number generator and distributions
    else if ( name == Rng::getFactoryName() ) {
        obj = new Rng();
    }
    else if ( name == Exponential::getFactoryName() ) {
        obj = new Exponential();
    }
    else if ( name == Lognormal::getFactoryName() ) {
        obj = new Lognormal();
    }
    else if ( name == Pareto::getFactoryName() ) {
        obj = new Pareto();
    }
    else if ( name == Uniform::getFactoryName() ) {
        obj = new Uniform();
    }

    // General I/O Devices
    else if ( name == IoHandler::getFactoryName() ) {
        obj = new IoHandler();
    }
    else if ( name == IoData::getFactoryName() ) {
        obj = new IoData();
    }

    // Earth models
    else if ( name == EarthModel::getFactoryName() ) {
        obj = new EarthModel();
    }

    // Thread pool
    else if ( name == ThreadPool::getFactoryName() ) {
       obj = new ThreadPool();
    }

    // Ubf
    else if ( name == ubf::Agent::getFactoryName() ) {
        obj = new ubf::Agent();
    }
    else if ( name == ubf::Arbiter::getFactoryName() ) {
        obj = new ubf::Arbiter();
    }

    return obj;
}

}
}

