
#include "openeaagles/recorder/PrintSelected.h"
#include "openeaagles/recorder/generated-code/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.h"

#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Integer.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

#include <iomanip>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {


//==============================================================================
// Class PrintSelected
//==============================================================================
IMPLEMENT_SUBCLASS(PrintSelected,"PrintSelected")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(PrintSelected)
   "messageToken",   // 1) Message ID (token)
   "fieldName",      // 2) Full field name (e.g., Eaagles.Recorder.Pb.PlayerId.name)
   "compareToValS",  // 3) value to compare (string)
   "compareToValI",  // 4) value to compare (num)
   "compareToValD",  // 5) value to compare (dbl)
   "condition",      // 6) EQ, LT, or GT (ignored for bool and strings)
   "timeOnly",       // 7) match time conditions only. Print ALL messages that match
END_SLOTTABLE(PrintSelected)

// Map slot table to handles 
BEGIN_SLOT_MAP(PrintSelected)
   ON_SLOT( 1, setSlotMsgToken,        Basic::Number)
   ON_SLOT( 2, setSlotFieldName,       Basic::String)
   ON_SLOT( 3, setSlotCompareToStr,    Basic::String)
   ON_SLOT( 4, setSlotCompareToNum,    Basic::Number)
   ON_SLOT( 5, setSlotCompareToDbl,    Basic::Number)
   ON_SLOT( 6, setSlotCondition,       Basic::String)
   ON_SLOT( 7, setSlotTimeOnly,        Basic::Number)
END_SLOT_MAP()

EMPTY_SERIALIZER(PrintSelected)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
PrintSelected::PrintSelected()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void PrintSelected::initData()
{

   msgToken = 0;
   fieldName = "";
   compareValD = 0.0;
   compareValS = "";
   compareValI = 0;
   condition = EQ;

   fieldNameStr = "";
   compareStr = "";
   recMsg = 0;
   eventMsg = 0;
   foundSelected = false;
   printHeader = false;
   timeOnly = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void PrintSelected::copyData(const PrintSelected& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   msgToken = org.msgToken;
   fieldName = org.fieldName;
   compareValD = org.compareValD;
   compareValS = org.compareValS;
   compareValI = org.compareValI;
   condition = org.condition;

   fieldNameStr = org.fieldNameStr;
   compareStr = org.compareStr;
   foundSelected = org.foundSelected;
   printHeader = org.printHeader;
   timeOnly = org.timeOnly;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void PrintSelected::deleteData()
{

}

// Slots

bool PrintSelected::setSlotMsgToken(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      msgToken = msg->getInt();
   }
   return ok;
}

bool PrintSelected::setSlotFieldName(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      fieldName = msg->getCopyString();
      fieldNameStr.assign(fieldName);
   }
   return ok;

}

bool PrintSelected::setSlotCompareToStr(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      compareValS = msg->getCopyString();
      compareStr.assign(compareValS);
   }
   return ok;
}

bool PrintSelected::setSlotCompareToNum(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      compareValI = msg->getInt();
   }
   return ok;
}

bool PrintSelected::setSlotCompareToDbl(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      compareValD = msg->getDouble();
   }
   return ok;
}


bool PrintSelected::setSlotCondition(const Basic::String* const msg)
{

   bool ok = false;
   if (msg != 0) {
      ok = true;

      if ((*msg == "EQ") || (*msg == "eq")) {
         condition = EQ;
      }
      else if ((*msg == "LT") || (*msg == "lt")) {
         condition = LT;
      }
      else if ((*msg == "GT") || (*msg == "gt")) {
         condition = GT;
      }
      else {
         // error (or set default)
         condition = EQ;
      }
   }

   return ok;
}

bool PrintSelected::setSlotTimeOnly(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      timeOnly = msg->getBoolean();
   }
   return ok;
}
//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* PrintSelected::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// processRecordImp
//------------------------------------------------------------------------------
void PrintSelected::processRecordImp(const DataRecordHandle* const handle)
{

   if (handle == 0) return;  // cannot continue
   const Pb::DataRecord* dataRecord = handle->getRecord();
   if (dataRecord == 0) return;  // cannot continue

   // using reflection:
   // save to: const google::protobuf::Message* recMsg
   recMsg = dataRecord;
   //const google::protobuf::Message& root = *recMsg;
   const google::protobuf::Descriptor* descriptor = recMsg->GetDescriptor();
   const google::protobuf::Reflection* reflection = recMsg->GetReflection();

   const google::protobuf::FieldDescriptor* idField = descriptor->FindFieldByName("id");
   unsigned int id = reflection->GetUInt32(*recMsg, idField);

   foundSelected = false;

   // Process time message
   processMessage( &dataRecord->time());

   // Process the event message
   const google::protobuf::Message* processMsg = 0;
   std::string msgCat = "";
   std::string msgType = "";


   switch (msgToken) {
      case REID_FILE_ID: {
         processMsg = &dataRecord->file_id_msg();
         msgCat =  "FILE     ";
         msgType = "ID       ";
         static bool firstTime = true;
         if ((firstTime) || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_NEW_PLAYER: {
         processMsg = &dataRecord->new_player_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "NEW      ";
         static bool firstTime = true;
         if ((firstTime) || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_REMOVED: {
         processMsg = &dataRecord->player_removed_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "REMOVED  ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_DATA: {
         processMsg = &dataRecord->player_data_msg();
         msgCat =  "PLAYER   ";
         msgType = "DATA     ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) {
            printHeader = true;
         }
         firstTime = false;
         break;
      }
      case REID_PLAYER_DAMAGED: {
         processMsg = &dataRecord->player_damaged_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "DAMAGED  ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_COLLISION: {
         processMsg = &dataRecord->player_collision_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "COLLISION";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_CRASH: {
         processMsg = &dataRecord->player_crash_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "CRASH    ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_KILLED: {
         processMsg = &dataRecord->player_killed_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "KILLED   ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_RELEASED: {
         processMsg = &dataRecord->weapon_release_event_msg();
         msgCat =  "WEAPON   ";
         msgType = "RELEASED ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_HUNG: {
         processMsg = &dataRecord->weapon_hung_event_msg();
         msgCat =  "WEAPON   ";
         msgType = "HUNG     ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_DETONATION: {
         processMsg = &dataRecord->weapon_detonation_event_msg();
         msgCat =  "WEAPON  ";
         msgType = "DETONATE";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_GUN_FIRED: {
         processMsg = &dataRecord->gun_fired_event_msg();
         msgCat =  "GUN     ";
         msgType = "FIRED   ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_NEW_TRACK: {
         processMsg = &dataRecord->new_track_event_msg();
         msgCat =  "TRACK   ";
         msgType = "NEW     ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_TRACK_REMOVED: {
         processMsg = &dataRecord->track_removed_event_msg();
         msgCat =  "TRACK   ";
         msgType = "REMOVED ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_TRACK_DATA: {
         processMsg = &dataRecord->track_data_msg();
         msgCat =  "TRACK   ";
         msgType = "DATA    ";
         static bool firstTime = true;
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      default: break;
   }

   // Process the message and see if the conditions are found
   if (!timeOnly && (processMsg != 0)) {
      processMessage(processMsg);
   }

   // If the condition has been found and this is the message we want, print it
   std::stringstream soutFields;
   std::stringstream soutVals;
   if (foundSelected &&  timeOnly) {
      // print the message, whatever it is, because it matches the time criteria
      std::string msgName = getEventMsgName(recMsg);  // This also sets the event message
      if (eventMsg != 0) {
         soutFields << std::left << std::setw(32) << msgName << "\t" << "HEADER" << "\t";
         soutVals << std::left << std::setw(32) << msgName << "\t" "VALUES" << "\t";

         soutFields <<  std::left << std::setw(12)<< "exec time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "sim time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "utc time  "<< "\t" ;

         std::string timeStr = "";

         timeStr = printTimeMsg(dataRecord->time().exec_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().sim_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().utc_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().sim_time() << "\t";
     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().exec_time() << "\t";
     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().utc_time() << "\t";

    //     printMessage(soutFields, soutVals, recMsg);  // Use this to include time and message ID
         printMessage(soutFields, soutVals, eventMsg);
         printToOutput( soutFields.str().c_str() );
         printToOutput( soutVals.str().c_str() );
      }
   }
   else if (foundSelected && (id == msgToken)) {

      // Print the message name

      soutFields << msgCat << "\t" << "HEADER" << "\t";
      soutVals << msgCat << "\t" << msgType << "\t";

      // Print the time
  //    const Pb::Time* timeMsg  = &dataRecord->time();
  //    printMessage(soutFields, soutVals, timeMsg);  // print out the time message

         soutFields <<  std::left << std::setw(12)<< "exec time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "sim time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "utc time  "<< "\t" ;

         std::string timeStr = "";

         timeStr = printTimeMsg(dataRecord->time().exec_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().sim_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().utc_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

      // Print out the event message
      // print the message, whatever it is, because it matches the time criteria
      std::string msgName = getEventMsgName(recMsg);  // This also sets event message
      if (eventMsg != 0) {
         printMessage(soutFields, soutVals, eventMsg);
      }

//      if (processMsg != 0) printMessage(soutFields, soutVals, processMsg);  // print out the event message
      // Print to output:
      if (printHeader) {
         printHeader = false;
         printToOutput( soutFields.str().c_str() );
      }
      printToOutput( soutVals.str().c_str() );
   }


}


//------------------------------------------------------------------------------
// processMessage(): Recursive function to go through all messages
//---------------------------------------------------------------------------
void PrintSelected::processMessage(const google::protobuf::Message* const msg)
{

   const google::protobuf::Message& root = *msg;
   const google::protobuf::Descriptor* descriptor = msg->GetDescriptor();
   const google::protobuf::Reflection* reflection = msg->GetReflection();

   int fieldCount = descriptor->field_count();
   const google::protobuf::FieldDescriptor* fieldDescriptor = 0;

   // look at fields to find a match with the slot condition
   if (fieldCount > 0) {
      // Look at each field in this message
      for (int i = 0; i < fieldCount; i++) {

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // what type is this field?
         google::protobuf::FieldDescriptor::CppType cppType = fieldDescriptor->cpp_type();

         // If this field is a message, then call this again
         if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
            // Get sub message
            const google::protobuf::Message& sub_message = reflection->GetMessage(root, fieldDescriptor);

            // If sub message exists, process it
            if (&sub_message != NULL) {
               // call this until no more messages
               processMessage(&sub_message);
            }
         }
         else {
            // field is not a message. Check File name for match
            if (fieldDescriptor->full_name() == fieldNameStr) {

               // This is the right field. Now check the value, based on type
               if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_STRING) {
                  std::string str = reflection->GetString(root, fieldDescriptor);
                  std::string matchStr = getCompareToStr();
                  if (str == matchStr) {
                     // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_INT32) {
                  int num = reflection->GetInt32(root, fieldDescriptor);
                  if (((condition == EQ) && (num == getCompareToNum())) ||
                     ((condition == GT) && (num > getCompareToNum())) ||
                     ((condition == LT) && (num < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_INT64) {
                  long long num = reflection->GetInt64(root, fieldDescriptor);
                  if (((condition == EQ) && (num == getCompareToNum())) ||
                     ((condition == GT) && (num > getCompareToNum())) ||
                     ((condition == LT) && (num < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_UINT32) {
                  unsigned int num = reflection->GetUInt32(root, fieldDescriptor);
                  if (((condition == EQ) && (num == getCompareToNum())) ||
                     ((condition == GT) && (static_cast<int>(num) > getCompareToNum())) ||
                     ((condition == LT) && (static_cast<int>(num) < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_FLOAT) {
                  double num = static_cast<double>(reflection->GetFloat(root, fieldDescriptor));
                  if ( ((condition == EQ) && equal(num, getCompareToDbl())) ||
                     ((condition == GT) && (num > getCompareToDbl())) ||
                     ((condition == LT) && (num < getCompareToDbl()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE) {
                  double num = reflection->GetDouble(root, fieldDescriptor);
                  if ( ((condition == EQ) && equal(num, getCompareToDbl())) ||
                     ((condition == GT) && (num > getCompareToDbl())) ||
                     ((condition == LT) && (num < getCompareToDbl()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_BOOL) {
                  bool num = reflection->GetBool(root, fieldDescriptor);
                  if (num == getCompareToBool()) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
                  const google::protobuf::EnumValueDescriptor* enumVal = reflection->GetEnum(root, fieldDescriptor);
                  int enumIndex = enumVal->index();
                  if (((condition == EQ) && (enumIndex == getCompareToNum())) ||
                     ((condition == GT) && (enumIndex > getCompareToNum())) ||
                     ((condition == LT) && (enumIndex < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
            }
         }
      }
   }

}

//------------------------------------------------------------------------------
// printMessage(): Recursive function to print all fields in this message
//---------------------------------------------------------------------------
void PrintSelected::printMessage(std::ostream& soutFields, std::ostream& soutVals, const google::protobuf::Message* const msg)
{

   const google::protobuf::Descriptor* descriptor = msg->GetDescriptor();
   const google::protobuf::Reflection* reflection = msg->GetReflection();

   const google::protobuf::Message& root = *msg;
   std::string msgName = descriptor->name();

   int fieldCount = descriptor->field_count();

   const google::protobuf::FieldDescriptor* fieldDescriptor = 0;

   std::string msgDivider = ";   ";
   std::string fieldDivider = ", ";

   bool lastField = false;

   if (fieldCount > 0) {
      for (int i = 0; i < fieldCount; i++) {
         if ((i+1) == fieldCount) lastField = true;

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // what type is this field?
         google::protobuf::FieldDescriptor::CppType cppType = fieldDescriptor->cpp_type();

         if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {

            // do the same again for this message, etc.
            const google::protobuf::Message& sub_message = reflection->GetMessage(root, fieldDescriptor);

            // Are there sub-messages?
            if (&sub_message != NULL) {

               // call this until no more messages
               printMessage(soutFields, soutVals, &sub_message);
            }
         }
         else {
            // not a message: Print the field
            if (reflection->HasField(root, fieldDescriptor)) {
               soutFields <<  std::left << std::setw(12) << fieldDescriptor->name(); // Field name

               // get the value
               switch (cppType) {
                  case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
                     soutVals <<  std::left << std::setw(12)<<  reflection->GetString(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_INT32: {
                     soutVals <<  std::left << std::setw(12)<<  reflection->GetInt32(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_INT64: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetInt64(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_UINT32: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetUInt32(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetUInt64(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE: {
                     soutVals  <<  std::left << std::setw(12)<< reflection->GetDouble(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT: {
                     soutVals  <<  std::left << std::setw(12)<< reflection->GetFloat(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_BOOL: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetBool(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
                     const google::protobuf::EnumValueDescriptor* enumVal = reflection->GetEnum(root, fieldDescriptor);
                     int enumIndex = enumVal->index();
                     soutVals <<  std::left << std::setw(12)<<  enumIndex;
                     break;
                  }
                  default: {
                     soutVals << "   \t";
                     break;
                  }
               }
            }  // end if has field
            else {
               // This field had no data. We should skip it
            }
            if (lastField) {
               // print message divider
               soutFields << msgDivider;
               soutVals << msgDivider;
            }
            else {
               // print field Divider
               soutFields << fieldDivider;
               soutVals << fieldDivider;
            }
         }
      }
   }

}

// Set selection data by function call:
//------------------------------------------------------------------------------
// setMsgToken(): Set Event Token of interest
//---------------------------------------------------------------------------
bool PrintSelected::setMsgToken(const unsigned int token)
{
   msgToken = token;
   return true;
}

//------------------------------------------------------------------------------
// setFieldOfInterest(): Set field of interest
//---------------------------------------------------------------------------
bool PrintSelected::setFieldOfInterest(const std::string field )
{
   fieldNameStr = field;
   fieldName = field.c_str();
   return true;
}
//------------------------------------------------------------------------------
// setCompareToValue(): Set string to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const std::string strVal)
{
   compareValS = strVal.c_str();
   compareStr.assign(compareValS);
   return true;
}

//------------------------------------------------------------------------------
// setCompareToValue(): Set integer value to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const int numVal )
{
   compareValI = numVal;
   return true;
}

//------------------------------------------------------------------------------
// setCompareToValue(): Set double value to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const double dblVal )
{
   compareValD = dblVal;
   return true;
}

//------------------------------------------------------------------------------
// setCompareCondition(): Set comparison condition
//---------------------------------------------------------------------------
bool PrintSelected::setCompareCondition(const Condition cc )
{
   condition = cc;
   return true;
}

//------------------------------------------------------------------------------
// setTimeOnly(): Set time only flag
//---------------------------------------------------------------------------
bool PrintSelected::setTimeOnly(const bool flg)
{
   timeOnly = flg;
   return true;
}

//------------------------------------------------------------------------------
// getEventMsgName(): Return the name of the event message in a data record
//---------------------------------------------------------------------------
std::string PrintSelected::getEventMsgName(const google::protobuf::Message* const msg)
{
   std::string name = "";

   const google::protobuf::Descriptor* descriptor = msg->GetDescriptor();
   const google::protobuf::Reflection* reflection = msg->GetReflection();

   const google::protobuf::Message& root = *msg;

   int fieldCount = descriptor->field_count();
   const google::protobuf::FieldDescriptor* fieldDescriptor = 0;
   eventMsg = 0;

   if (descriptor->field_count() > 0) {
      for (int i = 0; i < fieldCount; i++) {

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // Is this a message? 
         if (fieldDescriptor->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
            // Yes, check has field, then save the name
            if (reflection->HasField(root, fieldDescriptor)) {
               if (fieldDescriptor->name() != "time") {
                  name = fieldDescriptor->name();
                  eventMsg = &(reflection->GetMessage(root, fieldDescriptor));
               }
            }
         }
      }
   }

   return name;
}

//------------------------------------------------------------------------------
// printTimeMsg() -- format a time string for printing
//------------------------------------------------------------------------------
std::string PrintSelected::printTimeMsg(double time)
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // utc time
    Basic::Time::getHHMMSS(LCreal(time), &hh, &mm, &ss);
    std::sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    std::string timeStr;
    timeStr.assign(cbuf);
    return timeStr;

}

} // End Recorder namespace
} // End Eaagles namespace
