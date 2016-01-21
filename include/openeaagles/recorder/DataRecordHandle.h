//------------------------------------------------------------------------------
// Class: DataRecordHandle
//------------------------------------------------------------------------------
#ifndef __oe_Recorder_DataRecordHandle_H__
#define __oe_Recorder_DataRecordHandle_H__

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Recorder {

   // Main (protocol buffer) data record
   namespace Pb { class DataRecord; }

//------------------------------------------------------------------------------
// Class: DataRecordHandle
// Description: Handle to a DataRecord, so we can ref count it.
//
// Notes:
// 
//    1) This handle will 'own' the DataRecord ...
//
//    2) When this handle is destroyed, the DataRecord will be deleted.
//
//    3) Using the assignment operator ( e.g., handle1 = handle2; ), the contents
//       of handle2's DataRecord will be copied into handle1's DataRecord.
//
//------------------------------------------------------------------------------
class DataRecordHandle : public basic::Object
{
    DECLARE_SUBCLASS(DataRecordHandle, basic::Object)

public:
   DataRecordHandle(Pb::DataRecord* const record);

   const Pb::DataRecord* getRecord() const;

protected:
   DataRecordHandle();  // Default Constructor

private:
   Pb::DataRecord* record;
};

inline const Pb::DataRecord* DataRecordHandle::getRecord() const { return record; }

} // End Recorder namespace
} // End oe namespace

#endif
