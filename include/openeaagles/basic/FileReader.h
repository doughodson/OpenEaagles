//------------------------------------------------------------------------------
// Class: FileReader
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_FileReader_H__
#define __Eaagles_Basic_FileReader_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

class Number;
class String;

//------------------------------------------------------------------------------
// Class: FileReader
// Base class:  Object -> FileReader
//
// Description: Manages the reading of fixed record length files
//
// Factory name: FileReader
// Slots:
//     pathname     <String>      ! Path to the file (default: null string)
//     filename     <String>      ! File name (appended to pathname) (default: null string)
//     recordLength <Number>      ! Length (in characters) of the records (default: 0)
//
//
// Public member functions:
//
//    bool open()
//       Opens 'filename' in directory 'pathname' with 'recordLength'
//       characters per record.  If 'recordLength' is not provided, it
//       must be set using setRecordLength() before the first access.
//       Returns true of all is well.
//
//    bool isReady()
//       Returns true if the file is open and ready to use.
//
//    bool setPathname(const char* path)
//       Sets the path name to the file.  If provided, this string is used
//       as a prefix to file name.
//
//    bool setFilename(const char* file)
//       Sets the name of the file to be read.  If the path name is provided,
//       the file name is appended to the path name.
//
//    int getRecordNumber() const
//       Returns the current record number.
//
//    bool setRecordNumber(int num)
//       Sets the current record number to 'num' and returns true if all is well.
//
//    int getRecordLength() const
//       Returns the current record length.
//
//    bool setRecordLength(int len)
//       Sets the current record length to 'len' and returns true if all is well.
//
//    const char* getRecord()
//       Reads the current record.
//
//    const char* getRecord(int num)
//       Reads record number 'num'.
//
//    const char* getRecord(int n, int len)
//       Reads 'len' characters starting at record number 'num'.
//       'Len' can be up to 4 times recordLength().
//
//    const char* getFirstRecord()
//    const char* getNextRecord()
//       Reads the first/next record.
//
//    backRecord()
//    backRecord(int n)
//       Decrements the current record by 'n' or 1.
//
//    skipRecord()
//    skipRecord(int n)
//       Increments the current record by 'n' or 1.
//
//
// Notes:
//
//  1) Record numbers range from 1 to the number of records in the file.
//
//  2) getRecord(), getFirstRecord() and getNextRecord() return 0
//     for invalid record numbers (e.g., beyond end of file).
//
//  3) The 'const char*' returned from getRecord(), getFirstRecord(),
//     and getNextRecord() point to an internal buffer which is only
//     valid until the next read.
//
//  4) The file name and path names are limited to 255 characters.
//
//------------------------------------------------------------------------------
class FileReader : public Object {
    DECLARE_SUBCLASS(FileReader,Object)

public:
    FileReader();

   bool open();
   bool isReady();

   const char* getPathname() const        { return pathname; }
   bool setPathname(const char* path);

   const char* getFilename() const        { return filename; }
   bool setFilename(const char* file);

   int getRecordLength() const            { return rlen; }
   bool setRecordLength(const int len);

   int getRecordNumber() const            { return rnum; }
   bool setRecordNumber(const int num);

   const char* getRecord(const int n = -1, const int len = 0);

   void backRecord(const int n = 1);
   void skipRecord(const int n = 1);

   const char* getFirstRecord();
   const char* getNextRecord();

private:
   bool setSlotPathname(String* const msg);
   bool setSlotFilename(String* const msg);
   bool setSlotRecordLength(Number* const msg);

   std::ifstream* dbf;

   int   rnum;          // record number
   int   crnum;         // current (in memory) record number
   int   rlen;          // record length
   char* rec;           // the (in memory) record

   static const size_t FILENAME_LENGTH = 256; // Max length of file name
   static const size_t PATHNAME_LENGTH = 256; // Max length of path name
   char  filename[FILENAME_LENGTH];           // file name
   char  pathname[PATHNAME_LENGTH];           // path name
};


//------------------------------------------------------------------------------
// inline member functions
//------------------------------------------------------------------------------
inline bool FileReader::setRecordNumber(const int num)
{
   rnum = num;
   return true;
}

inline void FileReader::backRecord(const int n)
{
   int i = rnum - n;
   if (i >= 0) rnum = i;
}

inline void FileReader::skipRecord(const int n)
{
   rnum += n;
}

inline const char* FileReader::getNextRecord()
{
   rnum++;
   return getRecord();
}

inline const char* FileReader::getFirstRecord()
{
   rnum = 1;
   return getRecord();
}

} // End Basic namespace
} // End Eaagles namespace

#endif
