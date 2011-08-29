//------------------------------------------------------------------------------
// Classes:   Logger, Logger::LogEvent
//------------------------------------------------------------------------------
#ifndef __Logger_H_03103FAB_82F5_460e_BA9F_2050042905C3__
#define __Logger_H_03103FAB_82F5_460e_BA9F_2050042905C3__

#include "openeaagles/basic/Component.h"
#include <fstream>

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:   Logger
// Base class:  Object -> Component -> Logger
// Description: General Message/Event Logger
//
// Form name: Logger
// Slots:
//     file       <String>     ! Log file name (default: empty sring)
//     path       <String>     ! Path to log directory (optional). (default: empty string)
//     topLine    <String>     ! Optional top (first) line of file. (default: 0)
//
//
// Public member functions:
//
//      bool isOpen()
//      bool isFailed()
//          Get the state of the log file.
//
//      log(char* msg)
//      log(LogEvent* event)
//          Add an entry to the log file.
//
//      updateTC(LCreal dt)
//          Update time critical part of this component (empty)
//
//      updateData(LCreal dt)
//          Update background part of this component (tries to open the logfile)
//------------------------------------------------------------------------------
class Logger : public Component {
    DECLARE_SUBCLASS(Logger,Component)

public:
    // -------------------------------------------------
    // Class:   LogEvent
    // Base class:  Object -> LogEvent
    // Description: Abstract class for all logger events
    // -------------------------------------------------
    class LogEvent : public Object {
        DECLARE_SUBCLASS(LogEvent,Object)
    public:
        LogEvent();
        virtual const char* getDescription() =0;
    };

public:
    Logger();

    bool isOpen()       { return opened; }
    bool isFailed()     { return failed; }
    
    virtual void log(const char* const msg);
    virtual void log(LogEvent* const event);

    // Component interface
    virtual void updateTC(const LCreal dt = 0.0f);
    virtual void updateData(const LCreal dt = 0.0);
    
protected:
    virtual bool setSlotFilename(const String* const msg);
    virtual bool setSlotPathName(const String* const msg);
    virtual bool setSlotTopLine(const String* const msg);
    
    virtual bool openFile();

private:
    std::ofstream*   lout;       // Output stream
    String*        filename;   // Log file name
    String*        pathname;   // Path to log file directory
    const String*  topLine;    // Optional top (first) line of output
    bool             opened;     // File opened
    bool             failed;     // Open or write failed
};

} // End Basic namespace
} // End Eaagles namespace


#endif	/* __Logger_H_03103FAB_82F5_460e_BA9F_2050042905C3__ */
