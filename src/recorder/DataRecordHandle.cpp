
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"

namespace oe {
namespace recorder {

IMPLEMENT_SUBCLASS(DataRecordHandle,"DataRecordHandle")
EMPTY_SLOTTABLE(DataRecordHandle)
EMPTY_SERIALIZER(DataRecordHandle)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
DataRecordHandle::DataRecordHandle()
{
   STANDARD_CONSTRUCTOR()
   record = nullptr;
}

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataRecordHandle::DataRecordHandle(pb::DataRecord* const r) : record(r)
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DataRecordHandle::copyData(const DataRecordHandle& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) record = new pb::DataRecord();

   // Copy the record
   *record = *org.record;
}

// deleteData() -- delete member data
void DataRecordHandle::deleteData()
{
   if (record != nullptr) { delete record;  record = nullptr; }
}

}
}
