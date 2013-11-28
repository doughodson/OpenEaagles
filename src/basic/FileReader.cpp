//------------------------------------------------------------------------------
// Class: FileReader
//------------------------------------------------------------------------------

#include <fstream>

#include "openeaagles/basic/FileReader.h"

#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(FileReader,"FileReader")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(FileReader)
    "pathname",      // 1) Path to the file
    "filename",      // 2) File name (appended to pathname)
    "recordLength",  // 3) Length (in characters) of the records
END_SLOTTABLE(FileReader)

// Map slot table to handles 
BEGIN_SLOT_MAP(FileReader)
    ON_SLOT(1,setSlotPathname,String)
    ON_SLOT(2,setSlotFilename,String)
    ON_SLOT(3,setSlotRecordLength,Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileReader::FileReader()
{
   STANDARD_CONSTRUCTOR()

   dbf = 0;

   rec = 0;
   rlen = 0;

   rnum = 1;
   crnum = -1;


   pathname[0] = '\0';
   filename[0] = '\0';
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void FileReader::copyData(const FileReader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
      rec = 0;
      dbf = 0;
      pathname[0] = '\0';
      filename[0] = '\0';
   }

   // Close the old file (we'll need to open() the new one)
   if (dbf != 0) dbf->close();

   lcStrcpy(pathname, PATHNAME_LENGTH, org.pathname);
   lcStrcpy(filename, FILENAME_LENGTH, org.filename);

   rnum = org.rnum;
   crnum = org.crnum;

   setRecordLength(org.getRecordLength());
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void FileReader::deleteData()
{
   // Close file and delete stream
   if (dbf != 0) {
      dbf->close();
      delete dbf;
      dbf = 0;
   }

   // Delete the record buffer
   if (rec != 0) {
      delete[] rec;
      rec = 0;
   }
}


//------------------------------------------------------------------------------
// isReady() -- Returns true we're ready to get records
//------------------------------------------------------------------------------
bool FileReader::isReady()
{
   bool ready = false;
   if (dbf != 0 && rec != 0) {
      if (rlen > 0 && dbf->is_open()) ready = true;
   }
   return ready;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setPathname() -- sets the path name (limit to 255 chars)
bool FileReader::setPathname(const char* const path)
{
   bool ok = false;
   if (path != 0) {
      lcStrncpy(pathname, (PATHNAME_LENGTH-1), path, (PATHNAME_LENGTH-1));
      pathname[PATHNAME_LENGTH-1] = '\0';
      ok = true;
   }
   return ok;
}

// setFilename() -- sets the file name (limit to 255 chars)
bool FileReader::setFilename(const char* const file)
{
   bool ok = false;
   if (file != 0) {
      lcStrncpy(filename, (FILENAME_LENGTH-1), file, (FILENAME_LENGTH-1));
      filename[FILENAME_LENGTH-1] = '\0';
      ok = true;
   }
   return ok;
}

// setRecordLength() -- sets the record length & creates the internal buffer
bool FileReader::setRecordLength(const int len)
{
   if (rlen != len) {
      if (rec != 0) {
         delete[] rec;
      }
      rlen = len;
      int tlen = rlen*4;       // allow space for 4 times the record length
      rec = new char[tlen+1];
      rec[0] = '\0';
   }
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool FileReader::setSlotPathname(String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setPathname( *msg );
   }
   return ok;
}

bool FileReader::setSlotFilename(String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setFilename( *msg );
   }
   return ok;
}

bool FileReader::setSlotRecordLength(Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setRecordLength( msg->getInt() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Open
//------------------------------------------------------------------------------
bool FileReader::open()
{
   // Create the full file name
   const size_t FILE_LENGTH = 512; // Max length of file
   char file[FILE_LENGTH];
   lcStrcpy(file, FILE_LENGTH, pathname);
   lcStrcat(file, FILE_LENGTH, "/");
   lcStrcat(file, FILE_LENGTH, filename);

   // Open the file ...
   if (dbf != 0) {
      // close any previous files
      dbf->close();
   }
   else {
      // Create the input stream
      dbf = new std::ifstream();
   }
   dbf->open(file);
   dbf->clear();

   rnum = 1;
   crnum = -1;
   return (dbf->is_open());
}


//------------------------------------------------------------------------------
// getRecord() --
//------------------------------------------------------------------------------
const char* FileReader::getRecord(const int nn, const int ll)
{
   // return nothing if we're not ready (e.g., the record length has not been set)
   if ( !isReady() ) return 0;

   // Set record number
   int n = nn;
   if (n == -1) n = rnum;
   else rnum = n;

   // Set length to read
   int len = ll;
   if (len == 0) len = rlen;

   // Read the record
   bool ok = false;
   if (!dbf->seekg(rlen*(n-1), std::ios::beg).eof()) {
      dbf->read(rec, len);
      if (!dbf->eof() && !dbf->fail()) ok = true;
   }

   if (ok) {
      crnum = rnum;
      rec[len] = '\0';
      return rec;
   }
   else {
      dbf->clear();
      crnum = -1;
      return 0;
   }

}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* FileReader::getSlotByIndex(const int si)
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
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (strlen(pathname) > 0) {
        indent(sout,i+j);
        sout << "pathname: \"";
        sout << pathname;
        sout << "\"" << std::endl;
    }

    if (strlen(filename) > 0) {
        indent(sout,i+j);
        sout << "filename: \"";
        sout << filename;
        sout << "\"" << std::endl;
    }

    if (rlen > 0) {
        indent(sout,i+j);
        sout << "recordLength: ";
        sout << rlen;
        sout << std::endl;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
