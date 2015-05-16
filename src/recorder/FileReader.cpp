
#include "openeaagles/recorder/FileReader.h"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/basic/String.h"
#include <fstream>
#include <cstdlib>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {

//==============================================================================
// Class FileReader
//==============================================================================
IMPLEMENT_SUBCLASS(FileReader,"RecorderFileReader")

// Slot table for this form type
BEGIN_SLOTTABLE(FileReader)
    "filename",         // 1) Data file name
    "pathname",         // 2) Path to the data file directory (optional)
END_SLOTTABLE(FileReader)

// Map slot table to handles
BEGIN_SLOT_MAP(FileReader)
    ON_SLOT( 1, setFilename, Basic::String)
    ON_SLOT( 2, setPathName, Basic::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileReader::FileReader()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void FileReader::initData()
{
   ibuf = new char[MAX_INPUT_BUFFER_SIZE];
   sin = nullptr;
   filename = nullptr;
   pathname = nullptr;
   fileOpened = false;
   fileFailed = false;
   firstPassFlg = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void FileReader::copyData(const FileReader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Need to re-open the file
   if (sin != nullptr) {
      if (isOpen()) sin->close();
      delete sin;
   }
   sin = nullptr;
   setFilename(org.filename);
   setPathName(org.pathname);
   fileOpened = false;
   fileFailed = false;
   firstPassFlg = true;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void FileReader::deleteData()
{
   if (sin != nullptr) {
      if (isOpen()) sin->close();
      delete sin;
   }
   sin = nullptr;

   setFilename(nullptr);
   setPathName(nullptr);

   if (ibuf != nullptr) { delete[] ibuf; ibuf = nullptr; }
}


//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------
bool FileReader::isOpen() const
{
   return fileOpened && sin != nullptr && sin->is_open();
}

bool FileReader::isFailed() const
{
   return fileFailed || (sin != nullptr && sin->fail());
}


//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool FileReader::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // local flags (default is success)
   bool tOpened = true;
   bool tFailed = false;

   // Need a file name
   if (filename == nullptr || filename->len() ==  0) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "FileReader::openFile(): Unable to open data file: no file name" << std::endl;
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
      // Make sure that it exists
      //---
      bool validName = doesFileExist(fullname);

      //---
      // When we have a valid file name ...
      //---
      if ( validName ) {
         //---
         // Make sure we have an input stream
         //---
         if (sin == nullptr) sin = new std::ifstream();

         //---
         // Open the file (binary input mode)
         //---
         sin->open(fullname, std::ios_base::in | std::ios_base::binary );

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "FileReader::openFile() Opening data file = " << fullname << std::endl;
         }

         if (sin->fail()) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileReader::openFile(): Failed to open data file: " << fullname << std::endl;
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
void FileReader::closeFile()
{
   if (isOpen()) {
      sin->close();
      fileOpened = false;
      fileFailed = false;
   }
}

//------------------------------------------------------------------------------
// Read a record
//------------------------------------------------------------------------------
const DataRecordHandle* FileReader::readRecordImp()
{
   DataRecordHandle* handle = nullptr;

   // First pass?  Does the file need to be opened?
   if (firstPassFlg) {
      if ( !isOpen() && !isFailed() ) {
         openFile();
      }
      firstPassFlg = false;
   }


   // When the file is open and ready ...
   if ( isOpen() && !isFailed() && !sin->eof() ) {

      // Number of bytes in the next serialized DataRecord
      unsigned int n = 0;

      // ---
      // Read the size of the next serialized DataRecord
      // ---
      char nbuff[8];
      sin->read(nbuff, 4);

      // Check for error or eof
      if ( sin->eof() || sin->fail() ) {
         fileFailed = sin->fail();
         if (fileFailed && isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
            std::cerr << "FileReader::readRecord() -- error reading data record size" << std::endl;
         }
      }

      // Ok then get the size of the message from the buffer
      else {
         nbuff[4] = '\0';
         n = std::atoi(nbuff);
      }


      // ---
      // Read the serialized DataRecord from the file, parse it as a DataRecord
      // and put it into a Handle.
      // ---
      if (n > 0) {

         // Read message into ibuf
         sin->read(ibuf, n);

         // Check for error or eof
         if ( sin->eof() || sin->fail() ) {
            if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
               std::cerr << "FileReader::readRecord() -- error reading data record" << std::endl;
            }
            fileFailed = true;
         }

         // Ok, create the DataRecord with handle
         else {

            // Parse the DataRecord
            std::string wireFormat(ibuf, n);
            Pb::DataRecord* dataRecord = new Pb::DataRecord();
            bool ok = dataRecord->ParseFromString(wireFormat);

            // Create a handle for the DataRecord (it now has ownership)
            if (ok) {
               handle = new DataRecordHandle(dataRecord);
            }

            // parsing error
            else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
               std::cerr << "FileReader::readRecord() -- ParseFromString() error" << std::endl;
               delete dataRecord;
               dataRecord = nullptr;
            }
         }

      }

   }

   return handle;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool FileReader::setFilename(const Basic::String* const msg)
{
   if (filename != nullptr) { filename->unref(); filename = nullptr; }
   if (msg != nullptr) filename = new Basic::String(*msg);

   return true;
}

bool FileReader::setPathName(const Basic::String* const msg)
{
   if (pathname != nullptr) { pathname->unref(); pathname = nullptr; }
   if (msg != nullptr) pathname = new Basic::String(*msg);

   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* FileReader::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& FileReader::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
