
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/recorder/generated-code/DataRecord.pb.h"

namespace Eaagles {
namespace Recorder {

IMPLEMENT_SUBCLASS(DataRecordHandle,"DataRecordHandle")
EMPTY_SLOTTABLE(DataRecordHandle)
EMPTY_SERIALIZER(DataRecordHandle)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
DataRecordHandle::DataRecordHandle()
{
   STANDARD_CONSTRUCTOR()
   record = 0;
}

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataRecordHandle::DataRecordHandle(Pb::DataRecord* const r) : record(r)
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DataRecordHandle::copyData(const DataRecordHandle& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) record = new Pb::DataRecord();

   // Copy the record
   *record = *org.record;
}

// deleteData() -- delete member data
void DataRecordHandle::deleteData()
{
   if (record != 0) { delete record;  record = 0; }
}

} // End Recorder namespace
} // End Eaagles namespace
