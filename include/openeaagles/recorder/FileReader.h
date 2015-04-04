//------------------------------------------------------------------------------
// Class: FileReader
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_FileReader_H__
#define __Eaagles_Recorder_FileReader_H__

#include "openeaagles/recorder/InputHandler.h"

namespace Eaagles {
   namespace Basic { class String; }
namespace Recorder {

//------------------------------------------------------------------------------
// Class:   FileReader
// Description: Read and parse data records from a file
//
// Factory name: FileReader
// Slots:
//     filename       <String>     ! Data file name (required)
//     pathname       <String>     ! Path to the data file's directory (optional)
//
// Notes
//    1) The data file consists of a sequence of serialized data records
//    that are preceded by 4 bytes that provided the size of each data record
//    in bytes.  The 4 bytes are stored as an ascii string with leading spaces
//    (e.g., " 123")
//------------------------------------------------------------------------------
class FileReader : public InputHandler
{
    DECLARE_SUBCLASS(FileReader, InputHandler)

public:
   static const unsigned int MAX_INPUT_BUFFER_SIZE = 2000;

public:
   FileReader();

   bool isOpen() const;             // Is the data file open?
   bool isFailed() const;           // Did we have an open or read error?

   virtual bool openFile();         // Open the data file
   virtual void closeFile();        // Close the data file

   // File and path names; set before calling openFile()
   virtual bool setFilename(const Basic::String* const msg);
   virtual bool setPathName(const Basic::String* const msg);

protected:
   const DataRecordHandle* readRecordImp() override;

private:
   void initData();

   char* ibuf;                      // Input data buffer

   std::ifstream* sin;              // Input stream
   const Basic::String* filename;   // File name
   const Basic::String* pathname;   // Path to the data file's directory
   bool fileOpened;                 // File opened
   bool fileFailed;                 // Open or read failed
   bool firstPassFlg;               // First pass flag
};

} // End Recorder namespace
} // End Eaagles namespace

#endif
