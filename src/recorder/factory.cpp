
#include "openeaagles/recorder/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/recorder/DataRecorder.hpp"
#include "openeaagles/recorder/FileWriter.hpp"
#include "openeaagles/recorder/FileReader.hpp"
#include "openeaagles/recorder/OutputHandler.hpp"
#include "openeaagles/recorder/NetInput.hpp"
#include "openeaagles/recorder/NetOutput.hpp"
#include "openeaagles/recorder/TabPrinter.hpp"
#include "openeaagles/recorder/PrintPlayer.hpp"
#include "openeaagles/recorder/PrintSelected.hpp"

#include <string>

namespace oe {
namespace recorder {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == FileWriter::getFactoryName() ) {
        obj = new FileWriter();
    }
    else if ( name == FileReader::getFactoryName() ) {
        obj = new FileReader();
    }
    else if ( name == NetInput::getFactoryName() ) {
        obj = new NetInput();
    }
    else if ( name == NetOutput::getFactoryName() ) {
        obj = new NetOutput();
    }
    else if ( name == OutputHandler::getFactoryName() ) {
        obj = new OutputHandler();
    }
    else if ( name == TabPrinter::getFactoryName() ) {
        obj = new TabPrinter();
    }
    else if ( name == PrintPlayer::getFactoryName() ) {
        obj = new PrintPlayer();
    }
    else if ( name == DataRecorder::getFactoryName()) {
        obj = new DataRecorder();
    }
    else if ( name == PrintSelected::getFactoryName() ) {
        obj = new PrintSelected();
    }

    return obj;
}

}
}
