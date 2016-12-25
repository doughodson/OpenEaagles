
#ifndef __oe_simulation_recorder_macros__
#define __oe_simulation_recorder_macros__

//------------------------------------------------------------------------------
// Recorder macros
//------------------------------------------------------------------------------

// Macros used to sample data
//
//       BEGIN_RECORD_DATA_SAMPLE(pRecorder, token)
//          Starts a record data sample section
//          'pRecorder' is a pointer to the data recorder
//          'token' is the recorder event token (see openeaagles/simulation/recorderTokens.h)
//
//       END_RECORD_DATA_SAMPLE()
//          Completes the record data sample section
//
//       SAMPLE_1_OBJECT(  P1 )
//       SAMPLE_2_OBJECTS( P1, P2 )
//       SAMPLE_3_OBJECTS( P1, P2, P3 )
//       SAMPLE_4_OBJECTS( P1, P2, P3, P4 )
//          Sample data from 1 to 4 objects
//
//       SAMPLE_1_VALUE(  V1 )
//       SAMPLE_2_VALUES( V1, V2 )
//       SAMPLE_3_VALUES( V1, V2, V3 )
//       SAMPLE_4_VALUES( V1, V2, V3, V4 )
//          From 1 to 4 sample values
//
// Examples:
//
//    Simple event
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_SIMPLE_EVENT )
//          END_RECORD_DATA_SAMPLE()
//
//    Event with a single object
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_SMALL_EVENT )
//             SAMPLE_1_OBJECT( newPlayer )
//          END_RECORD_DATA_SAMPLE()
//
//    Event with several objects and values
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_BIG_EVENT )
//             SAMPLE_2_OBJECTS( ownship, tgtPlayer )
//             SAMPLE_2_VALUES( someValue, anotherValue )
//          END_RECORD_DATA_SAMPLE()
//
//  Notes:
//    1) If the pointer to the Data Recorder is zero (null)
//       then no data is recorded
//

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

#define BEGIN_RECORD_DATA_SAMPLE(pRecorder, token)                                 \
{                                                                                  \
   ::oe::simulation::DataRecorder* _p = pRecorder;                                 \
   if (pRecorder != nullptr)  {                                                    \
      unsigned int _token = token;                                                 \
      const ::oe::base::Object* _obj[4] = { nullptr, nullptr, nullptr, nullptr };  \
      double _val[4] = { 0.0, 0.0, 0.0, 0.0 };

#define SAMPLE_1_OBJECT(P1)               _obj[0] = P1;
#define SAMPLE_2_OBJECTS(P1, P2)          _obj[0] = P1; _obj[1] = P2;
#define SAMPLE_3_OBJECTS(P1, P2, P3)      _obj[0] = P1; _obj[1] = P2; _obj[2] = P3;
#define SAMPLE_4_OBJECTS(P1, P2, P3, P4)  _obj[0] = P1; _obj[1] = P2; _obj[2] = P3; _obj[3] = P4; 

#define SAMPLE_1_VALUE(V1)                _val[0] = V1; 
#define SAMPLE_2_VALUES(V1, V2)           _val[0] = V1; _val[1] = V2;
#define SAMPLE_3_VALUES(V1, V2, V3)       _val[0] = V1; _val[1] = V2; _val[2] = V3;
#define SAMPLE_4_VALUES(V1, V2, V3, V4)   _val[0] = V1; _val[1] = V2; _val[2] = V3; _val[3] = V4; 

#define END_RECORD_DATA_SAMPLE()                      \
      _p->recordData(_token, _obj, _val);             \
   }                                                  \
}

#endif
