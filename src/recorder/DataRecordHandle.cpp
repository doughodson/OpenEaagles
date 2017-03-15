
#include "openeaagles/recorder/DataRecordHandle.hpp"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"

namespace oe {
namespace recorder {

IMPLEMENT_SUBCLASS(DataRecordHandle,"DataRecordHandle")
EMPTY_SLOTTABLE(DataRecordHandle)
EMPTY_SERIALIZER(DataRecordHandle)

DataRecordHandle::DataRecordHandle()
{
   STANDARD_CONSTRUCTOR()
}

DataRecordHandle::DataRecordHandle(pb::DataRecord* const r) : record(r)
{
   STANDARD_CONSTRUCTOR()
}

void DataRecordHandle::copyData(const DataRecordHandle& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) record = new pb::DataRecord();

   // Copy the record
   *record = *org.record;
}

void DataRecordHandle::deleteData()
{
   if (record != nullptr) { delete record;  record = nullptr; }
}

}
}
