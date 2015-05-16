
#include "openeaagles/recorder/FileWriter.h"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.h"

#include "openeaagles/basic/String.h"
#include <fstream>
#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {

//==============================================================================
// Class FileWriter
//==============================================================================
IMPLEMENT_SUBCLASS(FileWriter,"RecorderFileWriter")

// Slot table for this form type
BEGIN_SLOTTABLE(FileWriter)
    "filename",         // 1) Data file name (required)
    "pathname",         // 2) Path to the data file directory (optional)
END_SLOTTABLE(FileWriter)

// Map slot table to handles
BEGIN_SLOT_MAP(FileWriter)
    ON_SLOT( 1, setFilename, Basic::String)
    ON_SLOT( 2, setPathName, Basic::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileWriter::FileWriter()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void FileWriter::initData()
{
   sout = nullptr;
   fullFilename = nullptr;
   filename = nullptr;
   pathname = nullptr;
   fileOpened = false;
   fileFailed = false;
   eodFlag    = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void FileWriter::copyData(const FileWriter& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setFilename(org.filename);
   setPathName(org.pathname);

   // Need to re-open the file
   if (sout != nullptr) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = nullptr;
   fileOpened = false;
   fileFailed = false;
   eodFlag    = false;
   setFullFilename(nullptr);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void FileWriter::deleteData()
{
   if (sout != nullptr) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = nullptr;

   setFilename(nullptr);
   setPathName(nullptr);
}


//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool FileWriter::shutdownNotification()
{
   // Close the file, if it's still open
   if (isOpen()) closeFile();

   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------

// Is the data file open?
bool FileWriter::isOpen() const
{
   return fileOpened && sout != nullptr && sout->is_open();
}

// Did we have an open or write error?
bool FileWriter::isFailed() const
{
   return fileFailed || (sout != nullptr && sout->fail());
}

// File name with path and possible version number
const char* FileWriter::getFullFilename() const
{
   return fullFilename;
}

// File name as entered
const char* FileWriter::getFilename() const
{
   const char* p = nullptr;
   if (filename != nullptr) p = *filename;
   return p;
}

// Path to file
const char* FileWriter::getPathname() const
{
   const char* p = nullptr;
   if (pathname != nullptr) p = *pathname;
   return p;
}


//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool FileWriter::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // clear the old 'full' file name
   setFullFilename(nullptr);

   // local flags (default is success)
   bool tOpened = true;
   bool tFailed = false;
   eodFlag = false;

   // Need a file name
   if (filename == nullptr || filename->len() ==  0) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "FileWriter::openFile(): Unable to open data file: no file name" << std::endl;
      }
      tOpened = false;
      tFailed = true;
   }
   else {

      //---
      // Allocate space for the full file name
      //---
      size_t nameLength = 0;
      if (pathname != nullptr) {
         nameLength += pathname->len();     // add the length of the path name
         nameLength += 1;                         // add a character for the slash
      }
      nameLength += filename->len();           // add the length of the file name
      nameLength += 4;                         // add characters for possible version number, "_V99"
      nameLength += 1;                         // Add one for the null(0) at the end of the string

      char* fullname = new char[nameLength];
      fullname[0] = '\0';

      //---
      // Create the (initial) full file name
      //---
      if (pathname != nullptr && pathname->len() > 0) {
         lcStrcat(fullname, nameLength ,*pathname);
         lcStrcat(fullname, nameLength, "/");
      }
      lcStrcat(fullname,nameLength,*filename);

      //---
      // Make sure that it doesn't already exist (we don't want to over write good data).
      //---
      bool validName = !doesFileExist(fullname);
      if ( !validName ) {
         // If the file already exists, try appending a version number "v99" ..

         char* origname = new char[nameLength];
         lcStrcpy(origname, nameLength, fullname);

         validName = false;
         for (unsigned int i = 1; i <= 99 && !validName; i++) {
            std::sprintf(fullname, "%s_v%02d", origname, i);
            validName = !doesFileExist(fullname);
         }

         if ( !validName ) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileWriter::openFile(): All version of the data file already exists: " << origname << std::endl;
            }
            tOpened = false;
            tFailed = true;
         }
         delete[] origname;
      }

      //---
      // When we have a valid file name ...
      //---
      if ( validName ) {

         // The file name with the path and version number
         setFullFilename(fullname);

         //---
         // Make sure we have an output stream
         //---
         if (sout == nullptr) sout = new std::ofstream();

         //---
         // Open the file (binary output mode)
         //---
         sout->open(fullname, std::ios_base::out | std::ios_base::binary );

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "FileWriter::openFile() Opening data file = " << fullname << std::endl;
         }

         if (sout->fail()) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileWriter::openFile(): Failed to open data file: " << fullname << std::endl;
            }
            tOpened = false;
            tFailed = true;
         }

      }

      delete[] fullname;
   }

   fileOpened = tOpened;
   fileFailed = tFailed;
   return fileOpened;
}


//------------------------------------------------------------------------------
// Close the data file
//------------------------------------------------------------------------------
void FileWriter::closeFile()
{
   if (isOpen()) {

      // ---
      // Write the EOD message, if it hasn't already been.
      if (!eodFlag) {
         eodFlag = true;

         // write something to signify don't read any more (e.g., last message)
         Pb::DataRecord* lastMsg = new Pb::DataRecord();

         // This will be the token representing the last message, but it can be
         // anything that is not one of the other event messages
         lastMsg->set_id(REID_END_OF_DATA);

         // Time is also required, although not used:
         Pb::Time* time = lastMsg->mutable_time();
         time->set_exec_time(0);
         time->set_sim_time(0);
         time->set_utc_time(0);

         // get a handle
         DataRecordHandle* handle = new DataRecordHandle(lastMsg);

         // write the message
         processRecordImp(handle);
         handle->unref();
         handle = nullptr;
      }

      // now close the file
      sout->close();
      fileOpened = false;
      fileFailed = false;

   }
}


//------------------------------------------------------------------------------
// Serialize and write a DataRecord to a file
//------------------------------------------------------------------------------
void FileWriter::processRecordImp(const DataRecordHandle* const handle)
{
   bool thisIsEodMsg = false;

   // ---
   // Open the file, if it hasn't been already ...
   // ---
   if ( !fileOpened && !fileFailed ) openFile();

   // ---
   // When the file is open and ready ...
   // --- serialize and write the data record.
   // ---
   if ( fileOpened ) {

      // The DataRecord to be sent
      const Pb::DataRecord* dataRecord = handle->getRecord();

      // Serialize the DataRecord
      std::string wireFormat;
      bool ok = dataRecord->SerializeToString(&wireFormat);

      // Write the serialized DataRecord with its length to the file
      if (ok) {
         unsigned int n = wireFormat.length();

         // Convert size to an integer string
         char nbuff[8];
         std::sprintf(nbuff, "%04d", n);

         // Convert the leading zeros to spaces
         if (nbuff[0] == '0') {
            nbuff[0] = ' ';
            if (nbuff[1] == '0') {
               nbuff[1] = ' ';
               if (nbuff[2] == '0') {
                  nbuff[2] = ' ';
               }
            }
         }

         // Write the size of the serialized DataRecord as an ascii string
         sout->write(nbuff, 4);

         // Write the serialized DataRecord
         sout->write( wireFormat.c_str(), n );
      }

      else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
         // If we had an error serializing the DataRecord
         std::cerr << "FileWriter::processRecordImp() -- SerializeToString() error" << std::endl;
      }

      // Check for END_OF_DATA message
      thisIsEodMsg = (dataRecord->id() == REID_END_OF_DATA);

   }

   // ---
   // Close the file at END_OF_DATA message
   // ---
   if (!eodFlag && thisIsEodMsg) {
      eodFlag = true;
      closeFile();
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

void FileWriter::setFullFilename(const char* const name)
{
   if (fullFilename != nullptr) {
      delete[] fullFilename;
      fullFilename = nullptr;
   }
   if (name != nullptr) {
      size_t n = std::strlen(name) + 1;
      fullFilename = new char[n];
      lcStrcpy(fullFilename, n, name);
   }
}

bool FileWriter::setFilename(const Basic::String* const msg)
{
   if (filename != nullptr) { filename->unref(); filename = nullptr; }
   if (msg != nullptr) filename = new Basic::String(*msg);

    return true;
}

bool FileWriter::setPathName(const Basic::String* const msg)
{
   if (pathname != nullptr) { pathname->unref(); pathname = nullptr; }
   if (msg != nullptr) pathname = new Basic::String(*msg);

   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* FileWriter::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& FileWriter::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // File name
    if (filename != nullptr && filename->len() > 0) {
        indent(sout,i+j);
        sout << "filename: \"" << *filename << "\""<< std::endl;
    }

    // Path name
    if (pathname != nullptr && pathname->len() > 0) {
        indent(sout,i+j);
        sout << "pathname: \"" << *pathname << "\"" << std::endl;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Recorder namespace
} // End Eaagles namespace
