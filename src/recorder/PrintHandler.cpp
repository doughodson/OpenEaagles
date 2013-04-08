
#include "openeaagles/recorder/PrintHandler.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Number.h"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {


//==============================================================================
// Class PrintHandler
//==============================================================================
IMPLEMENT_SUBCLASS(PrintHandler,"PrintHandler")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(PrintHandler)
   "filename",     // 1) Data file name (required)
   "pathname",     // 2) Path to the data file directory (optional)
END_SLOTTABLE(PrintHandler)

// Map slot table to handles 
BEGIN_SLOT_MAP(PrintHandler)
   ON_SLOT( 1, setFilename,        Basic::String)
   ON_SLOT( 2, setPathName,        Basic::String)
END_SLOT_MAP()

EMPTY_SERIALIZER(PrintHandler)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
PrintHandler::PrintHandler()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void PrintHandler::initData()
{
   fullFilename = 0;
   filename = 0;
   pathname = 0;
   sout = 0;
   fileOpened = false;
   fileFailed = false;
   firstPassFlg = true;
   fileEmpty = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void PrintHandler::copyData(const PrintHandler& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setFilename(org.filename);
   setPathName(org.pathname);

   // Need to re-open the file
   if (sout != 0) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = 0;
   fileOpened = false;
   fileFailed = false;
   firstPassFlg = true;
   fileEmpty = true;
   setFullFilename(0);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void PrintHandler::deleteData()
{
   if (sout != 0) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = 0;

   setFilename(0);
   setPathName(0);
}



//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------

// Is the data file open?
bool PrintHandler::isOpen() const
{
   return fileOpened && sout != 0 && sout->is_open();
}

// Did we have an open or write error?
bool PrintHandler::isFailed() const
{
   return fileFailed || (sout != 0 && sout->fail());
}

// File name with path and possible version number
const char* PrintHandler::getFullFilename() const
{
   return fullFilename;
}

// File name as entered
const char* PrintHandler::getFilename() const
{
   const char* p = 0;
   if (filename != 0) p = *filename;
   return p;
}

// Path to file
const char* PrintHandler::getPathname() const
{
   const char* p = 0;
   if (pathname != 0) p = *pathname;
   return p;
}

// Is the data file empty?
bool PrintHandler::isFileEmpty() const
{
   return fileEmpty;
}

//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool PrintHandler::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // If we don't have a file name then we're using the standard output
   if (filename == 0 || filename->len() ==  0) return true;

   // clear the old 'full' file name
   setFullFilename(0);

   // local flags (default is success)
   bool tOpened = true;
   bool tFailed = false;


   //---
   // Allocate space for the full file name
   //---
   size_t nameLength = 0;
   if (pathname != 0) {
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
   if (pathname != 0 && pathname->len() > 0) {
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
            std::cerr << "PrintHandler::openFile(): All version of the data file already exists: " << origname << std::endl;
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
      if (sout == 0) sout = new std::ofstream();

      //---
      // Open the file
      //---
      sout->open(fullname, std::ios_base::out);

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "PrintHandler::openFile() Opening data file = " << fullname << std::endl;
      }

      if (sout->fail()) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PrintHandler::openFile(): Failed to open data file: " << fullname << std::endl;
         }
         tOpened = false;
         tFailed = true;
      }

   }

   delete[] fullname;

   fileOpened = tOpened;
   fileFailed = tFailed;

   return fileOpened;
}


//------------------------------------------------------------------------------
// Close the data file
//------------------------------------------------------------------------------
void PrintHandler::closeFile()
{
   if (isOpen()) {

      // close the file
      if (sout != 0) sout->close();

      fileOpened = false;
      fileFailed = false;
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

void PrintHandler::setFullFilename(const char* const name)
{
   if (fullFilename != 0) {
      delete[] fullFilename;
      fullFilename = 0;
   }
   if (name != 0) {
      size_t n = strlen(name) + 1;
      fullFilename = new char[n];
      lcStrcpy(fullFilename, n, name);
   }
}

bool PrintHandler::setFilename(const Basic::String* const msg)
{
   if (filename != 0) { filename->unref(); filename = 0; }
   if (msg != 0) filename = new Basic::String(*msg);

    return true;
}

bool PrintHandler::setPathName(const Basic::String* const msg)
{
   if (pathname != 0) { pathname->unref(); pathname = 0; }
   if (msg != 0) pathname = new Basic::String(*msg);

   return true;
}


//------------------------------------------------------------------------------
// print to output stream
//------------------------------------------------------------------------------
void PrintHandler::printToOutput(const char* const msg)
{
   // First pass?  Do we need to open a file?
   if (firstPassFlg) {
      if ( filename != 0 && !isOpen() && !isFailed() ) {
         openFile();
      }
      firstPassFlg = false;
   }

   // Output to a file?
   if (sout != 0 && isOpen()) {
      *sout << msg << std::endl;
      fileEmpty = false;
   }

   // Or to the standard output stream
   else {
      std::cout << msg << std::endl;
   }
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* PrintHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// Print the data from a DataRecord
//------------------------------------------------------------------------------
void PrintHandler::processRecordImp(const DataRecordHandle* const handle)
{
   // baseclass
}

} // End Recorder namespace
} // End Eaagles namespace
