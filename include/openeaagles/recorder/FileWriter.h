//------------------------------------------------------------------------------
// Class: FileWriter
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_FileWriter_H__
#define __Eaagles_Recorder_FileWriter_H__

#include "openeaagles/recorder/OutputHandler.h"

namespace Eaagles {
   namespace Basic { class String; }
namespace Recorder {

//------------------------------------------------------------------------------
// Class:   FileWriter
// Description: Serialize and write the data from a protocol buffer DataRecord
//              message to a file.
//
// Factory name: FileWriter
// Slots:
//     filename       <String>     ! Data file name
//     pathname       <String>     ! Path to the data file's directory (optional)
//
// Note:
//    1) The data file consists of a sequence of serialized data records
//    that are preceded by 4 bytes that provided the size of each data record
//    in bytes.  The 4 bytes are stored as an ascii string with leading spaces
//    (e.g., " 123")
//
//    2) During open(), if the file already exists then a version number is appended
//    to the end of the file name.  (e.g., filename_v01 to filename_v99)
//
//    3) If the file hasn't been manually opened with openFile(), the file will
//    be automatically open with the first data message.
//
//    4) File will be closed with an end of data (REID_END_OF_DATA) message.
//    Calling openFile() or sending any additional data messages will open
//    a new file with a new version number.
//------------------------------------------------------------------------------
class FileWriter : public OutputHandler {
    DECLARE_SUBCLASS(FileWriter, OutputHandler)

public:
   FileWriter();

   bool isOpen() const;                   // Is the data file open?
   bool isFailed() const;                 // Did we have an open or write error?

   bool openFile();                       // Open the data file
   void closeFile();                      // Close the data file

   const char* getFilename() const;       // File name as entered
   const char* getPathname() const;       // Path to file

   const char* getFullFilename() const;   // File name with path and possible version number
                                          // (valid only while file is open)

   // File and path names; set before calling openFile()
   virtual bool setFilename(const Basic::String* const msg);
   virtual bool setPathName(const Basic::String* const msg);

protected:
   void setFullFilename(const char* const name);

   // OutputHandler class private functions
   virtual void processRecordImp(const DataRecordHandle* const handle);

   // Basic::Component protected interface functions
   virtual bool shutdownNotification();     // We're shutting down

private:
   void initData();

   std::ofstream* sout;             // Output stream

   char* fullFilename;              // Full file name of the output file
   const Basic::String* filename;   // Output file name
   const Basic::String* pathname;   // Path to the output file directory

   bool fileOpened;                 // File opened
   bool fileFailed;                 // Open or write failed
   bool eodFlag;                    // REID_END_OF_DATA message has been written
};

} // End Recorder namespace
} // End Eaagles namespace

#endif
