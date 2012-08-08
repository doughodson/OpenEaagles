//------------------------------------------------------------------------------
// Recorder macros and 
// DataRecorder and RecorderComponent inline functions
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_DataRecorder_Inline__
#define __Eaagles_Simulation_DataRecorder_Inline__


//==============================================================================
// Recorder Macros
//
//    BEGIN_RECORD_DATA_SAMPLE(pRecorder, token)
//       Starts a record data sample section
//       'pRecorder' is a pointer to the data recorder
//       'token' is the recorder event token (see openeaagles/simulation/recorderTokens.h)
//
//    SAMPLE_1_OBJECT(  P0 )
//    SAMPLE_2_OBJECTS( P0, P1 )
//    SAMPLE_3_OBJECTS( P0, P1, P2 )
//    SAMPLE_4_OBJECTS( P0, P1, P2, P3 )
//       Sample data from 1 to 4 objects
//
//    SAMPLE_1_VALUE(  V0 )
//    SAMPLE_2_VALUES( V0, V1 )
//    SAMPLE_3_VALUES( V0, V1, V2 )
//    SAMPLE_4_VALUES( V0, V1, V2, V3 )
//       From 1 to 4 sample values
//
//    END_RECORD_DATA_SAMPLE()
//       Completes the record data sample section
//
//
// Example:
//
//    DataRecorder* p = getSimulation()->getDataRecorder();
//    BEGIN_RECORD_DATA_SAMPLE( p, REID_NEW_PLAYER )
//       SAMPLE_1_OBJECT( newPlayer )
//    END_RECORD_DATA_SAMPLE()
//
//==============================================================================

#define BEGIN_RECORD_DATA_SAMPLE(pRecorder, token)             \
{                                                              \
   ::Eaagles::Simulation::DataRecorder* _p = pRecorder;        \
   if (pRecorder != 0)  {                                      \
      unsigned int _token = token;                             \
      const ::Eaagles::Basic::Object* _obj[4] = { 0, 0, 0, 0 };  \
      double _val[4] = { 0, 0, 0, 0 };

#define SAMPLE_1_OBJECT(P1)               _obj[0] = P1;
#define SAMPLE_2_OBJECTS(P1, P2)          _obj[0] = P1; _obj[1] = P2;
#define SAMPLE_3_OBJECTS(P1, P2, P3)      _obj[0] = P1; _obj[1] = P2; _obj[2] = P3;
#define SAMPLE_4_OBJECTS(P1, P2, P3, P4)  _obj[0] = P1; _obj[1] = P2; _obj[2] = P3; _obj[3] = P4; 

#define SAMPLE_1_VALUE(V1)                _val[0] = V1; 
#define SAMPLE_2_VALUES(V1, V2)           _val[0] = V1; _val[1] = V2;
#define SAMPLE_3_VALUES(V1, V2, V3)       _val[0] = V1; _val[1] = V2; _val[2] = V3;
#define SAMPLE_4_VALUES(V1, V2, V3, V4)   _val[0] = V1; _val[1] = V2; _val[2] = V3; _val[3] = V4; 

#define END_RECORD_DATA_SAMPLE()                      \
      _p->recordData(_token, _obj, _val);              \
   }                                                  \
}

//==============================================================================
// DataRecorder inline functions
//==============================================================================

// Record Data function
inline bool DataRecorder::recordData(
      const unsigned int id,
      const Basic::Object* pObjects[4],
      const double values[4]
   )
{
   bool recorded = false;
   if (isDataEnabled(id)) {
      recorded = recordDataImp(id, pObjects, values);
      if (!recorded) processUnhandledId(id);
   }
   return recorded;
}


//==============================================================================
// RecorderComponent inline functions
//==============================================================================

// Data record enabled function
inline bool RecorderComponent::isDataEnabled(const unsigned int id) const
{
   bool ok = true;   // default is enabled

   if (id != REID_END_OF_DATA) { // END_OF_DATA message is always enabled

      // Do we have an enabled list?
      if (numEnabled > 0 && enabledList != 0) {
         ok = false; // yes -- then check to see if this message is enabled
         for (unsigned int i = 0; !ok && i < numEnabled; i++) {
            ok = (id == enabledList[i]);
         }
      }

      // Otherwise, do we have an disabled list?
      else if (numDisabled > 0 && disabledList != 0) {
         // yes -- then check to see if this message should be disabled
         for (unsigned int i = 0; ok && i < numDisabled; i++) {
            ok =  (id != disabledList[i]);
         }
      }
   }

   return ok;
}

#endif
