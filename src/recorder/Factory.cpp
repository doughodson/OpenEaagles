//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/recorder/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/recorder/DataRecorder.h"
#include "openeaagles/recorder/FileWriter.h"
#include "openeaagles/recorder/FileReader.h"
#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/NetInput.h"
#include "openeaagles/recorder/NetOutput.h"
#include "openeaagles/recorder/TabPrinter.h"
#include "openeaagles/recorder/PrintPlayer.h"
#include "openeaagles/recorder/PrintSelected.h"

#include <cstring>

namespace Eaagles {
namespace Recorder {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    if ( std::strcmp(name, FileWriter::getFactoryName()) == 0 ) {
        obj = new FileWriter();
    }
    else if ( std::strcmp(name, FileReader::getFactoryName()) == 0 ) {
        obj = new FileReader();
    }
    else if ( std::strcmp(name, NetInput::getFactoryName()) == 0 ) {
        obj = new NetInput();
    }
    else if ( std::strcmp(name, NetOutput::getFactoryName()) == 0 ) {
        obj = new NetOutput();
    }
    else if ( std::strcmp(name, OutputHandler::getFactoryName()) == 0 ) {
        obj = new OutputHandler();
    }
    else if ( std::strcmp(name, TabPrinter::getFactoryName()) == 0 ) {
        obj = new TabPrinter();
    }
    else if ( std::strcmp(name, PrintPlayer::getFactoryName()) == 0 ) {
        obj = new PrintPlayer();
    }
    else if ( std::strcmp(name, DataRecorder::getFactoryName()) == 0 ) {
        obj = new DataRecorder();
    }
    else if ( std::strcmp(name, PrintSelected::getFactoryName()) == 0 ) {
        obj = new PrintSelected();
    }

    return obj;
}

}  // end namespace Recorder
}  // end namespace Eaagles
