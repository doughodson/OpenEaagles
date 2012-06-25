//------------------------------------------------------------------------------
// Form function for the 'recorder' library
//------------------------------------------------------------------------------

#include "openeaagles/recorder/recorderFF.h"

#include "openeaagles/recorder/DataRecorder.h"
#include "openeaagles/recorder/FileWriter.h"
#include "openeaagles/recorder/FileReader.h"
#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/NetInput.h"
#include "openeaagles/recorder/NetOutput.h"
#include "openeaagles/recorder/TabPrinter.h"
#include "openeaagles/recorder/PrintPlayer.h"
#include "openeaagles/recorder/PrintSelected.h"


namespace Eaagles {
namespace Recorder {

Basic::Object* recorderFormFunc(const char* formname)
{
   Basic::Object* newform = 0;

   // Forms
   if ( strcmp(formname, FileWriter::getFormName()) == 0 ) {
      newform = new FileWriter();
   }
   else if ( strcmp(formname, FileReader::getFormName()) == 0 ) {
      newform = new FileReader();
   }
   else if ( strcmp(formname, NetInput::getFormName()) == 0 ) {
      newform = new NetInput();
   }
   else if ( strcmp(formname, NetOutput::getFormName()) == 0 ) {
      newform = new NetOutput();
   }
   else if ( strcmp(formname, OutputHandler::getFormName()) == 0 ) {
      newform = new OutputHandler();
   }
   else if ( strcmp(formname, TabPrinter::getFormName()) == 0 ) {
      newform = new TabPrinter();
   }
   else if ( strcmp(formname, PrintPlayer::getFormName()) == 0 ) {
      newform = new PrintPlayer();
   }
   else if ( strcmp(formname, DataRecorder::getFormName()) == 0 ) {
      newform = new DataRecorder();
   }
   else if ( strcmp(formname, PrintSelected::getFormName()) == 0 ) {
      newform = new PrintSelected();
   }

    return newform;
}

} // End Recorder namespace
} // End Eaagles namespace
