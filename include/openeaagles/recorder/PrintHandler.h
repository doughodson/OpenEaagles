//------------------------------------------------------------------------------
// Class: PrintHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_PrintHandler_H__
#define __Eaagles_Recorder_PrintHandler_H__

#include "openeaagles/recorder/OutputHandler.h"
#include <string>
#include <sstream>
#include <fstream>

namespace Eaagles {
namespace Recorder {

//------------------------------------------------------------------------------
// Class:   PrintHandler
// Description: Common print handler used by TabPrinter, etc.
//
// Notes:
//    1) Default is to print to the standard output
//    2) If a filename is given then ...
//       a) the file is created and the output is written to that file
//       b) if the file already exists, a new file is still created
//          with a version number appended to the file name.
//
// Factory name: PrintHandler
// Slots:
//    filename    <String>    ! Data file name (default: use std out)
//    pathname    <String>    ! Path to the data file directory (optional)
//------------------------------------------------------------------------------
class PrintHandler : public OutputHandler
{
    DECLARE_SUBCLASS(PrintHandler, OutputHandler)

public:
   PrintHandler();

   // Print to the output stream
   void printToOutput(const char* const msg);

   bool isOpen() const;                   // Is the data file open?
   bool isFailed() const;                 // Did we have an open or write error?
   bool isFileEmpty() const;              // Has the file been written to yet?

   virtual bool openFile();               // Open the data file
   virtual void closeFile();              // Close the data file

   const char* getFilename() const;       // File name as entered
   const char* getPathname() const;       // Path to file

   const char* getFullFilename() const;   // File name with path and possible version number
                                          // (valid only while file is open)

   // File and path names; set before calling openFile()
   virtual bool setFilename(const Basic::String* const msg);
   virtual bool setPathName(const Basic::String* const msg);

protected:
   void setFullFilename(const char* const name);

   void processRecordImp(const DataRecordHandle* const handle) override;

private:
   void initData();

   // from FileWriter.h:
   std::ofstream* sout;             // output file stream (pointer)
   char* fullFilename;              // Full file name of the output file
   const Basic::String* filename;   // Output file name
   const Basic::String* pathname;   // Path to the output file directory

   bool fileOpened;                 // File opened
   bool fileFailed;                 // Open or write failed
   bool firstPassFlg;               // First pass flag
   bool fileEmpty;                  // Is the file opened and empty?
};

} // End Recorder namespace
} // End Eaagles namespace

#endif
