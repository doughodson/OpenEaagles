
#include "openeaagles/recorder/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/recorder/DataRecorder.h"
#include "openeaagles/recorder/FileWriter.h"
#include "openeaagles/recorder/FileReader.h"
#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/NetInput.h"
#include "openeaagles/recorder/NetOutput.h"
#include "openeaagles/recorder/TabPrinter.h"
#include "openeaagles/recorder/PrintPlayer.h"
#include "openeaagles/recorder/PrintSelected.h"

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
