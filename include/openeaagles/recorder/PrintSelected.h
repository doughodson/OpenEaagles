//------------------------------------------------------------------------------
// Class: PrintSelected
//------------------------------------------------------------------------------
#ifndef __oe_recorder_PrintSelected_H__
#define __oe_recorder_PrintSelected_H__

#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/PrintHandler.h"

#include <string>
#include <sstream>
#include <fstream>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

namespace oe {
      namespace base { class Float; class Integer; }
namespace recorder {
   namespace pb {
      class Time; class FileIdMsg; class NewPlayerEventMsg; class PlayerRemovedEventMsg; class PlayerDataMsg;
      class PlayerDamagedEventMsg; class PlayerCollisionEventMsg; class PlayerCrashEventMsg;
      class PlayerKilledEventMsg; class WeaponReleaseEventMsg; class WeaponHungEventMsg;
      class WeaponDetonationEventMsg; class GunFiredEventMsg; class NewTrackEventMsg;
      class TrackRemovedEventMsg; class TrackDataMsg; class PlayerId; class PlayerState;
      class TrackData; class EmissionData;
   }

//------------------------------------------------------------------------------
// Class:   PrintSelected
// Description: Print selected data record message data
//
// Factory name: PrintSelected
// Slots:
//   messageToken    <base::Number>   ! Message ID (token)
//   fieldName       <base::String>   ! Full field name (e.g., oe.Recorder.Pb.PlayerId.name)
//   compareToValS   <base::String>   ! value to compare (string)
//   compareToValI   <base::Number>   ! value to compare (num)
//   compareToValD   <base::Number>   ! value to compare (dbl)
//   condition       <base::String>   ! EQ, LT, or GT (ignored for bool and strings)
//   timeOnly        <base::Number>   ! match time conditions only. Print ALL messages that match
//------------------------------------------------------------------------------
class PrintSelected : public PrintHandler
{
    DECLARE_SUBCLASS(PrintSelected, PrintHandler)

public:

   PrintSelected();

   virtual bool setSlotMsgToken(const base::Number* const msg);
   virtual bool setSlotFieldName(const base::String* const msg);

   virtual bool setSlotCompareToStr(const base::String* const msg);
   virtual bool setSlotCompareToNum(const base::Number* const msg);
   virtual bool setSlotCompareToDbl(const base::Number* const msg);
   virtual bool setSlotCondition(const base::String* const msg);
   virtual bool setSlotTimeOnly(const base::Number* const msg);
   enum Condition { EQ, LT, GT };

   unsigned int getMsgToken() const;
   std::string getFieldName() const;
   std::string getCompareToStr() const;
   double getCompareToDbl() const;
   int getCompareToNum() const;
   bool getCompareToBool() const;

   // Set comparison criteria:
   bool setMsgToken(const unsigned int token);
   bool setFieldOfInterest(const std::string& fieldName );
   bool setCompareToValue(const std::string& strVal);
   bool setCompareToValue(const int numVal );
   bool setCompareToValue(const double dblVal );
   bool setCompareCondition(const Condition cc );
   bool setTimeOnly(const bool flg );

protected:

   virtual void processRecordImp(const DataRecordHandle* const handle) override;

   // Recursive function to look at each embedded message
   void processMessage(const google::protobuf::Message* const msg);

   // Recursive function to print all messages within a top-level message
   void printMessage(std::ostream& soutFields, std::ostream& soutVals, const google::protobuf::Message* const msg);

   // Return the event message name
   std::string getEventMsgName(const google::protobuf::Message* const msg);

   std::string printTimeMsg(double time);

private:
   void initData();

   // slot data:

   unsigned int msgToken;
   double compareValD;
   int compareValI;
   Condition condition;

   std::string fieldNameStr;
   std::string compareStr;
   const google::protobuf::Message* recMsg;
   const google::protobuf::Message* eventMsg;
   bool foundSelected;
   bool printHeader;
   bool timeOnly;


};

inline unsigned int PrintSelected::getMsgToken() const { return msgToken; }
inline std::string PrintSelected::getFieldName() const { return fieldNameStr; }
inline std::string PrintSelected::getCompareToStr() const { return compareStr; }
inline double PrintSelected::getCompareToDbl() const { return compareValD; }
inline int PrintSelected::getCompareToNum() const { return compareValI; }
inline bool PrintSelected::getCompareToBool() const
{
   if (compareValI == 0) return false;
   else return true;
}

}
}

#endif
