//------------------------------------------------------------------------------
// DataRecorder macros
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_DataRecorder_Inline__
#define __Eaagles_Recorder_DataRecorder_Inline__

//==============================================================================
// inline functions
//==============================================================================
inline OutputHandler* DataRecorder::getOutputHandler()                {  return outputHandler; }
inline const OutputHandler* DataRecorder::getOutputHandler() const    {  return outputHandler; }

//==============================================================================
// DataRecorder Macros
//
//    BEGIN_RECORDER_HANDLER_TABLE(ThisType) and END_RECORDER_HANDLER_TABLE() 
//       Implement the beginning and ending sections of the recordDataImp() function.
//
//    ON_RECORDER_EVENT_ID( id, func )
//       If 'id' matches the recorder event ID pass to the recordDataImp()
//       then the function 'func' is called.
//
//==============================================================================

#define BEGIN_RECORDER_HANDLER_TABLE(ThisType)                 \
bool ThisType::recordDataImp(                                  \
      const unsigned int _id,                                  \
      const Eaagles::Basic::Object* _obj[4],                   \
      const double _val[4] )                                   \
{                                                              \
   bool _recorded = false;



#define END_RECORDER_HANDLER_TABLE()                                       \
   if (!_recorded) _recorded = BaseClass::recordDataImp(_id, _obj, _val);  \
   return _recorded;                                                       \
}


#define ON_RECORDER_EVENT_ID(token,recorderDataFunc)            \
   if (!_recorded && token == _id) {                           \
      _recorded = recorderDataFunc(_obj, _val);        \
   }


#endif
